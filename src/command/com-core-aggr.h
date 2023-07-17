/*
    This file is part of Perun2.
    Perun2 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    Peruns2 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with Perun2. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef COM_CORE_AGGR_H
#define COM_CORE_AGGR_H

#include "com-core.h"
#include "com-aggregate.h"


namespace perun2::comm
{

template <typename T>
struct C_AggrDelivery : Command
{
public:
   C_AggrDelivery(Aggregate* aggr, _genptr<T>& val, p_perun2& p2)
      : aggregate(aggr), value(std::move(val)), locationContext(p2.contexts.getLocationContext()) { };

protected:
   LocationContext* const locationContext;
   Aggregate* const aggregate;
   _genptr<T> value;
};


struct C_AggrCopy_This : Command
{
public:
   C_AggrCopy_This(Aggregate* aggr, FileContext& ctx)
      : aggregate(aggr), context(ctx) { };

   void run() override;

private:
   Aggregate* const aggregate;
   FileContext& context;
};


struct C_AggrCopy_String : C_AggrDelivery<p_str>
{
public:
   C_AggrCopy_String(Aggregate* aggr, _genptr<p_str>& val, p_perun2& p2)
      : C_AggrDelivery<p_str>(aggr, val, p2) {};

   void run() override;
};


struct C_AggrCopy_List : C_AggrDelivery<p_list>
{
public:
   C_AggrCopy_List(Aggregate* aggr, _genptr<p_list>& val, p_perun2& p2)
      : C_AggrDelivery<p_list>(aggr, val, p2) {};

   void run() override;
};


struct C_AggrSelect_This : Command
{
public:
   C_AggrSelect_This(Aggregate* aggr, FileContext& ctx)
      : aggregate(aggr), context(ctx) { };

   void run() override;

private:
   Aggregate* const aggregate;
   FileContext& context;
};


struct C_AggrSelect_String : C_AggrDelivery<p_str>
{
public:
   C_AggrSelect_String(Aggregate* aggr, _genptr<p_str>& val, p_perun2& p2)
      : C_AggrDelivery<p_str>(aggr, val, p2) {};

   void run() override;
};


struct C_AggrSelect_List : C_AggrDelivery<p_list>
{
public:
   C_AggrSelect_List(Aggregate* aggr, _genptr<p_list>& val, p_perun2& p2)
      : C_AggrDelivery<p_list>(aggr, val, p2) {};

   void run() override;
};


void logCopyError(p_perun2& p2, const p_str& name);
void logCopySuccess(p_perun2& p2, const p_str& name);
void logSelectError(p_perun2& p2, const p_str& name);
void logSelectSuccess(p_perun2& p2, const p_str& name);


template <typename T>
struct C_Aggr : Command
{
public:
   C_Aggr(_genptr<T>& val, p_perun2& p2)
      : value(std::move(val)), perun2(p2), 
        locationContext(p2.contexts.getLocationContext()) { };

protected:
   LocationContext* locationContext;
   _genptr<T> value;
   p_perun2& perun2;
};


struct C_Copy_String : C_Aggr<p_str>
{
public:
   C_Copy_String(_genptr<p_str>& val, p_perun2& p2)
      : C_Aggr<p_str>(val, p2) {};

   void run() override;
};


struct C_Copy_List : C_Aggr<p_list>
{
public:
   C_Copy_List(_genptr<p_list>& val, p_perun2& p2)
      : C_Aggr<p_list>(val, p2) {};

   void run() override;
};


struct Selector
{
public:
   Selector() = delete;
   Selector(p_str& pth, p_str& prnt, p_perun2& p2)
      : path(pth), parent(prnt), perun2(p2) { };

   void reset();
   void insertValue();
   void run();

private:
   std::map<p_str, p_set> selectPaths;
   p_str prevParent;
   p_set* prevSet;
   p_bool isFirst = true;

   p_str& path;
   p_str& parent;

   p_perun2& perun2;
};


struct C_Select_String : C_Aggr<p_str>
{
public:
   C_Select_String(_genptr<p_str>& val, p_perun2& p2)
      : C_Aggr<p_str>(val, p2) {};

   void run() override;
};


struct C_Select_List : C_Aggr<p_list>
{
public:
   C_Select_List(_genptr<p_list>& val, p_perun2& p2)
      : C_Aggr<p_list>(val, p2), 
        selector(path, parent, p2) { };
   void run() override;

private:
   p_str path;
   p_str parent;
   Selector selector;
};


struct C_Select_Definition : Command
{
public:
   C_Select_Definition(p_defptr& val, p_perun2& p2) 
      : value(std::move(val)), perun2(p2), 
      locationContext(p2.contexts.getLocationContext()), 
      selector(path, parent, p2) { };

   void run() override;

private:
   LocationContext* locationContext;
   p_defptr value;
   p_perun2& perun2;
   
   p_str path;
   p_str parent;
   Selector selector;
};


struct C_Select_ContextDefinition : Command
{
public:
   C_Select_ContextDefinition(p_defptr& val, p_perun2& p2, FileContext* ctx) 
      : value(std::move(val)), perun2(p2), context(ctx), 
        selector(context->v_path->value, context->v_parent->value, p2) { };

   void run() override;

private:
   p_defptr value;
   p_perun2& perun2;
   FileContext* const context;
   Selector selector;
};


}

#endif /* COM_CORE_AGGR_H */

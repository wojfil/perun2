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
   C_AggrDelivery(Aggregate* aggr, _genptr<T>& val, _p2& p2)
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


struct C_AggrCopy_String : C_AggrDelivery<_str>
{
public:
   C_AggrCopy_String(Aggregate* aggr, _genptr<_str>& val, _p2& p2)
      : C_AggrDelivery<_str>(aggr, val, p2) {};

   void run() override;
};


struct C_AggrCopy_List : C_AggrDelivery<_list>
{
public:
   C_AggrCopy_List(Aggregate* aggr, _genptr<_list>& val, _p2& p2)
      : C_AggrDelivery<_list>(aggr, val, p2) {};

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


struct C_AggrSelect_String : C_AggrDelivery<_str>
{
public:
   C_AggrSelect_String(Aggregate* aggr, _genptr<_str>& val, _p2& p2)
      : C_AggrDelivery<_str>(aggr, val, p2) {};

   void run() override;
};


struct C_AggrSelect_List : C_AggrDelivery<_list>
{
public:
   C_AggrSelect_List(Aggregate* aggr, _genptr<_list>& val, _p2& p2)
      : C_AggrDelivery<_list>(aggr, val, p2) {};

   void run() override;
};


void logCopyError(_p2& p2, const _str& name);
void logCopySuccess(_p2& p2, const _str& name);
void logSelectError(_p2& p2, const _str& name);
void logSelectSuccess(_p2& p2, const _str& name);


template <typename T>
struct C_Aggr : Command
{
public:
   C_Aggr(_genptr<T>& val, _p2& p2)
      : value(std::move(val)), perun2(p2), 
        locationContext(p2.contexts.getLocationContext()) { };

protected:
   LocationContext* locationContext;
   _genptr<T> value;
   _p2& perun2;
};


struct C_Copy_String : C_Aggr<_str>
{
public:
   C_Copy_String(_genptr<_str>& val, _p2& p2)
      : C_Aggr<_str>(val, p2) {};

   void run() override;
};


struct C_Copy_List : C_Aggr<_list>
{
public:
   C_Copy_List(_genptr<_list>& val, _p2& p2)
      : C_Aggr<_list>(val, p2) {};

   void run() override;
};


struct C_Select_String : C_Aggr<_str>
{
public:
   C_Select_String(_genptr<_str>& val, _p2& p2)
      : C_Aggr<_str>(val, p2) {};

   void run() override;
};


struct C_Select_List : C_Aggr<_list>
{
public:
   C_Select_List(_genptr<_list>& val, _p2& p2)
      : C_Aggr<_list>(val, p2) {};
   void run() override;
};

}

#endif /* COM_CORE_AGGR_H */

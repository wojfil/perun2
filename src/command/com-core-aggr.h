/*
    This file is part of Uroboros.
    Uroboros is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    Uroboros is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with Uroboros. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef COM_CORE_AGGR_H
#define COM_CORE_AGGR_H

#include "com-core.h"
#include "com-listener.h"
#include "com-aggregate.h"


namespace uro::comm
{

template <typename T>
struct C_AggrDelivery : Command_L
{
public:
   C_AggrDelivery(Aggregate* aggr, _genptr<T>& val, Uroboros& uro)
      : aggregate(aggr), value(std::move(val)), Command_L(uro) {};

protected:
   Aggregate* aggregate;
   _genptr<T> value;
};


struct C_AggrCopy_String : C_AggrDelivery<_str>
{
public:
   C_AggrCopy_String(Aggregate* aggr, _genptr<_str>& val, Uroboros& uro)
      : C_AggrDelivery<_str>(aggr, val, uro) {};

   void run() override;
};


struct C_AggrCopy_List : C_AggrDelivery<_list>
{
public:
   C_AggrCopy_List(Aggregate* aggr, _genptr<_list>& val, Uroboros& uro)
      : C_AggrDelivery<_list>(aggr, val, uro) {};

   void run() override;
};


struct C_AggrSelect_String : C_AggrDelivery<_str>
{
public:
   C_AggrSelect_String(Aggregate* aggr, _genptr<_str>& val, Uroboros& uro)
      : C_AggrDelivery<_str>(aggr, val, uro) {};

   void run() override;
};


struct C_AggrSelect_List : C_AggrDelivery<_list>
{
public:
   C_AggrSelect_List(Aggregate* aggr, _genptr<_list>& val, Uroboros& uro)
      : C_AggrDelivery<_list>(aggr, val, uro) {};

   void run() override;
};


void logCopyError(Uroboros& uro, const _str& name);
void logCopySuccess(Uroboros& uro, const _str& name);
void logSelectError(Uroboros& uro, const _str& name);
void logSelectSuccess(Uroboros& uro, const _str& name);


template <typename T>
struct C_Aggr : Command_L
{
public:
   C_Aggr(_genptr<T>& val, Uroboros& uro)
      : value(std::move(val)), Command_L(uro) {};

protected:
   _genptr<T> value;
};


struct C_Copy_String : C_Aggr<_str>
{
public:
   C_Copy_String(_genptr<_str>& val, Uroboros& uro)
      : C_Aggr<_str>(val, uro) {};

   void run() override;
};


struct C_Copy_List : C_Aggr<_list>
{
public:
   C_Copy_List(_genptr<_list>& val, Uroboros& uro)
      : C_Aggr<_list>(val, uro) {};

   void run() override;
};


struct C_Select_String : C_Aggr<_str>
{
public:
   C_Select_String(_genptr<_str>& val, Uroboros& uro)
      : C_Aggr<_str>(val, uro) {};

   void run() override;
};


struct C_Select_List : C_Aggr<_list>
{
public:
   C_Select_List(_genptr<_list>& val, Uroboros& uro)
      : C_Aggr<_list>(val, uro) {};
   void run() override;
};

}

#endif /* COM_CORE_AGGR_H */

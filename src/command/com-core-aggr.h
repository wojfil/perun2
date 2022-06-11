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
#include "aggregate.h"



template <typename T>
struct C_AggrDelivery
{
public:
   C_AggrDelivery(Aggregate* aggr, Generator<T>* val)
      : aggregate(aggr), value(val) {};

   ~C_AggrDelivery() {
      delete value;
   }

protected:
   Aggregate* aggregate;
   Generator<T>* value;
};


struct C_AggrCopy_String : C_AggrDelivery<_str>, Command
{
public:
   C_AggrCopy_String(Aggregate* aggr, Generator<_str>* val)
      : C_AggrDelivery<_str>(aggr, val) {};

   void run() override;
};


struct C_AggrCopy_List : C_AggrDelivery<_list>, Command
{
public:
   C_AggrCopy_List(Aggregate* aggr, Generator<_list>* val)
      : C_AggrDelivery<_list>(aggr, val) {};

   void run() override;
};


struct C_AggrSelect_String : C_AggrDelivery<_str>, Command
{
public:
   C_AggrSelect_String(Aggregate* aggr, Generator<_str>* val)
      : C_AggrDelivery<_str>(aggr, val) {};

   void run() override;
};


struct C_AggrSelect_List : C_AggrDelivery<_list>, Command
{
public:
   C_AggrSelect_List(Aggregate* aggr, Generator<_list>* val)
      : C_AggrDelivery<_list>(aggr, val) {};

   void run() override;
};


void logCopyError(const _str& name);
void logCopySuccess(const _str& name);
void logSelectError(const _str& name);
void logSelectSuccess(const _str& name);


template <typename T>
struct C_Aggr
{
public:
   C_Aggr(Generator<T>* val) : value(val) {};

   ~C_Aggr() {
      delete value;
   }

protected:
   Generator<T>* value;
};


struct C_Copy_String : C_Aggr<_str>, Command
{
public:
   C_Copy_String(Generator<_str>* val) : C_Aggr<_str>(val) {};
   void run() override;
};


struct C_Copy_List : C_Aggr<_list>, Command
{
public:
   C_Copy_List(Generator<_list>* val) : C_Aggr<_list>(val) {};
   void run() override;
};


struct C_Select_String : C_Aggr<_str>, Command
{
public:
   C_Select_String(Generator<_str>* val) : C_Aggr<_str>(val) {};
   void run() override;
};


struct C_Select_List : C_Aggr<_list>, Command
{
public:
   C_Select_List(Generator<_list>* val) : C_Aggr<_list>(val) {};
   void run() override;
};


#endif /* COM_CORE_AGGR_H */

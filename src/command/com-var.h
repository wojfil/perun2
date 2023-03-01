/*
    This file is part of Uroboros2.
    Uroboros2 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    Uroboros2 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with Uroboros2. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef COM_VAR_H
#define COM_VAR_H

#include "com.h"
#include "..\var.h"
#include "..\util.h"
#include "..\datatype\datatype.h"


namespace uro::comm
{

template <typename T>
struct VarAssignment : Command
{
public:
   VarAssignment<T> (Variable<T>& var, _genptr<T>& val)
      : variable(var), value(std::move(val)) { };

   void run() override {
      variable.value = value->getValue();
   };

private:
   Variable<T>& variable;
   _genptr<T> value;
};


template <typename T>
struct VarAdd_ : Command
{
public:
   VarAdd_<T> (Variable<T>& var, _genptr<T>& val)
      : variable(var), value(std::move(val)) { };

   void run() override {
      variable.value += value->getValue();
   };

private:
   Variable<T>& variable;
   _genptr<T> value;
};


template <typename T>
struct VarSubtract : Command
{
public:
   VarSubtract<T> (Variable<T>& var, _genptr<T>& val)
      : variable(var), value(std::move(val)) { };

   void run() override {
      variable.value -= value->getValue();
   };

private:
   Variable<T>& variable;
   _genptr<T> value;
};


struct VarNumMultiply : Command
{
public:
   VarNumMultiply(Variable<_num>& var, _genptr<_num>& val)
      : variable(var), value(std::move(val)) { };

   void run() override;

private:
   Variable<_num>& variable;
   _genptr<_num> value;
};


struct VarNumDivide : Command
{
public:
   VarNumDivide(Variable<_num>& var, _genptr<_num>& val)
      : variable(var), value(std::move(val)) { };

   void run() override;

private:
   Variable<_num>& variable;
   _genptr<_num> value;
};


struct VarPerMultiply : Command
{
public:
   VarPerMultiply(Variable<_per>& var, _genptr<_num>& val)
      : variable(var), value(std::move(val)) { };

   void run() override;

private:
   Variable<_per>& variable;
   _genptr<_num> value;
};


struct VarPerDivide : Command
{
public:
   VarPerDivide(Variable<_per>& var, _genptr<_num>& val)
      : variable(var), value(std::move(val)) { };

   void run() override;

private:
   Variable<_per>& variable;
   _genptr<_num> value;
};


struct VarModulo : Command
{
public:
   VarModulo(Variable<_num>& var, _genptr<_num>& val)
      : variable(var), value(std::move(val)) { };

   void run() override;

private:
   Variable<_num>& variable;
   _genptr<_num> value;
};


struct VarIncrement : Command
{
public:
   VarIncrement(Variable<_num>& var) : variable(var) { };
   void run() override;

private:
   Variable<_num>& variable;
};


struct VarDecrement : Command
{
public:
   VarDecrement(Variable<_num>& var) : variable(var) { };
   void run() override;

private:
   Variable<_num>& variable;
};


struct VarTimeAdd : Command
{
public:
   VarTimeAdd(Variable<_tim>& var, _genptr<_per>& per)
      : variable(var), period(std::move(per)) { };

   void run() override;

private:
   Variable<_tim>& variable;
   _genptr<_per> period;
};


struct VarTimeSubtract : Command
{
public:
   VarTimeSubtract(Variable<_tim>& var, _genptr<_per>& per)
      : variable(var), period(std::move(per)) { };

   void run() override;

private:
   Variable<_tim>& variable;
   _genptr<_per> period;
};


struct VarCharAssignment: Command
{
public:
   VarCharAssignment(Variable<_str>& var, _genptr<_str>& val, _genptr<_num>& ind)
      : variable(var), value(std::move(val)), index(std::move(ind)) { };

   void run() override;

private:
   Variable<_str>& variable;
   _genptr<_str> value;
   _genptr<_num> index;
};


struct VarTimeUnitAssignment : Command
{
public:
   VarTimeUnitAssignment(Variable<_tim>& var, _genptr<_num>& val,
      const Period::PeriodUnit& un) : variable(var), value(std::move(val)), unit(un){ };

   void run() override;

private:
   Variable<_tim>& variable;
   _genptr<_num> value;
   const Period::PeriodUnit unit;
};


struct VarTimeUnitChange : Command
{
public:
   VarTimeUnitChange(Variable<_tim>& var, _genptr<_num>& val,
      const Period::PeriodUnit& un, const _bool neg)
      : variable(var), value(std::move(val)), unit(un), negative(neg) { };

   void run() override;

private:
   Variable<_tim>& variable;
   _genptr<_num> value;
   const Period::PeriodUnit unit;
   const _bool negative;
};


struct VarTimeUnitIncrement : Command
{
public:
   VarTimeUnitIncrement(Variable<_tim>& var, const Period::PeriodUnit& un)
      : variable(var), unit(un) { };

   void run() override;

private:
   Variable<_tim>& variable;
   const Period::PeriodUnit unit;
};


struct VarTimeUnitDecrement : Command
{
public:
   VarTimeUnitDecrement(Variable<_tim>& var, const Period::PeriodUnit& un)
      : variable(var), unit(un) { };

   void run() override;

private:
   Variable<_tim>& variable;
   const Period::PeriodUnit unit;
};

}

#endif /* COM_VAR_H */

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

#ifndef COM_VAR_H
#define COM_VAR_H

#include "com.h"
#include "..\var\var.h"
#include "..\util.h"
#include "..\datatype\datatype.h"


template <typename T>
struct VarAssignment : Command
{
public:
   VarAssignment<T> (Variable<T>* var, Generator<T>* val)
      : variable(var), value(val) { };

   ~VarAssignment<T>() {
      delete value;
   }

   void run() override {
      (variable->value) = (value->getValue());
   };

private:
   Variable<T>* variable;
   Generator<T>* value;
};


template <typename T>
struct VarAdd_ : Command
{
public:
   VarAdd_<T> (Variable<T>* var, Generator<T>* val)
      : variable(var), value(val) { };

   ~VarAdd_<T>() {
      delete value;
   }

   void run() override {
      (variable->value) += (value->getValue());
   };

private:
   Variable<T>* variable;
   Generator<T>* value;
};


template <typename T>
struct VarSubtract : Command
{
public:
   VarSubtract<T> (Variable<T>* var, Generator<T>* val)
      : variable(var), value(val) { };

   ~VarSubtract<T>() {
      delete value;
   }

   void run() override {
      (variable->value) -= (value->getValue());
   };

private:
   Variable<T>* variable;
   Generator<T>* value;
};


struct VarNumMultiply : Command
{
public:
   VarNumMultiply(Variable<_num>* var, Generator<_num>* val)
      : variable(var), value(val) { };

   ~VarNumMultiply() {
      delete value;
   }

   void run() override;

private:
   Variable<_num>* variable;
   Generator<_num>* value;
};


struct VarNumDivide : Command
{
public:
   VarNumDivide(Variable<_num>* var, Generator<_num>* val)
      : variable(var), value(val) { };

   ~VarNumDivide() {
      delete value;
   }

   void run() override;

private:
   Variable<_num>* variable;
   Generator<_num>* value;
};


struct VarPerMultiply : Command
{
public:
   VarPerMultiply(Variable<_per>* var, Generator<_num>* val)
      : variable(var), value(val) { };

   ~VarPerMultiply() {
      delete value;
   }

   void run() override;

private:
   Variable<_per>* variable;
   Generator<_num>* value;
};


struct VarPerDivide : Command
{
public:
   VarPerDivide(Variable<_per>* var, Generator<_num>* val)
      : variable(var), value(val) { };

   ~VarPerDivide() {
      delete value;
   }

   void run() override;

private:
   Variable<_per>* variable;
   Generator<_num>* value;
};


struct VarModulo : Command
{
public:
   VarModulo(Variable<_num>* var, Generator<_num>* val)
      : variable(var), value(val) { };

   ~VarModulo() {
      delete value;
   }

   void run() override;

private:
   Variable<_num>* variable;
   Generator<_num>* value;
};


struct VarIncrement : Command
{
public:
   VarIncrement(Variable<_num>* var) : variable(var) { };
   void run() override;

private:
   Variable<_num>* variable;
};


struct VarDecrement : Command
{
public:
   VarDecrement(Variable<_num>* var) : variable(var) { };
   void run() override;

private:
   Variable<_num>* variable;
};


struct VarTimeAdd : Command
{
public:
   VarTimeAdd(Variable<_tim>* var, Generator<_per>* per)
      : variable(var), period(per) { };

   ~VarTimeAdd() {
      delete period;
   }

   void run() override;

private:
   Variable<_tim>* variable;
   Generator<_per>* period;
};


struct VarTimeSubtract : Command
{
public:
   VarTimeSubtract(Variable<_tim>* var, Generator<_per>* per)
      : variable(var), period(per) { };

   ~VarTimeSubtract() {
      delete period;
   }

   void run() override;

private:
   Variable<_tim>* variable;
   Generator<_per>* period;
};


struct VarCharAssignment: Command
{
public:
   VarCharAssignment(Variable<_str>* var, Generator<_str>* val, Generator<_num>* ind)
      : variable(var), value(val), index(ind) { };

   ~VarCharAssignment() {
      delete value;
      delete index;
   }

   void run() override;

private:
   Variable<_str>* variable;
   Generator<_str>* value;
   Generator<_num>* index;
};


struct VarTimeUnitAssignment : Command
{
public:
   VarTimeUnitAssignment(Variable<_tim>* var, Generator<_num>* val,
      const Period::PeriodUnit un) : variable(var), value(val), unit(un){ };

   ~VarTimeUnitAssignment() {
      delete value;
   }

   void run() override;

private:
   Variable<_tim>* variable;
   Generator<_num>* value;
   const Period::PeriodUnit unit;
};


struct VarTimeUnitChange : Command
{
public:
   VarTimeUnitChange(Variable<_tim>* var, Generator<_num>* val,
      const Period::PeriodUnit un, const bool neg)
      : variable(var), value(val), unit(un), negative(neg) { };

   ~VarTimeUnitChange() {
      delete value;
   }

   void run() override;

private:
   Variable<_tim>* variable;
   Generator<_num>* value;
   const Period::PeriodUnit unit;
   const bool negative;
};


struct VarTimeUnitIncrement : Command
{
public:
   VarTimeUnitIncrement(Variable<_tim>* var, const Period::PeriodUnit un)
      : variable(var), unit(un) { };

   void run() override;

private:
   Variable<_tim>* variable;
   const Period::PeriodUnit unit;
};


struct VarTimeUnitDecrement : Command
{
public:
   VarTimeUnitDecrement(Variable<_tim>* var, const Period::PeriodUnit un)
      : variable(var), unit(un) { };

   void run() override;

private:
   Variable<_tim>* variable;
   const Period::PeriodUnit unit;
};



#endif /* COM_VAR_H */

/*
    This file is part of Perun2.
    Perun2 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    Perun2 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with Perun2. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef COM_VAR_H
#define COM_VAR_H

#include "com.h"
#include "..\var.h"
#include "..\util.h"
#include "..\datatype\datatype.h"


namespace perun2::comm
{

template <typename T>
struct VarAssignment : Command
{
public:
   VarAssignment<T> (Variable<T>& var, p_genptr<T>& val)
      : variable(var), value(std::move(val)) { };

   void run() override {
      variable.value = value->getValue();
   };

private:
   Variable<T>& variable;
   p_genptr<T> value;
};


template <typename T>
struct VarAdd_ : Command
{
public:
   VarAdd_<T> (Variable<T>& var, p_genptr<T>& val)
      : variable(var), value(std::move(val)) { };

   void run() override {
      variable.value += value->getValue();
   };

private:
   Variable<T>& variable;
   p_genptr<T> value;
};


template <typename T>
struct VarSubtract : Command
{
public:
   VarSubtract<T> (Variable<T>& var, p_genptr<T>& val)
      : variable(var), value(std::move(val)) { };

   void run() override {
      variable.value -= value->getValue();
   };

private:
   Variable<T>& variable;
   p_genptr<T> value;
};


struct VarNumMultiply : Command
{
public:
   VarNumMultiply(Variable<p_num>& var, p_genptr<p_num>& val)
      : variable(var), value(std::move(val)) { };

   void run() override;

private:
   Variable<p_num>& variable;
   p_genptr<p_num> value;
};


struct VarNumDivide : Command
{
public:
   VarNumDivide(Variable<p_num>& var, p_genptr<p_num>& val)
      : variable(var), value(std::move(val)) { };

   void run() override;

private:
   Variable<p_num>& variable;
   p_genptr<p_num> value;
};


struct VarPerMultiply : Command
{
public:
   VarPerMultiply(Variable<p_per>& var, p_genptr<p_num>& val)
      : variable(var), value(std::move(val)) { };

   void run() override;

private:
   Variable<p_per>& variable;
   p_genptr<p_num> value;
};


struct VarPerDivide : Command
{
public:
   VarPerDivide(Variable<p_per>& var, p_genptr<p_num>& val)
      : variable(var), value(std::move(val)) { };

   void run() override;

private:
   Variable<p_per>& variable;
   p_genptr<p_num> value;
};


struct VarModulo : Command
{
public:
   VarModulo(Variable<p_num>& var, p_genptr<p_num>& val)
      : variable(var), value(std::move(val)) { };

   void run() override;

private:
   Variable<p_num>& variable;
   p_genptr<p_num> value;
};


struct VarIncrement : Command
{
public:
   VarIncrement(Variable<p_num>& var) : variable(var) { };
   void run() override;

private:
   Variable<p_num>& variable;
};


struct VarDecrement : Command
{
public:
   VarDecrement(Variable<p_num>& var) : variable(var) { };
   void run() override;

private:
   Variable<p_num>& variable;
};


struct VarTimeAdd : Command
{
public:
   VarTimeAdd(Variable<p_tim>& var, p_genptr<p_per>& per)
      : variable(var), period(std::move(per)) { };

   void run() override;

private:
   Variable<p_tim>& variable;
   p_genptr<p_per> period;
};


struct VarTimeSubtract : Command
{
public:
   VarTimeSubtract(Variable<p_tim>& var, p_genptr<p_per>& per)
      : variable(var), period(std::move(per)) { };

   void run() override;

private:
   Variable<p_tim>& variable;
   p_genptr<p_per> period;
};


struct VarCharAssignment: Command
{
public:
   VarCharAssignment(Variable<p_str>& var, p_genptr<p_str>& val, p_genptr<p_num>& ind)
      : variable(var), value(std::move(val)), index(std::move(ind)) { };

   void run() override;

private:
   Variable<p_str>& variable;
   p_genptr<p_str> value;
   p_genptr<p_num> index;
};


struct VarTimeUnitAssignment : Command
{
public:
   VarTimeUnitAssignment(Variable<p_tim>& var, p_genptr<p_num>& val,
      const Period::PeriodUnit& un) : variable(var), value(std::move(val)), unit(un){ };

   void run() override;

private:
   Variable<p_tim>& variable;
   p_genptr<p_num> value;
   const Period::PeriodUnit unit;
};


struct VarTimeUnitChange : Command
{
public:
   VarTimeUnitChange(Variable<p_tim>& var, p_genptr<p_num>& val,
      const Period::PeriodUnit& un, const p_bool neg)
      : variable(var), value(std::move(val)), unit(un), negative(neg) { };

   void run() override;

private:
   Variable<p_tim>& variable;
   p_genptr<p_num> value;
   const Period::PeriodUnit unit;
   const p_bool negative;
};


struct VarTimeUnitIncrement : Command
{
public:
   VarTimeUnitIncrement(Variable<p_tim>& var, const Period::PeriodUnit& un)
      : variable(var), unit(un) { };

   void run() override;

private:
   Variable<p_tim>& variable;
   const Period::PeriodUnit unit;
};


struct VarTimeUnitDecrement : Command
{
public:
   VarTimeUnitDecrement(Variable<p_tim>& var, const Period::PeriodUnit& un)
      : variable(var), unit(un) { };

   void run() override;

private:
   Variable<p_tim>& variable;
   const Period::PeriodUnit unit;
};

}

#endif /* COM_VAR_H */

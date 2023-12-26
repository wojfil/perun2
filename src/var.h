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

#ifndef VAR_H_INCLUDED
#define VAR_H_INCLUDED

#include "datatype/generator.h"
#include "datatype/primitives.h"


namespace perun2
{
   enum VarType
   {
      vt_User,       // variable created by user
      vt_Attribute,  // file attribute
      vt_Special     // other vars: global constant, index, this, success...
   };

   template <typename T>
   struct Variable : Generator<T>
   {
   public:
      Variable(const VarType vt)
         : type(vt) { };
      Variable(const VarType vt, const T& val)
         : type(vt), value(val) { };

      Variable() = delete;
      Variable(Variable const&) = delete;
      Variable& operator= (Variable const&) = delete;

      p_bool isConstant() const override
      {
         return this->isConstant_;
      };

      T getValue() override
      {
         return this->value;
      };

      p_bool isImmutable() const
      {
         return this->type != VarType::vt_User;
      }

      void makeNotConstant()
      {
         this->isConstant_ = false;
      }

      T value;
      p_bool isConstant_ = false;
      const VarType type;
   };


   template <typename T>
   struct VariableReference : Generator<T>
   {
   public:
      VariableReference(Variable<T>* var)
         : variable(*var) { };

      VariableReference() = delete;
      VariableReference(VariableReference const&) = delete;
      VariableReference& operator= (VariableReference const&) = delete;

      p_bool isConstant() const override
      {
         return this->variable.isConstant();
      };

      T getValue() override
      {
         return this->variable.getValue();
      };

   private:
      Variable<T>& variable;
   };
}

#endif // VAR_H_INCLUDED

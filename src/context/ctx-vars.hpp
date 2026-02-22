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

#pragma once

#include "../datatype/datatype.hpp"
#include "../var.hpp"
#include <unordered_map>


namespace perun2
{

   template <typename T>
   using p_varptr = std::unique_ptr<Variable<T>>;

   template <typename T>
   using p_varptrs = std::unordered_map<p_str, p_varptr<T>>;


   struct VarsContext
   {
   public:

      void takeVarsPtr(p_varptrs<p_bool>*& result) { result = &this->bools; };
      void takeVarsPtr(p_varptrs<p_tim>*& result) { result = &this->times; };
      void takeVarsPtr(p_varptrs<p_per>*& result) { result = &this->periods; };
      void takeVarsPtr(p_varptrs<p_str>*& result) { result = &this->strings; };
      void takeVarsPtr(p_varptrs<p_num>*& result) { result = &this->numbers; };
      void takeVarsPtr(p_varptrs<p_tlist>*& result) { result = &this->timeLists; };
      void takeVarsPtr(p_varptrs<p_nlist>*& result) { result = &this->numLists; };
      void takeVarsPtr(p_varptrs<p_list>*& result) { result = &this->lists; };

      template <typename T>
      p_bool takeVar(const p_str& var, Variable<T>*& result)
      {
         p_varptrs<T>* vars;
         this->takeVarsPtr(vars);
         auto v = vars->find(var);
         if (v != vars->end()) {
            result = v->second.get();
            return true;
         }

         return false;
      }

      template <typename T>
      Variable<T>* insertVar(const p_str& var, const VarType type)
      {
         p_varptrs<T>* vars;
         this->takeVarsPtr(vars);
         auto a = vars->insert(std::make_pair(var, std::make_unique<Variable<T>>(type)));
         return a.first->second.get();
      }

      p_varptrs<p_bool> bools;
      p_varptrs<p_tim> times;
      p_varptrs<p_per> periods;
      p_varptrs<p_str> strings;
      p_varptrs<p_num> numbers;
      p_varptrs<p_tlist> timeLists;
      p_varptrs<p_nlist> numLists;
      p_varptrs<p_list> lists;
   };


   struct UserVarsContext
   {
      VarsContext userVars;
   };


   typedef std::unique_ptr<UserVarsContext>        p_ucptr;
   
}

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

#include "var-context.h"
#include "../exception.h"
#include "var-runtime.h"


namespace uro::vars
{

VariablesContext::VariablesContext(Hashes& hsh, Variables& vars)
   : hashes(hsh), variables(vars) { }

void VariablesContext::setAttribute(const Token& tk, uro::_uro& uro)
{
   if (this->hashes.HASH_GROUP_ATTR.find(tk.value.word.h) ==
       this->hashes.HASH_GROUP_ATTR.end())
   {
      return;
   }

   const _size size = this->attrs.size();
   if (size == 0) {
      this->attributeException(tk, uro);
   }

   this->attrs[size - 1]->add(tk);
}

void VariablesContext::setCoreComAttribute(const _str& comName, const _int& line)
{
   const _size size = this->attrs.size();

   if (size == 0) {
      throw SyntaxError(str(L"the subject of command '", comName,
         L"' is undefined here"), line);
   }

   Attribute* last = this->attrs[size - 1];
   last->set(ATTR_EXISTS);
   last->set(ATTR_FULLNAME);
   last->set(ATTR_PATH);
}

void VariablesContext::setTimeComAttribute(const _str& comName, const _int& line)
{
   const _size size = this->attrs.size();

   if (size == 0) {
      throw SyntaxError(str(L"the subject of command '", comName,
         L" to' is undefined here"), line);
   }

   Attribute* last = this->attrs[size - 1];
   last->set(ATTR_EXISTS);
   last->set(ATTR_FULLNAME);
   last->set(ATTR_PATH);
   last->set(ATTR_ACCESS);
   last->set(ATTR_CREATION);
   last->set(ATTR_MODIFICATION);
   last->set(ATTR_CHANGE);
}

void VariablesContext::attributeException(const Token& tk, uro::_uro& uro)
{
   throw SyntaxError(str(L"the value of variable '", tk.getOriginString(uro),
      L"' is undefined here"), tk.line);
}

void VariablesContext::addAttribute(_attrptr& attr)
{
   this->attrs.push_back(attr.get());
}

void VariablesContext::retreatAttribute()
{
   this->attrs.pop_back();
}

_bool VariablesContext::anyAttribute()
{
   return !this->attrs.empty();
}

Attribute* VariablesContext::getLastAttribute()
{
   const _size len = this->attrs.size();
   return len == 0 ? nullptr : this->attrs.back();
}

void VariablesContext::markAttributesToRun()
{
   const _size len = this->attrs.size();
   for (_size i = 0; i < len; i++) {
      (*(this->attrs[i])).markToRun();
   }
}

void VariablesContext::addAggregate(comm::_aggrptr& aggr)
{
   this->aggrs.push_back(aggr.get());

   // when we enter a loop
   // variables of constant values from the past
   // no longer can be treated as constant values
   this->variables.makeAllNotConstant();
}

void VariablesContext::retreatAggregate()
{
   this->aggrs.pop_back();
}

_bool VariablesContext::anyAggregate()
{
   return !this->aggrs.empty();
}

comm::Aggregate* VariablesContext::getLastAggregate()
{
   const _size len = this->aggrs.size();
   return len == 0 ? nullptr : this->aggrs[len - 1];
}

}

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

#include "var-group.h"
#include "../exception.h"
#include "../hash.h"


std::vector<Attribute*> g_attrs;
std::vector<Aggregate*> g_aggrs;


void setAttribute(const Token& tk)
{
   if (HASH_GROUP_ATTR.find(tk.value.h1) == HASH_GROUP_ATTR.end()) {
      return;
   }

   const _size size = g_attrs.size();
   if (size == 0) {
      attributeException(tk);
   }

   g_attrs[size - 1]->add(tk);
}

void setCoreComAttribute(const _str& comName, const _int& line)
{
   const _size size = g_attrs.size();

   if (size == 0) {
      throw SyntaxException(L"the subject of command '" + comName
        + L"' is undefined here", line);
   }

   Attribute* last = g_attrs[size - 1];
   last->set(ATTR_EXISTS);
   last->set(ATTR_FULLNAME);
   last->set(ATTR_PATH);
}

void attributeException(const Token& tk)
{
   throw SyntaxException(L"the value of variable '" + tk.originString
      + L"' is undefined here", tk.line);
}

void addAttribute(Attribute* attr)
{
   g_attrs.push_back(attr);
}

void retreatAttribute()
{
   g_attrs.pop_back();
}

_boo anyAttribute()
{
   return !g_attrs.empty();
}

Attribute* getLastAttribute()
{
   const _size len = g_attrs.size();
   return len == 0 ? nullptr : g_attrs.back();
}

void markAttributesToRun()
{
   const _size len = g_attrs.size();
   for (_size i = 0; i < len; i++) {
      (*(g_attrs[i])).markToRun = true;
   }
}

void addAggregate(Aggregate* aggr)
{
   g_aggrs.push_back(aggr);
}

void retreatAggregate()
{
   g_aggrs.pop_back();
}

Aggregate* getLastAggregate()
{
   const _size len = g_aggrs.size();
   return len == 0 ? nullptr : g_aggrs[len - 1];
}

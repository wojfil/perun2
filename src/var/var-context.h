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

#ifndef VAR_GROUP_H_INCLUDED
#define VAR_GROUP_H_INCLUDED

#include "../attribute.h"
#include "../hash.h"
#include "../command/com-aggregate.h"
#include "../datatype/datatype.h"


namespace uro::vars
{

struct Variables;

struct VariablesContext
{
public:

   VariablesContext(Hashes& hsh, Variables& vars);

   void setAttribute(const Token& tk, uro::Uroboros& uro);
   void setCoreComAttribute(const _str& comName, const _int& line);
   void setTimeComAttribute(const _str& comName, const _int& line);
   void attributeException(const Token& tk, uro::Uroboros& uro);
   void addAttribute(Attribute* attr);
   void retreatAttribute();
   _bool anyAttribute();
   Attribute* getLastAttribute();
   void markAttributesToRun();

   void addAggregate(comm::Aggregate* aggr);
   void retreatAggregate();
   _bool anyAggregate();
   comm::Aggregate* getLastAggregate();

private:

   Hashes& hashes;
   Variables& variables;
   std::vector<Attribute*> attrs;
   std::vector<comm::Aggregate*> aggrs;

};

}

#endif // VAR_GROUP_H_INCLUDED

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

#ifndef COM_ARG_H_INCLUDED
#define COM_ARG_H_INCLUDED

#include "com.h"
#include "../attribute.h"
#include "../attr-memory.h"
#include "../datatype/datatype.h"
#include "../datatype/generator.h"
#include "com-aggregate.h"
#include "../uroboros.h"
#include "../patterns.h"


namespace uro::comm
{

struct IterationLoop : Command
{
public:
   IterationLoop(Command* com, Uroboros& uro)
      : uroboros(uro), command(com), hasMemory(false), hasAttribute(false),
        attrMemory(uro.vars.inner), inner(uro.vars.inner) { };

   IterationLoop(Command* com, Attribute* attr, const _bool& hasmem, Uroboros& uro)
      : uroboros(uro), command(com), attribute(attr), attrMemory(AttributeMemory(attr, uro.vars.inner)),
        hasMemory(hasmem), hasAttribute(true), inner(uro.vars.inner) { };

   IterationLoop(Command* com, Attribute* attr, Attribute* memAttr, const _bool& hasmem, Uroboros& uro)
      : uroboros(uro), command(com), attribute(attr), attrMemory(AttributeMemory(memAttr, uro.vars.inner)),
        hasMemory(hasmem), hasAttribute(true), inner(uro.vars.inner) { };


protected:
   Uroboros& uroboros;
   InnerVariables& inner;
   Command* command;
   Attribute* attribute;
   AttributeMemory attrMemory;
   const _bool hasMemory;
   const _bool hasAttribute;
   P_MEMORY_MEMBER;
};


struct CS_StringComArg : IterationLoop
{
public:
   CS_StringComArg(_genptr<_str>& str, Command* com, Uroboros& uro)
      : IterationLoop(com, uro), string(std::move(str)) { };

   CS_StringComArg(_genptr<_str>& str, Command* com, Attribute* attr, const _bool& hasmem, Uroboros& uro)
      : IterationLoop(com, attr, hasmem, uro), string(std::move(str)) { };

   CS_StringComArg(_genptr<_str>& str, Command* com, Attribute* attr, Attribute* memAttr, const _bool& hasmem, Uroboros& uro)
      : IterationLoop(com, attr, memAttr, hasmem, uro), string(std::move(str)) { };

   ~CS_StringComArg() 
   {
      delete command;
   };

   void run() override;

private:
   _genptr<_str> string;
};


struct CS_ListComArg : IterationLoop
{
public:
   CS_ListComArg(_genptr<_list>& li, Command* com, Uroboros& uro)
      : IterationLoop(com, uro), list(std::move(li)) { };

   CS_ListComArg(_genptr<_list>& li, Command* com, Attribute* attr, const _bool& hasmem, Uroboros& uro)
      : IterationLoop(com, attr, hasmem, uro), list(std::move(li)) { };

   CS_ListComArg(_genptr<_list>& li, Command* com, Attribute* attr, Attribute* memAttr, const _bool& hasmem, Uroboros& uro)
      : IterationLoop(com, attr, memAttr, hasmem, uro), list(std::move(li)) { };

   ~CS_ListComArg() 
   {
      delete command;
   };

   void run() override;

private:
   _genptr<_list> list;
};


struct CS_DefinitionComArg : IterationLoop
{
public:
   CS_DefinitionComArg(_defptr& def, Command* com, Uroboros& uro)
      : IterationLoop(com, uro), definition(std::move(def)) { };

   CS_DefinitionComArg(_defptr& def, Command* com, Attribute* attr, const _bool& hasmem, Uroboros& uro)
      : IterationLoop(com, attr, hasmem, uro), definition(std::move(def)) { };

   CS_DefinitionComArg(_defptr& def, Command* com, Attribute* attr, Attribute* memAttr, const _bool& hasmem, Uroboros& uro)
      : IterationLoop(com, attr, memAttr, hasmem, uro), definition(std::move(def)) { };

   ~CS_DefinitionComArg()
   {
      delete command;
   };

   void run() override;

private:
   _defptr definition;
};

}

#endif // COM_ARG_H_INCLUDED

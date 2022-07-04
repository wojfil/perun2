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

#ifndef COM-ARG_H_INCLUDED
#define COM-ARG_H_INCLUDED

#include "com.h"
#include "../attribute.h"
#include "../attr-memory.h"
#include "../datatype/datatype.h"
#include "../datatype/generator.h"
#include "aggregate.h"
#include "../uroboros.h"




struct IterationLoop : Command
{
public:
   IterationLoop(Command* com, Uroboros* uro)
      : uroboros(uro), command(com), hasMemory(false), hasAttribute(false),
        attrMemory(nullptr, nullptr), inner(&uro->vars.inner) { };// attrmem temp

   IterationLoop(Command* com, Attribute* attr, const _boo& hasmem, Uroboros* uro)
      : uroboros(uro), command(com), attribute(attr), attrMemory(AttributeMemory(attr, &uro->vars.inner)),
        hasMemory(hasmem), hasAttribute(true), inner(&uro->vars.inner) { };

   IterationLoop(Command* com, Attribute* attr, Attribute* memAttr, const _boo& hasmem, Uroboros* uro)
      : uroboros(uro), command(com), attribute(attr), attrMemory(AttributeMemory(memAttr, &uro->vars.inner)),
        hasMemory(hasmem), hasAttribute(true), inner(&uro->vars.inner) { };


protected:
   Uroboros* uroboros;
   InnerVariables* inner;
   Command* command;
   Attribute* attribute;
   AttributeMemory attrMemory;
   const _boo hasMemory;
   const _boo hasAttribute;

   _str prevThis;
   _num prevIndex;
};


struct CS_StringComArg : IterationLoop
{
public:
   CS_StringComArg(Generator<_str>* str, Command* com, Uroboros* uro)
      : IterationLoop(com, uro), string(str) { };

   CS_StringComArg(Generator<_str>* str, Command* com, Attribute* attr, const _boo& hasmem, Uroboros* uro)
      : IterationLoop(com, attr, hasmem, uro), string(str) { };

   CS_StringComArg(Generator<_str>* str, Command* com, Attribute* attr, Attribute* memAttr, const _boo& hasmem, Uroboros* uro)
      : IterationLoop(com, attr, memAttr, hasmem, uro), string(str) { };

   ~CS_StringComArg() {
      delete string;
      delete command;
   };

   void run() override;

private:
   Generator<_str>* string;
};


struct CS_ListComArg : IterationLoop
{
public:
   CS_ListComArg(Generator<_list>* li, Command* com, Uroboros* uro)
      : IterationLoop(com, uro), list(li) { };

   CS_ListComArg(Generator<_list>* li, Command* com, Attribute* attr, const _boo& hasmem, Uroboros* uro)
      : IterationLoop(com, attr, hasmem, uro), list(li) { };

   CS_ListComArg(Generator<_list>* li, Command* com, Attribute* attr, Attribute* memAttr, const _boo& hasmem, Uroboros* uro)
      : IterationLoop(com, attr, memAttr, hasmem, uro), list(li) { };

   ~CS_ListComArg() {
      delete list;
      delete command;
   };

   void run() override;

private:
   Generator<_list>* list;
};


struct CS_DefinitionComArg : IterationLoop
{
public:
   CS_DefinitionComArg(_def* def, Command* com, Uroboros* uro)
      : IterationLoop(com, uro), definition(def) { };

   CS_DefinitionComArg(_def* def, Command* com, Attribute* attr, const _boo& hasmem, Uroboros* uro)
      : IterationLoop(com, attr, hasmem, uro), definition(def) { };

   CS_DefinitionComArg(_def* def, Command* com, Attribute* attr, Attribute* memAttr, const _boo& hasmem, Uroboros* uro)
      : IterationLoop(com, attr, memAttr, hasmem, uro), definition(def) { };

   ~CS_DefinitionComArg() {
      delete definition;
      delete command;
   };

   void run() override;

private:
   _def* definition;
};


#endif // COM-ARG_H_INCLUDED

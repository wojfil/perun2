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

#define P_IL_ARGS_1 _comptr& com, _uro& uro
#define P_IL_ARGS_2 _comptr& com, _attrptr& attr, const _bool& hasmem, _uro& uro
#define P_IL_ARGS_3 _comptr& com, _attrptr& attr, _attrptr& memAttr, const _bool& hasmem, _uro& uro


struct IterationLoop : Command
{
public:
   IterationLoop(P_IL_ARGS_1);
   IterationLoop(P_IL_ARGS_2);
   IterationLoop(P_IL_ARGS_3);

protected:
   _uro& uroboros;
   InnerVariables& inner;
   _comptr command;
   _attrptr attribute;
   AttributeMemory attrMemory;
   const _bool hasMemory;
   const _bool hasAttribute;
   P_MEMORY_MEMBER;
};


struct CS_StringComArg : IterationLoop
{
public:
   CS_StringComArg(_genptr<_str>& str, P_IL_ARGS_1);
   CS_StringComArg(_genptr<_str>& str, P_IL_ARGS_2);
   CS_StringComArg(_genptr<_str>& str, P_IL_ARGS_3);
   void run() override;

private:
   _genptr<_str> string;
};


struct CS_ListComArg : IterationLoop
{
public:
   CS_ListComArg(_genptr<_list>& li, P_IL_ARGS_1);
   CS_ListComArg(_genptr<_list>& li, P_IL_ARGS_2);
   CS_ListComArg(_genptr<_list>& li, P_IL_ARGS_3);
   void run() override;

private:
   _genptr<_list> list;
};


struct CS_DefinitionComArg : IterationLoop
{
public:
   CS_DefinitionComArg(_defptr& def, P_IL_ARGS_1);
   CS_DefinitionComArg(_defptr& def, P_IL_ARGS_2);
   CS_DefinitionComArg(_defptr& def, P_IL_ARGS_3);
   void run() override;

private:
   _defptr definition;
};

}

#endif // COM_ARG_H_INCLUDED

/*
    This file is part of Perun2.
    Perun2 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    Peruns2 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with Perun2. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef COM_ARG_H_INCLUDED
#define COM_ARG_H_INCLUDED

#include "com.h"
#include "../attribute.h"
#include "../datatype/datatype.h"
#include "../datatype/generator.h"
#include "com-aggregate.h"
#include "../perun2.h"
#include "../datatype/patterns.h"


namespace perun2::comm
{

// iterate over some strings
// and treat them like names of files or their paths
// meanwhile, read their attributes
struct IterationLoop : Command
{
public:
   IterationLoop(_comptr& com, _fcptr& ctx, _p2& p2);

protected:
   _p2& perun2;
   _comptr command;
   _fcptr context;
};


struct CS_StringComArg : IterationLoop
{
public:
   CS_StringComArg(_genptr<_str>& str, _comptr& com, _fcptr& ctx, _p2& p2);
   void run() override;

private:
   _genptr<_str> string;
};


struct CS_ListComArg : IterationLoop
{
public:
   CS_ListComArg(_genptr<_list>& li, _comptr& com, _fcptr& ctx, _p2& p2);
   void run() override;

private:
   _genptr<_list> list;
};


struct CS_DefinitionComArg : IterationLoop
{
public:
   CS_DefinitionComArg(_defptr& def, _comptr& com, _fcptr& ctx, _p2& p2);
   void run() override;

private:
   _defptr definition;
};


}

#endif // COM_ARG_H_INCLUDED

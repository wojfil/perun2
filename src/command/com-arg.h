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
   IterationLoop(p_comptr& com, p_fcptr& ctx, Perun2Process& p2);

protected:
   Perun2Process& perun2;
   p_comptr command;
   p_fcptr context;
};


struct CS_StringComArg : IterationLoop
{
public:
   CS_StringComArg(p_genptr<p_str>& str, p_comptr& com, p_fcptr& ctx, Perun2Process& p2);
   void run() override;

private:
   p_genptr<p_str> string;
};


struct CS_ListComArg : IterationLoop
{
public:
   CS_ListComArg(p_genptr<p_list>& li, p_comptr& com, p_fcptr& ctx, Perun2Process& p2);
   void run() override;

private:
   p_genptr<p_list> list;
};


struct CS_DefinitionComArg : IterationLoop
{
public:
   CS_DefinitionComArg(p_defptr& def, p_comptr& com, p_fcptr& ctx, Perun2Process& p2);
   void run() override;

private:
   p_defptr definition;
};


}

#endif // COM_ARG_H_INCLUDED

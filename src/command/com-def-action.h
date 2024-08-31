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

#include "com.h"
#include "../datatype/datatype.h"
#include "../datatype/definition.h"
#include "../datatype/definition-action.h"
#include <stack>


namespace perun2
{
   struct Perun2Process;
   struct LocationContext;
}

namespace perun2::comm
{


struct SelectDefAction : DefinitionAction
{
public:
   SelectDefAction(Perun2Process& p2);
   void reset() override;
   void onDirectoryEnter() override;
   void onDirectoryExit() override;
   void add(const p_str& value) override;
   void finish() override;

private:
   std::stack<p_set> values;
   Perun2Process& perun2;
   LocationContext& locationContext;
};


struct C_SelectAsAction : Command
{
public:
   C_SelectAsAction(p_defptr& def, DefinitionAction& act, Perun2Process& p2);
   void run() override;

private:
   p_defptr definition;
   DefinitionAction& action;
   Perun2Process& perun2;
};


}

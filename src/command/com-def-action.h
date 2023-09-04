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

#ifndef COM_DEF_ACTION_H_INCLUDED
#define COM_DEF_ACTION_H_INCLUDED

#include "com.h"
#include "../datatype/datatype.h"
#include "../datatype/definition.h"
#include "../datatype/definition-action.h"
#include <stack>


namespace perun2
{
   struct p_perun2;
   struct LocationContext;
}

namespace perun2::comm
{


struct SelectDefAction : DefinitionAction
{
public:
   SelectDefAction(p_perun2& p2);
   void reset() override;
   void onDirectoryEnter() override;
   void onDirectoryExit() override;
   void add(const p_str& value) override;

private:
   std::vector<p_set> values;
   p_perun2& perun2;
   LocationContext& locationContext;
};


struct C_SelectAsAction : Command
{
public:
   C_SelectAsAction(p_defptr& def, DefinitionAction& act, p_perun2& p2);
   void run() override;

private:
   p_defptr definition;
   DefinitionAction& action;
   p_perun2& perun2;
};


}

#endif // COM_DEF_ACTION_H_INCLUDED

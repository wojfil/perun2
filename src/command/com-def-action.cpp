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

#include "com-def-action.h"
#include "com-core-aggr.h"
#include "../perun2.h"
#include "../os.h"


namespace perun2::comm
{


SelectDefAction::SelectDefAction(p_perun2& p2)
   : perun2(p2), locationContext(*p2.contexts.getLocationContext()) { };


void SelectDefAction::reset()
{
   while(! this->values.empty()) {
      this->values.pop();
   }
}


void SelectDefAction::onDirectoryEnter()
{
   this->values.emplace();
}


void SelectDefAction::onDirectoryExit()
{
   if (this->values.top().empty()) {
      this->values.pop();
      return;
   }

   const p_str root = os_parent(*this->values.top().begin());
   const p_bool success = os_select(root, this->values.top());

   if (success) {
      for (const p_str& element : this->values.top()) {
         logSelectSuccess(this->perun2, element);
      }
   }
   else {
      for (const p_str& element : this->values.top()) {
         logSelectError(this->perun2, element);
      }
   }

   this->perun2.contexts.success->value = success;
   this->values.pop();
}


void SelectDefAction::add(const p_str& value)
{
   const p_str s = os_join(this->locationContext.location->value, value);
   this->values.top().insert(s);
}


C_SelectAsAction::C_SelectAsAction(p_defptr& def, DefinitionAction& act, p_perun2& p2) 
   : definition(std::move(def)), action(act), perun2(p2) { };


void C_SelectAsAction::run()
{
    this->action.reset();

   while (this->definition->hasNext()) {
      if (this->perun2.isNotRunning()) {
         this->definition->reset();
         this->action.reset();
         return;
      }     

      this->action.add(this->definition->getValue());
   }
}


}

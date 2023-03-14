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

#include "terminator.h"
#include "perun2.h"


namespace perun2
{

_bool Terminator::initialized = false;
std::unordered_set<_p2*> Terminator::pointers = std::unordered_set<_p2*>();

void Terminator::addPtr(_p2* p2)
{
   if (!initialized) {
      initialized = true;
      SetConsoleCtrlHandler(HandlerRoutine, TRUE);
   }

   pointers.insert(p2);
}

void Terminator::removePtr(_p2* p2)
{
   pointers.erase(p2);
}

_int Terminator::HandlerRoutine(_ulong dwCtrlType)
{
   switch (dwCtrlType) {
      case CTRL_C_EVENT: {
         for (_p2* perun2 : pointers) {
            perun2->terminate();
         }
         return TRUE;
      }
      default: {
         return FALSE;
      }
   }
}

}

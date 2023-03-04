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

#include "terminator.h"
#include "uroboros.h"


namespace uro
{

_bool Terminator::initialized = false;
std::unordered_set<_uro*> Terminator::pointers = std::unordered_set<_uro*>();

void Terminator::addPtr(_uro* uro)
{
   if (!initialized) {
      initialized = true;
      SetConsoleCtrlHandler(HandlerRoutine, TRUE);
   }

   pointers.insert(uro);
}

void Terminator::removePtr(_uro* uro)
{
   pointers.erase(uro);
}

_int Terminator::HandlerRoutine(_ulong dwCtrlType)
{
   switch (dwCtrlType) {
      case CTRL_C_EVENT: {
         for (_uro* uro : pointers) {
            uro->terminate();
         }
         return TRUE;
      }
      default: {
         return FALSE;
      }
   }
}

}

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

p_bool Terminator::initialized = false;
std::unordered_set<p_perun2*> Terminator::processes;


void Terminator::init()
{
   if (!initialized) {
      initialized = true;
      SetConsoleCtrlHandler(HandlerRoutine, TRUE);
   }
}

void Terminator::addPtr(p_perun2* p2)
{
   processes.insert(p2);
}

void Terminator::removePtr(p_perun2* p2)
{
   processes.erase(p2);
}

p_int Terminator::HandlerRoutine(p_ulong dwCtrlType)
{
   switch (dwCtrlType) {
      case CTRL_C_EVENT: {
         for (p_perun2* p : processes) {
            p->terminate();
         }
         return TRUE;
      }
      default: {
         return FALSE;
      }
   }
}

void initTerminator()
{
   Terminator::init();
}

}

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

#include "terminator.h"
#include "uroboros.h"


namespace uro
{

Uroboros* Terminator::uroboros = nullptr;

Terminator::Terminator(Uroboros* uro)
{
   Terminator::uroboros = uro;
   SetConsoleCtrlHandler(HandlerRoutine, TRUE);
};

_int Terminator::HandlerRoutine(_ulong dwCtrlType)
{
  switch (dwCtrlType) {
    case CTRL_C_EVENT:
      Terminator::uroboros->state = State::s_Exit;
      if (Terminator::uroboros->sideProcess.running) {
         Terminator::uroboros->sideProcess.running = false;
         TerminateProcess(Terminator::uroboros->sideProcess.info.hProcess, 0);
      }
      return TRUE;
    default:
      return FALSE;
    }
}

}

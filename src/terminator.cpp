#include "terminator.h"
#include "uroboros.h"


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
      Terminator::uroboros->running = false;
      if (Terminator::uroboros->sideProcess.running) {
         Terminator::uroboros->sideProcess.running = false;
         TerminateProcess(Terminator::uroboros->sideProcess.info.hProcess, 0);
      }
      return TRUE;
    default:
      return FALSE;
    }
}
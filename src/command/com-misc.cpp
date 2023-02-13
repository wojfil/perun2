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

#include "com-misc.h"
#include <chrono>
#include <thread>
#include <algorithm>
#include "../os.h"
#include "../print.h"
#include "../uroboros.h"
#include "com-core.h"
#include <sstream>


namespace uro::comm
{

void C_PrintSingle::run()
{
   print(this->uroboros, this->value->getValue());
}

void C_PrintList::run()
{
   const _list list = this->value->getValue();
   const _size length = list.size();
   for (_size i = 0; this->uroboros.state == State::s_Running && i < length; i++) {
      print(this->uroboros, list[i]);
   }
}

void C_PrintDefinition::run()
{
   while (this->value->hasNext()) {
      if (!this->uroboros.state == State::s_Running) {
         this->value->reset();
         break;
      }
      print(this->uroboros, this->value->getValue());
   }
}

void C_PrintThis::run()
{
   print(this->uroboros, this->context.this_->value);
}

void C_SleepPeriod::run()
{
   os_sleepForMs(NINT_THOUSAND * this->value->getValue().toSeconds(), this->uroboros);
}

void C_SleepMs::run()
{
   os_sleepForMs(this->value->getValue().toInt(), this->uroboros);
}

void C_Break::run()
{
   this->uroboros.state = State::s_Break;
}

void C_Continue::run()
{
   this->uroboros.state = State::s_Continue;
}

void C_Exit::run()
{
   this->uroboros.state = State::s_Exit;
}

void C_Error::run()
{
   this->uroboros.state = State::s_Exit;
   this->uroboros.exitCode = EXITCODE_RUNTIME_ERROR;
}

void C_ErrorWithExitCode::run()
{
   this->uroboros.state = State::s_Exit;
   const _exitint code = static_cast<_exitint>(this->exitCode->getValue().toInt());
   this->uroboros.exitCode = (code == EXITCODE_OK)
      ? EXITCODE_RUNTIME_ERROR
      : code;
}

RunBase::RunBase(_uro& uro)
   : fileCtxs(uro.contextes.getFileContexts()), 
     locationCtx(uro.contextes.getLocationContext()),
     uro2Base(str(os_quoteEmbraced(os_uroborosPath()), 
      STRING_SPACE, STRING_MINUS, toStr(CHAR_FLAG_SILENT), STRING_SPACE)) { };

void RunBase::reloadContextes()
{
   for (FileContext*& fc : this->fileCtxs) {
      if (fc != nullptr) {
         fc->reloadData();
      }
   }
}

_str RunBase::getLocation()
{
   return this->locationCtx->location->value;
}

void C_Run::run()
{
   _str command = this->value->getValue();
   os_rawTrim(command);

   if (command.empty()) {
      commandLog(this->uroboros, L"Failed to run an empty command");
      this->uroboros.contextes.success->value = false;
      return;
   }

   const _str loc = this->getLocation();
   const _bool s = os_run(command, loc, this->uroboros);
   this->uroboros.contextes.success->value = s;

   if (s) {
      commandLog(this->uroboros, L"Run '", command, L"'");
   }
   else {
      commandLog(this->uroboros, L"Failed to run '", command, L"'");
   }

   this->reloadContextes();
}

void C_RunWith::run()
{
   _str base = value->getValue();
   os_rawTrim(base);

   if (!this->context->v_exists->value || base.empty()) {
      commandLog(this->uroboros, L"Failed to run ", getCCName(this->context->trimmed), L" with '", base, L"'");
      this->uroboros.contextes.success->value = false;
      return;
   }

   const _str com = str(base, L" ", os_quoteEmbraced(this->context->trimmed));
   const _str loc = this->getLocation();
   const _bool s = os_run(com, loc, this->uroboros);
   this->uroboros.contextes.success->value = s;

   if (s) {
      commandLog(this->uroboros, L"Run ", getCCName(this->context->trimmed), L" with '", base, L"'");
   }
   else {
      commandLog(this->uroboros, L"Failed to run ", getCCName(this->context->trimmed), L" with '", base, L"'");
   }

   this->reloadContextes();
}

void C_RunWithWithString::run()
{
   _str base = value->getValue();
   os_rawTrim(base);

   if (!this->context->v_exists->value || base.empty()) {
      commandLog(this->uroboros, L"Failed to run ", getCCName(this->context->trimmed), L" with '", base, L"'");
      this->uroboros.contextes.success->value = false;
      return;
   }

   const _str rawArg = argument->getValue();
   const _str arg = os_makeArg(rawArg);
   const _str com = str(base, L" ", os_quoteEmbraced(this->context->trimmed), L" ", arg);

   const _str loc = this->getLocation();
   const _bool s = os_run(com, loc, this->uroboros);
   this->uroboros.contextes.success->value = s;

   if (s) {
      commandLog(this->uroboros, L"Run ", getCCName(this->context->trimmed), L" with '", base, L"' with '", rawArg, L"'");
   }
   else {
      commandLog(this->uroboros, L"Failed to run ", getCCName(this->context->trimmed), L" with '", base, L"' with '", rawArg, L"'");
   }

   this->reloadContextes();
}

void C_RunWithWith::run()
{
   _str base = value->getValue();
   os_rawTrim(base);

   if (!this->context->v_exists->value || base.empty()) {
      commandLog(this->uroboros, L"Failed to run ", getCCName(this->context->trimmed), L" with '", base, L"'");
      this->uroboros.contextes.success->value = false;
      return;
   }

   const _list rawArgs = arguments->getValue();
   const _size len = rawArgs.size();

   if (len == 0) {
      const _str com = str(base, L" ", os_quoteEmbraced(this->context->trimmed));
      const _str loc = this->getLocation();
      const _bool s = os_run(com, loc, this->uroboros);
      this->uroboros.contextes.success->value = s;

      if (s) {
         commandLog(this->uroboros, L"Run ", getCCName(this->context->trimmed), L" with '", base, L"'");
      }
      else {
         commandLog(this->uroboros, L"Failed to run ", getCCName(this->context->trimmed), L" with '", base, L"'");
      }
   }
   else {
      _stream comStream;
      _stream logStream;
      const _str& first = rawArgs[0];
      logStream << str(getCCName(this->context->trimmed), L" with '", base, L"' with '", first, L"'");
      comStream << str(base, L" ", os_quoteEmbraced(this->context->trimmed), L" ", os_makeArg(first));

      for (_size i = 1; i < len; i++) {
         const _str& a = rawArgs[i];
         logStream << str(L", '", a, L"'");
         comStream << str(L" ", os_makeArg(a));
      }

      const _str com = comStream.str();
      const _str loc = this->getLocation();
      const _bool s = os_run(com, loc, this->uroboros);
      this->uroboros.contextes.success->value = s;

      if (s) {
         commandLog(this->uroboros, L"Run ", logStream.str());
      }
      else {
         commandLog(this->uroboros, L"Failed to run ", logStream.str());
      }
   }

   this->reloadContextes();
}

void C_RunWithUroboros2::run()
{
   if (!this->context->v_exists->value) {
      commandLog(this->uroboros, L"Failed to run ", getCCName(this->context->trimmed), L" with Uroboros2");
      this->uroboros.contextes.success->value = false;
      return;
   }

   const _str com = str(this->uro2Base, os_quoteEmbraced(this->context->trimmed));
   const _str loc = this->getLocation();
   const _bool s = os_run(com, loc, this->uroboros);
   this->uroboros.contextes.success->value = s;

   if (s) {
      commandLog(this->uroboros, L"Run ", getCCName(this->context->trimmed), L" with Uroboros2");
   }
   else {
      commandLog(this->uroboros, L"Failed to run ", getCCName(this->context->trimmed), L" with Uroboros2");
   }

   this->reloadContextes();
}

void C_RunWithUroboros2WithString::run()
{
   if (!this->context->v_exists->value) {
      commandLog(this->uroboros, L"Failed to run ", getCCName(this->context->trimmed), L" with Uroboros2");
      this->uroboros.contextes.success->value = false;
      return;
   }

   const _str rawArg = argument->getValue();
   const _str arg = os_makeArg(rawArg);
   const _str com = str(this->uro2Base, os_quoteEmbraced(this->context->trimmed), L" ", arg);

   const _str loc = this->getLocation();
   const _bool s = os_run(com, loc, this->uroboros);
   this->uroboros.contextes.success->value = s;

   if (s) {
      commandLog(this->uroboros, L"Run ", getCCName(this->context->trimmed), L" with Uroboros2 with '", rawArg, L"'");
   }
   else {
      commandLog(this->uroboros, L"Failed to run ", getCCName(this->context->trimmed), L" with Uroboros2 with '", rawArg, L"'");
   }

   this->reloadContextes();
}

void C_RunWithUroboros2With::run()
{
   if (!this->context->v_exists->value) {
      commandLog(this->uroboros, L"Failed to run ", getCCName(this->context->trimmed), L" with Uroboros2");
      this->uroboros.contextes.success->value = false;
      return;
   }

   const _list rawArgs = arguments->getValue();
   const _size len = rawArgs.size();

   if (len == 0) {
      const _str com = str(this->uro2Base, os_quoteEmbraced(this->context->trimmed));
      const _str loc = this->getLocation();
      const _bool s = os_run(com, loc, this->uroboros);
      this->uroboros.contextes.success->value = s;

      if (s) {
         commandLog(this->uroboros, L"Run ", getCCName(this->context->trimmed), L" with Uroboros2");
      }
      else {
         commandLog(this->uroboros, L"Failed to run ", getCCName(this->context->trimmed), L" with Uroboros2");
      }
   }
   else {
      _stream comStream;
      _stream logStream;
      const _str& first = rawArgs[0];
      logStream << str(getCCName(this->context->trimmed), L" with Uroboros2 with '", first, L"'");
      comStream << str(this->uro2Base, os_quoteEmbraced(this->context->trimmed), L" ", os_makeArg(first));

      for (_size i = 1; i < len; i++) {
         const _str& a = rawArgs[i];
         logStream << str(L", '", a, L"'");
         comStream << str(L" ", os_makeArg(a));
      }

      const _str com = comStream.str();
      const _str loc = this->getLocation();
      const _bool s = os_run(com, loc, this->uroboros);
      this->uroboros.contextes.success->value = s;

      if (s) {
         commandLog(this->uroboros, L"Run ", logStream.str());
      }
      else {
         commandLog(this->uroboros, L"Failed to run ", logStream.str());
      }
   }

   this->reloadContextes();
}

}

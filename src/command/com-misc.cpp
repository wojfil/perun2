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

#include "com-misc.h"
#include <chrono>
#include <thread>
#include <algorithm>
#include "../os.h"
#include "../perun2.h"
#include "com-core.h"
#include <sstream>


namespace perun2::comm
{

void C_PrintSingle::run()
{
   this->perun2.logger.print(this->value->getValue());
}

void C_PrintList::run()
{
   const _list list = this->value->getValue();
   const _size length = list.size();
   for (_size i = 0; this->perun2.state == State::s_Running && i < length; i++) {
      this->perun2.logger.print(list[i]);
   }
}

void C_PrintDefinition::run()
{
   while (this->value->hasNext()) {
      if (!this->perun2.state == State::s_Running) {
         this->value->reset();
         break;
      }
      this->perun2.logger.print(this->value->getValue());
   }
}

void C_PrintThis::run()
{
   this->perun2.logger.print(this->context.this_->value);
}

void C_SleepPeriod::run()
{
   os_sleepForMs(NINT_THOUSAND * this->value->getValue().toSeconds(), this->perun2);
}

void C_SleepMs::run()
{
   os_sleepForMs(this->value->getValue().toInt(), this->perun2);
}

void C_Break::run()
{
   this->perun2.state = State::s_Break;
}

void C_Continue::run()
{
   this->perun2.state = State::s_Continue;
}

void C_Exit::run()
{
   this->perun2.state = State::s_Exit;
}

void C_Error::run()
{
   this->perun2.state = State::s_Exit;
   this->perun2.exitCode = EXITCODE_RUNTIME_ERROR;
}

void C_ErrorWithExitCode::run()
{
   this->perun2.state = State::s_Exit;
   const _exitint code = static_cast<_exitint>(this->exitCode->getValue().toInt());
   this->perun2.exitCode = (code == EXITCODE_OK)
      ? EXITCODE_RUNTIME_ERROR
      : code;
}

RunBase::RunBase(_p2& p2)
   : perun2(p2),
     fileCtxs(p2.contexts.getFileContexts()), 
     locationCtx(p2.contexts.getLocationContext()),
     uro2Base(str(os_quoteEmbraced(os_executablePath()), 
      STRING_CHAR_SPACE, STRING_CHAR_MINUS, toStr(CHAR_FLAG_SILENT), STRING_CHAR_SPACE)) { };

void RunBase::reloadContexts()
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
      this->perun2.logger.log(L"Failed to run an empty command");
      this->perun2.contexts.success->value = false;
      return;
   }

   const _str loc = this->getLocation();
   const _bool s = os_run(command, loc, this->perun2);
   this->perun2.contexts.success->value = s;

   if (s) {
      this->perun2.logger.log(L"Run '", command, L"'");
   }
   else {
      this->perun2.logger.log(L"Failed to run '", command, L"'");
   }

   this->reloadContexts();
}

void C_RunWith::run()
{
   _str base = value->getValue();
   os_rawTrim(base);

   if (!this->context->v_exists->value || base.empty()) {
      this->perun2.logger.log(L"Failed to run ", getCCName(this->context->trimmed), L" with '", base, L"'");
      this->perun2.contexts.success->value = false;
      return;
   }

   const _str com = str(base, L" ", os_quoteEmbraced(this->context->trimmed));
   const _str loc = this->getLocation();
   const _bool s = os_run(com, loc, this->perun2);
   this->perun2.contexts.success->value = s;

   if (s) {
      this->perun2.logger.log(L"Run ", getCCName(this->context->trimmed), L" with '", base, L"'");
   }
   else {
      this->perun2.logger.log(L"Failed to run ", getCCName(this->context->trimmed), L" with '", base, L"'");
   }

   this->reloadContexts();
}

void C_RunWithWithString::run()
{
   _str base = value->getValue();
   os_rawTrim(base);

   if (!this->context->v_exists->value || base.empty()) {
      this->perun2.logger.log(L"Failed to run ", getCCName(this->context->trimmed), L" with '", base, L"'");
      this->perun2.contexts.success->value = false;
      return;
   }

   const _str rawArg = argument->getValue();
   const _str arg = os_makeArg(rawArg);
   const _str com = str(base, L" ", os_quoteEmbraced(this->context->trimmed), L" ", arg);

   const _str loc = this->getLocation();
   const _bool s = os_run(com, loc, this->perun2);
   this->perun2.contexts.success->value = s;

   if (s) {
      this->perun2.logger.log(L"Run ", getCCName(this->context->trimmed), L" with '", base, L"' with '", rawArg, L"'");
   }
   else {
      this->perun2.logger.log(L"Failed to run ", getCCName(this->context->trimmed), L" with '", base, L"' with '", rawArg, L"'");
   }

   this->reloadContexts();
}

void C_RunWithWith::run()
{
   _str base = value->getValue();
   os_rawTrim(base);

   if (!this->context->v_exists->value || base.empty()) {
      this->perun2.logger.log(L"Failed to run ", getCCName(this->context->trimmed), L" with '", base, L"'");
      this->perun2.contexts.success->value = false;
      return;
   }

   const _list rawArgs = arguments->getValue();
   const _size len = rawArgs.size();

   if (len == 0) {
      const _str com = str(base, L" ", os_quoteEmbraced(this->context->trimmed));
      const _str loc = this->getLocation();
      const _bool s = os_run(com, loc, this->perun2);
      this->perun2.contexts.success->value = s;

      if (s) {
         this->perun2.logger.log(L"Run ", getCCName(this->context->trimmed), L" with '", base, L"'");
      }
      else {
         this->perun2.logger.log(L"Failed to run ", getCCName(this->context->trimmed), L" with '", base, L"'");
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
      const _bool s = os_run(com, loc, this->perun2);
      this->perun2.contexts.success->value = s;

      if (s) {
         this->perun2.logger.log(L"Run ", logStream.str());
      }
      else {
         this->perun2.logger.log(L"Failed to run ", logStream.str());
      }
   }

   this->reloadContexts();
}

void C_RunWithPerun2::run()
{
   if (!this->context->v_exists->value) {
      this->perun2.logger.log(L"Failed to run ", getCCName(this->context->trimmed), L" with Perun2");
      this->perun2.contexts.success->value = false;
      return;
   }

   const _str com = str(this->uro2Base, os_quoteEmbraced(this->context->trimmed));
   const _str loc = this->getLocation();
   const _bool s = os_run(com, loc, this->perun2);
   this->perun2.contexts.success->value = s;

   if (s) {
      this->perun2.logger.log(L"Run ", getCCName(this->context->trimmed), L" with Perun2");
   }
   else {
      this->perun2.logger.log(L"Failed to run ", getCCName(this->context->trimmed), L" with Perun2");
   }

   this->reloadContexts();
}

void C_RunWithPerun2WithString::run()
{
   if (!this->context->v_exists->value) {
      this->perun2.logger.log(L"Failed to run ", getCCName(this->context->trimmed), L" with Perun2");
      this->perun2.contexts.success->value = false;
      return;
   }

   const _str rawArg = argument->getValue();
   const _str arg = os_makeArg(rawArg);
   const _str com = str(this->uro2Base, os_quoteEmbraced(this->context->trimmed), L" ", arg);

   const _str loc = this->getLocation();
   const _bool s = os_run(com, loc, this->perun2);
   this->perun2.contexts.success->value = s;

   if (s) {
      this->perun2.logger.log(L"Run ", getCCName(this->context->trimmed), L" with Perun2 with '", rawArg, L"'");
   }
   else {
      this->perun2.logger.log(L"Failed to run ", getCCName(this->context->trimmed), L" with Perun2 with '", rawArg, L"'");
   }

   this->reloadContexts();
}

void C_RunWithPerun2With::run()
{
   if (!this->context->v_exists->value) {
      this->perun2.logger.log(L"Failed to run ", getCCName(this->context->trimmed), L" with Perun2");
      this->perun2.contexts.success->value = false;
      return;
   }

   const _list rawArgs = arguments->getValue();
   const _size len = rawArgs.size();

   if (len == 0) {
      const _str com = str(this->uro2Base, os_quoteEmbraced(this->context->trimmed));
      const _str loc = this->getLocation();
      const _bool s = os_run(com, loc, this->perun2);
      this->perun2.contexts.success->value = s;

      if (s) {
         this->perun2.logger.log(L"Run ", getCCName(this->context->trimmed), L" with Perun2");
      }
      else {
         this->perun2.logger.log(L"Failed to run ", getCCName(this->context->trimmed), L" with Perun2");
      }
   }
   else {
      _stream comStream;
      _stream logStream;
      const _str& first = rawArgs[0];
      logStream << str(getCCName(this->context->trimmed), L" with Perun2 with '", first, L"'");
      comStream << str(this->uro2Base, os_quoteEmbraced(this->context->trimmed), L" ", os_makeArg(first));

      for (_size i = 1; i < len; i++) {
         const _str& a = rawArgs[i];
         logStream << str(L", '", a, L"'");
         comStream << str(L" ", os_makeArg(a));
      }

      const _str com = comStream.str();
      const _str loc = this->getLocation();
      const _bool s = os_run(com, loc, this->perun2);
      this->perun2.contexts.success->value = s;

      if (s) {
         this->perun2.logger.log(L"Run ", logStream.str());
      }
      else {
         this->perun2.logger.log(L"Failed to run ", logStream.str());
      }
   }

   this->reloadContexts();
}

}

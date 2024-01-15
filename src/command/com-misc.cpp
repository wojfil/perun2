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

#include "com-misc.h"
#include <chrono>
#include <thread>
#include <algorithm>
#include "../os/os.h"
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
   const p_list list = this->value->getValue();
   const p_size length = list.size();
   for (p_size i = 0; this->perun2.isRunning() && i < length; i++) {
      this->perun2.logger.print(list[i]);
   }
}

void C_PrintDefinition::run()
{
   while (this->value->hasNext()) {
      if (!this->perun2.isRunning()) {
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
   const p_num n = this->value->getValue();
   if (n.state == NumberState::NaN) {
      return;
   }

   os_sleepForMs(n.toInt(), this->perun2);
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
   const p_num n = this->exitCode->getValue();

   if (n.state == NumberState::NaN) {
      this->perun2.exitCode = EXITCODE_RUNTIME_ERROR;
      return;
   }

   const int code = static_cast<int>(n.toInt());
   this->perun2.exitCode = (code == EXITCODE_OK)
      ? EXITCODE_RUNTIME_ERROR
      : code;
}

RunBase::RunBase(p_perun2& p2)
   : perun2(p2),
     locationCtx(p2.contexts.getLocationContext()) { };


p_str RunBase::getLocation()
{
   return this->locationCtx->location->value;
}

void C_Run::run()
{
   p_str command = os_softTrim(this->value->getValue());

   if (command.empty()) {
      this->perun2.logger.log(L"Failed to run an empty command");
      this->perun2.contexts.success->value = false;
      return;
   }

   const p_str loc = this->getLocation();
   const p_bool s = os_run(command, loc, this->perun2);
   this->perun2.contexts.success->value = s;

   if (s) {
      this->perun2.logger.log(L"Run '", command, L"'");
   }
   else {
      this->perun2.logger.log(L"Failed to run '", command, L"'");
   }
}

void C_RunWith::run()
{
   p_str base = os_softTrim(value->getValue());

   if (!this->context->v_exists->value || base.empty()) {
      this->perun2.logger.log(L"Failed to run ", getCCName(this->context->trimmed), L" with '", base, L"'");
      this->perun2.contexts.success->value = false;
      return;
   }

   const p_str com = str(base, CHAR_SPACE, os_quoteEmbraced(this->context->trimmed));
   const p_str loc = this->getLocation();
   const p_bool s = os_run(com, loc, this->perun2);
   this->perun2.contexts.success->value = s;

   if (s) {
      this->perun2.logger.log(L"Run ", getCCName(this->context->trimmed), L" with '", base, L"'");
   }
   else {
      this->perun2.logger.log(L"Failed to run ", getCCName(this->context->trimmed), L" with '", base, L"'");
   }
}

void C_RunWithWithString::run()
{
   p_str base = os_softTrim(value->getValue());

   if (!this->context->v_exists->value || base.empty()) {
      this->perun2.logger.log(L"Failed to run ", getCCName(this->context->trimmed), L" with '", base, L"'");
      this->perun2.contexts.success->value = false;
      return;
   }

   const p_str rawArg = argument->getValue();
   const p_str arg = os_makeArg(rawArg);
   const p_str com = str(base, CHAR_SPACE, os_quoteEmbraced(this->context->trimmed), CHAR_SPACE, arg);

   const p_str loc = this->getLocation();
   const p_bool s = os_run(com, loc, this->perun2);
   this->perun2.contexts.success->value = s;

   if (s) {
      this->perun2.logger.log(L"Run ", getCCName(this->context->trimmed), L" with '", base, L"' with '", rawArg, L"'");
   }
   else {
      this->perun2.logger.log(L"Failed to run ", getCCName(this->context->trimmed), L" with '", base, L"' with '", rawArg, L"'");
   }
}

void C_RunWithWith::run()
{
   p_str base = os_softTrim(value->getValue());

   if (!this->context->v_exists->value || base.empty()) {
      this->perun2.logger.log(L"Failed to run ", getCCName(this->context->trimmed), L" with '", base, L"'");
      this->perun2.contexts.success->value = false;
      return;
   }

   const p_list rawArgs = arguments->getValue();
   const p_size len = rawArgs.size();

   if (len == 0) {
      const p_str com = str(base, CHAR_SPACE, os_quoteEmbraced(this->context->trimmed));
      const p_str loc = this->getLocation();
      const p_bool s = os_run(com, loc, this->perun2);
      this->perun2.contexts.success->value = s;

      if (s) {
         this->perun2.logger.log(L"Run ", getCCName(this->context->trimmed), L" with '", base, L"'");
      }
      else {
         this->perun2.logger.log(L"Failed to run ", getCCName(this->context->trimmed), L" with '", base, L"'");
      }
   }
   else {
      p_stream comStream;
      p_stream logStream;
      const p_str& first = rawArgs[0];
      logStream << str(getCCName(this->context->trimmed), L" with '", base, L"' with '", first, L"'");
      comStream << str(base, CHAR_SPACE, os_quoteEmbraced(this->context->trimmed), CHAR_SPACE, os_makeArg(first));

      for (p_size i = 1; i < len; i++) {
         const p_str& a = rawArgs[i];
         logStream << str(L", '", a, L"'");
         comStream << str(CHAR_SPACE, os_makeArg(a));
      }

      const p_str com = comStream.str();
      const p_str loc = this->getLocation();
      const p_bool s = os_run(com, loc, this->perun2);
      this->perun2.contexts.success->value = s;

      if (s) {
         this->perun2.logger.log(L"Run ", logStream.str());
      }
      else {
         this->perun2.logger.log(L"Failed to run ", logStream.str());
      }
   }
}

void C_RunWithPerun2::run()
{
   if (!this->context->v_exists->value) {
      this->perun2.logger.log(L"Failed to run ", getCCName(this->context->trimmed), L" with Perun2");
      this->perun2.contexts.success->value = false;
      return;
   }

   const p_str com = str(this->perun2.cache.cmdProcessStartingArgs, os_quoteEmbraced(this->context->trimmed));
   const p_str loc = this->getLocation();
   const p_bool s = os_run(com, loc, this->perun2);
   this->perun2.contexts.success->value = s;

   if (s) {
      this->perun2.logger.log(L"Run ", getCCName(this->context->trimmed), L" with Perun2");
   }
   else {
      this->perun2.logger.log(L"Failed to run ", getCCName(this->context->trimmed), L" with Perun2");
   }
}

void C_RunWithPerun2WithString::run()
{
   if (!this->context->v_exists->value) {
      this->perun2.logger.log(L"Failed to run ", getCCName(this->context->trimmed), L" with Perun2");
      this->perun2.contexts.success->value = false;
      return;
   }

   const p_str rawArg = argument->getValue();
   const p_str arg = os_makeArg(rawArg);
   const p_str com = str(this->perun2.cache.cmdProcessStartingArgs, os_quoteEmbraced(this->context->trimmed), CHAR_SPACE, arg);

   const p_str loc = this->getLocation();
   const p_bool s = os_run(com, loc, this->perun2);
   this->perun2.contexts.success->value = s;

   if (s) {
      this->perun2.logger.log(L"Run ", getCCName(this->context->trimmed), L" with Perun2 with '", rawArg, L"'");
   }
   else {
      this->perun2.logger.log(L"Failed to run ", getCCName(this->context->trimmed), L" with Perun2 with '", rawArg, L"'");
   }
}

void C_RunWithPerun2With::run()
{
   if (!this->context->v_exists->value) {
      this->perun2.logger.log(L"Failed to run ", getCCName(this->context->trimmed), L" with Perun2");
      this->perun2.contexts.success->value = false;
      return;
   }

   const p_list rawArgs = arguments->getValue();
   const p_size len = rawArgs.size();

   if (len == 0) {
      const p_str com = str(this->perun2.cache.cmdProcessStartingArgs, os_quoteEmbraced(this->context->trimmed));
      const p_str loc = this->getLocation();
      const p_bool s = os_run(com, loc, this->perun2);
      this->perun2.contexts.success->value = s;

      if (s) {
         this->perun2.logger.log(L"Run ", getCCName(this->context->trimmed), L" with Perun2");
      }
      else {
         this->perun2.logger.log(L"Failed to run ", getCCName(this->context->trimmed), L" with Perun2");
      }
   }
   else {
      p_stream comStream;
      p_stream logStream;
      const p_str& first = rawArgs[0];
      logStream << str(getCCName(this->context->trimmed), L" with Perun2 with '", first, L"'");
      comStream << str(this->perun2.cache.cmdProcessStartingArgs, os_quoteEmbraced(this->context->trimmed), CHAR_SPACE, os_makeArg(first));

      for (p_size i = 1; i < len; i++) {
         const p_str& a = rawArgs[i];
         logStream << str(L", '", a, L"'");
         comStream << str(CHAR_SPACE, os_makeArg(a));
      }

      const p_str com = comStream.str();
      const p_str loc = this->getLocation();
      const p_bool s = os_run(com, loc, this->perun2);
      this->perun2.contexts.success->value = s;

      if (s) {
         this->perun2.logger.log(L"Run ", logStream.str());
      }
      else {
         this->perun2.logger.log(L"Failed to run ", logStream.str());
      }
   }
}

}

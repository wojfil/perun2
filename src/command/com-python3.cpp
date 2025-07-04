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

#include "com-python3.h"
#include <chrono>
#include <thread>
#include <algorithm>
#include "../os/os.h"
#include "../perun2.h"
#include "com-core.h"
#include <sstream>


namespace perun2::comm
{

   
static void normalizeNewLines(const char (&old)[PYTHON3_PIPE_BUFFER_SIZE], char (&next)[PYTHON3_PIPE_BUFFER_SIZE])
{
   memset(next, 0, sizeof(char) * PYTHON3_PIPE_BUFFER_SIZE);

   p_size index = 0;
   
   for (p_size i = 0; i < 255; i++) {
      if (old[i] == '\0') {
         next[index] = '\0';
         break;
      }

      if (old[i] == '\r' && old[i + 1] == '\n') {
         next[index] = '\n';
         index++;
         i += 1;
      } else {
         next[index] = old[i];
         index++;
      }
   }
}


void Python3Base::runPython(const p_str& additionalArgs) const
{
   p_str python;
   const Python3State p3 = this->perun2.postParseData.getPython3State(python);

   if (p3 == Python3State::P3_NotInstalled) {
      this->perun2.contexts.success->value = false;
      this->perun2.logger.log(L"Failed to run Python3, because its installation has not been found");
      return;
   }

   if (p3 == Python3State::P3_DifferentVersionThan3) {
      this->perun2.contexts.success->value = false;
      this->perun2.logger.log(L"Failed to run Python3. You have Python installed, but its version is different from 3");
      return;
   }

   const p_str path = os_trim(this->python3->getValue());

   if (! os_isAbsolute(path)) {
      if (p3 == Python3State::P3_NotInstalled) {
         this->perun2.contexts.success->value = false;
         this->perun2.logger.log(L"Failed to run Python3, because the argument \"", path, L"\" is not an absolute path");
         return;
      }
   }

   if (! os_fileExists(path)) {
      this->perun2.contexts.success->value = false;
      this->perun2.logger.log(L"Failed to run Python3, because the file \"", path, L"\" does not exist");
      return;
   }

   const p_str location = this->getLocation();

   if (! os_directoryExists(location)) {
      this->perun2.contexts.success->value = false;
      this->perun2.logger.log(L"Failed to run Python3, because the working location \"", location, L"\" does not exist");
      return;
   }

   if (! this->perun2.isRunning()) {
      return;
   }

   if (this->perun2.arguments.hasFlag(FLAG_MAX_PERFORMANCE)) {
      this->runSilently(python, path, additionalArgs, location);
   }
   else {
      this->runLoudly(python, path, additionalArgs, location);
   }
}

void Python3Base::runLoudly(const p_str& python, const p_str& path, 
   const p_str& additionalArgs, const p_str& location) const
{
   HANDLE hRead;
   HANDLE hWrite;
   SECURITY_ATTRIBUTES sa = { sizeof(SECURITY_ATTRIBUTES), NULL, TRUE };

   if (! CreatePipe(&hRead, &hWrite, &sa, 0)) {
      this->perun2.contexts.success->value = false;
      this->perun2.logger.log(L"Failed to run Python3. A new pipe could not be created");
      return;
   }

   SetHandleInformation(hRead, HANDLE_FLAG_INHERIT, 0);

   STARTUPINFOW si = { sizeof(si) };
   PROCESS_INFORMATION& pi = this->perun2.sideProcess.info;
   ZeroMemory(&pi, sizeof(pi));

   si.dwFlags |= STARTF_USESTDHANDLES;
   si.hStdOutput = hWrite;
   si.hStdError  = hWrite;
   si.hStdInput  = GetStdHandle(STD_INPUT_HANDLE);

   const p_str baseCommand = prepareCmd(python, path, additionalArgs);
   std::wstring command = baseCommand;

   const BOOL creation = CreateProcessW(
      NULL, 
      &command[0], 
      NULL, NULL, 
      TRUE, 
      0, 
      NULL, 
      location.c_str(), 
      &si, 
      &pi);

   if (! creation) {
      CloseHandle(hRead);
      CloseHandle(hWrite);
      this->perun2.contexts.success->value = false;
      this->perun2.logger.log(L"Failed to run Python3. A new process could not be started");
      return;
   }

   this->perun2.sideProcess.running = true;
   
   CloseHandle(hWrite);

   char buffer[PYTHON3_PIPE_BUFFER_SIZE];
   char nextOutput[PYTHON3_PIPE_BUFFER_SIZE];
   DWORD bytesRead;
   p_bool broken = false;

   while (ReadFile(hRead, buffer, sizeof(buffer) - 1, &bytesRead, NULL) && bytesRead > 0) {
      if (! this->perun2.isRunning()) {
         broken = true;
         break;
      }

      buffer[bytesRead / sizeof(char)] = '\0';
      normalizeNewLines(buffer, nextOutput);
      p_cout << nextOutput << std::flush;
   }

   WaitForSingleObject(pi.hProcess, INFINITE);
   DWORD dwExitCode = 0;
   ::GetExitCodeProcess(pi.hProcess, &dwExitCode);

   this->perun2.sideProcess.running = false;

   CloseHandle(hRead);
   CloseHandle(pi.hProcess);
   CloseHandle(pi.hThread);

   const p_bool success = ! broken && perun2.isRunning() && dwExitCode == 0;
   this->perun2.contexts.success->value = success;

   if (success) {
      this->perun2.logger.log(L"Run Python3 \"", path, L"\"");
   }
   else {
      this->perun2.logger.log(L"Failed to run Python3 \"", path, L"\"");
   }
}


void Python3Base::runSilently(const p_str& python, const p_str& path, 
   const p_str& additionalArgs, const p_str& location) const
{
   const p_str command = prepareCmd(python, path, additionalArgs);
   const p_bool success = os_run(command, location, this->perun2);

   this->perun2.contexts.success->value = success;

   if (success) {
      this->perun2.logger.log(L"Run Python3 \"", path, L"\"");
   }
   else {
      this->perun2.logger.log(L"Failed to run Python3 \"", path, L"\"");
   }
}

Python3Base::Python3Base(p_genptr<p_str>& pyth3, Perun2Process& p2)
   : perun2(p2), 
     python3(std::move(pyth3)),
     locationCtx(p2.contexts.getLocationContext()) { };


p_str Python3Base::getLocation() const
{
   return this->locationCtx->location->value;
}

p_str Python3Base::prepareCmd(const p_str& python, const p_str& path, 
   const p_str& additionalArgs) const
{
   return additionalArgs.empty()
      ? str(L"\"", python, L"\" -u \"", path, L"\"")
      : str(L"\"", python, L"\" -u \"", path, L"\" ", additionalArgs);
}

void C_Python3::run()
{
   runPython(L"");
}

void C_Python3With::run()
{
   runPython(this->evalArguments());
}

p_str C_Python3With::evalArguments() const
{
   const p_list args = this->arguments->getValue();

   if (args.empty()) {
      return L"";
   }

   p_str result = os_makeArg(args[0]);

   for (size_t i = 1; i < args.size(); i++) {
      result += CHAR_SPACE;
      result += os_makeArg(args[i]);
   }

   return result;
}

}

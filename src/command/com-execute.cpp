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

#include "../../include/perun2/command/com-execute.hpp"
#include "../../include/perun2/os/os.hpp"
#include "../../include/perun2/perun2.hpp"
#include "../../include/perun2/command/com-core.hpp"
#include <chrono>
#include <thread>
#include <algorithm>
#include <sstream>


namespace perun2::comm
{

void normalizeNewLines(const char (&old)[EXECUTION_PIPE_BUFFER_SIZE],
   char (&next)[EXECUTION_PIPE_BUFFER_SIZE])
{
   memset(next, 0, sizeof(char) * EXECUTION_PIPE_BUFFER_SIZE);

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


Executor::Executor(Perun2Process& p2)
   : perun2(p2) { };


ExecutionResult Executor::executeLoudly(const p_str& command, const p_str& location) const
{
   HANDLE hRead;
   HANDLE hWrite;
   SECURITY_ATTRIBUTES sa = { sizeof(SECURITY_ATTRIBUTES), NULL, TRUE };

   if (! CreatePipe(&hRead, &hWrite, &sa, 0)) {
      this->perun2.contexts.success->value = false;
      return ExecutionResult::ER_Bad_PipeNotCreated;
   }

   SetHandleInformation(hRead, HANDLE_FLAG_INHERIT, 0);

   STARTUPINFOW si = { sizeof(si) };
   PROCESS_INFORMATION& pi = this->perun2.sideProcess.info;
   ZeroMemory(&pi, sizeof(pi));

   si.dwFlags |= STARTF_USESTDHANDLES;
   si.hStdOutput = hWrite;
   si.hStdError  = hWrite;
   si.hStdInput  = GetStdHandle(STD_INPUT_HANDLE);

   p_str alterableCommand = command;

   const BOOL creation = CreateProcessW(
      NULL, 
      &alterableCommand[0], 
      NULL, NULL, 
      TRUE, 
      0, 
      NULL, 
      location.empty() ? NULL : location.c_str(), 
      &si, 
      &pi);

   if (! creation) {
      CloseHandle(hRead);
      CloseHandle(hWrite);
      this->perun2.contexts.success->value = false;
      return ExecutionResult::ER_Bad_ProcessNotStarted;
   }

   this->perun2.sideProcess.running = true;
   
   CloseHandle(hWrite);

   char buffer[EXECUTION_PIPE_BUFFER_SIZE];
   char nextOutput[EXECUTION_PIPE_BUFFER_SIZE];
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

   return success ? ExecutionResult::ER_Good : ExecutionResult::ER_Bad;
}


SimpleExecutor::SimpleExecutor(Perun2Process& p2)
   : Executor(p2) { };

ExecutionResult SimpleExecutor::execute(const p_str& command) const
{
   return this->perun2.arguments.hasFlag(FLAG_MAX_PERFORMANCE)
      ? this->executeSilently(command, L"")
      : this->executeLoudly(command, L"");
}


ExecutionResult Executor::executeSilently(const p_str& command, const p_str& location) const
{
   const p_bool success = os_run(command, location, this->perun2);
   this->perun2.contexts.success->value = success;

   return success ? ExecutionResult::ER_Good : ExecutionResult::ER_Bad;
}

p_str Executor::mergeArguments(const p_list& args) const
{
   if (args.empty()) {
      return L"";
   }

   p_str result = os_makeArg(args[0]);

   for (p_size i = 1; i < args.size(); i++) {
      result += CHAR_SPACE;
      result += os_makeArg(args[i]);
   }

   return result;
}


ExecuteBase::ExecuteBase(const p_str& cmd, Perun2Process& p2)
   : Executor(p2), baseCommand(cmd), 
     locationCtx(*p2.contexts.getLocationContext()) { };

void ExecuteBase::execute(const p_str& additionalArgs) const
{
   const p_str location = this->getLocation();

   if (! os_directoryExists(location)) {
      this->perun2.contexts.success->value = false;
      this->perun2.logger.log(L"Failed to execute the command, because the working location \"", location, L"\" does not exist");
      return;
   }

   if (! this->perun2.isRunning()) {
      return;
   }

   const p_str command = additionalArgs.empty()
      ? baseCommand
      : str(baseCommand, CHAR_SPACE, additionalArgs);

   const ExecutionResult executionResult = this->perun2.arguments.hasFlag(FLAG_MAX_PERFORMANCE)
      ? this->executeSilently(command, location)
      : this->executeLoudly(command, location);

   switch (executionResult) {
      case ExecutionResult::ER_Good: {
         this->perun2.logger.log(L"Execute \"", command, L"\"");
         break;
      }
      case ExecutionResult::ER_Bad: {
         this->perun2.logger.log(L"Failed to execute \"", command, L"\"");
         break;
      }
      case ExecutionResult::ER_Bad_PipeNotCreated: {
         this->perun2.logger.log(L"Failed to execute \"", command, L"\". A new pipe could not be created");
         break;
      }
      case ExecutionResult::ER_Bad_ProcessNotStarted: {
         this->perun2.logger.log(L"Failed to execute \"", command, L"\". A new process could not be started");
         break;
      }
   }
}

p_str ExecuteBase::getLocation() const
{
   return this->locationCtx.location->value;
}

C_Execute::C_Execute(const p_str& cmd, Perun2Process& p2)
   : ExecuteBase(cmd, p2) { };

void C_Execute::run()
{
   this->execute(L"");
}

C_ExecuteWith::C_ExecuteWith(const p_str& cmd, p_genptr<p_list>& args, Perun2Process& p2)
   : ExecuteBase(cmd, p2), arguments(std::move(args)) { };

void C_ExecuteWith::run()
{
   const p_list args = this->arguments->getValue();
   this->execute(this->mergeArguments(args));
}

}

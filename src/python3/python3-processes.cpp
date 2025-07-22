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

#include "python3-processes.h"
#include "../context/ctx-file.h"
#include "../command/com-execute.h"
#include "../perun2.h"
#include "com-python3.h"
#include <iostream>
#include <thread>
#include <future>
#include <chrono>
#include <atomic>


namespace perun2::comm
{


AskablePython3Script::AskablePython3Script(const FileContext& fctx, const LocationContext& lctx, Perun2Process& p2)
   : sharedMemory(fctx, lctx, p2), perun2(p2) { };

p_bool AskablePython3Script::ask()
{
   return this->sharedMemory.ask();
}

void AskablePython3Script::python3StaticTypeAnalysis(const p_str& funcName, const p_str& filePath, const p_int line)
{
   const p_str analyzerScript = perun2.postParseData.getPython3AnalyzerPath();

   if (! os_fileExists(analyzerScript)) {
      throw SyntaxError(str(L"the function \"", funcName, 
         L"\" could not be prepared to run, because the internal source file \"", PYTHON_ANALYZER_ROOT_FILE,
         L"\" does not exist. To solve this problem, reinstall Perun2"), line);
   }



   // todo
}

p_str AskablePython3Script::askerPython3RunCmd(const p_str& python, const p_str& path, 
   const p_str& filePath, const p_int memoryId) const
{
   return str(L"\"", python, L"\" -u \"", path, L"\" \"", filePath, L"\" \"", toStr(memoryId), L"\"");
}

void AskablePython3Script::start(const p_str& askerScript, const p_str& funcName, 
   const p_str& filePath, const p_int line)
{
   if (filePath.empty()) {
      throw SyntaxError(str(L"the argument of the function \"", funcName, 
         L"\" is empty"), line);
   }

   if (! os_isAbsolute(filePath)) {
      throw SyntaxError(str(L"the argument of the function \"", funcName, 
         L"\" is not an absolute path"), line);
   }

   if (! os_fileExists(filePath)) {
      throw SyntaxError(str(L"the argument of the function \"", funcName, 
         L"\" does not point to an existing file"), line);
   }

   p_str python;
   const Python3State p3 = this->perun2.postParseData.getPython3State(python);

   if (p3 == Python3State::P3_NotInstalled) {
      throw SyntaxError(str(L"the function \"", funcName,
         L"\" could not be prepared to run. Python3 is not installed on this machine"), line);
   }

   if (p3 == Python3State::P3_DifferentVersionThan3) {
      throw SyntaxError(str(L"the function \"", funcName,
         L"\" could not be prepared to run. The version of Python on this machine is different from 3"), line);
   }

   if (! os_fileExists(filePath)) {
      throw SyntaxError(str(L"the function \"", funcName,
         L"\" could not be prepared to run. The file \"", filePath, L"\" does not exist"), line);
   }

   python3StaticTypeAnalysis(funcName, filePath, line);

   this->sharedMemory.makeMemoryId();
   const p_int memoryId = this->sharedMemory.getMemoryId();
   const p_str command = askerPython3RunCmd(python, askerScript, filePath, memoryId);
   std::promise<Python3AskerResult> promise;
   std::future<Python3AskerResult> future = promise.get_future();

   if (this->perun2.arguments.hasFlag(FLAG_MAX_PERFORMANCE)) {
      thread = std::make_unique<std::thread>(&AskablePython3Script::startSilently, 
         this, std::move(promise), command);
   }
   else {
      thread = std::make_unique<std::thread>(&AskablePython3Script::startLoudly, 
         this, std::move(promise), command);
   }

   const Python3AskerResult result = future.get();

   if (! this->sharedMemory.start()) {
      throw SyntaxError(str(L"the function \"", funcName,
         L"\" could not be prepared to run. Failed to run Python3 \"", filePath, 
         L"\". A connection to shared memory could not be opened"), line);
   }


   switch (result) {
      case Python3AskerResult::PAR_Bad: {
         throw SyntaxError(str(L"the function \"", funcName,
            L"\" could not be prepared to run. Failed to run Python3 \"", filePath, L"\""), line);
      }
      case Python3AskerResult::PAR_Bad_PipeNotCreated: {
         throw SyntaxError(str(L"the function \"", funcName,
            L"\" could not be prepared to run. Failed to run Python3 \"", filePath, L"\". A new pipe could not be created"), line);
      }
      case Python3AskerResult::PAR_Bad_ProcessNotStarted: {
         throw SyntaxError(str(L"the function \"", funcName,
            L"\" could not be prepared to run. Failed to run Python3 \"", filePath, L"\". A new process could not be started"), line);
      }
   }
}


void AskablePython3Script::startLoudly(std::promise<Python3AskerResult> midResultPromise, const p_str& command)
{
   HANDLE hRead;
   HANDLE hWrite;
   SECURITY_ATTRIBUTES sa = { sizeof(SECURITY_ATTRIBUTES), NULL, TRUE };

   if (! CreatePipe(&hRead, &hWrite, &sa, 0)) {
      midResultPromise.set_value(Python3AskerResult::PAR_Bad_PipeNotCreated);
      return;
   }

   SetHandleInformation(hRead, HANDLE_FLAG_INHERIT, 0);

   STARTUPINFOW si = { sizeof(si) };
   SideProcess sideProcess;
   PROCESS_INFORMATION& pi = sideProcess.info;
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
      NULL, 
      &si, 
      &pi);

   if (! creation) {
      CloseHandle(hRead);
      CloseHandle(hWrite);
      midResultPromise.set_value(Python3AskerResult::PAR_Bad_ProcessNotStarted);
      return;
   }

   sideProcess.running = true;
   python3Process.store(sideProcess);
   CloseHandle(hWrite);

   char buffer[EXECUTION_PIPE_BUFFER_SIZE];
   char nextOutput[EXECUTION_PIPE_BUFFER_SIZE];
   DWORD bytesRead;
   midResultPromise.set_value(Python3AskerResult::PAR_Good);

   while (ReadFile(hRead, buffer, sizeof(buffer) - 1, &bytesRead, NULL) && bytesRead > 0) {
      if (this->perun2.isNotRunning()) {
         break;
      }

      buffer[bytesRead / sizeof(char)] = '\0';
      normalizeNewLines(buffer, nextOutput);
      p_cout << nextOutput << std::flush;
   }

   sideProcess.running = false;
   python3Process.store(sideProcess);

   WaitForSingleObject(pi.hProcess, INFINITE);

   CloseHandle(hRead);
   CloseHandle(pi.hProcess);
   CloseHandle(pi.hThread);
}

void AskablePython3Script::startSilently(std::promise<Python3AskerResult> midResultPromise, const p_str& command)
{
   STARTUPINFOW si = { sizeof(si) };
   SideProcess sideProcess;
   PROCESS_INFORMATION& pi = sideProcess.info;
   ZeroMemory(&pi, sizeof(pi));

   std::unique_ptr<p_char[]> cmd = std::make_unique<p_char[]>(command.size() + 1);
   wcscpy(cmd.get(), command.c_str());
   cmd[command.size()] = CHAR_NULL;

   const BOOL creation = CreateProcessW(
      NULL,
      cmd.get(),
      NULL,NULL,FALSE,
      CREATE_NEW_PROCESS_GROUP | CREATE_NO_WINDOW,
      NULL,
      NULL,
      &si, &pi
   );

   if (! creation) {
      midResultPromise.set_value(Python3AskerResult::PAR_Bad_ProcessNotStarted);
      return;
   }

   sideProcess.running = true;
   python3Process.store(sideProcess);

   midResultPromise.set_value(Python3AskerResult::PAR_Good);
   WaitForSingleObject(pi.hProcess, INFINITE);
   
   sideProcess.running = false;
   python3Process.store(sideProcess);

   CloseHandle(pi.hProcess);
   CloseHandle(pi.hThread);
}


void AskablePython3Script::terminate()
{
   if (thread) {
      python3Process.load().terminate();
      thread->join();
      thread.reset();
      sharedMemory.terminate();
   }
}


Python3Processes::Python3Processes(Perun2Process& p2)
    : perun2(p2) { };

Python3Processes::~Python3Processes()
{
   this->terminate();
}

AskablePython3Script& Python3Processes::addAskableScript(const FileContext& fctx, const LocationContext& lctx, 
   const p_str& funcName, const p_str& filePath, const p_int line)
{
   const p_str askerScript = perun2.postParseData.getPython3AskerPath();

   if (! os_fileExists(askerScript)) {
      throw SyntaxError(str(L"the function \"", funcName, 
         L"\" could not be prepared to run, because the internal source file \"", PYTHON_ASKER_ROOT_FILE,
         L"\" does not exist. To solve this problem, reinstall Perun2"), line);
   }

   comm::Python3Base python3ForAnalysis(filePath, perun2);
   
   const p_str name = str(L"the function \"", funcName, L"\"");
   python3ForAnalysis.staticallyAnalyze(line, name);

   std::unique_ptr<AskablePython3Script> script = std::make_unique<AskablePython3Script>(fctx, lctx, perun2);
   this->askableScripts.emplace_back(std::move(script));
   AskablePython3Script& newScript = *this->askableScripts.back().get();
   newScript.start(askerScript, funcName, filePath, line);
   return newScript;
}

void Python3Processes::terminate()
{
   for (std::unique_ptr<AskablePython3Script>& script : askableScripts) {
      script->terminate();
   }
}

}

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
#include "../os/os.h"
#include "../perun2.h"
#include "../command/com-core.h"
#include <chrono>
#include <thread>
#include <algorithm>
#include <sstream>


namespace perun2::comm
{


Python3Base::Python3Base(p_genptr<p_str>& pyth3, Perun2Process& p2)
   : Executor(p2),  python3(std::move(pyth3)) { };


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

   const p_str command = prepareCmd(python, path, additionalArgs);

   const ExecutionResult executionResult = this->perun2.arguments.hasFlag(FLAG_MAX_PERFORMANCE)
      ? this->executeSilently(command, location)
      : this->executeLoudly(command, location);

   switch (executionResult) {
      case ExecutionResult::ER_Good: {
         this->perun2.logger.log(L"Run Python3 \"", path, L"\"");
         break;
      }
      case ExecutionResult::ER_Bad: {
         this->perun2.logger.log(L"Failed to run Python3 \"", path, L"\"");
         break;
      }
      case ExecutionResult::ER_Bad_PipeNotCreated: {
         this->perun2.logger.log(L"Failed to run Python3 \"", path, L"\". A new pipe could not be created");
         break;
      }
      case ExecutionResult::ER_Bad_ProcessNotStarted: {
         this->perun2.logger.log(L"Failed to run Python3 \"", path, L"\". A new process could not be started");
         break;
      }
   }
}

p_str Python3Base::prepareCmd(const p_str& python, const p_str& path, 
   const p_str& additionalArgs) const
{
   return additionalArgs.empty()
      ? str(L"\"", python, L"\" -u \"", path, L"\"")
      : str(L"\"", python, L"\" -u \"", path, L"\" ", additionalArgs);
}

C_Python3::C_Python3(p_genptr<p_str>& pyth3, Perun2Process& p2)
   : Python3Base(pyth3, p2) { };

void C_Python3::run()
{
   this->runPython(L"");
}

C_Python3With::C_Python3With(p_genptr<p_str>& pyth3, p_genptr<p_list>& args, Perun2Process& p2)
   : Python3Base(pyth3, p2), arguments(std::move(args)) { };

void C_Python3With::run()
{
   const p_list args = this->arguments->getValue();
   this->runPython(this->mergeArguments(args));
}

}

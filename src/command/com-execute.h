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

#pragma once

#include "com-listener.h"
#include "../datatype/datatype.h"
#include "../datatype/generator.h"
#include "com-aggregate.h"
#include "com-core.h"
#include "../attribute.h"
#include "../perun2.h"


namespace perun2::comm
{

p_constexpr p_size EXECUTION_PIPE_BUFFER_SIZE = 256;


enum ExecutionResult 
{
   ER_Good,
   ER_Bad,
   ER_Bad_PipeNotCreated,
   ER_Bad_ProcessNotStarted
};


void normalizeNewLines(const char (&old)[EXECUTION_PIPE_BUFFER_SIZE], 
   char (&next)[EXECUTION_PIPE_BUFFER_SIZE]);


struct Executor
{
public:
   Executor() = delete;
   Executor(Perun2Process& p2);

protected:
   ExecutionResult executeLoudly(const p_str& command, const p_str& location) const;
   ExecutionResult executeSilently(const p_str& command, const p_str& location) const;
   p_str getLocation() const;
   p_str mergeArguments(const p_list& args) const;

   Perun2Process& perun2;
   LocationContext* locationCtx;
};



struct ExecuteBase : Executor
{
public:
   ExecuteBase() = delete;
   ExecuteBase(const p_str& cmd, Perun2Process& p2);

protected:
   void execute(const p_str& additionalArgs) const;

private:
   const p_str baseCommand;
};


struct C_Execute : Command, ExecuteBase
{
public:
   C_Execute(const p_str& cmd, Perun2Process& p2);
   void run() override;
};


struct C_ExecuteWith : Command, ExecuteBase
{
public:
   C_ExecuteWith(const p_str& cmd, p_genptr<p_list>& args, Perun2Process& p2);
   void run() override;

private:
   p_genptr<p_list> arguments;
};


}

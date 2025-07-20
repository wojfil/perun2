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

#include "../command/com-execute.h"


namespace perun2::comm
{

enum Python3ParseResult {
   P3PR_Good,
   P3PR_MissingFile,
   P3PR_MissingFiled,

};

p_str python3RunCmd(const p_str& python, const p_str& path, const p_str& additionalArgs);
p_str python3StatAnalyzeCmd(const p_str& python, const p_str& path);

struct Python3Base : Executor
{
public:
   Python3Base() = delete;
   Python3Base(const p_str& script, Perun2Process& p2);

   void staticallyAnalyze(const p_int line, const p_str& name) const;

protected:
   void runPython(const p_str& additionalArgs) const;

private:
   const p_str scriptPath;
};


struct C_Python3 : Command, Python3Base
{
public:
   C_Python3(const p_str& script, Perun2Process& p2);
   void run() override;
};


struct C_Python3With : Command, Python3Base
{
public:
   C_Python3With(const p_str& script, p_genptr<p_list>& args, Perun2Process& p2);
   void run() override;

private:
   p_genptr<p_list> arguments;
};

}

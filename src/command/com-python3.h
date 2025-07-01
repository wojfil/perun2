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

static p_constexpr p_size PYTHON3_PIPE_BUFFER_SIZE = 256;

static void normalizeNewLines(const char (&old)[256], char (&next)[256]);

struct Python3Base
{
public:
   Python3Base() = delete;
   Python3Base(p_genptr<p_str>& pyth3, Perun2Process& p2);

protected:
   void runPython(const p_str& additionalArgs) const;

private:
   void runLoudly(const p_str& python, const p_str& path, 
      const p_str& additionalArgs, const p_str& location) const;

   void runSilently(const p_str& python, const p_str& path, 
      const p_str& additionalArgs, const p_str& location) const;

   p_str getLocation() const;
   p_str prepareCmd(const p_str& python, const p_str& path, 
      const p_str& additionalArgs) const;

   p_genptr<p_str> python3;
   Perun2Process& perun2;
   LocationContext* locationCtx;
};


struct C_Python3 : Command, Python3Base
{
public:
   C_Python3(p_genptr<p_str>& pyth3, Perun2Process& p2)
      : Python3Base(pyth3, p2) { };

   void run() override;
};


struct C_Python3With : Command, Python3Base
{
public:
   C_Python3With(p_genptr<p_str>& pyth3, p_genptr<p_list>& args, Perun2Process& p2)
      : Python3Base(pyth3, p2), arguments(std::move(args)) { };

   void run() override;

private:
   p_genptr<p_str> python3;
   p_genptr<p_list> arguments;
};

}

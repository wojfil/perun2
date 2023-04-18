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

#include "perun2.h"

perun2::_exitint wmain(perun2::_int argc, perun2::_char* argv[], perun2::_char* envp[])
{
   // these two inits are useful, but not mandatory
   // do not call them, if Perun2 is used like an imported external library
   // initConsole() provides some specific IO optimizations
   // initTerminator() overrides the default Ctrl+C termination signal
   // after that, it does not work as expected outside of Perun2, so be very cautious
   perun2::initConsole();
   perun2::initTerminator();

   perun2::Perun2 instance(argc, argv);
   instance.run();
   return instance.getExitCode();
}

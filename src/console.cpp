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

#ifndef UNICODE
#define UNICODE
#endif

#ifndef _UNICODE
#define _UNICODE
#endif

#include "console.hpp"
#include "datatype/primitives.hpp"

#include <locale>
#include <clocale>
#include <cstdlib>
#include <iostream>

extern "C" {
   #include <libavutil/log.h>
}


namespace perun2
{
   ConsoleSettings::ConsoleSettings()
   {
      generalInit();
      libInit();
   }

   void ConsoleSettings::generalInit() const
   {
      p_cin.tie(0);
      p_cout.tie(0);
      std::setlocale(LC_CTYPE, "");
   }

   void ConsoleSettings::libInit() const
   {
      av_log_set_level(AV_LOG_QUIET);
   }
}

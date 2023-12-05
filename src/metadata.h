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

#ifndef METADATA_H_INCLUDED
#define METADATA_H_INCLUDED

#include "datatype/primitives.h"

namespace perun2::metadata
{

p_constexpr p_char NAME[] =                L"Perun2";
p_constexpr p_char VERSION[] =             L"0.8.5.2";
p_constexpr p_char EXTENSION[] =           L"peru";
p_constexpr p_char WEBSITE_FRONT[] =       L"https://perun2.org";
p_constexpr p_char WEBSITE_DOCS[] =        L"https://perun2.org/docs";
p_constexpr p_char EXECUTABLE_NAME[] =     L"perun2";

p_constexpr p_size EXTENSION_LENGTH =      _countof(EXTENSION) - 1;

}

#endif // METADATA_H_INCLUDED

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

#include "../../datatype/text/wildcard.h"


namespace perun2::prog
{


// there are two wildcards
// *     any sequence of characters, can be empty space
// #     any sequence of numbers and dots, can be empty space
struct ProgramPatternComparer : WildcardComparer
{
public:
   ProgramPatternComparer() = delete;
   ProgramPatternComparer(const p_str& pat);

protected:
   p_size getMinLength(const p_str& pat) const override;
   Logic checkState(const p_size n, const p_size m) override;
};


}

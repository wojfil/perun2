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

#ifndef BRACKETS_H
#define BRACKETS_H

#include "tokens.h"

namespace perun2
{

struct BracketsInfo
{
public:
   void refresh(const Token& tk);
   p_bool isBracketFree() const;

private:
   p_int round = 0;
   p_int square = 0;
};

// this is called only once at the beginning of parsing
// look for common errors before any command has been built
void checkBracketsThoroughly(const Tokens& tks);

}

#endif /* BRACKETS_H */


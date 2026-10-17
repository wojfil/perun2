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

#include "../datatype.h"
#include "../../unicode/regex.h"
#include "../../unicode/convert.h"


namespace perun2::gen
{


struct Regexp : Generator<p_bool>
{
public:
   Regexp(p_genptr<p_str>& val, p_genptr<p_str>& pat);
   p_bool getValue() override;

private:
   p_genptr<p_str> value;
   p_genptr<p_str> pattern;
   p_str lastPattern;
   std::unique_ptr<UnicodeRegex> lastRegex;
};


struct RegexpConst : Generator<p_bool>
{
public:
   RegexpConst(p_genptr<p_str>& val, std::unique_ptr<UnicodeRegex>& pat);
   p_bool getValue() override;

private:
   p_genptr<p_str> value;
   std::unique_ptr<UnicodeRegex> pattern;
};


}

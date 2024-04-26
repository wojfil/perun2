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

#ifndef REGEXP_H_INCLUDED
#define REGEXP_H_INCLUDED

#include "../datatype.h"
#include <regex>


namespace perun2
{


struct Regexp : Generator<p_bool>
{
public:
   Regexp(p_genptr<p_str>& val, p_genptr<p_str>& pat);
   p_bool getValue() override;

private:
   p_genptr<p_str> value;
   p_genptr<p_str> pattern;
};


struct RegexpConst : Generator<p_bool>
{
public:
   RegexpConst(p_genptr<p_str>& val, const p_str& pat);
   p_bool getValue() override;

private:
   p_genptr<p_str> value;
   const std::wregex pattern;
};


}

#endif // REGEXP_H_INCLUDED

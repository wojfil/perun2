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

#include "regexp.hpp"


namespace perun2::gen
{

Regexp::Regexp(p_genptr<p_str>& val, p_genptr<p_str>& pat)
   : value(std::move(val)), pattern(std::move(pat)) { };


p_bool Regexp::getValue()
{
   std::wregex pattern(this->pattern->getValue());
   return std::regex_search(this->value->getValue(), pattern);
}


RegexpConst::RegexpConst(p_genptr<p_str>& val, const p_str& pat)
   : value(std::move(val)), pattern(pat) { };


p_bool RegexpConst::getValue()
{
   return std::regex_search(this->value->getValue(), this->pattern);
}


}

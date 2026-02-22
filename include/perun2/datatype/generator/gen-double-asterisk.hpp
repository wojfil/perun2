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

#include "gen-os.hpp"
#include "../text/wildcard.hpp"
#include <memory>


namespace perun2::gen
{

// these wildcards are used only internally
// they should be characters restricted for a file name
// so no collision happens
p_constexpr p_char WILDCARD_SINGLE_ASTERISK = CHAR_SMALLER;
p_constexpr p_char WILDCARD_DOUBLE_ASTERISK = CHAR_GREATER;

typedef std::unique_ptr<RecursiveAll> p_rallptr;


struct DoubleAsteriskPattern : Definition, WildcardComparer
{
public:
   DoubleAsteriskPattern() = delete;
   DoubleAsteriskPattern(p_rallptr& def, Perun2Process& p2, const p_str& pat, const p_str& pref, const p_int retr);
   p_bool setAction(p_daptr& act) override;

   p_bool hasNext() override;
   void reset() override;

protected:
   p_bool hasSpecialStart() const;
   p_size getMinLength(const p_str& pat) const override;
   Logic checkState(const p_size n, const p_size m) override;

private:
   const p_str prefix;
   const p_size startId;
   p_rallptr definition;
   FileContext* context;
   Perun2Process& perun2;
   p_bool first = true;
   p_num index = NINT_ZERO;
   const p_bool hasRetreats;
   const p_str retreat;

   // is true if the pattern starts with a double asterisk and is followed by a path separator
   const p_bool specialStart;
};


}

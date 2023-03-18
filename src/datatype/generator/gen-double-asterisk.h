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

#ifndef GEN_DOUBLE_ASTERISK_H_INCLUDED
#define GEN_DOUBLE_ASTERISK_H_INCLUDED

#include "gen-os.h"
#include "../wildcard.h"
#include <memory>


namespace perun2::gen
{

// these wildcards are used only internally
// they should be characters restricted for a file name
// so no collision happens
_constexpr _char WILDCARD_SINGLE_ASTERISK = CHAR_SMALLER;
_constexpr _char WILDCARD_DOUBLE_ASTERISK = CHAR_GREATER;

typedef std::unique_ptr<RecursiveAll> _rallptr;


struct DoubleAsteriskPattern : Definition, WildcardComparer
{
public:
   DoubleAsteriskPattern() = delete;
   DoubleAsteriskPattern(_rallptr& def, _p2& p2, const _str& pat, const _str& pref);

   _bool hasNext() override;
   void reset() override;

protected:
   _bool hasSpecialStart() const;
   _size getMinLength(const _str& pat) const override;
   WildcardCharState checkState(const _size n, const _size m) override;

private:
   const _str preffix;
   const _size startId;
   _rallptr definition;
   FileContext* context;
   _p2& perun2;
   _bool first = true;
   _num index = NINT_ZERO;

   // is true if the pattern starts with a double asterisk and is followed by a path separator
   const _bool specialStart;
};


}

#endif // GEN_DOUBLE_ASTERISK_H_INCLUDED

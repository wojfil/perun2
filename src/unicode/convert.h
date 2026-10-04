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

#include "../datatype/datatype.h"
#include <unicode/unistr.h>
#include <optional>

namespace perun2 
{

struct Perun2;

p_str utf8_to_utf32(const std::string& utf8);
std::string utf32_to_utf8(const p_str& utf32);
std::wstring utf32_to_utf16(const p_str& utf32);
p_str utf16_to_utf32(const std::wstring& utf16);
p_str toLowercase(const p_str& input);
p_str toUppercase(const p_str& input);
icu::UnicodeString utf32_to_unicode(const p_str& utf32);
p_str unicode_to_utf32(const icu::UnicodeString& unicode);
p_list toGraphemes(const p_str& value, icu::BreakIterator& iterator);
p_bool hasOnlyOneCharGraphemes(const p_str& value);
p_bool isLetterGrapheme(const p_str& grapheme);
p_bool isDigitGrapheme(const p_str& grapheme);
p_bool isWhitespaceGrapheme(const p_str& grapheme);


}

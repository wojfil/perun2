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

#include "convert.h"

#include <unicode/locid.h>
#include <unicode/unistr.h>
#include <unicode/ustream.h>
#include <unicode/uchar.h>
#include <unicode/brkiter.h>
#include <charconv>


namespace perun2 
{


p_str utf8_to_utf32(const std::string& utf8) 
{
    icu::UnicodeString ustr = icu::UnicodeString::fromUTF8(utf8);
    return unicode_to_utf32(ustr);
}


std::string utf32_to_utf8(const p_str& utf32) 
{
    const UChar32* src = reinterpret_cast<const UChar32*>(utf32.data());
    icu::UnicodeString ustr = icu::UnicodeString::fromUTF32(src, static_cast<int32_t>(utf32.size()));
    std::string utf8;
    ustr.toUTF8String(utf8);
    return utf8;
}


std::wstring utf32_to_utf16(const p_str& utf32)
{
    std::wstring result;
    result.reserve(utf32.size() * 2);
    
    for (p_char cp : utf32) {
        if (cp <= 0xFFFF) {
            if (cp >= 0xD800 && cp <= 0xDFFF) {
                result.push_back(static_cast<wchar_t>(0xFFFD));
            }
            else {
                result.push_back(static_cast<wchar_t>(cp));
            }
        } 
        else if (cp <= 0x10FFFF) {
            cp -= 0x10000;
            result.push_back(static_cast<wchar_t>(0xD800 | (cp >> 10)));
            result.push_back(static_cast<wchar_t>(0xDC00 | (cp & 0x3FF)));
        } 
        else {
            result.push_back(static_cast<wchar_t>(0xFFFD));
        }
    }

    return result;
}


p_str utf16_to_utf32(const std::wstring& utf16)
{
    p_str result;
    result.reserve(utf16.size());

    for (p_size i = 0; i < utf16.size(); ++i) {
        p_char cp = static_cast<p_char>(utf16[i]);

        if (cp >= 0xD800 && cp <= 0xDBFF) {
            if (i + 1 < utf16.size()) {
                p_char low = static_cast<p_char>(utf16[i + 1]);

                if (low >= 0xDC00 && low <= 0xDFFF) {
                    cp = 0x10000
                       + ((cp - 0xD800) << 10)
                       + (low - 0xDC00);

                    ++i;
                }
                else {
                    cp = 0xFFFD;
                }
            }
            else {
                cp = 0xFFFD;
            }
        }
        else if (cp >= 0xDC00 && cp <= 0xDFFF) {
            cp = 0xFFFD;
        }

        result.push_back(cp);
    }

    return result;
}


p_str toLowercase(const p_str& input)
{
    const icu::Locale& locale = icu::Locale::getRoot();
    icu::UnicodeString ustr = utf32_to_unicode(input);
    ustr.toLower(locale);
    return unicode_to_utf32(ustr);
}


p_str toUppercase(const p_str& input)
{
    const icu::Locale& locale = icu::Locale::getRoot();
    icu::UnicodeString ustr = utf32_to_unicode(input);
    ustr.toUpper(locale);
    return unicode_to_utf32(ustr);
}


icu::UnicodeString utf32_to_unicode(const p_str& utf32)
{
    const UChar32* src = reinterpret_cast<const UChar32*>(utf32.data());

    icu::UnicodeString ustr = icu::UnicodeString::fromUTF32(src, 
        static_cast<int32_t>(utf32.size())
    );
    
    return ustr;
}


p_str unicode_to_utf32(const icu::UnicodeString& unicode)
{
    int32_t length = unicode.countChar32();
    p_str result;
    result.resize(length);
    UErrorCode error = U_ZERO_ERROR;
    unicode.toUTF32(reinterpret_cast<UChar32*>(result.data()), length, error);
    return result;
}


std::optional<p_list> toGraphemes(const p_str& value, icu::BreakIterator& iterator)
{
    p_list result;

    icu::UnicodeString ustr = icu::UnicodeString::fromUTF32(
        reinterpret_cast<const UChar32*>(value.data()),
        static_cast<int32_t>(value.size())
    );

    iterator.setText(ustr);
    int32_t start = iterator.first();

    for (int32_t end = iterator.next(); end != icu::BreakIterator::DONE; start = end, end = iterator.next())
    {
        icu::UnicodeString cluster = ustr.tempSubStringBetween(start, end);
        result.emplace_back(unicode_to_utf32(cluster));
    }

    return result;
}


p_bool hasOnlyOneCharGraphemes(const p_str& value)
{
    // this implementation is leaky and has false negatives
    // but it does not matter - it must be fast so simple strings are quickly detected
    // this function is used only for optimization detection
    for (const p_char c : value) {
        if (c <= 0x7F) {
            continue;
        }

        if (c == 0x200D) {
            return false;
        }

        if ((c >= 0x300 && c <= 0x36F) ||
            (c >= 0x1AB0 && c <= 0x1AFF) ||
            (c >= 0x1DC0 && c <= 0x1DFF) ||
            (c >= 0x20D0 && c <= 0x20FF) ||
            (c >= 0xFE20 && c <= 0xFE2F))
        {
            return false;
        }

        if ((c >= 0xFE00 && c <= 0xFE0F) ||
            (c >= 0xE0100 && c <= 0xE01EF))
        {
            return false;
        }
    }

    return true;
}


p_bool isLetterGrapheme(const p_str& grapheme)
{
    for (p_char c : grapheme) {
        if (u_isalpha(c)) {
            return true;
        }
    }

    return false;
}

}

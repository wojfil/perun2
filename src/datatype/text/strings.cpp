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

#include "strings.h"

namespace perun2
{

const p_list ROMAN_STRING_LITERALS = 
{ 
   L"I", L"IV", L"V", L"IX", L"X", L"XL", L"L", L"XC", L"C", L"CD", L"D", L"CM", L"M",
   (L"I" L"̅" L"V" L"̅"), (L"V" L"̅"), (L"I" L"̅" L"X" L"̅"), (L"X" L"̅"),
   (L"X" L"̅" L"L" L"̅"), (L"L" L"̅"), (L"X" L"̅" L"C" L"̅"),
   (L"C" L"̅"), (L"C" L"̅" L"D" L"̅"), (L"D" L"̅"),
   (L"C" L"̅" L"M" L"̅"), (L"M" L"̅")
};

const p_list STRINGS_ASCII = 
{
   L" ", L" ", L" ", L" ", L" ", L" ", L" ", L" ", L" ", L" ", L" ", L" ", L" ", L" ", L" ", L" ",
   L" ", L" ", L" ", L" ", L" ", L" ", L" ", L" ", L" ", L" ", L" ", L" ", L" ", L" ", L" ", L" ",
   L" ", L"!", L"\"", L"#", L"$", L"%", L"&", L"'", L"(", L")", L"*", L"+", L",", L"-", L".", L"/",
   L"0", L"1", L"2", L"3", L"4", L"5", L"6", L"7", L"8", L"9", L":", L";", L"<", L"=", L">", L"?",
   L"@", L"A", L"B", L"C", L"D", L"E", L"F", L"G", L"H", L"I", L"J", L"K", L"L", L"M", L"N", L"O",
   L"P", L"Q", L"R", L"S", L"T", L"U", L"V", L"W", L"X", L"Y", L"Z", L"[", L"\\", L"]", L"^", L"_",
   L"`", L"a", L"b", L"c", L"d", L"e", L"f", L"g", L"h", L"i", L"j", L"k", L"l", L"m", L"n", L"o",
   L"p", L"q", L"r", L"s", L"t", L"u", L"v", L"w", L"x", L"y", L"z", L"{", L"|", L"}", L"~", L" "
};

p_str toStr(const p_char ch) 
{
   return p_str(1, ch);
}

p_ndouble stringToDouble(const p_str& value)
{
   p_stream ss(value);
   p_ndouble n;
   ss >> n;
   return n;
}

void str_toLower(p_str& value)
{
   for (p_char& ch : value) {
      ch = std::tolower(ch, std::locale(""));
   }
}

void str_toUpper(p_str& value)
{
   for (p_char& ch : value) {
      ch = std::toupper(ch, std::locale(""));
   }
}

void str_trim(p_str& value)
{
   if (value.empty()) {
      return;
   }

   p_int left;
   for (left = 0; left < value.size(); left++) {
      if (! char_isSpace(value[left])) {
         break;
      }
   }

   if (left == value.size()) {
      value.clear();
      return;
   }

   p_int right;
   for (right = value.size() - 1; right >= 0; --right) {
      if (! char_isSpace(value[right])) {
         break;
      }
   }

   value = right == value.size() - 1
      ? value.substr(left)
      : value.substr(left, right - left + 1);
}

void str_trimEndNewLines(p_str& value)
{
   if (value.empty()) {
      return;
   }

   p_int i = value.size() - 1;

   for (; i >= 0; i--) {
      if (value[i] != L'\r' && value[i] != L'\n') {
         break;
      }
   }

   if (i != static_cast<p_int>(value.size() - 1)) {
      value = value.substr(0, i + 1);
   }
}

p_bool str_startsWith(const p_str& value, const p_str& phrase) 
{
   if (value.size() < phrase.size()) {
      return false;
   }

   for (p_size i = 0; i < phrase.size(); i++) {
      if (value[i] != phrase[i]) {
         return false;
      }
   }

   return true;
}

const p_list STRINGS_MONTHS = 
{
   STRING_JANUARY, STRING_FEBRUARY, STRING_MARCH,
   STRING_APRIL, STRING_MAY, STRING_JUNE,
   STRING_JULY, STRING_AUGUST, STRING_SEPTEMBER,
   STRING_OCTOBER, STRING_NOVEMBER, STRING_DECEMBER
};

const p_list STRINGS_WEEKDAYS = 
{
   STRING_MONDAY, STRING_TUESDAY, STRING_WEDNESDAY,
   STRING_THURSDAY, STRING_FRIDAY,
   STRING_SATURDAY, STRING_SUNDAY
};

const p_list STRINGS_PERIOD_SINGLE = 
{
   STRING_YEAR, STRING_MONTH, STRING_WEEK, STRING_DAY,
   STRING_HOUR, STRING_MINUTE, STRING_SECOND
};

const p_list STRINGS_PERIOD_MULTI = 
{
   STRING_YEARS, STRING_MONTHS, STRING_WEEKS, STRING_DAYS,
   STRING_HOURS, STRING_MINUTES, STRING_SECONDS
};

const p_list STRINGS_ATTR = 
{
   STRING_ACCESS, STRING_ARCHIVE, STRING_CHANGE, STRING_COMPRESSED,
   STRING_CREATION, STRING_DEPTH, STRING_DRIVE, STRING_EMPTY, STRING_EXISTS,
   STRING_ENCRYPTED, STRING_EXTENSION, STRING_FULLNAME,
   STRING_HIDDEN, STRING_ISDIRECTORY, STRING_ISFILE, STRING_LIFETIME,
   STRING_MODIFICATION, STRING_NAME, STRING_PARENT, STRING_PATH,
   STRING_READONLY, STRING_SIZE, 
   STRING_WIDTH, STRING_HEIGHT, STRING_DURATION, STRING_ISIMAGE, STRING_ISVIDEO
};

const p_list STRINGS_TIME_ATTR = 
{
   STRING_ACCESS, STRING_CHANGE, STRING_CREATION, STRING_MODIFICATION
};

const p_list STRINGS_TIME_VAR = 
{
   STRING_ACCESS, STRING_CHANGE, STRING_CREATION, STRING_MODIFICATION,
   STRING_NOW, STRING_TODAY
};

const p_list STRINGS_ALTERABLE_ATTR = 
{
   STRING_ACCESS, STRING_ARCHIVE, STRING_CHANGE, STRING_COMPRESSED,
   STRING_CREATION, STRING_EMPTY, STRING_EXISTS, STRING_ENCRYPTED,
   STRING_HIDDEN, STRING_ISDIRECTORY, STRING_ISFILE, STRING_LIFETIME,
   STRING_MODIFICATION, STRING_READONLY, STRING_SIZE, 
   STRING_ISIMAGE, STRING_ISVIDEO, STRING_WIDTH, STRING_HEIGHT, STRING_DURATION
};

const p_list STRINGS_VARS_IMMUTABLES = 
{
   STRING_THIS, STRING_ACCESS, STRING_ARCHIVE, STRING_CHANGE,
   STRING_COMPRESSED, STRING_CREATION, STRING_DEPTH, STRING_DRIVE,
   STRING_EMPTY, STRING_ENCRYPTED, STRING_EXISTS, STRING_EXTENSION,
   STRING_FULLNAME, STRING_HIDDEN, STRING_INDEX, STRING_ISDIRECTORY,
   STRING_ISFILE, STRING_LIFETIME, STRING_LOCATION, STRING_MODIFICATION,
   STRING_NAME, STRING_PARENT, STRING_PATH, STRING_READONLY, 
   STRING_SIZE, STRING_SUCCESS, STRING_NOW, STRING_TODAY,
   STRING_YESTERDAY, STRING_TOMORROW, STRING_JANUARY, STRING_FEBRUARY,
   STRING_MARCH, STRING_APRIL, STRING_MAY, STRING_JUNE, STRING_JULY,
   STRING_AUGUST, STRING_SEPTEMBER, STRING_OCTOBER, STRING_NOVEMBER,
   STRING_DECEMBER, STRING_MONDAY, STRING_TUESDAY, STRING_WEDNESDAY,
   STRING_THURSDAY, STRING_FRIDAY, STRING_SATURDAY, STRING_SUNDAY,
   STRING_ALPHABET, STRING_ASCII, STRING_ARGUMENTS, STRING_DESKTOP,
   STRING_PERUN2, STRING_ORIGIN, STRING_DIRECTORIES,
   STRING_FILES, STRING_RECURSIVEFILES, STRING_RECURSIVEDIRECTORIES,
   STRING_WIDTH, STRING_HEIGHT, STRING_DURATION, STRING_ISIMAGE, STRING_ISVIDEO,
   STRING_VIDEOS, STRING_RECURSIVEVIDEOS, STRING_IMAGES, STRING_RECURSIVEIMAGES
};

const p_list STRINGS_FUNC_BOO_STR = 
{
   STRING_ISLOWER, STRING_ISUPPER, STRING_ISNUMBER,
   STRING_ISLETTER, STRING_ISDIGIT, STRING_ISBINARY, STRING_ISHEX
};

const p_list STRINGS_FUNC_NUM_NUM = 
{
   STRING_ABSOLUTE, STRING_CEIL, STRING_FLOOR,
   STRING_ROUND, STRING_SIGN, STRING_SQRT, STRING_TRUNCATE
};

const p_list STRINGS_AGGRFUNC = 
{
   STRING_AVERAGE, STRING_SUM, STRING_MIN, STRING_MAX, STRING_MEDIAN
};

const p_list STRINGS_FUNC_STR_STR = 
{
   STRING_DIGITS,  STRING_LETTERS, STRING_LOWER, STRING_TRIM,
   STRING_UPPER, STRING_REVERSE, STRING_AFTERDIGITS, STRING_AFTERLETTERS,
   STRING_BEFOREDIGITS, STRING_BEFORELETTERS, STRING_CAPITALIZE, STRING_PARENT,
   STRING_RAW
};

const p_list STRINGS_FUNC_STR_STR_NUM = 
{
   STRING_REPEAT, STRING_LEFT, STRING_RIGHT, STRING_FILL
};

const p_list STRINGS_FUNC_STR_NUM = 
{
   STRING_ROMAN, STRING_BINARY, STRING_HEX
};

const p_list STRINGS_FUNC_TIM_NUM = 
{
   STRING_CHRISTMAS, STRING_EASTER, STRING_NEWYEAR
};

}

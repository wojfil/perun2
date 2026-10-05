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
#include "../../unicode/convert.h"
#include <charconv>

namespace perun2
{

const p_list ROMAN_STRING_LITERALS = 
{ 
   U"I", U"IV", U"V", U"IX", U"X", U"XL", U"L", U"XC", U"C", U"CD", U"D", U"CM", U"M",
   (U"I" U"\u0305" U"V" U"\u0305"), (U"V" U"\u0305"), (U"I" U"\u0305" U"X" U"\u0305"), (U"X" U"\u0305"),
   (U"X" U"\u0305" U"L" U"\u0305"), (U"L" U"\u0305"), (U"X" U"\u0305" U"C" U"\u0305"),
   (U"C" U"\u0305"), (U"C" U"\u0305" U"D" U"\u0305"), (U"D" U"\u0305"),
   (U"C" U"\u0305" U"M" U"\u0305"), (U"M" U"\u0305")
};

const p_list STRINGS_ASCII = 
{
   U" ", U" ", U" ", U" ", U" ", U" ", U" ", U" ", U" ", U" ", U" ", U" ", U" ", U" ", U" ", U" ",
   U" ", U" ", U" ", U" ", U" ", U" ", U" ", U" ", U" ", U" ", U" ", U" ", U" ", U" ", U" ", U" ",
   U" ", U"!", U"\"", U"#", U"$", U"%", U"&", U"'", U"(", U")", U"*", U"+", U",", U"-", U".", U"/",
   U"0", U"1", U"2", U"3", U"4", U"5", U"6", U"7", U"8", U"9", U":", U";", U"<", U"=", U">", U"?",
   U"@", U"A", U"B", U"C", U"D", U"E", U"F", U"G", U"H", U"I", U"J", U"K", U"U", U"M", U"N", U"O",
   U"P", U"Q", U"R", U"S", U"T", U"U", U"V", U"W", U"X", U"Y", U"Z", U"[", U"\\", U"]", U"^", U"_",
   U"`", U"a", U"b", U"c", U"d", U"e", U"f", U"g", U"h", U"i", U"j", U"k", U"l", U"m", U"n", U"o",
   U"p", U"q", U"r", U"s", U"t", U"u", U"v", U"w", U"x", U"y", U"z", U"{", U"|", U"}", U"~", U" "
};

p_str intToString(const p_nint value)
{
   char buf[64];
   auto [ptr, ec] = std::to_chars(buf, buf + sizeof(buf), value);

   if (ec != std::errc{}) {
      return {};
   }

   std::u32string result;
   result.reserve(ptr - buf);
   for (char* p = buf; p != ptr; ++p) {
      result.push_back(static_cast<char32_t>(*p));
   }

   return result;
};

p_str doubleToString(const p_ndouble value)
{
   p_ostream stream;
   stream << std::fixed << value;
   const p_str str = utf8_to_utf32(stream.str());
   const p_size len = str.size();

   for (p_int i = len - 1; i >= 0; i--)  {
      const p_char ch = str[i];
      if (ch != CHAR_0) {
         if (ch == CHAR_DOT) {
            return i == 0
               ? charToString(CHAR_0)
               : str.substr(0, i);
         }
         else {
            for (p_size j = 0; j < len; j++) {
               if (str[j] == CHAR_DOT) {
                  return str.substr(0, i + 1);
               }
            }

            return str;
         }
      }
   }

   return charToString(CHAR_0);
};

p_str charToString(const p_char value)
{
   return p_str(1, value);
};

p_ndouble stringToDouble(const p_str& value)
{
   p_stream ss(utf32_to_utf8(value));
   p_ndouble n;
   ss >> n;
   return n;
}

void str_toLower(p_str& value)
{
   value = toLowercase(value);
}

void str_toUpper(p_str& value)
{
   value = toUppercase(value);
}

void str_trim(p_str& value)
{
   if (value.empty()) {
      return;
   }

   p_size left = 0;
   for (; left < value.size(); left++) {
      if (! char_isSpace(value[left])) {
         break;
      }
   }

   if (left == value.size()) {
      value.clear();
      return;
   }

   p_int right;
   for (right = static_cast<p_int>(value.size() - 1); right >= 0; --right) {
      if (! char_isSpace(value[static_cast<p_size>(right)])) {
         break;
      }
   }

   value = static_cast<p_size>(right) == (value.size() - 1)
      ? value.substr(left)
      : value.substr(left, static_cast<p_size>(right) + 1 - left);
}

void str_trimEndNewLines(p_str& value)
{
   if (value.empty()) {
      return;
   }

   p_int i = value.size() - 1;

   for (; i >= 0; i--) {
      if (value[i] != U'\r' && value[i] != U'\n') {
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

const p_str ROMAN_VINCULUM_THOUSAND =     U"I" U"\u0305";

const p_str STRING_POPUP_TITLE =          U"Perun2";
const p_str STRING_GOOD =                 U"good";

const p_str EMPTY_STRING =                U"";
const p_str STRING_NO_PERIOD =            U"no period";
const p_str STRING_NOTHING =              U"nothing";
const p_str STRING_NEVER =                U"never";
const p_str STRING_DOWNLOADS =            U"downloads";

const p_str STRING_ARG_VERSION =          U"--version";
const p_str STRING_ARG_DOCS =             U"--docs";
const p_str STRING_ARG_WEBSITE =          U"--website";
const p_str STRING_ARG_HELP =             U"--help";

const p_str STRING_ICON_SUFFIX =          U".ico";

const p_str STRING_PENDRIVE =             U"pendrive";
const p_str STRING_PENDRIVES =            U"pendrives";
const p_str STRING_PROGRAMS =             U"programs";

const p_str STRING_WEEKDAY_MONDAY =       U"Monday";
const p_str STRING_WEEKDAY_TUESDAY =      U"Tuesday";
const p_str STRING_WEEKDAY_WEDNESDAY =    U"Wednesday";
const p_str STRING_WEEKDAY_THURSDAY =     U"Thursday";
const p_str STRING_WEEKDAY_FRIDAY =       U"Friday";
const p_str STRING_WEEKDAY_SATURDAY =     U"Saturday";
const p_str STRING_WEEKDAY_SUNDAY =       U"Sunday";

const p_str STRING_MONTH_JANUARY =        U"January";
const p_str STRING_MONTH_FEBRUARY =       U"February";
const p_str STRING_MONTH_MARCH =          U"March";
const p_str STRING_MONTH_APRIL =          U"April";
const p_str STRING_MONTH_MAY =            U"May";
const p_str STRING_MONTH_JUNE =           U"June";
const p_str STRING_MONTH_JULY =           U"July";
const p_str STRING_MONTH_AUGUST =         U"August";
const p_str STRING_MONTH_SEPTEMBER =      U"September";
const p_str STRING_MONTH_OCTOBER =        U"October";
const p_str STRING_MONTH_NOVEMBER =       U"November";
const p_str STRING_MONTH_DECEMBER =       U"December";

const p_str STRING_BOOL =                 U"bool";
const p_str STRING_PERIOD =               U"period";
const p_str STRING_DEFINITION =           U"definition";
const p_str STRING_NUMERIC_LIST =         U"numeric list";
const p_str STRING_TIME_LIST =            U"time list";
const p_str STRING_LIST =                 U"list";

const p_str STRING_NAN =                  U"nan";
const p_str STRING_PRINTABLE_NAN =        U"NaN";

const p_str STRING_THIS =                 U"this";
const p_str STRING_ACCESS =               U"access";
const p_str STRING_ARCHIVE =              U"archive";
const p_str STRING_CHANGE =               U"change";
const p_str STRING_COMPRESSED =           U"compressed";
const p_str STRING_CREATION =             U"creation";
const p_str STRING_DEPTH =                U"depth";
const p_str STRING_DRIVE =                U"drive";
const p_str STRING_DURATION =             U"duration";
const p_str STRING_EMPTY =                U"empty";
const p_str STRING_ENCRYPTED =            U"encrypted";
const p_str STRING_EXISTS =               U"exists";
const p_str STRING_EXTENSION =            U"extension";
const p_str STRING_FULLNAME =             U"fullname";
const p_str STRING_HEIGHT =               U"height";
const p_str STRING_HIDDEN =               U"hidden";
const p_str STRING_INDEX =                U"index";
const p_str STRING_ISDIRECTORY =          U"isdirectory";
const p_str STRING_ISFILE =               U"isfile";
const p_str STRING_ISIMAGE =              U"isimage";
const p_str STRING_ISVIDEO =              U"isvideo";
const p_str STRING_LIFETIME =             U"lifetime";
const p_str STRING_LOCATION =             U"location";
const p_str STRING_MODIFICATION =         U"modification";
const p_str STRING_NAME =                 U"name";
const p_str STRING_PARENT =               U"parent";
const p_str STRING_PATH =                 U"path";
const p_str STRING_READONLY =             U"readonly";
const p_str STRING_SIZE =                 U"size";
const p_str STRING_WIDTH =                U"width";

const p_str STRING_SUCCESS =              U"success";
const p_str STRING_NOW =                  U"now";
const p_str STRING_TODAY =                U"today";
const p_str STRING_YESTERDAY =            U"yesterday";
const p_str STRING_TOMORROW =             U"tomorrow";
const p_str STRING_JANUARY =              U"january";
const p_str STRING_FEBRUARY =             U"february";
const p_str STRING_MARCH =                U"march";
const p_str STRING_APRIL =                U"april";
const p_str STRING_MAY =                  U"may";
const p_str STRING_JUNE =                 U"june";
const p_str STRING_JULY =                 U"july";
const p_str STRING_AUGUST =               U"august";
const p_str STRING_SEPTEMBER =            U"september";
const p_str STRING_OCTOBER =              U"october";
const p_str STRING_NOVEMBER =             U"november";
const p_str STRING_DECEMBER =             U"december";
const p_str STRING_MONDAY =               U"monday";
const p_str STRING_TUESDAY =              U"tuesday";
const p_str STRING_WEDNESDAY =            U"wednesday";
const p_str STRING_THURSDAY =             U"thursday";
const p_str STRING_FRIDAY =               U"friday";
const p_str STRING_SATURDAY =             U"saturday";
const p_str STRING_SUNDAY =               U"sunday";
const p_str STRING_ALPHABET =             U"alphabet";
const p_str STRING_ASCII =                U"ascii";
const p_str STRING_ARGUMENTS =            U"arguments";
const p_str STRING_DESKTOP =              U"desktop";
const p_str STRING_PERUN2 =               U"perun2";
const p_str STRING_ORIGIN =               U"origin";
const p_str STRING_DIRECTORIES =          U"directories";
const p_str STRING_FILES =                U"files";
const p_str STRING_IMAGES =               U"images";
const p_str STRING_VIDEOS =               U"videos";
const p_str STRING_RECURSIVEDIRECTORIES = U"recursivedirectories";
const p_str STRING_RECURSIVEFILES =       U"recursivefiles";
const p_str STRING_RECURSIVEIMAGES =      U"recursiveimages";
const p_str STRING_RECURSIVEVIDEOS =      U"recursivevideos";
const p_str STRING_YEAR =                 U"year";
const p_str STRING_MONTH =                U"month";
const p_str STRING_WEEK =                 U"week";
const p_str STRING_DAY =                  U"day";
const p_str STRING_HOUR =                 U"hour";
const p_str STRING_MINUTE =               U"minute";
const p_str STRING_SECOND =               U"second";
const p_str STRING_YEARS =                U"years";
const p_str STRING_MONTHS =               U"months";
const p_str STRING_WEEKS =                U"weeks";
const p_str STRING_DAYS =                 U"days";
const p_str STRING_HOURS =                U"hours";
const p_str STRING_MINUTES =              U"minutes";
const p_str STRING_SECONDS =              U"seconds";
const p_str STRING_DATE =                 U"date";
const p_str STRING_WEEKDAY =              U"weekday";
const p_str STRING_WEEKDAY_CAMELCASE =    U"weekDay";
const p_str STRING_ISLOWER =              U"islower";
const p_str STRING_ISUPPER =              U"isupper";
const p_str STRING_ISNUMBER =             U"isnumber";
const p_str STRING_ISLETTER =             U"isletter";
const p_str STRING_ISDIGIT =              U"isdigit";
const p_str STRING_ISBINARY =             U"isbinary";
const p_str STRING_ISHEX =                U"ishex";
const p_str STRING_ANYINSIDE =            U"anyinside";
const p_str STRING_ANY =                  U"any";
const p_str STRING_EXIST =                U"exist";
const p_str STRING_COUNT =                U"count";
const p_str STRING_CONTAINS =             U"contains";
const p_str STRING_EXISTSINSIDE =         U"existsinside";
const p_str STRING_EXISTINSIDE =          U"existinside";
const p_str STRING_STARTSWITH =           U"startswith";
const p_str STRING_ENDSWITH =             U"endswith";
const p_str STRING_FINDTEXT =             U"findtext";
const p_str STRING_ABSOLUTE =             U"absolute";
const p_str STRING_CEIL =                 U"ceil";
const p_str STRING_FLOOR =                U"floor";
const p_str STRING_ROUND =                U"round";
const p_str STRING_SIGN =                 U"sign";
const p_str STRING_SQRT =                 U"sqrt";
const p_str STRING_TRUNCATE =             U"truncate";
const p_str STRING_AVERAGE =              U"average";
const p_str STRING_SUM =                  U"sum";
const p_str STRING_MIN =                  U"min";
const p_str STRING_MAX =                  U"max";
const p_str STRING_MEDIAN =               U"median";
const p_str STRING_LENGTH =               U"length";
const p_str STRING_FROMBINARY =           U"frombinary";
const p_str STRING_FROMHEX =              U"fromhex";
const p_str STRING_NUMBER =               U"number";
const p_str STRING_COUNTINSIDE =          U"countinside";
const p_str STRING_POWER =                U"power";
const p_str STRING_FIRST =                U"first";
const p_str STRING_LAST =                 U"last";
const p_str STRING_RANDOM =               U"random";
const p_str STRING_AFTER =                U"after";
const p_str STRING_BEFORE =               U"before";
const p_str STRING_REVERSED =             U"reversed";
const p_str STRING_DIGITS =               U"digits";
const p_str STRING_LETTERS =              U"letters";
const p_str STRING_LOWER =                U"lower";
const p_str STRING_TRIM =                 U"trim";
const p_str STRING_UPPER =                U"upper";
const p_str STRING_REVERSE =              U"reverse";
const p_str STRING_AFTERDIGITS =          U"afterdigits";
const p_str STRING_AFTERLETTERS =         U"afterletters";
const p_str STRING_BEFOREDIGITS =         U"beforedigits";
const p_str STRING_BEFORELETTERS =        U"beforeletters";
const p_str STRING_CAPITALIZE =           U"capitalize";
const p_str STRING_REPLACE =              U"replace";
const p_str STRING_SUBSTRING =            U"substring";
const p_str STRING_CONCATENATE =          U"concatenate";
const p_str STRING_STRING =               U"string";
const p_str STRING_MONTHNAME =            U"monthname";
const p_str STRING_WEEKDAYNAME =          U"weekdayname";
const p_str STRING_JOIN =                 U"join";
const p_str STRING_REPEAT =               U"repeat";
const p_str STRING_LEFT =                 U"left";
const p_str STRING_RIGHT =                U"right";
const p_str STRING_FILL =                 U"fill";
const p_str STRING_ROMAN =                U"roman";
const p_str STRING_BINARY =               U"binary";
const p_str STRING_HEX =                  U"hex";
const p_str STRING_CHRISTMAS =            U"christmas";
const p_str STRING_EASTER =               U"easter";
const p_str STRING_NEWYEAR =              U"newyear";
const p_str STRING_TIME =                 U"time";
const p_str STRING_CHARACTERS =           U"characters";
const p_str STRING_WORDS =                U"words";
const p_str STRING_SPLIT =                U"split";
const p_str STRING_NUMBERS =              U"numbers";
const p_str STRING_SHIFTMONTH =           U"shiftmonth";
const p_str STRING_SHIFTWEEKDAY =         U"shiftweekday";
const p_str STRING_ISNAN =                U"isnan";
const p_str STRING_ISNEVER =              U"isnever";
const p_str STRING_CLOCK =                U"clock";
const p_str STRING_RAW =                  U"raw";
const p_str STRING_RESEMBLANCE =          U"resemblance";
const p_str STRING_ASKPYTHON =            U"askpython";
const p_str STRING_ASKPYTHON3 =           U"askpython3";
const p_str STRING_EXECUTE =              U"execute";

const p_str STRING_COPY =                 U"copy";
const p_str STRING_CREATE =               U"create";
const p_str STRING_CREATEFILE =           U"createfile";
const p_str STRING_CREATEDIRECTORY =      U"createdirectory";
const p_str STRING_CREATEFILES =          U"createfiles";
const p_str STRING_CREATEDIRECTORIES =    U"createdirectories";
const p_str STRING_DELETE =               U"delete";
const p_str STRING_DROP =                 U"drop";
const p_str STRING_HIDE =                 U"hide";
const p_str STRING_LOCK =                 U"lock";
const p_str STRING_MOVE =                 U"move";
const p_str STRING_OPEN =                 U"open";
const p_str STRING_REACCESS =             U"reaccess";
const p_str STRING_RECREATE =             U"recreate";
const p_str STRING_RECHANGE =             U"rechange";
const p_str STRING_REMODIFY =             U"remodify";
const p_str STRING_RENAME =               U"rename";
const p_str STRING_SELECT =               U"select";
const p_str STRING_UNHIDE =               U"unhide";
const p_str STRING_UNLOCK =               U"unlock";
const p_str STRING_POPUP =                U"popup";
const p_str STRING_FORCE =                U"force";
const p_str STRING_STACK =                U"stack";
const p_str STRING_TRUE =                 U"true";
const p_str STRING_FALSE =                U"false";
const p_str STRING_AND =                  U"and";
const p_str STRING_OR =                   U"or";
const p_str STRING_XOR =                  U"xor";
const p_str STRING_NOT =                  U"not";
const p_str STRING_PRINT =                U"print";
const p_str STRING_RUN =                  U"run";
const p_str STRING_SLEEP =                U"sleep";
const p_str STRING_IN =                   U"in";
const p_str STRING_LIKE =                 U"like";
const p_str STRING_RESEMBLES =            U"resembles";
const p_str STRING_REGEXP =               U"regexp";
const p_str STRING_BETWEEN =              U"between";
const p_str STRING_ELSE =                 U"else";
const p_str STRING_IF =                   U"if";
const p_str STRING_FOREACH =              U"foreach";
const p_str STRING_INSIDE =               U"inside";
const p_str STRING_TIMES =                U"times";
const p_str STRING_WHILE =                U"while";
const p_str STRING_EVERY =                U"every";
const p_str STRING_FINAL =                U"final";
const p_str STRING_LIMIT =                U"limit";
const p_str STRING_ORDER =                U"order";
const p_str STRING_SKIP =                 U"skip";
const p_str STRING_WHERE =                U"where";
const p_str STRING_FROM =                 U"from";
const p_str STRING_AS =                   U"as";
const p_str STRING_BY =                   U"by";
const p_str STRING_TO =                   U"to";
const p_str STRING_EXTENSIONLESS =        U"extensionless";
const p_str STRING_WITH =                 U"with";
const p_str STRING_ASC =                  U"asc";
const p_str STRING_DESC =                 U"desc";
const p_str STRING_BREAK =                U"break";
const p_str STRING_CONTINUE =             U"continue";
const p_str STRING_EXIT =                 U"exit";
const p_str STRING_ERROR =                U"error";
const p_str STRING_PYTHON =               U"python";
const p_str STRING_PYTHON3 =              U"python3";

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

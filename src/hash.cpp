/*
    This file is part of Uroboros.
    Uroboros is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    Uroboros is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with Uroboros. If not, see <http://www.gnu.org/licenses/>.
*/

#include "hash.h"
#include <unordered_map>
#include <algorithm>

// case-insensitive hash of a string
// not cryptographically secure, but works efficiently
_size stringHash(const _str& value)
{
   _str lower;
   lower.resize(value.size());
   std::transform(value.begin(), value.end(), lower.begin(), ::towlower);
   return std::hash<_str>{}(lower);
}

// call this if the string is guaranteed to be lowercase
_size rawStringHash(const _str& value)
{
   return std::hash<_str>{}(value);
}

// as explained in 'hash.h'
Hashes::Hashes()
{
   this->HASH_NOTHING = rawStringHash(L"");
   this->HASH_VAR_THIS = rawStringHash(L"this");

   this->HASH_VAR_ACCESS = rawStringHash(L"access");
   this->HASH_VAR_ARCHIVE = rawStringHash(L"archive");
   this->HASH_VAR_CHANGE = rawStringHash(L"change");
   this->HASH_VAR_COMPRESSED = rawStringHash(L"compressed");
   this->HASH_VAR_CREATION = rawStringHash(L"creation");
   this->HASH_VAR_DEPTH = rawStringHash(L"depth");
   this->HASH_VAR_DRIVE = rawStringHash(L"drive");
   this->HASH_VAR_EMPTY = rawStringHash(L"empty");
   this->HASH_VAR_ENCRYPTED = rawStringHash(L"encrypted");
   this->HASH_VAR_EXISTS = rawStringHash(L"exists");
   this->HASH_VAR_EXTENSION = rawStringHash(L"extension");
   this->HASH_VAR_FULLNAME = rawStringHash(L"fullname");
   this->HASH_VAR_HIDDEN = rawStringHash(L"hidden");
   this->HASH_VAR_INDEX = rawStringHash(L"index");
   this->HASH_VAR_ISDIRECTORY = rawStringHash(L"isdirectory");
   this->HASH_VAR_ISFILE = rawStringHash(L"isfile");
   this->HASH_VAR_LIFETIME = rawStringHash(L"lifetime");
   this->HASH_VAR_LOCATION = rawStringHash(L"location");
   this->HASH_VAR_MODIFICATION = rawStringHash(L"modification");
   this->HASH_VAR_NAME = rawStringHash(L"name");
   this->HASH_VAR_PARENT = rawStringHash(L"parent");
   this->HASH_VAR_PATH = rawStringHash(L"path");
   this->HASH_VAR_READONLY = rawStringHash(L"readonly");
   this->HASH_VAR_SIZE = rawStringHash(L"size");
   this->HASH_VAR_SUCCESS = rawStringHash(L"success");

   this->HASH_VAR_NOW = rawStringHash(L"now");
   this->HASH_VAR_TODAY = rawStringHash(L"today");
   this->HASH_VAR_YESTERDAY = rawStringHash(L"yesterday");
   this->HASH_VAR_TOMORROW = rawStringHash(L"tomorrow");

   this->HASH_VAR_JANUARY = rawStringHash(L"january");
   this->HASH_VAR_FEBRUARY = rawStringHash(L"february");
   this->HASH_VAR_MARCH = rawStringHash(L"march");
   this->HASH_VAR_APRIL = rawStringHash(L"april");
   this->HASH_VAR_MAY = rawStringHash(L"may");
   this->HASH_VAR_JUNE = rawStringHash(L"june");
   this->HASH_VAR_JULY = rawStringHash(L"july");
   this->HASH_VAR_AUGUST = rawStringHash(L"august");
   this->HASH_VAR_SEPTEMBER = rawStringHash(L"september");
   this->HASH_VAR_OCTOBER = rawStringHash(L"october");
   this->HASH_VAR_NOVEMBER = rawStringHash(L"november");
   this->HASH_VAR_DECEMBER = rawStringHash(L"december");

   this->HASH_VAR_MONDAY = rawStringHash(L"monday");
   this->HASH_VAR_TUESDAY = rawStringHash(L"tuesday");
   this->HASH_VAR_WEDNESDAY = rawStringHash(L"wednesday");
   this->HASH_VAR_THURSDAY = rawStringHash(L"thursday");
   this->HASH_VAR_FRIDAY = rawStringHash(L"friday");
   this->HASH_VAR_SATURDAY = rawStringHash(L"saturday");
   this->HASH_VAR_SUNDAY = rawStringHash(L"sunday");

   this->HASH_VAR_ALPHABET = rawStringHash(L"alphabet");
   this->HASH_VAR_ASCII = rawStringHash(L"ascii");
   this->HASH_VAR_ARGUMENTS = rawStringHash(L"arguments");

   this->HASH_VAR_DESKTOP = rawStringHash(L"desktop");
   this->HASH_VAR_UROBOROS = rawStringHash(L"uroboros");

   this->HASH_VAR_DIRECTORIES = rawStringHash(L"directories");
   this->HASH_VAR_FILES = rawStringHash(L"files");
   this->HASH_VAR_RECURSIVEFILES = rawStringHash(L"recursivefiles");
   this->HASH_VAR_RECURSIVEDIRECTORIES = rawStringHash(L"recursivedirectories");
   this->HASH_VAR_FILE = rawStringHash(L"file");
   this->HASH_VAR_DIRECTORY = rawStringHash(L"directory");

   this->HASH_PER_YEAR = rawStringHash(L"year");
   this->HASH_PER_MONTH = rawStringHash(L"month");
   this->HASH_PER_WEEK = rawStringHash(L"week");
   this->HASH_PER_DAY = rawStringHash(L"day");
   this->HASH_PER_HOUR = rawStringHash(L"hour");
   this->HASH_PER_MINUTE = rawStringHash(L"minute");
   this->HASH_PER_SECOND = rawStringHash(L"second");

   this->HASH_PER_YEARS = rawStringHash(L"years");
   this->HASH_PER_MONTHS = rawStringHash(L"months");
   this->HASH_PER_WEEKS = rawStringHash(L"weeks");
   this->HASH_PER_DAYS = rawStringHash(L"days");
   this->HASH_PER_HOURS = rawStringHash(L"hours");
   this->HASH_PER_MINUTES = rawStringHash(L"minutes");
   this->HASH_PER_SECONDS = rawStringHash(L"seconds");

   this->HASH_PER_DATE = rawStringHash(L"date");
   this->HASH_PER_WEEKDAY = rawStringHash(L"weekday");

   this->HASH_FUNC_ISLOWER = rawStringHash(L"islower");
   this->HASH_FUNC_ISUPPER = rawStringHash(L"isupper");
   this->HASH_FUNC_ISNUMBER = rawStringHash(L"isnumber");
   this->HASH_FUNC_ISLETTER = rawStringHash(L"isletter");
   this->HASH_FUNC_ISDIGIT = rawStringHash(L"isdigit");
   this->HASH_FUNC_ISBINARY = rawStringHash(L"isbinary");
   this->HASH_FUNC_ISHEX = rawStringHash(L"ishex");

   this->HASH_FUNC_EXIST = rawStringHash(L"exist");
   this->HASH_FUNC_ANY = rawStringHash(L"any");
   this->HASH_FUNC_ANYINSIDE = rawStringHash(L"anyinside");
   this->HASH_FUNC_CONTAINS = rawStringHash(L"contains");
   this->HASH_FUNC_EXISTSINSIDE = rawStringHash(L"existsinside");
   this->HASH_FUNC_EXISTINSIDE = rawStringHash(L"existinside");
   this->HASH_FUNC_STARTSWITH = rawStringHash(L"startswith");
   this->HASH_FUNC_ENDSWITH = rawStringHash(L"endswith");

   this->HASH_FUNC_ABSOLUTE = rawStringHash(L"absolute");
   this->HASH_FUNC_CEIL = rawStringHash(L"ceil");
   this->HASH_FUNC_FLOOR = rawStringHash(L"floor");
   this->HASH_FUNC_ROUND = rawStringHash(L"round");
   this->HASH_FUNC_SIGN = rawStringHash(L"sign");
   this->HASH_FUNC_SQRT = rawStringHash(L"sqrt");
   this->HASH_FUNC_TRUNCATE = rawStringHash(L"truncate");

   this->HASH_FUNC_AVERAGE = rawStringHash(L"average");
   this->HASH_FUNC_SUM = rawStringHash(L"sum");
   this->HASH_FUNC_MIN = rawStringHash(L"min");
   this->HASH_FUNC_MAX = rawStringHash(L"max");
   this->HASH_FUNC_MEDIAN = rawStringHash(L"median");

   this->HASH_FUNC_LENGTH = rawStringHash(L"length");
   this->HASH_FUNC_FROMBINARY = rawStringHash(L"frombinary");
   this->HASH_FUNC_FROMHEX = rawStringHash(L"fromhex");
   this->HASH_FUNC_SIZE = rawStringHash(L"size");
   this->HASH_FUNC_NUMBER = rawStringHash(L"number");
   this->HASH_FUNC_COUNT = rawStringHash(L"count");
   this->HASH_FUNC_COUNTINSIDE = rawStringHash(L"countinside");
   this->HASH_FUNC_POWER = rawStringHash(L"power");

   this->HASH_FUNC_FIRST = rawStringHash(L"first");
   this->HASH_FUNC_LAST = rawStringHash(L"last");
   this->HASH_FUNC_RANDOM = rawStringHash(L"random");
   this->HASH_FUNC_LIFETIME = rawStringHash(L"lifetime");

   this->HASH_FUNC_AFTER = rawStringHash(L"after");
   this->HASH_FUNC_BEFORE = rawStringHash(L"before");
   this->HASH_FUNC_REVERSED = rawStringHash(L"reversed");

   this->HASH_FUNC_DIGITS = rawStringHash(L"digits");
   this->HASH_FUNC_LETTERS = rawStringHash(L"letters");
   this->HASH_FUNC_LOWER = rawStringHash(L"lower");
   this->HASH_FUNC_TRIM = rawStringHash(L"trim");
   this->HASH_FUNC_UPPER = rawStringHash(L"upper");
   this->HASH_FUNC_REVERSE = rawStringHash(L"reverse");
   this->HASH_FUNC_AFTERDIGITS = rawStringHash(L"afterdigits");
   this->HASH_FUNC_AFTERLETTERS = rawStringHash(L"afterletters");
   this->HASH_FUNC_BEFOREDIGITS = rawStringHash(L"beforedigits");
   this->HASH_FUNC_BEFORELETTERS = rawStringHash(L"beforeletters");
   this->HASH_FUNC_CAPITALIZE = rawStringHash(L"capitalize");

   this->HASH_FUNC_REPLACE = rawStringHash(L"replace");
   this->HASH_FUNC_SUBSTRING = rawStringHash(L"substring");
   this->HASH_FUNC_CONCATENATE = rawStringHash(L"concatenate");
   this->HASH_FUNC_PATH = rawStringHash(L"path");
   this->HASH_FUNC_STRING = rawStringHash(L"string");
   this->HASH_FUNC_MONTHNAME = rawStringHash(L"monthname");
   this->HASH_FUNC_WEEKDAYNAME = rawStringHash(L"weekdayname");
   this->HASH_FUNC_JOIN = rawStringHash(L"join");

   this->HASH_FUNC_REPEAT = rawStringHash(L"repeat");
   this->HASH_FUNC_LEFT = rawStringHash(L"left");
   this->HASH_FUNC_RIGHT = rawStringHash(L"right");
   this->HASH_FUNC_FILL = rawStringHash(L"fill");

   this->HASH_FUNC_ROMAN = rawStringHash(L"roman");
   this->HASH_FUNC_BINARY = rawStringHash(L"binary");
   this->HASH_FUNC_HEX = rawStringHash(L"hex");

   this->HASH_FUNC_CHRISTMAS = rawStringHash(L"christmas");
   this->HASH_FUNC_EASTER = rawStringHash(L"easter");
   this->HASH_FUNC_NEWYEAR = rawStringHash(L"newyear");

   this->HASH_FUNC_DATE = rawStringHash(L"date");
   this->HASH_FUNC_TIME = rawStringHash(L"time");
   this->HASH_FUNC_CHARACTERS = rawStringHash(L"characters");
   this->HASH_FUNC_WORDS = rawStringHash(L"words");
   this->HASH_FUNC_SPLIT = rawStringHash(L"split");
   this->HASH_FUNC_NUMBERS = rawStringHash(L"numbers");

   this->HASH_GROUP_MONTHS = {
      this->HASH_VAR_JANUARY, this->HASH_VAR_FEBRUARY, this->HASH_VAR_MARCH,
      this->HASH_VAR_APRIL, this->HASH_VAR_MAY, this->HASH_VAR_JUNE,
      this->HASH_VAR_JULY, this->HASH_VAR_AUGUST, this->HASH_VAR_SEPTEMBER,
      this->HASH_VAR_OCTOBER, this->HASH_VAR_NOVEMBER, this->HASH_VAR_DECEMBER
   };

   this->HASH_GROUP_WEEKDAYS = {
      this->HASH_VAR_MONDAY, this->HASH_VAR_TUESDAY, this->HASH_VAR_WEDNESDAY,
      this->HASH_VAR_THURSDAY, this->HASH_VAR_FRIDAY,
      this->HASH_VAR_SATURDAY, this->HASH_VAR_SUNDAY
   };

   this->HASH_GROUP_PERIOD_SINGLE = {
      this->HASH_PER_YEAR, this->HASH_PER_MONTH, this->HASH_PER_WEEK, this->HASH_PER_DAY,
      this->HASH_PER_HOUR, this->HASH_PER_MINUTE, this->HASH_PER_SECOND
   };

   this->HASH_GROUP_PERIOD_MULTI = {
      this->HASH_PER_YEARS, this->HASH_PER_MONTHS, this->HASH_PER_WEEKS, this->HASH_PER_DAYS,
      this->HASH_PER_HOURS, this->HASH_PER_MINUTES, this->HASH_PER_SECONDS
   };

   this->HASH_GROUP_TIME_ATTR = {
      this->HASH_VAR_ACCESS, this->HASH_VAR_CREATION, this->HASH_VAR_MODIFICATION, this->HASH_VAR_CHANGE
   };

   this->HASH_GROUP_ATTR = {
      this->HASH_VAR_ACCESS, this->HASH_VAR_ARCHIVE, this->HASH_VAR_CHANGE, this->HASH_VAR_COMPRESSED,
      this->HASH_VAR_CREATION, this->HASH_VAR_DEPTH, this->HASH_VAR_DRIVE, this->HASH_VAR_EMPTY,
      this->HASH_VAR_ENCRYPTED, this->HASH_VAR_EXISTS, this->HASH_VAR_EXTENSION, this->HASH_VAR_FULLNAME,
      this->HASH_VAR_HIDDEN, this->HASH_VAR_ISDIRECTORY, this->HASH_VAR_ISFILE, this->HASH_VAR_LIFETIME,
      this->HASH_VAR_MODIFICATION, this->HASH_VAR_NAME, this->HASH_VAR_PARENT, this->HASH_VAR_PATH,
      this->HASH_VAR_READONLY, this->HASH_VAR_SIZE
   };

   this->HASH_GROUP_INNERVAR = {
      this->HASH_VAR_ACCESS, this->HASH_VAR_ARCHIVE, this->HASH_VAR_CHANGE, this->HASH_VAR_COMPRESSED,
      this->HASH_VAR_CREATION, this->HASH_VAR_DEPTH, this->HASH_VAR_DRIVE, this->HASH_VAR_EMPTY,
      this->HASH_VAR_ENCRYPTED, this->HASH_VAR_EXISTS, this->HASH_VAR_EXTENSION, this->HASH_VAR_FULLNAME,
      this->HASH_VAR_HIDDEN, this->HASH_VAR_ISDIRECTORY, this->HASH_VAR_ISFILE, this->HASH_VAR_LIFETIME,
      this->HASH_VAR_MODIFICATION, this->HASH_VAR_NAME, this->HASH_VAR_PARENT, this->HASH_VAR_PATH,
      this->HASH_VAR_READONLY, this->HASH_VAR_SIZE,
      this->HASH_VAR_THIS, this->HASH_VAR_INDEX, this->HASH_VAR_LOCATION, this->HASH_VAR_SUCCESS
   };

   this->HASH_GROUP_FUNC_BOO_STR = {
      this->HASH_FUNC_ISLOWER, this->HASH_FUNC_ISUPPER, this->HASH_FUNC_ISNUMBER, this->HASH_VAR_ARCHIVE,
      this->HASH_VAR_COMPRESSED, this->HASH_VAR_EMPTY, this->HASH_VAR_ENCRYPTED, this->HASH_VAR_EXISTS,
      this->HASH_VAR_HIDDEN, this->HASH_VAR_ISDIRECTORY, this->HASH_VAR_ISFILE, this->HASH_VAR_READONLY,
      this->HASH_FUNC_ISLETTER, this->HASH_FUNC_ISDIGIT, this->HASH_FUNC_ISBINARY, this->HASH_FUNC_ISHEX
   };

   this->HASH_GROUP_FUNC_NUM_NUM = {
      this->HASH_FUNC_ABSOLUTE, this->HASH_FUNC_CEIL, this->HASH_FUNC_FLOOR,
      this->HASH_FUNC_ROUND, this->HASH_FUNC_SIGN, this->HASH_FUNC_SQRT, this->HASH_FUNC_TRUNCATE
   };

   this->HASH_GROUP_AGGRFUNC = {
      this->HASH_FUNC_AVERAGE, this->HASH_FUNC_SUM, this->HASH_FUNC_MIN, this->HASH_FUNC_MAX, this->HASH_FUNC_MEDIAN
   };

   this->HASH_GROUP_FUNC_STR_STR = {
      this->HASH_FUNC_DIGITS,  this->HASH_FUNC_LETTERS, this->HASH_FUNC_LOWER, this->HASH_FUNC_TRIM,
      this->HASH_FUNC_UPPER, this->HASH_FUNC_REVERSE, this->HASH_FUNC_AFTERDIGITS, this->HASH_FUNC_AFTERLETTERS,
      this->HASH_FUNC_BEFOREDIGITS, this->HASH_FUNC_BEFORELETTERS, this->HASH_FUNC_CAPITALIZE,
      this->HASH_VAR_DRIVE, this->HASH_VAR_EXTENSION, this->HASH_VAR_FULLNAME, this->HASH_VAR_NAME, this->HASH_VAR_PARENT
   };

   this->HASH_GROUP_FUNC_STR_STR_NUM = {
      this->HASH_FUNC_REPEAT, this->HASH_FUNC_LEFT, this->HASH_FUNC_RIGHT, this->HASH_FUNC_FILL
   };

   this->HASH_GROUP_FUNC_STR_NUM = {
      this->HASH_FUNC_ROMAN, this->HASH_FUNC_BINARY, this->HASH_FUNC_HEX
   };

   this->HASH_GROUP_FUNC_TIM_NUM = {
      this->HASH_FUNC_CHRISTMAS, this->HASH_FUNC_EASTER, this->HASH_FUNC_NEWYEAR
   };

   this->HASH_MAP_MONTHS = {
      { this->HASH_VAR_JANUARY,   1 },
      { this->HASH_VAR_FEBRUARY,  2 },
      { this->HASH_VAR_MARCH,     3 },
      { this->HASH_VAR_APRIL,     4 },
      { this->HASH_VAR_MAY,       5 },
      { this->HASH_VAR_JUNE,      6 },
      { this->HASH_VAR_JULY,      7 },
      { this->HASH_VAR_AUGUST,    8 },
      { this->HASH_VAR_SEPTEMBER, 9 },
      { this->HASH_VAR_OCTOBER,  10 },
      { this->HASH_VAR_NOVEMBER, 11 },
      { this->HASH_VAR_DECEMBER, 12 }
   };

   this->HASH_MAP_WEEKDAYS = {
      { this->HASH_VAR_MONDAY,    1 },
      { this->HASH_VAR_TUESDAY,   2 },
      { this->HASH_VAR_WEDNESDAY, 3 },
      { this->HASH_VAR_THURSDAY,  4 },
      { this->HASH_VAR_FRIDAY,    5 },
      { this->HASH_VAR_SATURDAY,  6 },
      { this->HASH_VAR_SUNDAY,    7 }
   };

   this->HASH_MAP_PERIOD_UNITS = {
      { this->HASH_PER_YEAR, Period::u_Years },
      { this->HASH_PER_YEARS, Period::u_Years },
      { this->HASH_PER_MONTH, Period::u_Months },
      { this->HASH_PER_MONTHS, Period::u_Months },
      { this->HASH_PER_WEEK, Period::u_Weeks },
      { this->HASH_PER_WEEKS, Period::u_Weeks },
      { this->HASH_PER_DAY, Period::u_Days },
      { this->HASH_PER_DAYS, Period::u_Days },
      { this->HASH_PER_HOUR, Period::u_Hours },
      { this->HASH_PER_HOURS, Period::u_Hours },
      { this->HASH_PER_MINUTE, Period::u_Minutes },
      { this->HASH_PER_MINUTES, Period::u_Minutes },
      { this->HASH_PER_SECOND, Period::u_Seconds },
      { this->HASH_PER_SECONDS, Period::u_Seconds }
   };
}

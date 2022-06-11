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
void initHashes()
{
   HASH_NOTHING = rawStringHash(L"");
   HASH_VAR_THIS = rawStringHash(L"this");

   HASH_VAR_ACCESS = rawStringHash(L"access");
   HASH_VAR_ARCHIVE = rawStringHash(L"archive");
   HASH_VAR_CHANGE = rawStringHash(L"change");
   HASH_VAR_COMPRESSED = rawStringHash(L"compressed");
   HASH_VAR_CREATION = rawStringHash(L"creation");
   HASH_VAR_DEPTH = rawStringHash(L"depth");
   HASH_VAR_DRIVE = rawStringHash(L"drive");
   HASH_VAR_EMPTY = rawStringHash(L"empty");
   HASH_VAR_ENCRYPTED = rawStringHash(L"encrypted");
   HASH_VAR_EXISTS = rawStringHash(L"exists");
   HASH_VAR_EXTENSION = rawStringHash(L"extension");
   HASH_VAR_FULLNAME = rawStringHash(L"fullname");
   HASH_VAR_HIDDEN = rawStringHash(L"hidden");
   HASH_VAR_INDEX = rawStringHash(L"index");
   HASH_VAR_ISDIRECTORY = rawStringHash(L"isdirectory");
   HASH_VAR_ISFILE = rawStringHash(L"isfile");
   HASH_VAR_LIFETIME = rawStringHash(L"lifetime");
   HASH_VAR_LOCATION = rawStringHash(L"location");
   HASH_VAR_MODIFICATION = rawStringHash(L"modification");
   HASH_VAR_NAME = rawStringHash(L"name");
   HASH_VAR_PARENT = rawStringHash(L"parent");
   HASH_VAR_PATH = rawStringHash(L"path");
   HASH_VAR_READONLY = rawStringHash(L"readonly");
   HASH_VAR_SIZE = rawStringHash(L"size");
   HASH_VAR_SUCCESS = rawStringHash(L"success");

   HASH_VAR_NOW = rawStringHash(L"now");
   HASH_VAR_TODAY = rawStringHash(L"today");
   HASH_VAR_YESTERDAY = rawStringHash(L"yesterday");
   HASH_VAR_TOMORROW = rawStringHash(L"tomorrow");

   HASH_VAR_JANUARY = rawStringHash(L"january");
   HASH_VAR_FEBRUARY = rawStringHash(L"february");
   HASH_VAR_MARCH = rawStringHash(L"march");
   HASH_VAR_APRIL = rawStringHash(L"april");
   HASH_VAR_MAY = rawStringHash(L"may");
   HASH_VAR_JUNE = rawStringHash(L"june");
   HASH_VAR_JULY = rawStringHash(L"july");
   HASH_VAR_AUGUST = rawStringHash(L"august");
   HASH_VAR_SEPTEMBER = rawStringHash(L"september");
   HASH_VAR_OCTOBER = rawStringHash(L"october");
   HASH_VAR_NOVEMBER = rawStringHash(L"november");
   HASH_VAR_DECEMBER = rawStringHash(L"december");

   HASH_VAR_MONDAY = rawStringHash(L"monday");
   HASH_VAR_TUESDAY = rawStringHash(L"tuesday");
   HASH_VAR_WEDNESDAY = rawStringHash(L"wednesday");
   HASH_VAR_THURSDAY = rawStringHash(L"thursday");
   HASH_VAR_FRIDAY = rawStringHash(L"friday");
   HASH_VAR_SATURDAY = rawStringHash(L"saturday");
   HASH_VAR_SUNDAY = rawStringHash(L"sunday");

   HASH_VAR_ALPHABET = rawStringHash(L"alphabet");
   HASH_VAR_ASCII = rawStringHash(L"ascii");
   HASH_VAR_ARGUMENTS = rawStringHash(L"arguments");

   HASH_VAR_DESKTOP = rawStringHash(L"desktop");
   HASH_VAR_UROBOROS = rawStringHash(L"uroboros");

   HASH_VAR_ALL = rawStringHash(L"all");
   HASH_VAR_DIRECTORIES = rawStringHash(L"directories");
   HASH_VAR_FILES = rawStringHash(L"files");
   HASH_VAR_RECURSIVEFILES = rawStringHash(L"recursivefiles");
   HASH_VAR_RECURSIVEDIRECTORIES = rawStringHash(L"recursivedirectories");
   HASH_VAR_FILE = rawStringHash(L"file");
   HASH_VAR_DIRECTORY = rawStringHash(L"directory");

   HASH_PER_YEAR = rawStringHash(L"year");
   HASH_PER_MONTH = rawStringHash(L"month");
   HASH_PER_WEEK = rawStringHash(L"week");
   HASH_PER_DAY = rawStringHash(L"day");
   HASH_PER_HOUR = rawStringHash(L"hour");
   HASH_PER_MINUTE = rawStringHash(L"minute");
   HASH_PER_SECOND = rawStringHash(L"second");

   HASH_PER_YEARS = rawStringHash(L"years");
   HASH_PER_MONTHS = rawStringHash(L"months");
   HASH_PER_WEEKS = rawStringHash(L"weeks");
   HASH_PER_DAYS = rawStringHash(L"days");
   HASH_PER_HOURS = rawStringHash(L"hours");
   HASH_PER_MINUTES = rawStringHash(L"minutes");
   HASH_PER_SECONDS = rawStringHash(L"seconds");

   HASH_PER_DATE = rawStringHash(L"date");
   HASH_PER_WEEKDAY = rawStringHash(L"weekday");

   HASH_FUNC_ISLOWER = rawStringHash(L"islower");
   HASH_FUNC_ISUPPER = rawStringHash(L"isupper");
   HASH_FUNC_ISNUMBER = rawStringHash(L"isnumber");
   HASH_FUNC_ISLETTER = rawStringHash(L"isletter");
   HASH_FUNC_ISDIGIT = rawStringHash(L"isdigit");
   HASH_FUNC_ISBINARY = rawStringHash(L"isbinary");
   HASH_FUNC_ISHEX = rawStringHash(L"ishex");

   HASH_FUNC_EXIST = rawStringHash(L"exist");
   HASH_FUNC_ANY = rawStringHash(L"any");
   HASH_FUNC_ANYINSIDE = rawStringHash(L"anyinside");
   HASH_FUNC_CONTAINS = rawStringHash(L"contains");
   HASH_FUNC_EXISTSINSIDE = rawStringHash(L"existsinside");
   HASH_FUNC_EXISTINSIDE = rawStringHash(L"existinside");
   HASH_FUNC_STARTSWITH = rawStringHash(L"startswith");
   HASH_FUNC_ENDSWITH = rawStringHash(L"endswith");

   HASH_FUNC_ABSOLUTE = rawStringHash(L"absolute");
   HASH_FUNC_CEIL = rawStringHash(L"ceil");
   HASH_FUNC_FLOOR = rawStringHash(L"floor");
   HASH_FUNC_ROUND = rawStringHash(L"round");
   HASH_FUNC_SIGN = rawStringHash(L"sign");
   HASH_FUNC_SQRT = rawStringHash(L"sqrt");
   HASH_FUNC_TRUNCATE = rawStringHash(L"truncate");

   HASH_FUNC_AVERAGE = rawStringHash(L"average");
   HASH_FUNC_SUM = rawStringHash(L"sum");
   HASH_FUNC_MIN = rawStringHash(L"min");
   HASH_FUNC_MAX = rawStringHash(L"max");
   HASH_FUNC_MEDIAN = rawStringHash(L"median");

   HASH_FUNC_LENGTH = rawStringHash(L"length");
   HASH_FUNC_FROMBINARY = rawStringHash(L"frombinary");
   HASH_FUNC_FROMHEX = rawStringHash(L"fromhex");
   HASH_FUNC_SIZE = rawStringHash(L"size");
   HASH_FUNC_NUMBER = rawStringHash(L"number");
   HASH_FUNC_COUNT = rawStringHash(L"count");
   HASH_FUNC_COUNTINSIDE = rawStringHash(L"countinside");
   HASH_FUNC_POWER = rawStringHash(L"power");

   HASH_FUNC_FIRST = rawStringHash(L"first");
   HASH_FUNC_LAST = rawStringHash(L"last");
   HASH_FUNC_RANDOM = rawStringHash(L"random");
   HASH_FUNC_LIFETIME = rawStringHash(L"lifetime");

   HASH_FUNC_AFTER = rawStringHash(L"after");
   HASH_FUNC_BEFORE = rawStringHash(L"before");
   HASH_FUNC_REVERSED = rawStringHash(L"reversed");

   HASH_FUNC_DIGITS = rawStringHash(L"digits");
   HASH_FUNC_LETTERS = rawStringHash(L"letters");
   HASH_FUNC_LOWER = rawStringHash(L"lower");
   HASH_FUNC_TRIM = rawStringHash(L"trim");
   HASH_FUNC_UPPER = rawStringHash(L"upper");
   HASH_FUNC_REVERSE = rawStringHash(L"reverse");
   HASH_FUNC_AFTERDIGITS = rawStringHash(L"afterdigits");
   HASH_FUNC_AFTERLETTERS = rawStringHash(L"afterletters");
   HASH_FUNC_BEFOREDIGITS = rawStringHash(L"beforedigits");
   HASH_FUNC_BEFORELETTERS = rawStringHash(L"beforeletters");
   HASH_FUNC_CAPITALIZE = rawStringHash(L"capitalize");

   HASH_FUNC_REPLACE = rawStringHash(L"replace");
   HASH_FUNC_SUBSTRING = rawStringHash(L"substring");
   HASH_FUNC_CONCATENATE = rawStringHash(L"concatenate");
   HASH_FUNC_PATH = rawStringHash(L"path");
   HASH_FUNC_STRING = rawStringHash(L"string");
   HASH_FUNC_MONTHNAME = rawStringHash(L"monthname");
   HASH_FUNC_WEEKDAYNAME = rawStringHash(L"weekdayname");
   HASH_FUNC_JOIN = rawStringHash(L"join");

   HASH_FUNC_REPEAT = rawStringHash(L"repeat");
   HASH_FUNC_LEFT = rawStringHash(L"left");
   HASH_FUNC_RIGHT = rawStringHash(L"right");
   HASH_FUNC_FILL = rawStringHash(L"fill");

   HASH_FUNC_ROMAN = rawStringHash(L"roman");
   HASH_FUNC_BINARY = rawStringHash(L"binary");
   HASH_FUNC_HEX = rawStringHash(L"hex");

   HASH_FUNC_CHRISTMAS = rawStringHash(L"christmas");
   HASH_FUNC_EASTER = rawStringHash(L"easter");
   HASH_FUNC_NEWYEAR = rawStringHash(L"newyear");

   HASH_FUNC_DATE = rawStringHash(L"date");
   HASH_FUNC_TIME = rawStringHash(L"time");
   HASH_FUNC_CHARACTERS = rawStringHash(L"characters");
   HASH_FUNC_WORDS = rawStringHash(L"words");
   HASH_FUNC_SPLIT = rawStringHash(L"split");
   HASH_FUNC_NUMBERS = rawStringHash(L"numbers");

   HASH_GROUP_MONTHS = {
      HASH_VAR_JANUARY, HASH_VAR_FEBRUARY, HASH_VAR_MARCH,
      HASH_VAR_APRIL, HASH_VAR_MAY, HASH_VAR_JUNE,
      HASH_VAR_JULY, HASH_VAR_AUGUST, HASH_VAR_SEPTEMBER,
      HASH_VAR_OCTOBER, HASH_VAR_NOVEMBER, HASH_VAR_DECEMBER
   };

   HASH_GROUP_WEEKDAYS = {
      HASH_VAR_MONDAY, HASH_VAR_TUESDAY, HASH_VAR_WEDNESDAY,
      HASH_VAR_THURSDAY, HASH_VAR_FRIDAY,
      HASH_VAR_SATURDAY, HASH_VAR_SUNDAY
   };

   HASH_GROUP_PERIOD_SINGLE = {
      HASH_PER_YEAR, HASH_PER_MONTH, HASH_PER_WEEK, HASH_PER_DAY,
      HASH_PER_HOUR, HASH_PER_MINUTE, HASH_PER_SECOND
   };

   HASH_GROUP_PERIOD_MULTI = {
      HASH_PER_YEARS, HASH_PER_MONTHS, HASH_PER_WEEKS, HASH_PER_DAYS,
      HASH_PER_HOURS, HASH_PER_MINUTES, HASH_PER_SECONDS
   };

   HASH_GROUP_TIME_ATTR = {
      HASH_VAR_ACCESS, HASH_VAR_CREATION, HASH_VAR_MODIFICATION, HASH_VAR_CHANGE
   };

   HASH_GROUP_ATTR = {
      HASH_VAR_ACCESS, HASH_VAR_ARCHIVE, HASH_VAR_CHANGE, HASH_VAR_COMPRESSED,
      HASH_VAR_CREATION, HASH_VAR_DEPTH, HASH_VAR_DRIVE, HASH_VAR_EMPTY,
      HASH_VAR_ENCRYPTED, HASH_VAR_EXISTS, HASH_VAR_EXTENSION, HASH_VAR_FULLNAME,
      HASH_VAR_HIDDEN, HASH_VAR_ISDIRECTORY, HASH_VAR_ISFILE, HASH_VAR_LIFETIME,
      HASH_VAR_MODIFICATION, HASH_VAR_NAME, HASH_VAR_PARENT, HASH_VAR_PATH,
      HASH_VAR_READONLY, HASH_VAR_SIZE
   };

   HASH_GROUP_INNERVAR = {
      HASH_VAR_ACCESS, HASH_VAR_ARCHIVE, HASH_VAR_CHANGE, HASH_VAR_COMPRESSED,
      HASH_VAR_CREATION, HASH_VAR_DEPTH, HASH_VAR_DRIVE, HASH_VAR_EMPTY,
      HASH_VAR_ENCRYPTED, HASH_VAR_EXISTS, HASH_VAR_EXTENSION, HASH_VAR_FULLNAME,
      HASH_VAR_HIDDEN, HASH_VAR_ISDIRECTORY, HASH_VAR_ISFILE, HASH_VAR_LIFETIME,
      HASH_VAR_MODIFICATION, HASH_VAR_NAME, HASH_VAR_PARENT, HASH_VAR_PATH,
      HASH_VAR_READONLY, HASH_VAR_SIZE,
      HASH_VAR_THIS, HASH_VAR_INDEX, HASH_VAR_LOCATION, HASH_VAR_SUCCESS
   };

   HASH_GROUP_FUNC_BOO_STR = {
      HASH_FUNC_ISLOWER, HASH_FUNC_ISUPPER, HASH_FUNC_ISNUMBER, HASH_VAR_ARCHIVE,
      HASH_VAR_COMPRESSED, HASH_VAR_EMPTY, HASH_VAR_ENCRYPTED, HASH_VAR_EXISTS,
      HASH_VAR_HIDDEN, HASH_VAR_ISDIRECTORY, HASH_VAR_ISFILE, HASH_VAR_READONLY,
      HASH_FUNC_ISLETTER, HASH_FUNC_ISDIGIT, HASH_FUNC_ISBINARY, HASH_FUNC_ISHEX
   };

   HASH_GROUP_FUNC_NUM_NUM = {
      HASH_FUNC_ABSOLUTE, HASH_FUNC_CEIL, HASH_FUNC_FLOOR,
      HASH_FUNC_ROUND, HASH_FUNC_SIGN, HASH_FUNC_SQRT, HASH_FUNC_TRUNCATE
   };

   HASH_GROUP_AGGRFUNC = {
      HASH_FUNC_AVERAGE, HASH_FUNC_SUM, HASH_FUNC_MIN, HASH_FUNC_MAX, HASH_FUNC_MEDIAN
   };

   HASH_GROUP_FUNC_STR_STR = {
      HASH_FUNC_DIGITS,  HASH_FUNC_LETTERS, HASH_FUNC_LOWER, HASH_FUNC_TRIM,
      HASH_FUNC_UPPER, HASH_FUNC_REVERSE, HASH_FUNC_AFTERDIGITS, HASH_FUNC_AFTERLETTERS,
      HASH_FUNC_BEFOREDIGITS, HASH_FUNC_BEFORELETTERS, HASH_FUNC_CAPITALIZE,
      HASH_VAR_DRIVE, HASH_VAR_EXTENSION, HASH_VAR_FULLNAME, HASH_VAR_NAME, HASH_VAR_PARENT
   };

   HASH_GROUP_FUNC_STR_STR_NUM = {
      HASH_FUNC_REPEAT, HASH_FUNC_LEFT, HASH_FUNC_RIGHT, HASH_FUNC_FILL
   };

   HASH_GROUP_FUNC_STR_NUM = {
      HASH_FUNC_ROMAN, HASH_FUNC_BINARY, HASH_FUNC_HEX
   };

   HASH_GROUP_FUNC_TIM_NUM = {
      HASH_FUNC_CHRISTMAS, HASH_FUNC_EASTER, HASH_FUNC_NEWYEAR
   };

   HASH_MAP_MONTHS = {
      { HASH_VAR_JANUARY,   1 },
      { HASH_VAR_FEBRUARY,  2 },
      { HASH_VAR_MARCH,     3 },
      { HASH_VAR_APRIL,     4 },
      { HASH_VAR_MAY,       5 },
      { HASH_VAR_JUNE,      6 },
      { HASH_VAR_JULY,      7 },
      { HASH_VAR_AUGUST,    8 },
      { HASH_VAR_SEPTEMBER, 9 },
      { HASH_VAR_OCTOBER,  10 },
      { HASH_VAR_NOVEMBER, 11 },
      { HASH_VAR_DECEMBER, 12 }
   };

   HASH_MAP_WEEKDAYS = {
      { HASH_VAR_MONDAY,    1 },
      { HASH_VAR_TUESDAY,   2 },
      { HASH_VAR_WEDNESDAY, 3 },
      { HASH_VAR_THURSDAY,  4 },
      { HASH_VAR_FRIDAY,    5 },
      { HASH_VAR_SATURDAY,  6 },
      { HASH_VAR_SUNDAY,    7 }
   };

   HASH_MAP_PERIOD_UNITS = {
      { HASH_PER_YEAR, Period::u_Years },
      { HASH_PER_YEARS, Period::u_Years },
      { HASH_PER_MONTH, Period::u_Months },
      { HASH_PER_MONTHS, Period::u_Months },
      { HASH_PER_WEEK, Period::u_Weeks },
      { HASH_PER_WEEKS, Period::u_Weeks },
      { HASH_PER_DAY, Period::u_Days },
      { HASH_PER_DAYS, Period::u_Days },
      { HASH_PER_HOUR, Period::u_Hours },
      { HASH_PER_HOURS, Period::u_Hours },
      { HASH_PER_MINUTE, Period::u_Minutes },
      { HASH_PER_MINUTES, Period::u_Minutes },
      { HASH_PER_SECOND, Period::u_Seconds },
      { HASH_PER_SECONDS, Period::u_Seconds }
   };
}


_size HASH_NOTHING;

_size HASH_VAR_THIS;
_size HASH_VAR_ACCESS;
_size HASH_VAR_ARCHIVE;
_size HASH_VAR_CHANGE;
_size HASH_VAR_COMPRESSED;
_size HASH_VAR_CREATION;
_size HASH_VAR_DEPTH;
_size HASH_VAR_DRIVE;
_size HASH_VAR_EMPTY;
_size HASH_VAR_ENCRYPTED;
_size HASH_VAR_EXISTS;
_size HASH_VAR_EXTENSION;
_size HASH_VAR_FULLNAME;
_size HASH_VAR_HIDDEN;
_size HASH_VAR_INDEX;
_size HASH_VAR_ISDIRECTORY;
_size HASH_VAR_ISFILE;
_size HASH_VAR_LIFETIME;
_size HASH_VAR_LOCATION;
_size HASH_VAR_MODIFICATION;
_size HASH_VAR_NAME;
_size HASH_VAR_PARENT;
_size HASH_VAR_PATH;
_size HASH_VAR_READONLY;
_size HASH_VAR_SIZE;
_size HASH_VAR_SUCCESS;

_size HASH_VAR_NOW;
_size HASH_VAR_TODAY;
_size HASH_VAR_YESTERDAY;
_size HASH_VAR_TOMORROW;

_size HASH_VAR_JANUARY;
_size HASH_VAR_FEBRUARY;
_size HASH_VAR_MARCH;
_size HASH_VAR_APRIL;
_size HASH_VAR_MAY;
_size HASH_VAR_JUNE;
_size HASH_VAR_JULY;
_size HASH_VAR_AUGUST;
_size HASH_VAR_SEPTEMBER;
_size HASH_VAR_OCTOBER;
_size HASH_VAR_NOVEMBER;
_size HASH_VAR_DECEMBER;

_size HASH_VAR_MONDAY;
_size HASH_VAR_TUESDAY;
_size HASH_VAR_WEDNESDAY;
_size HASH_VAR_THURSDAY;
_size HASH_VAR_FRIDAY;
_size HASH_VAR_SATURDAY;
_size HASH_VAR_SUNDAY;

_size HASH_VAR_ALPHABET;
_size HASH_VAR_ASCII;
_size HASH_VAR_ARGUMENTS;

_size HASH_VAR_DESKTOP;
_size HASH_VAR_UROBOROS;

_size HASH_VAR_ALL;
_size HASH_VAR_DIRECTORIES;
_size HASH_VAR_FILES;
_size HASH_VAR_RECURSIVEFILES;
_size HASH_VAR_RECURSIVEDIRECTORIES;
_size HASH_VAR_FILE;
_size HASH_VAR_DIRECTORY;

_size HASH_PER_YEAR;
_size HASH_PER_MONTH;
_size HASH_PER_WEEK;
_size HASH_PER_DAY;
_size HASH_PER_HOUR;
_size HASH_PER_MINUTE;
_size HASH_PER_SECOND;

_size HASH_PER_YEARS;
_size HASH_PER_MONTHS;
_size HASH_PER_WEEKS;
_size HASH_PER_DAYS;
_size HASH_PER_HOURS;
_size HASH_PER_MINUTES;
_size HASH_PER_SECONDS;

_size HASH_PER_DATE;
_size HASH_PER_WEEKDAY;

_size HASH_FUNC_ISLOWER;
_size HASH_FUNC_ISUPPER;
_size HASH_FUNC_ISNUMBER;
_size HASH_FUNC_ISLETTER;
_size HASH_FUNC_ISDIGIT;
_size HASH_FUNC_ISBINARY;
_size HASH_FUNC_ISHEX;

_size HASH_FUNC_EXIST;
_size HASH_FUNC_ANY;
_size HASH_FUNC_ANYINSIDE;
_size HASH_FUNC_CONTAINS;
_size HASH_FUNC_EXISTSINSIDE;
_size HASH_FUNC_EXISTINSIDE;
_size HASH_FUNC_STARTSWITH;
_size HASH_FUNC_ENDSWITH;

_size HASH_FUNC_ABSOLUTE;
_size HASH_FUNC_CEIL;
_size HASH_FUNC_FLOOR;
_size HASH_FUNC_ROUND;
_size HASH_FUNC_SIGN;
_size HASH_FUNC_SQRT;
_size HASH_FUNC_TRUNCATE;

_size HASH_FUNC_AVERAGE;
_size HASH_FUNC_SUM;
_size HASH_FUNC_MIN;
_size HASH_FUNC_MAX;
_size HASH_FUNC_MEDIAN;

_size HASH_FUNC_LENGTH;
_size HASH_FUNC_FROMBINARY;
_size HASH_FUNC_FROMHEX;
_size HASH_FUNC_SIZE;
_size HASH_FUNC_NUMBER;
_size HASH_FUNC_COUNT;
_size HASH_FUNC_COUNTINSIDE;
_size HASH_FUNC_POWER;

_size HASH_FUNC_FIRST;
_size HASH_FUNC_LAST;
_size HASH_FUNC_RANDOM;
_size HASH_FUNC_LIFETIME;

_size HASH_FUNC_AFTER;
_size HASH_FUNC_BEFORE;
_size HASH_FUNC_REVERSED;

_size HASH_FUNC_DIGITS;
_size HASH_FUNC_LETTERS;
_size HASH_FUNC_LOWER;
_size HASH_FUNC_TRIM;
_size HASH_FUNC_UPPER;
_size HASH_FUNC_REVERSE;
_size HASH_FUNC_AFTERDIGITS;
_size HASH_FUNC_AFTERLETTERS;
_size HASH_FUNC_BEFOREDIGITS;
_size HASH_FUNC_BEFORELETTERS;
_size HASH_FUNC_CAPITALIZE;

_size HASH_FUNC_REPLACE;
_size HASH_FUNC_SUBSTRING;
_size HASH_FUNC_CONCATENATE;
_size HASH_FUNC_PATH;
_size HASH_FUNC_STRING;
_size HASH_FUNC_MONTHNAME;
_size HASH_FUNC_WEEKDAYNAME;
_size HASH_FUNC_JOIN;

_size HASH_FUNC_REPEAT;
_size HASH_FUNC_LEFT;
_size HASH_FUNC_RIGHT;
_size HASH_FUNC_FILL;

_size HASH_FUNC_ROMAN;
_size HASH_FUNC_BINARY;
_size HASH_FUNC_HEX;

_size HASH_FUNC_CHRISTMAS;
_size HASH_FUNC_EASTER;
_size HASH_FUNC_NEWYEAR;

_size HASH_FUNC_DATE;
_size HASH_FUNC_TIME;
_size HASH_FUNC_CHARACTERS;
_size HASH_FUNC_WORDS;
_size HASH_FUNC_SPLIT;
_size HASH_FUNC_NUMBERS;

std::set<_size> HASH_GROUP_MONTHS;
std::set<_size> HASH_GROUP_WEEKDAYS;
std::set<_size> HASH_GROUP_PERIOD_SINGLE;
std::set<_size> HASH_GROUP_PERIOD_MULTI;
std::set<_size> HASH_GROUP_TIME_ATTR;
std::set<_size> HASH_GROUP_ATTR;
std::set<_size> HASH_GROUP_INNERVAR;
std::set<_size> HASH_GROUP_FUNC_BOO_STR;
std::set<_size> HASH_GROUP_FUNC_NUM_NUM;
std::set<_size> HASH_GROUP_AGGRFUNC;
std::set<_size> HASH_GROUP_FUNC_STR_STR;
std::set<_size> HASH_GROUP_FUNC_STR_STR_NUM;
std::set<_size> HASH_GROUP_FUNC_STR_NUM;
std::set<_size> HASH_GROUP_FUNC_TIM_NUM;
std::map<_size, _tnum> HASH_MAP_MONTHS;
std::map<_size, _tnum> HASH_MAP_WEEKDAYS;
std::map<_size, Period::PeriodUnit> HASH_MAP_PERIOD_UNITS;


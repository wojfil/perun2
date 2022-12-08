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
#include "datatype/strings.h"
#include <unordered_map>
#include <algorithm>


namespace uro
{

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

// prepare data for future hash comparisons
Hashes::Hashes() :
   HASH_NOTHING(rawStringHash(EMPTY_STRING)),
   HASH_VAR_THIS(rawStringHash(L"this")),

   HASH_VAR_ACCESS(rawStringHash(L"access")),
   HASH_VAR_ARCHIVE(rawStringHash(L"archive")),
   HASH_VAR_CHANGE(rawStringHash(L"change")),
   HASH_VAR_COMPRESSED(rawStringHash(L"compressed")),
   HASH_VAR_CREATION(rawStringHash(L"creation")),
   HASH_VAR_DEPTH(rawStringHash(L"depth")),
   HASH_VAR_DRIVE(rawStringHash(L"drive")),
   HASH_VAR_EMPTY(rawStringHash(L"empty")),
   HASH_VAR_ENCRYPTED(rawStringHash(L"encrypted")),
   HASH_VAR_EXISTS(rawStringHash(L"exists")),
   HASH_VAR_EXTENSION(rawStringHash(L"extension")),
   HASH_VAR_FULLNAME(rawStringHash(L"fullname")),
   HASH_VAR_HIDDEN(rawStringHash(L"hidden")),
   HASH_VAR_INDEX(rawStringHash(L"index")),
   HASH_VAR_ISDIRECTORY(rawStringHash(L"isdirectory")),
   HASH_VAR_ISFILE(rawStringHash(L"isfile")),
   HASH_VAR_LIFETIME(rawStringHash(L"lifetime")),
   HASH_VAR_LOCATION(rawStringHash(L"location")),
   HASH_VAR_MODIFICATION(rawStringHash(L"modification")),
   HASH_VAR_NAME(rawStringHash(L"name")),
   HASH_VAR_PARENT(rawStringHash(L"parent")),
   HASH_VAR_PATH(rawStringHash(L"path")),
   HASH_VAR_READONLY(rawStringHash(L"readonly")),
   HASH_VAR_SIZE(rawStringHash(L"size")),
   HASH_VAR_SUCCESS(rawStringHash(L"success")),

   HASH_VAR_NOW(rawStringHash(L"now")),
   HASH_VAR_TODAY(rawStringHash(L"today")),
   HASH_VAR_YESTERDAY(rawStringHash(L"yesterday")),
   HASH_VAR_TOMORROW(rawStringHash(L"tomorrow")),

   HASH_VAR_JANUARY(rawStringHash(L"january")),
   HASH_VAR_FEBRUARY(rawStringHash(L"february")),
   HASH_VAR_MARCH(rawStringHash(L"march")),
   HASH_VAR_APRIL(rawStringHash(L"april")),
   HASH_VAR_MAY(rawStringHash(L"may")),
   HASH_VAR_JUNE(rawStringHash(L"june")),
   HASH_VAR_JULY(rawStringHash(L"july")),
   HASH_VAR_AUGUST(rawStringHash(L"august")),
   HASH_VAR_SEPTEMBER(rawStringHash(L"september")),
   HASH_VAR_OCTOBER(rawStringHash(L"october")),
   HASH_VAR_NOVEMBER(rawStringHash(L"november")),
   HASH_VAR_DECEMBER(rawStringHash(L"december")),

   HASH_VAR_MONDAY(rawStringHash(L"monday")),
   HASH_VAR_TUESDAY(rawStringHash(L"tuesday")),
   HASH_VAR_WEDNESDAY(rawStringHash(L"wednesday")),
   HASH_VAR_THURSDAY(rawStringHash(L"thursday")),
   HASH_VAR_FRIDAY(rawStringHash(L"friday")),
   HASH_VAR_SATURDAY(rawStringHash(L"saturday")),
   HASH_VAR_SUNDAY(rawStringHash(L"sunday")),

   HASH_VAR_ALPHABET(rawStringHash(L"alphabet")),
   HASH_VAR_ASCII(rawStringHash(L"ascii")),
   HASH_VAR_ARGUMENTS(rawStringHash(L"arguments")),

   HASH_VAR_DESKTOP(rawStringHash(L"desktop")),
   HASH_VAR_UROBOROS(rawStringHash(L"uroboros")),
   HASH_VAR_DEVICE(rawStringHash(L"device")),
   HASH_VAR_ROOT(rawStringHash(L"root")),

   HASH_VAR_DIRECTORIES(rawStringHash(L"directories")),
   HASH_VAR_FILES(rawStringHash(L"files")),
   HASH_VAR_RECURSIVEFILES(rawStringHash(L"recursivefiles")),
   HASH_VAR_RECURSIVEDIRECTORIES(rawStringHash(L"recursivedirectories")),
   HASH_VAR_FILE(rawStringHash(L"file")),
   HASH_VAR_DIRECTORY(rawStringHash(L"directory")),

   HASH_PER_YEAR(rawStringHash(L"year")),
   HASH_PER_MONTH(rawStringHash(L"month")),
   HASH_PER_WEEK(rawStringHash(L"week")),
   HASH_PER_DAY(rawStringHash(L"day")),
   HASH_PER_HOUR(rawStringHash(L"hour")),
   HASH_PER_MINUTE(rawStringHash(L"minute")),
   HASH_PER_SECOND(rawStringHash(L"second")),

   HASH_PER_YEARS(rawStringHash(L"years")),
   HASH_PER_MONTHS(rawStringHash(L"months")),
   HASH_PER_WEEKS(rawStringHash(L"weeks")),
   HASH_PER_DAYS(rawStringHash(L"days")),
   HASH_PER_HOURS(rawStringHash(L"hours")),
   HASH_PER_MINUTES(rawStringHash(L"minutes")),
   HASH_PER_SECONDS(rawStringHash(L"seconds")),

   HASH_PER_DATE(rawStringHash(L"date")),
   HASH_PER_WEEKDAY(rawStringHash(L"weekday")),

   HASH_FUNC_ISLOWER(rawStringHash(L"islower")),
   HASH_FUNC_ISUPPER(rawStringHash(L"isupper")),
   HASH_FUNC_ISNUMBER(rawStringHash(L"isnumber")),
   HASH_FUNC_ISLETTER(rawStringHash(L"isletter")),
   HASH_FUNC_ISDIGIT(rawStringHash(L"isdigit")),
   HASH_FUNC_ISBINARY(rawStringHash(L"isbinary")),
   HASH_FUNC_ISHEX(rawStringHash(L"ishex")),

   HASH_FUNC_EXIST(rawStringHash(L"exist")),
   HASH_FUNC_ANY(rawStringHash(L"any")),
   HASH_FUNC_ANYINSIDE(rawStringHash(L"anyinside")),
   HASH_FUNC_CONTAINS(rawStringHash(L"contains")),
   HASH_FUNC_EXISTSINSIDE(rawStringHash(L"existsinside")),
   HASH_FUNC_EXISTINSIDE(rawStringHash(L"existinside")),
   HASH_FUNC_STARTSWITH(rawStringHash(L"startswith")),
   HASH_FUNC_ENDSWITH(rawStringHash(L"endswith")),
   HASH_FUNC_FIND(rawStringHash(L"find")),

   HASH_FUNC_ABSOLUTE(rawStringHash(L"absolute")),
   HASH_FUNC_CEIL(rawStringHash(L"ceil")),
   HASH_FUNC_FLOOR(rawStringHash(L"floor")),
   HASH_FUNC_ROUND(rawStringHash(L"round")),
   HASH_FUNC_SIGN(rawStringHash(L"sign")),
   HASH_FUNC_SQRT(rawStringHash(L"sqrt")),
   HASH_FUNC_TRUNCATE(rawStringHash(L"truncate")),

   HASH_FUNC_AVERAGE(rawStringHash(L"average")),
   HASH_FUNC_SUM(rawStringHash(L"sum")),
   HASH_FUNC_MIN(rawStringHash(L"min")),
   HASH_FUNC_MAX(rawStringHash(L"max")),
   HASH_FUNC_MEDIAN(rawStringHash(L"median")),

   HASH_FUNC_LENGTH(rawStringHash(L"length")),
   HASH_FUNC_FROMBINARY(rawStringHash(L"frombinary")),
   HASH_FUNC_FROMHEX(rawStringHash(L"fromhex")),
   HASH_FUNC_SIZE(rawStringHash(L"size")),
   HASH_FUNC_NUMBER(rawStringHash(L"number")),
   HASH_FUNC_COUNT(rawStringHash(L"count")),
   HASH_FUNC_COUNTINSIDE(rawStringHash(L"countinside")),
   HASH_FUNC_POWER(rawStringHash(L"power")),

   HASH_FUNC_FIRST(rawStringHash(L"first")),
   HASH_FUNC_LAST(rawStringHash(L"last")),
   HASH_FUNC_RANDOM(rawStringHash(L"random")),
   HASH_FUNC_LIFETIME(rawStringHash(L"lifetime")),

   HASH_FUNC_AFTER(rawStringHash(L"after")),
   HASH_FUNC_BEFORE(rawStringHash(L"before")),
   HASH_FUNC_REVERSED(rawStringHash(L"reversed")),

   HASH_FUNC_DIGITS(rawStringHash(L"digits")),
   HASH_FUNC_LETTERS(rawStringHash(L"letters")),
   HASH_FUNC_LOWER(rawStringHash(L"lower")),
   HASH_FUNC_TRIM(rawStringHash(L"trim")),
   HASH_FUNC_UPPER(rawStringHash(L"upper")),
   HASH_FUNC_REVERSE(rawStringHash(L"reverse")),
   HASH_FUNC_AFTERDIGITS(rawStringHash(L"afterdigits")),
   HASH_FUNC_AFTERLETTERS(rawStringHash(L"afterletters")),
   HASH_FUNC_BEFOREDIGITS(rawStringHash(L"beforedigits")),
   HASH_FUNC_BEFORELETTERS(rawStringHash(L"beforeletters")),
   HASH_FUNC_CAPITALIZE(rawStringHash(L"capitalize")),

   HASH_FUNC_REPLACE(rawStringHash(L"replace")),
   HASH_FUNC_SUBSTRING(rawStringHash(L"substring")),
   HASH_FUNC_CONCATENATE(rawStringHash(L"concatenate")),
   HASH_FUNC_PATH(rawStringHash(L"path")),
   HASH_FUNC_STRING(rawStringHash(L"string")),
   HASH_FUNC_MONTHNAME(rawStringHash(L"monthname")),
   HASH_FUNC_WEEKDAYNAME(rawStringHash(L"weekdayname")),
   HASH_FUNC_JOIN(rawStringHash(L"join")),

   HASH_FUNC_REPEAT(rawStringHash(L"repeat")),
   HASH_FUNC_LEFT(rawStringHash(L"left")),
   HASH_FUNC_RIGHT(rawStringHash(L"right")),
   HASH_FUNC_FILL(rawStringHash(L"fill")),

   HASH_FUNC_ROMAN(rawStringHash(L"roman")),
   HASH_FUNC_BINARY(rawStringHash(L"binary")),
   HASH_FUNC_HEX(rawStringHash(L"hex")),

   HASH_FUNC_CHRISTMAS(rawStringHash(L"christmas")),
   HASH_FUNC_EASTER(rawStringHash(L"easter")),
   HASH_FUNC_NEWYEAR(rawStringHash(L"newyear")),

   HASH_FUNC_DATE(rawStringHash(L"date")),
   HASH_FUNC_TIME(rawStringHash(L"time")),
   HASH_FUNC_CHARACTERS(rawStringHash(L"characters")),
   HASH_FUNC_WORDS(rawStringHash(L"words")),
   HASH_FUNC_SPLIT(rawStringHash(L"split")),
   HASH_FUNC_NUMBERS(rawStringHash(L"numbers")),

   HASH_GROUP_MONTHS({
     this->HASH_VAR_JANUARY, this->HASH_VAR_FEBRUARY, this->HASH_VAR_MARCH,
     this->HASH_VAR_APRIL, this->HASH_VAR_MAY, this->HASH_VAR_JUNE,
     this->HASH_VAR_JULY, this->HASH_VAR_AUGUST, this->HASH_VAR_SEPTEMBER,
     this->HASH_VAR_OCTOBER, this->HASH_VAR_NOVEMBER, this->HASH_VAR_DECEMBER
   }),

   HASH_GROUP_WEEKDAYS({
     this->HASH_VAR_MONDAY, this->HASH_VAR_TUESDAY, this->HASH_VAR_WEDNESDAY,
     this->HASH_VAR_THURSDAY, this->HASH_VAR_FRIDAY,
     this->HASH_VAR_SATURDAY, this->HASH_VAR_SUNDAY
   }),

   HASH_GROUP_PERIOD_SINGLE({
     this->HASH_PER_YEAR, this->HASH_PER_MONTH, this->HASH_PER_WEEK, this->HASH_PER_DAY,
     this->HASH_PER_HOUR, this->HASH_PER_MINUTE, this->HASH_PER_SECOND
   }),

   HASH_GROUP_PERIOD_MULTI({
     this->HASH_PER_YEARS, this->HASH_PER_MONTHS, this->HASH_PER_WEEKS, this->HASH_PER_DAYS,
     this->HASH_PER_HOURS, this->HASH_PER_MINUTES, this->HASH_PER_SECONDS
   }),

   HASH_GROUP_TIME_ATTR({
     this->HASH_VAR_ACCESS, this->HASH_VAR_CREATION, this->HASH_VAR_MODIFICATION, this->HASH_VAR_CHANGE
   }),

   HASH_GROUP_ATTR({
     this->HASH_VAR_ACCESS, this->HASH_VAR_ARCHIVE, this->HASH_VAR_CHANGE, this->HASH_VAR_COMPRESSED,
     this->HASH_VAR_CREATION, this->HASH_VAR_DEPTH, this->HASH_VAR_DRIVE, this->HASH_VAR_EMPTY,
     this->HASH_VAR_ENCRYPTED, this->HASH_VAR_EXISTS, this->HASH_VAR_EXTENSION, this->HASH_VAR_FULLNAME,
     this->HASH_VAR_HIDDEN, this->HASH_VAR_ISDIRECTORY, this->HASH_VAR_ISFILE, this->HASH_VAR_LIFETIME,
     this->HASH_VAR_MODIFICATION, this->HASH_VAR_NAME, this->HASH_VAR_PARENT, this->HASH_VAR_PATH,
     this->HASH_VAR_READONLY, this->HASH_VAR_SIZE
   }),

   HASH_GROUP_INNERVAR({
     this->HASH_VAR_ACCESS, this->HASH_VAR_ARCHIVE, this->HASH_VAR_CHANGE, this->HASH_VAR_COMPRESSED,
     this->HASH_VAR_CREATION, this->HASH_VAR_DEPTH, this->HASH_VAR_DRIVE, this->HASH_VAR_EMPTY,
     this->HASH_VAR_ENCRYPTED, this->HASH_VAR_EXISTS, this->HASH_VAR_EXTENSION, this->HASH_VAR_FULLNAME,
     this->HASH_VAR_HIDDEN, this->HASH_VAR_ISDIRECTORY, this->HASH_VAR_ISFILE, this->HASH_VAR_LIFETIME,
     this->HASH_VAR_MODIFICATION, this->HASH_VAR_NAME, this->HASH_VAR_PARENT, this->HASH_VAR_PATH,
     this->HASH_VAR_READONLY, this->HASH_VAR_SIZE,
     this->HASH_VAR_THIS, this->HASH_VAR_INDEX, this->HASH_VAR_LOCATION, this->HASH_VAR_SUCCESS
   }),

   HASH_GROUP_FUNC_BOO_STR({
     this->HASH_FUNC_ISLOWER, this->HASH_FUNC_ISUPPER, this->HASH_FUNC_ISNUMBER, this->HASH_VAR_ARCHIVE,
     this->HASH_VAR_COMPRESSED, this->HASH_VAR_EMPTY, this->HASH_VAR_ENCRYPTED, this->HASH_VAR_EXISTS,
     this->HASH_VAR_HIDDEN, this->HASH_VAR_ISDIRECTORY, this->HASH_VAR_ISFILE, this->HASH_VAR_READONLY,
     this->HASH_FUNC_ISLETTER, this->HASH_FUNC_ISDIGIT, this->HASH_FUNC_ISBINARY, this->HASH_FUNC_ISHEX
   }),

   HASH_GROUP_FUNC_NUM_NUM({
     this->HASH_FUNC_ABSOLUTE, this->HASH_FUNC_CEIL, this->HASH_FUNC_FLOOR,
     this->HASH_FUNC_ROUND, this->HASH_FUNC_SIGN, this->HASH_FUNC_SQRT, this->HASH_FUNC_TRUNCATE
   }),

   HASH_GROUP_AGGRFUNC({
     this->HASH_FUNC_AVERAGE, this->HASH_FUNC_SUM, this->HASH_FUNC_MIN, this->HASH_FUNC_MAX, this->HASH_FUNC_MEDIAN
   }),

   HASH_GROUP_FUNC_STR_STR({
     this->HASH_FUNC_DIGITS,  this->HASH_FUNC_LETTERS, this->HASH_FUNC_LOWER, this->HASH_FUNC_TRIM,
     this->HASH_FUNC_UPPER, this->HASH_FUNC_REVERSE, this->HASH_FUNC_AFTERDIGITS, this->HASH_FUNC_AFTERLETTERS,
     this->HASH_FUNC_BEFOREDIGITS, this->HASH_FUNC_BEFORELETTERS, this->HASH_FUNC_CAPITALIZE,
     this->HASH_VAR_DRIVE, this->HASH_VAR_EXTENSION, this->HASH_VAR_FULLNAME, this->HASH_VAR_NAME, this->HASH_VAR_PARENT
   }),

   HASH_GROUP_FUNC_STR_STR_NUM({
     this->HASH_FUNC_REPEAT, this->HASH_FUNC_LEFT, this->HASH_FUNC_RIGHT, this->HASH_FUNC_FILL
   }),

   HASH_GROUP_FUNC_STR_NUM({
     this->HASH_FUNC_ROMAN, this->HASH_FUNC_BINARY, this->HASH_FUNC_HEX
   }),

   HASH_GROUP_FUNC_TIM_NUM({
     this->HASH_FUNC_CHRISTMAS, this->HASH_FUNC_EASTER, this->HASH_FUNC_NEWYEAR
   }),

   HASH_MAP_MONTHS({
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
   }),

   HASH_MAP_WEEKDAYS({
      { this->HASH_VAR_MONDAY,    1 },
      { this->HASH_VAR_TUESDAY,   2 },
      { this->HASH_VAR_WEDNESDAY, 3 },
      { this->HASH_VAR_THURSDAY,  4 },
      { this->HASH_VAR_FRIDAY,    5 },
      { this->HASH_VAR_SATURDAY,  6 },
      { this->HASH_VAR_SUNDAY,    7 }
   }),

   HASH_MAP_PERIOD_UNITS({
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
   }) { };

}

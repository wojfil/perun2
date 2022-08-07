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

#ifndef HASH_H_INCLUDED
#define HASH_H_INCLUDED

#include "datatype/primitives.h"
#include "datatype/period.h"
#include <set>
#include <map>


_size stringHash(const _str& value);
_size rawStringHash(const _str& value);


// prepare hashes for language keywords
// so, instead of comparing strings, we will compare hashes
struct Hashes
{
public:

   Hashes();

   const _size HASH_NOTHING;
   const _size HASH_VAR_THIS;

   const _size HASH_VAR_ACCESS;
   const _size HASH_VAR_ARCHIVE;
   const _size HASH_VAR_CHANGE;
   const _size HASH_VAR_COMPRESSED;
   const _size HASH_VAR_CREATION;
   const _size HASH_VAR_DEPTH;
   const _size HASH_VAR_DRIVE;
   const _size HASH_VAR_EMPTY;
   const _size HASH_VAR_ENCRYPTED;
   const _size HASH_VAR_EXISTS;
   const _size HASH_VAR_EXTENSION;
   const _size HASH_VAR_FULLNAME;
   const _size HASH_VAR_HIDDEN;
   const _size HASH_VAR_INDEX;
   const _size HASH_VAR_ISDIRECTORY;
   const _size HASH_VAR_ISFILE;
   const _size HASH_VAR_LIFETIME;
   const _size HASH_VAR_LOCATION;
   const _size HASH_VAR_MODIFICATION;
   const _size HASH_VAR_NAME;
   const _size HASH_VAR_PARENT;
   const _size HASH_VAR_PATH;
   const _size HASH_VAR_READONLY;
   const _size HASH_VAR_SIZE;
   const _size HASH_VAR_SUCCESS;

   const _size HASH_VAR_NOW;
   const _size HASH_VAR_TODAY;
   const _size HASH_VAR_YESTERDAY;
   const _size HASH_VAR_TOMORROW;

   const _size HASH_VAR_JANUARY;
   const _size HASH_VAR_FEBRUARY;
   const _size HASH_VAR_MARCH;
   const _size HASH_VAR_APRIL;
   const _size HASH_VAR_MAY;
   const _size HASH_VAR_JUNE;
   const _size HASH_VAR_JULY;
   const _size HASH_VAR_AUGUST;
   const _size HASH_VAR_SEPTEMBER;
   const _size HASH_VAR_OCTOBER;
   const _size HASH_VAR_NOVEMBER;
   const _size HASH_VAR_DECEMBER;

   const _size HASH_VAR_MONDAY;
   const _size HASH_VAR_TUESDAY;
   const _size HASH_VAR_WEDNESDAY;
   const _size HASH_VAR_THURSDAY;
   const _size HASH_VAR_FRIDAY;
   const _size HASH_VAR_SATURDAY;
   const _size HASH_VAR_SUNDAY;

   const _size HASH_VAR_ALPHABET;
   const _size HASH_VAR_ASCII;
   const _size HASH_VAR_ARGUMENTS;

   const _size HASH_VAR_DESKTOP;
   const _size HASH_VAR_UROBOROS;
   const _size HASH_VAR_DEVICE;

   const _size HASH_VAR_DIRECTORIES;
   const _size HASH_VAR_FILES;
   const _size HASH_VAR_RECURSIVEFILES;
   const _size HASH_VAR_RECURSIVEDIRECTORIES;
   const _size HASH_VAR_FILE;
   const _size HASH_VAR_DIRECTORY;

   const _size HASH_PER_YEAR;
   const _size HASH_PER_MONTH;
   const _size HASH_PER_WEEK;
   const _size HASH_PER_DAY;
   const _size HASH_PER_HOUR;
   const _size HASH_PER_MINUTE;
   const _size HASH_PER_SECOND;

   const _size HASH_PER_YEARS;
   const _size HASH_PER_MONTHS;
   const _size HASH_PER_WEEKS;
   const _size HASH_PER_DAYS;
   const _size HASH_PER_HOURS;
   const _size HASH_PER_MINUTES;
   const _size HASH_PER_SECONDS;

   const _size HASH_PER_DATE;
   const _size HASH_PER_WEEKDAY;

   const _size HASH_FUNC_ISLOWER;
   const _size HASH_FUNC_ISUPPER;
   const _size HASH_FUNC_ISNUMBER;
   const _size HASH_FUNC_ISLETTER;
   const _size HASH_FUNC_ISDIGIT;
   const _size HASH_FUNC_ISBINARY;
   const _size HASH_FUNC_ISHEX;

   const _size HASH_FUNC_EXIST;
   const _size HASH_FUNC_ANY;
   const _size HASH_FUNC_ANYINSIDE;
   const _size HASH_FUNC_CONTAINS;
   const _size HASH_FUNC_EXISTSINSIDE;
   const _size HASH_FUNC_EXISTINSIDE;
   const _size HASH_FUNC_STARTSWITH;
   const _size HASH_FUNC_ENDSWITH;

   const _size HASH_FUNC_ABSOLUTE;
   const _size HASH_FUNC_CEIL;
   const _size HASH_FUNC_FLOOR;
   const _size HASH_FUNC_ROUND;
   const _size HASH_FUNC_SIGN;
   const _size HASH_FUNC_SQRT;
   const _size HASH_FUNC_TRUNCATE;

   const _size HASH_FUNC_AVERAGE;
   const _size HASH_FUNC_SUM;
   const _size HASH_FUNC_MIN;
   const _size HASH_FUNC_MAX;
   const _size HASH_FUNC_MEDIAN;

   const _size HASH_FUNC_LENGTH;
   const _size HASH_FUNC_FROMBINARY;
   const _size HASH_FUNC_FROMHEX;
   const _size HASH_FUNC_SIZE;
   const _size HASH_FUNC_NUMBER;
   const _size HASH_FUNC_COUNT;
   const _size HASH_FUNC_COUNTINSIDE;
   const _size HASH_FUNC_POWER;

   const _size HASH_FUNC_FIRST;
   const _size HASH_FUNC_LAST;
   const _size HASH_FUNC_RANDOM;
   const _size HASH_FUNC_LIFETIME;

   const _size HASH_FUNC_AFTER;
   const _size HASH_FUNC_BEFORE;
   const _size HASH_FUNC_REVERSED;

   const _size HASH_FUNC_DIGITS;
   const _size HASH_FUNC_LETTERS;
   const _size HASH_FUNC_LOWER;
   const _size HASH_FUNC_TRIM;
   const _size HASH_FUNC_UPPER;
   const _size HASH_FUNC_REVERSE;
   const _size HASH_FUNC_AFTERDIGITS;
   const _size HASH_FUNC_AFTERLETTERS;
   const _size HASH_FUNC_BEFOREDIGITS;
   const _size HASH_FUNC_BEFORELETTERS;
   const _size HASH_FUNC_CAPITALIZE;

   const _size HASH_FUNC_REPLACE;
   const _size HASH_FUNC_SUBSTRING;
   const _size HASH_FUNC_CONCATENATE;
   const _size HASH_FUNC_PATH;
   const _size HASH_FUNC_STRING;
   const _size HASH_FUNC_MONTHNAME;
   const _size HASH_FUNC_WEEKDAYNAME;
   const _size HASH_FUNC_JOIN;

   const _size HASH_FUNC_REPEAT;
   const _size HASH_FUNC_LEFT;
   const _size HASH_FUNC_RIGHT;
   const _size HASH_FUNC_FILL;

   const _size HASH_FUNC_ROMAN;
   const _size HASH_FUNC_BINARY;
   const _size HASH_FUNC_HEX;

   const _size HASH_FUNC_CHRISTMAS;
   const _size HASH_FUNC_EASTER;
   const _size HASH_FUNC_NEWYEAR;

   const _size HASH_FUNC_DATE;
   const _size HASH_FUNC_TIME;
   const _size HASH_FUNC_CHARACTERS;
   const _size HASH_FUNC_WORDS;
   const _size HASH_FUNC_SPLIT;
   const _size HASH_FUNC_NUMBERS;

   const std::set<_size> HASH_GROUP_MONTHS;
   const std::set<_size> HASH_GROUP_WEEKDAYS;
   const std::set<_size> HASH_GROUP_PERIOD_SINGLE;
   const std::set<_size> HASH_GROUP_PERIOD_MULTI;
   const std::set<_size> HASH_GROUP_TIME_ATTR;
   const std::set<_size> HASH_GROUP_ATTR;
   const std::set<_size> HASH_GROUP_INNERVAR;
   const std::set<_size> HASH_GROUP_FUNC_BOO_STR;
   const std::set<_size> HASH_GROUP_FUNC_NUM_NUM;
   const std::set<_size> HASH_GROUP_AGGRFUNC;
   const std::set<_size> HASH_GROUP_FUNC_STR_STR;
   const std::set<_size> HASH_GROUP_FUNC_STR_STR_NUM;
   const std::set<_size> HASH_GROUP_FUNC_STR_NUM;
   const std::set<_size> HASH_GROUP_FUNC_TIM_NUM;
   const std::map<_size, _tnum> HASH_MAP_MONTHS;
   const std::map<_size, _tnum> HASH_MAP_WEEKDAYS;
   const std::map<_size, Period::PeriodUnit> HASH_MAP_PERIOD_UNITS;

};


#endif // HASH_H_INCLUDED

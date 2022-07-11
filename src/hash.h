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

};


#endif // HASH_H_INCLUDED

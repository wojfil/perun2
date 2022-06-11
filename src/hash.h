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

void initHashes();

// prepare hashes for language keywords
// so, instead of comparing strings, we will compare hashes

extern _size HASH_NOTHING;

extern _size HASH_VAR_THIS;
extern _size HASH_VAR_ACCESS;
extern _size HASH_VAR_ARCHIVE;
extern _size HASH_VAR_CHANGE;
extern _size HASH_VAR_COMPRESSED;
extern _size HASH_VAR_CREATION;
extern _size HASH_VAR_DEPTH;
extern _size HASH_VAR_DRIVE;
extern _size HASH_VAR_EMPTY;
extern _size HASH_VAR_ENCRYPTED;
extern _size HASH_VAR_EXISTS;
extern _size HASH_VAR_EXTENSION;
extern _size HASH_VAR_FULLNAME;
extern _size HASH_VAR_HIDDEN;
extern _size HASH_VAR_INDEX;
extern _size HASH_VAR_ISDIRECTORY;
extern _size HASH_VAR_ISFILE;
extern _size HASH_VAR_LIFETIME;
extern _size HASH_VAR_LOCATION;
extern _size HASH_VAR_MODIFICATION;
extern _size HASH_VAR_NAME;
extern _size HASH_VAR_PARENT;
extern _size HASH_VAR_PATH;
extern _size HASH_VAR_READONLY;
extern _size HASH_VAR_SIZE;
extern _size HASH_VAR_SUCCESS;

extern _size HASH_VAR_NOW;
extern _size HASH_VAR_TODAY;
extern _size HASH_VAR_YESTERDAY;
extern _size HASH_VAR_TOMORROW;

extern _size HASH_VAR_JANUARY;
extern _size HASH_VAR_FEBRUARY;
extern _size HASH_VAR_MARCH;
extern _size HASH_VAR_APRIL;
extern _size HASH_VAR_MAY;
extern _size HASH_VAR_JUNE;
extern _size HASH_VAR_JULY;
extern _size HASH_VAR_AUGUST;
extern _size HASH_VAR_SEPTEMBER;
extern _size HASH_VAR_OCTOBER;
extern _size HASH_VAR_NOVEMBER;
extern _size HASH_VAR_DECEMBER;

extern _size HASH_VAR_MONDAY;
extern _size HASH_VAR_TUESDAY;
extern _size HASH_VAR_WEDNESDAY;
extern _size HASH_VAR_THURSDAY;
extern _size HASH_VAR_FRIDAY;
extern _size HASH_VAR_SATURDAY;
extern _size HASH_VAR_SUNDAY;

extern _size HASH_VAR_ALPHABET;
extern _size HASH_VAR_ASCII;
extern _size HASH_VAR_ARGUMENTS;

extern _size HASH_VAR_DESKTOP;
extern _size HASH_VAR_UROBOROS;

extern _size HASH_VAR_ALL;
extern _size HASH_VAR_DIRECTORIES;
extern _size HASH_VAR_FILES;
extern _size HASH_VAR_RECURSIVEFILES;
extern _size HASH_VAR_RECURSIVEDIRECTORIES;
extern _size HASH_VAR_FILE;
extern _size HASH_VAR_DIRECTORY;

extern _size HASH_PER_YEAR;
extern _size HASH_PER_MONTH;
extern _size HASH_PER_WEEK;
extern _size HASH_PER_DAY;
extern _size HASH_PER_HOUR;
extern _size HASH_PER_MINUTE;
extern _size HASH_PER_SECOND;

extern _size HASH_PER_YEARS;
extern _size HASH_PER_MONTHS;
extern _size HASH_PER_WEEKS;
extern _size HASH_PER_DAYS;
extern _size HASH_PER_HOURS;
extern _size HASH_PER_MINUTES;
extern _size HASH_PER_SECONDS;

extern _size HASH_PER_DATE;
extern _size HASH_PER_WEEKDAY;

extern _size HASH_FUNC_ISLOWER;
extern _size HASH_FUNC_ISUPPER;
extern _size HASH_FUNC_ISNUMBER;
extern _size HASH_FUNC_ISLETTER;
extern _size HASH_FUNC_ISDIGIT;
extern _size HASH_FUNC_ISBINARY;
extern _size HASH_FUNC_ISHEX;

extern _size HASH_FUNC_EXIST;
extern _size HASH_FUNC_ANY;
extern _size HASH_FUNC_ANYINSIDE;
extern _size HASH_FUNC_CONTAINS;
extern _size HASH_FUNC_EXISTSINSIDE;
extern _size HASH_FUNC_EXISTINSIDE;
extern _size HASH_FUNC_STARTSWITH;
extern _size HASH_FUNC_ENDSWITH;

extern _size HASH_FUNC_ABSOLUTE;
extern _size HASH_FUNC_CEIL;
extern _size HASH_FUNC_FLOOR;
extern _size HASH_FUNC_ROUND;
extern _size HASH_FUNC_SIGN;
extern _size HASH_FUNC_SQRT;
extern _size HASH_FUNC_TRUNCATE;

extern _size HASH_FUNC_AVERAGE;
extern _size HASH_FUNC_SUM;
extern _size HASH_FUNC_MIN;
extern _size HASH_FUNC_MAX;
extern _size HASH_FUNC_MEDIAN;

extern _size HASH_FUNC_LENGTH;
extern _size HASH_FUNC_FROMBINARY;
extern _size HASH_FUNC_FROMHEX;
extern _size HASH_FUNC_SIZE;
extern _size HASH_FUNC_NUMBER;
extern _size HASH_FUNC_COUNT;
extern _size HASH_FUNC_COUNTINSIDE;
extern _size HASH_FUNC_POWER;

extern _size HASH_FUNC_FIRST;
extern _size HASH_FUNC_LAST;
extern _size HASH_FUNC_RANDOM;
extern _size HASH_FUNC_LIFETIME;

extern _size HASH_FUNC_AFTER;
extern _size HASH_FUNC_BEFORE;
extern _size HASH_FUNC_REVERSED;

extern _size HASH_FUNC_DIGITS;
extern _size HASH_FUNC_LETTERS;
extern _size HASH_FUNC_LOWER;
extern _size HASH_FUNC_TRIM;
extern _size HASH_FUNC_UPPER;
extern _size HASH_FUNC_REVERSE;
extern _size HASH_FUNC_AFTERDIGITS;
extern _size HASH_FUNC_AFTERLETTERS;
extern _size HASH_FUNC_BEFOREDIGITS;
extern _size HASH_FUNC_BEFORELETTERS;
extern _size HASH_FUNC_CAPITALIZE;

extern _size HASH_FUNC_REPLACE;
extern _size HASH_FUNC_SUBSTRING;
extern _size HASH_FUNC_CONCATENATE;
extern _size HASH_FUNC_PATH;
extern _size HASH_FUNC_STRING;
extern _size HASH_FUNC_MONTHNAME;
extern _size HASH_FUNC_WEEKDAYNAME;
extern _size HASH_FUNC_JOIN;

extern _size HASH_FUNC_REPEAT;
extern _size HASH_FUNC_LEFT;
extern _size HASH_FUNC_RIGHT;
extern _size HASH_FUNC_FILL;

extern _size HASH_FUNC_ROMAN;
extern _size HASH_FUNC_BINARY;
extern _size HASH_FUNC_HEX;

extern _size HASH_FUNC_CHRISTMAS;
extern _size HASH_FUNC_EASTER;
extern _size HASH_FUNC_NEWYEAR;

extern _size HASH_FUNC_DATE;
extern _size HASH_FUNC_TIME;
extern _size HASH_FUNC_CHARACTERS;
extern _size HASH_FUNC_WORDS;
extern _size HASH_FUNC_SPLIT;
extern _size HASH_FUNC_NUMBERS;

extern std::set<_size> HASH_GROUP_MONTHS;
extern std::set<_size> HASH_GROUP_WEEKDAYS;
extern std::set<_size> HASH_GROUP_PERIOD_SINGLE;
extern std::set<_size> HASH_GROUP_PERIOD_MULTI;
extern std::set<_size> HASH_GROUP_TIME_ATTR;
extern std::set<_size> HASH_GROUP_ATTR;
extern std::set<_size> HASH_GROUP_INNERVAR;
extern std::set<_size> HASH_GROUP_FUNC_BOO_STR;
extern std::set<_size> HASH_GROUP_FUNC_NUM_NUM;
extern std::set<_size> HASH_GROUP_AGGRFUNC;
extern std::set<_size> HASH_GROUP_FUNC_STR_STR;
extern std::set<_size> HASH_GROUP_FUNC_STR_STR_NUM;
extern std::set<_size> HASH_GROUP_FUNC_STR_NUM;
extern std::set<_size> HASH_GROUP_FUNC_TIM_NUM;
extern std::map<_size, _tnum> HASH_MAP_MONTHS;
extern std::map<_size, _tnum> HASH_MAP_WEEKDAYS;
extern std::map<_size, Period::PeriodUnit> HASH_MAP_PERIOD_UNITS;

#endif // HASH_H_INCLUDED

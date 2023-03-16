/*
    This file is part of Perun2.
    Perun2 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    Peruns2 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with Perun2. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef HASH_H_INCLUDED
#define HASH_H_INCLUDED

#include "datatype/primitives.h"
#include "datatype/period.h"
#include <unordered_set>
#include <unordered_map>


namespace perun2
{

typedef std::size_t   _hash;

_hash stringHash(const _str& value);
_hash rawStringHash(const _str& value);

// prepare hashes for language keywords
// so, instead of comparing strings, we will compare hashes
struct Hashes
{
public:

   Hashes();

   const _hash HASH_NOTHING;
   const _hash HASH_VAR_THIS;

   const _hash HASH_VAR_ACCESS;
   const _hash HASH_VAR_ARCHIVE;
   const _hash HASH_VAR_CHANGE;
   const _hash HASH_VAR_COMPRESSED;
   const _hash HASH_VAR_CREATION;
   const _hash HASH_VAR_DEPTH;
   const _hash HASH_VAR_DRIVE;
   const _hash HASH_VAR_EMPTY;
   const _hash HASH_VAR_ENCRYPTED;
   const _hash HASH_VAR_EXISTS;
   const _hash HASH_VAR_EXTENSION;
   const _hash HASH_VAR_FULLNAME;
   const _hash HASH_VAR_HIDDEN;
   const _hash HASH_VAR_INDEX;
   const _hash HASH_VAR_ISDIRECTORY;
   const _hash HASH_VAR_ISFILE;
   const _hash HASH_VAR_LIFETIME;
   const _hash HASH_VAR_LOCATION;
   const _hash HASH_VAR_MODIFICATION;
   const _hash HASH_VAR_NAME;
   const _hash HASH_VAR_PARENT;
   const _hash HASH_VAR_PATH;
   const _hash HASH_VAR_READONLY;
   const _hash HASH_VAR_SIZE;
   const _hash HASH_VAR_SUCCESS;

   const _hash HASH_VAR_NOW;
   const _hash HASH_VAR_TODAY;
   const _hash HASH_VAR_YESTERDAY;
   const _hash HASH_VAR_TOMORROW;

   const _hash HASH_VAR_JANUARY;
   const _hash HASH_VAR_FEBRUARY;
   const _hash HASH_VAR_MARCH;
   const _hash HASH_VAR_APRIL;
   const _hash HASH_VAR_MAY;
   const _hash HASH_VAR_JUNE;
   const _hash HASH_VAR_JULY;
   const _hash HASH_VAR_AUGUST;
   const _hash HASH_VAR_SEPTEMBER;
   const _hash HASH_VAR_OCTOBER;
   const _hash HASH_VAR_NOVEMBER;
   const _hash HASH_VAR_DECEMBER;

   const _hash HASH_VAR_MONDAY;
   const _hash HASH_VAR_TUESDAY;
   const _hash HASH_VAR_WEDNESDAY;
   const _hash HASH_VAR_THURSDAY;
   const _hash HASH_VAR_FRIDAY;
   const _hash HASH_VAR_SATURDAY;
   const _hash HASH_VAR_SUNDAY;

   const _hash HASH_VAR_ALPHABET;
   const _hash HASH_VAR_ASCII;
   const _hash HASH_VAR_ARGUMENTS;

   const _hash HASH_VAR_DESKTOP;
   const _hash HASH_VAR_PERUN2;
   const _hash HASH_VAR_ORIGIN;

   const _hash HASH_VAR_DIRECTORIES;
   const _hash HASH_VAR_FILES;
   const _hash HASH_VAR_RECURSIVEFILES;
   const _hash HASH_VAR_RECURSIVEDIRECTORIES;

   const _hash HASH_PER_YEAR;
   const _hash HASH_PER_MONTH;
   const _hash HASH_PER_WEEK;
   const _hash HASH_PER_DAY;
   const _hash HASH_PER_HOUR;
   const _hash HASH_PER_MINUTE;
   const _hash HASH_PER_SECOND;

   const _hash HASH_PER_YEARS;
   const _hash HASH_PER_MONTHS;
   const _hash HASH_PER_WEEKS;
   const _hash HASH_PER_DAYS;
   const _hash HASH_PER_HOURS;
   const _hash HASH_PER_MINUTES;
   const _hash HASH_PER_SECONDS;

   const _hash HASH_PER_DATE;
   const _hash HASH_PER_WEEKDAY;

   const _hash HASH_FUNC_ISLOWER;
   const _hash HASH_FUNC_ISUPPER;
   const _hash HASH_FUNC_ISNUMBER;
   const _hash HASH_FUNC_ISLETTER;
   const _hash HASH_FUNC_ISDIGIT;
   const _hash HASH_FUNC_ISBINARY;
   const _hash HASH_FUNC_ISHEX;

   const _hash HASH_FUNC_ANYINSIDE;
   const _hash HASH_FUNC_CONTAINS;
   const _hash HASH_FUNC_EXISTSINSIDE;
   const _hash HASH_FUNC_EXISTINSIDE;
   const _hash HASH_FUNC_STARTSWITH;
   const _hash HASH_FUNC_ENDSWITH;
   const _hash HASH_FUNC_FINDTEXT;

   const _hash HASH_FUNC_ABSOLUTE;
   const _hash HASH_FUNC_CEIL;
   const _hash HASH_FUNC_FLOOR;
   const _hash HASH_FUNC_ROUND;
   const _hash HASH_FUNC_SIGN;
   const _hash HASH_FUNC_SQRT;
   const _hash HASH_FUNC_TRUNCATE;

   const _hash HASH_FUNC_AVERAGE;
   const _hash HASH_FUNC_SUM;
   const _hash HASH_FUNC_MIN;
   const _hash HASH_FUNC_MAX;
   const _hash HASH_FUNC_MEDIAN;

   const _hash HASH_FUNC_LENGTH;
   const _hash HASH_FUNC_FROMBINARY;
   const _hash HASH_FUNC_FROMHEX;
   const _hash HASH_FUNC_SIZE;
   const _hash HASH_FUNC_NUMBER;
   const _hash HASH_FUNC_COUNTINSIDE;
   const _hash HASH_FUNC_POWER;

   const _hash HASH_FUNC_ANY;
   const _hash HASH_FUNC_EXIST;
   const _hash HASH_FUNC_EXISTS;
   const _hash HASH_FUNC_COUNT;

   const _hash HASH_FUNC_FIRST;
   const _hash HASH_FUNC_LAST;
   const _hash HASH_FUNC_RANDOM;
   const _hash HASH_FUNC_LIFETIME;

   const _hash HASH_FUNC_AFTER;
   const _hash HASH_FUNC_BEFORE;
   const _hash HASH_FUNC_REVERSED;

   const _hash HASH_FUNC_DIGITS;
   const _hash HASH_FUNC_LETTERS;
   const _hash HASH_FUNC_LOWER;
   const _hash HASH_FUNC_TRIM;
   const _hash HASH_FUNC_UPPER;
   const _hash HASH_FUNC_REVERSE;
   const _hash HASH_FUNC_AFTERDIGITS;
   const _hash HASH_FUNC_AFTERLETTERS;
   const _hash HASH_FUNC_BEFOREDIGITS;
   const _hash HASH_FUNC_BEFORELETTERS;
   const _hash HASH_FUNC_CAPITALIZE;

   const _hash HASH_FUNC_REPLACE;
   const _hash HASH_FUNC_SUBSTRING;
   const _hash HASH_FUNC_CONCATENATE;
   const _hash HASH_FUNC_PATH;
   const _hash HASH_FUNC_STRING;
   const _hash HASH_FUNC_MONTHNAME;
   const _hash HASH_FUNC_WEEKDAYNAME;
   const _hash HASH_FUNC_JOIN;

   const _hash HASH_FUNC_REPEAT;
   const _hash HASH_FUNC_LEFT;
   const _hash HASH_FUNC_RIGHT;
   const _hash HASH_FUNC_FILL;

   const _hash HASH_FUNC_ROMAN;
   const _hash HASH_FUNC_BINARY;
   const _hash HASH_FUNC_HEX;

   const _hash HASH_FUNC_CHRISTMAS;
   const _hash HASH_FUNC_EASTER;
   const _hash HASH_FUNC_NEWYEAR;

   const _hash HASH_FUNC_DATE;
   const _hash HASH_FUNC_TIME;
   const _hash HASH_FUNC_CHARACTERS;
   const _hash HASH_FUNC_WORDS;
   const _hash HASH_FUNC_SPLIT;
   const _hash HASH_FUNC_NUMBERS;

   const _hash HASH_KW_COPY;
   const _hash HASH_KW_CREATE;
   const _hash HASH_KW_CREATEFILE;
   const _hash HASH_KW_CREATEDIRECTORY;
   const _hash HASH_KW_CREATEFILES;
   const _hash HASH_KW_CREATEDIRECTORIES;
   const _hash HASH_KW_DELETE;
   const _hash HASH_KW_DROP;
   const _hash HASH_KW_HIDE;
   const _hash HASH_KW_LOCK;
   const _hash HASH_KW_MOVE;
   const _hash HASH_KW_OPEN;
   const _hash HASH_KW_REACCESS;
   const _hash HASH_KW_RECREATE;
   const _hash HASH_KW_RECHANGE;
   const _hash HASH_KW_REMODIFY;
   const _hash HASH_KW_RENAME;
   const _hash HASH_KW_SELECT;
   const _hash HASH_KW_UNHIDE;
   const _hash HASH_KW_UNLOCK;
   const _hash HASH_KW_FORCE;
   const _hash HASH_KW_STACK;
   const _hash HASH_KW_TRUE;
   const _hash HASH_KW_FALSE;
   const _hash HASH_KW_AND;
   const _hash HASH_KW_OR;
   const _hash HASH_KW_XOR;
   const _hash HASH_KW_NOT;
   const _hash HASH_KW_PRINT;
   const _hash HASH_KW_RUN;
   const _hash HASH_KW_SLEEP;
   const _hash HASH_KW_IN;
   const _hash HASH_KW_LIKE;
   const _hash HASH_KW_ELSE;
   const _hash HASH_KW_IF;
   const _hash HASH_KW_INSIDE;
   const _hash HASH_KW_TIMES;
   const _hash HASH_KW_WHILE;
   const _hash HASH_KW_EVERY;
   const _hash HASH_KW_FINAL;
   const _hash HASH_KW_LIMIT;
   const _hash HASH_KW_ORDER;
   const _hash HASH_KW_SKIP;
   const _hash HASH_KW_WHERE;
   const _hash HASH_KW_AS;
   const _hash HASH_KW_BY;
   const _hash HASH_KW_TO;
   const _hash HASH_KW_EXTENSIONLESS;
   const _hash HASH_KW_WITH;
   const _hash HASH_KW_ASC;
   const _hash HASH_KW_DESC;
   const _hash HASH_KW_BREAK;
   const _hash HASH_KW_CONTINUE;
   const _hash HASH_KW_EXIT;
   const _hash HASH_KW_ERROR;

   const std::unordered_set<_hash> HASH_GROUP_MONTHS;
   const std::unordered_set<_hash> HASH_GROUP_WEEKDAYS;
   const std::unordered_set<_hash> HASH_GROUP_PERIOD_SINGLE;
   const std::unordered_set<_hash> HASH_GROUP_PERIOD_MULTI;
   const std::unordered_set<_hash> HASH_GROUP_ATTR;
   const std::unordered_set<_hash> HASH_GROUP_TIME_ATTR;
   const std::unordered_set<_hash> HASH_GROUP_ALTERABLE_ATTR;
   const std::unordered_set<_hash> HASH_GROUP_VARS_IMMUTABLES;
   const std::unordered_set<_hash> HASH_GROUP_FUNC_BOO_STR;
   const std::unordered_set<_hash> HASH_GROUP_FUNC_NUM_NUM;
   const std::unordered_set<_hash> HASH_GROUP_AGGRFUNC;
   const std::unordered_set<_hash> HASH_GROUP_FUNC_STR_STR;
   const std::unordered_set<_hash> HASH_GROUP_FUNC_STR_STR_NUM;
   const std::unordered_set<_hash> HASH_GROUP_FUNC_STR_NUM;
   const std::unordered_set<_hash> HASH_GROUP_FUNC_TIM_NUM;
   const std::unordered_map<_hash, _tnum> HASH_MAP_MONTHS;
   const std::unordered_map<_hash, _tnum> HASH_MAP_WEEKDAYS;
   const std::unordered_map<_hash, Period::PeriodUnit> HASH_MAP_PERIOD_UNITS;

};

}

#endif // HASH_H_INCLUDED

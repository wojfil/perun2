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

#include "gen-number.h"


namespace uro::gen
{

_num Negation::getValue() {
   return -value->getValue();
};

_num Addition::getValue() {
   return value1->getValue() + value2->getValue();
};

_num Subtraction::getValue() {
   return value1->getValue() - value2->getValue();
};

_num Multiplication::getValue() {
   return value1->getValue() * value2->getValue();
};

_num Division::getValue() {
   return value1->getValue() / value2->getValue();
};

_num Modulo::getValue() {
   return value1->getValue() % value2->getValue();
};

_num TimeYears::getValue() {
   return Number(static_cast<_nint>(time->getValue().year));
};

_num TimeMonths::getValue() {
   return _num(static_cast<_nint>(time->getValue().month));
};

_num TimeWeekDay::getValue() {
   return _num(static_cast<_nint>(time->getValue().getWeekDay()));
};

_num TimeDays::getValue() {
   return _num(static_cast<_nint>(time->getValue().day));
};

_num TimeHours::getValue() {
   return _num(static_cast<_nint>(time->getValue().hour));
};

_num TimeMinutes::getValue() {
   return _num(static_cast<_nint>(time->getValue().minute));
};

_num TimeSeconds::getValue() {
   return _num(static_cast<_nint>(time->getValue().second));
};

TimeMemberAtIndex::~TimeMemberAtIndex() {
   delete times;
   delete index;
};

_bool TimeMemberAtIndex::getElement()
{
   result = times->getValue();
   const _size length = result.size();

   if (length == 0) {
      return false;
   }

   const _nint id = index->getValue().toInt();

   if (id < 0LL || id >= length) {
      return false;
   }

   time = &(result[id]);
   return true;
};

_num TimeYearsAtIndex::getValue() {
   return getElement() ? _num(time->year) : _num();
};

_num TimeMonthsAtIndex::getValue() {
   return getElement() ? _num(time->month) : _num();
};

_num TimeWeekDayAtIndex::getValue() {
   return getElement() ? _num(time->getWeekDay()) : _num();
};

_num TimeDaysAtIndex::getValue() {
   return getElement() ? _num(time->day) : _num();
};

_num TimeHoursAtIndex::getValue() {
   return getElement() ? _num(time->hour) : _num();
};

_num TimeMinutesAtIndex::getValue() {
   return getElement() ? _num(time->minute) : _num();
};

_num TimeSecondsAtIndex::getValue() {
   return getElement() ? _num(time->second) : _num();
};

}

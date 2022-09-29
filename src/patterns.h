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

#ifndef PATTERNS_H_INCLUDED
#define PATTERNS_H_INCLUDED


#define P_MEMORY_MEMBER _str prevThis; \
   _numi prevIndex; \
   _numi prevDepth;


#define P_MEMORY_LOAD this->prevThis = this->inner->this_s.value; \
   this->prevIndex = this->inner->index.value; \
   this->prevDepth = this->inner->depth.value;


#define P_MEMORY_RESTORE this->inner->this_s.value = this->prevThis; \
   this->inner->index.value = this->prevIndex; \
   this->inner->depth.value = this->prevDepth;


#define P_CHECK_LOOP_BREAK if (this->uroboros->state == State::s_Break) { \
      this->uroboros->state = State::s_Running; \
      break; \
   } \
   else if (this->uroboros->state == State::s_Continue) { \
      this->uroboros->state = State::s_Running; \
      continue; \
   }


#define P_CHECK_SOFT_LOOP_BREAK switch (this->uroboros->state) { \
      case State::s_Break: \
      case State::s_Continue: { \
         this->uroboros->state = State::s_Running; \
         break; \
      } \
   }


#define P_DIVIDE_BY_KEYWORD(kw) std::pair<Tokens, Tokens> pair = tks.divideByKeyword(Keyword::kw); \
   Tokens& left = pair.first; \
   Tokens& right = pair.second; \


#endif // PATTERNS_H_INCLUDED

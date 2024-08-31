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

#pragma once


#define P_CHECK_LOOP_BREAK if (this->perun2.state == State::s_Break) { \
      this->perun2.state = State::s_Running; \
      break; \
   } \
   else if (this->perun2.state == State::s_Continue) { \
      this->perun2.state = State::s_Running; \
      continue; \
   } \
   else if (this->perun2.state == State::s_Exit) { \
      return; \
   }


#define P_CHECK_DEFINITION_LOOP_BREAK if (this->perun2.state == State::s_Break) { \
      this->perun2.state = State::s_Running; \
      this->definition->reset(); \
      break; \
   } \
   else if (this->perun2.state == State::s_Continue) { \
      this->perun2.state = State::s_Running; \
      continue; \
   } \
   else if (this->perun2.state == State::s_Exit) { \
      return; \
   }


#define P_CHECK_SOFT_LOOP_BREAK switch (this->perun2.state) { \
      case State::s_Break: \
      case State::s_Continue: { \
         this->perun2.state = State::s_Running; \
         break; \
      } \
      case State::s_Exit: { \
         return; \
      } \
   }


#define P_DIVIDE_BY_KEYWORD(kw) std::pair<Tokens, Tokens> pair = tks.divideByKeyword(Keyword::kw); \
   Tokens& left = pair.first; \
   Tokens& right = pair.second; 


#define P_CHECK_IF_PERUN2_IS_RUNNING if (this->perun2.isNotRunning()) { return; }


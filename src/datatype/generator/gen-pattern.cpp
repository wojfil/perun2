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

#include "gen-pattern.h"
#include "gen-string.h"
#include "gen-generic.h"
#include "gen-definition.h"
#include "../../os.h"


namespace uro::gen
{

_bool PatternParser::parse(const _str& originPattern, _def*& result, const _int& line) const
{
   const _str pattern = os_trim(originPattern);

   if (pattern == L"*") {
      result = this->defGenerator.generatePattern(
         new LocationReference(this->uroboros), OsElement::oe_All, OS_SEPARATOR_ASTERISK, false);
      return true;
   }
   else if (pattern == L"**") {
      result = this->defGenerator.generatePattern(
         new LocationReference(this->uroboros), OsElement::oe_RecursiveAll, OS_SEPARATOR_ASTERISK, false);
      return true;
   }

   const _uint32 info = os_patternInfo(pattern);

   if (!(info & PATTERN_INFO_VALID)) {
      throw SyntaxException(str(L"asterisk pattern '", originPattern, L"' is not valid"), line);
   }

   const _bool isAbsolute = info & PATTERN_INFO_IS_ABSOLUTE;
   const _size length = pattern.size();
   _int separatorId = -1;

   for (_size i = (isAbsolute ? 3 : 0); i < length; i++) {
      switch (pattern[i]) {
         case OS_SEPARATOR: {
            separatorId = static_cast<_int>(i);
            break;
         }
         case L'*': {
            goto exitParser;
         }
      }
   }

exitParser:

   Generator<_str>* base;
   if (separatorId == -1) {
      if (isAbsolute) {
         base = new Constant<_str>(pattern.substr(0, 2));
      }
      else {
         base = new LocationReference(this->uroboros);
      }
   }
   else {
      if (isAbsolute) {
         base = new Constant<_str>(pattern.substr(0, separatorId));
      }
      else {
         base = new RelativeLocation(new Constant<_str>(pattern.substr(0, separatorId)), this->uroboros);
      }
   }

   if (info & PATTERN_INFO_ONE_ASTERISK) {
      _int separatorId2 = -1;

      for (_size i = separatorId + ((isAbsolute && separatorId == -1) ? 4 : 1); i < length; i++) {
         if (pattern[i] == OS_SEPARATOR) {
            separatorId2 = static_cast<_int>(i);
            break;
         }
      }

      if (separatorId2 == -1) {
         if (separatorId == -1) {
            result = this->defGenerator.generatePattern(base, OsElement::oe_All, 
               str(OS_SEPARATOR_STRING, isAbsolute ? pattern.substr(3) : pattern), isAbsolute);
         }
         else {
            result = this->defGenerator.generatePattern(base, OsElement::oe_All, 
               pattern.substr(separatorId), isAbsolute);
         }
         
         return true;
      }

      const _str suffix = pattern.substr(separatorId2 + 1);
      const _int patternStart = (separatorId == -1 ? ((isAbsolute ? 3 : 0)) : separatorId);
      const _int patternLength = separatorId2 - patternStart;
      const _str p = (separatorId == -1) 
         ? str(OS_SEPARATOR_STRING, pattern.substr(patternStart, patternLength))
         : pattern.substr(patternStart, patternLength);

      _def* d = this->defGenerator.generatePattern(base, OsElement::oe_All, p, isAbsolute);
      result = new DefinitionSuffix(d, this->uroboros, suffix, isAbsolute);
      return true;
   }

   if (info & PATTERN_INFO_ONE_ASTERISK) {
      // todo
   }

   if (info & PATTERN_INFO_DOUBLE_ASTERISK) {
      // todo
   }

   delete base;
   return false;
};

}

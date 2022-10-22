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

#define P_PATTERN_START_ID (isAbsolute ? 3 : 0)


_bool PatternParser::parse(const _str& originPattern, _def*& result, const _int& line) const
{
   const _str pattern = os_trim(originPattern);

   if (pattern == L"*") {
      result = this->defGenerator.generatePattern(
         new LocationReference(this->uroboros), OsElement::oe_All, OS_SEPARATOR_ASTERISK, false, EMPTY_STRING);
      return true;
   }
   else if (pattern == L"**") {
      result = this->defGenerator.generatePattern(
         new LocationReference(this->uroboros), OsElement::oe_RecursiveAll, OS_SEPARATOR_ASTERISK, false, EMPTY_STRING);
      return true;
   }

   const _uint32 info = os_patternInfo(pattern);

   if ((info & PATTERN_INFO_VALID) == 0 || (info & PATTERN_INFO_DOUBLE_ASTERISK) != 0) {
      throw SyntaxException(str(L"asterisk pattern '", originPattern, L"' is not valid"), line);
   }

   const _bool isAbsolute = info & PATTERN_INFO_IS_ABSOLUTE;
   const _size totalLength = pattern.size();
   _int separatorId = -1;

   for (_size i = P_PATTERN_START_ID; i < totalLength; i++) {
      switch (pattern[i]) {
         case OS_SEPARATOR: {
            separatorId = static_cast<_int>(i);
            break;
         }
         case L'*': {
            goto exitParseBeginning;
         }
      }
   }

exitParseBeginning:

   _str prefix;

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
         prefix = pattern.substr(0, separatorId);
         base = new RelativeLocation(new Constant<_str>(prefix), this->uroboros);
         prefix += OS_SEPARATOR;
      }
   }

   if ((info & PATTERN_INFO_ONE_ASTERISK) != 0) {
      _int separatorId2 = -1;

      for (_size i = separatorId + ((isAbsolute && separatorId == -1) ? 4 : 1); i < totalLength; i++) {
         if (pattern[i] == OS_SEPARATOR) {
            separatorId2 = static_cast<_int>(i);
            break;
         }
      }

      if (separatorId2 == -1) {
         if (separatorId == -1) {
            result = this->defGenerator.generatePattern(base, OsElement::oe_All,
               str(OS_SEPARATOR_STRING, isAbsolute ? pattern.substr(3) : pattern), isAbsolute, prefix);
         }
         else {
            const _str s = pattern.substr(separatorId);
            result = this->defGenerator.generatePattern(base, OsElement::oe_All, s, isAbsolute, prefix);
         }

         return true;
      }

      const _str suffix = str(OS_SEPARATOR_STRING, pattern.substr(separatorId2 + 1));
      const _int patternStart = separatorId == -1 ? P_PATTERN_START_ID : separatorId;
      const _int patternLength = separatorId2 - patternStart;
      const _str p = (separatorId == -1)
         ? str(OS_SEPARATOR_STRING, pattern.substr(patternStart, patternLength))
         : pattern.substr(patternStart, patternLength);

      _def* d = this->defGenerator.generatePattern(base, OsElement::oe_Directories, p, isAbsolute, prefix);
      result = new DefinitionSuffix(d, this->uroboros, suffix, isAbsolute, false);
      return true;
   }

   _size start = separatorId == -1 ? P_PATTERN_START_ID : static_cast<_size>(separatorId + 1);
   _bool hasAsterisk = false;
   _str asteriskPart;
   _str suffixPart;
   std::vector<PatternUnit> units;

   for (_size i = start; i < totalLength; i++) {
      switch (pattern[i]) {
         case OS_SEPARATOR: {
            this->addUnit(asteriskPart, suffixPart, pattern.substr(start, i - start), hasAsterisk, units);
            start = i + 1;
            hasAsterisk = false;
            break;
         }
         case L'*': {
            hasAsterisk = true;
            break;
         }
      }
   }

   this->addUnit(asteriskPart, suffixPart, pattern.substr(start), hasAsterisk, units);

   if (suffixPart.empty()) {
      units.emplace_back(asteriskPart);
   }
   else {
      units.emplace_back(asteriskPart, suffixPart);
   }

   const _size ulen = units.size();

   if (ulen == 1) {
      const PatternUnit& u = units[0];
      const _str p = str(OS_SEPARATOR_STRING, u.asteriskPart);

      if (u.suffixPart.empty()) {
         result = this->defGenerator.generatePattern(base, OsElement::oe_All, p, isAbsolute, prefix);
      }
      else {
         _def* d = this->defGenerator.generatePattern(base, OsElement::oe_Directories, p, isAbsolute, prefix);
         result = new DefinitionSuffix(d, this->uroboros, u.suffixPart, isAbsolute, false);
      }

      return true;
   }
   else if (ulen == 2) {
      const PatternUnit& u0 = units[0];
      const PatternUnit& u1 = units[1];

      const _str p0 = str(OS_SEPARATOR_STRING, u0.asteriskPart);

      _def* d0;

      if (u0.suffixPart.empty()) {
         d0 = this->defGenerator.generatePattern(base, OsElement::oe_Directories, p0, isAbsolute, prefix);
      }
      else {
         _def* d = this->defGenerator.generatePattern(base, OsElement::oe_Directories, p0, isAbsolute, prefix);
         d0 = new DefinitionSuffix(d, this->uroboros, u0.suffixPart, isAbsolute, true);
      }

      LocationVessel* vessel = new LocationVessel(isAbsolute, this->uroboros);
      _def* d1;
      const _str p1 = str(OS_SEPARATOR_STRING, u1.asteriskPart);

      if (u1.suffixPart.empty()) {
         d1 = this->defGenerator.generatePattern(vessel, OsElement::oe_All, p1, isAbsolute, prefix);
      }
      else {
         _def* d = this->defGenerator.generatePattern(vessel, OsElement::oe_Directories, p1, isAbsolute, prefix);
         d1 = new DefinitionSuffix(d, this->uroboros, u1.suffixPart, isAbsolute, false);
      }

      result = new NestedDefiniton(vessel, d1, d0, isAbsolute);
      return true;
   }


   delete base;
   return false;
}

void PatternParser::addUnit(_str& asteriskPart, _str& suffixPart, const _str& part,
   const _bool& hasAsterisk, std::vector<PatternUnit>& units) const
{
   if (asteriskPart.empty()) {
      asteriskPart = part;
   }
   else {
      if (hasAsterisk) {
         if (suffixPart.empty()) {
            units.emplace_back(asteriskPart);
         }
         else {
            units.emplace_back(asteriskPart, suffixPart);
            suffixPart.clear();
         }
         asteriskPart = part;
      }
      else {
         suffixPart = str(suffixPart, OS_SEPARATOR_STRING, part);
      }
   }
}

}

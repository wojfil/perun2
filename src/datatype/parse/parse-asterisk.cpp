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

#include "parse-asterisk.h"
#include "../generator/gen-string.h"
#include "../generator/gen-generic.h"
#include "../generator/gen-definition.h"
#include "../../os.h"


namespace uro::parse
{

#define P_PATTERN_START_ID (isAbsolute ? 3 : 0)


_bool parseAsteriskPattern(_defptr& result, const _str& originPattern, const _int& line, Uroboros& uro)
{
   const _str pattern = os_trim(originPattern);

   if (pattern == L"*") {
      _genptr<_str> loc(new gen::LocationReference(uro));
      result = std::make_unique<gen::Uro_All>(loc, uro, OS_SEPARATOR_ASTERISK, false, EMPTY_STRING);
      return true;
   }

   if (pattern == L"**") {
      _genptr<_str> loc(new gen::LocationReference(uro));
      result = std::make_unique<gen::Uro_RecursiveAll>(loc, uro, OS_SEPARATOR_ASTERISK, false, EMPTY_STRING);
      return true;
   }

   const _uint32 info = os_patternInfo(pattern);

   if ((info & ASTERISK_INFO_VALID) == 0 || (info & ASTERISK_INFO_DOUBLE_ASTERISK) != 0) {
      throw SyntaxException(str(L"asterisk pattern '", originPattern, L"' is not valid"), line);
      return false;
   }

   const _bool isAbsolute = info & ASTERISK_INFO_IS_ABSOLUTE;
   const _size totalLength = pattern.size();
   _int separatorId = -1;

   for (_size i = P_PATTERN_START_ID; i < totalLength; i++) {
      switch (pattern[i]) {
         case OS_SEPARATOR: {
            separatorId = static_cast<_int>(i);
            break;
         }
         case L'*': {
            goto exitAsteriskBeginning;
         }
      }
   }

exitAsteriskBeginning:

   _str prefix;
   _genptr<_str> base;

   if (separatorId == -1) {
      if (isAbsolute) {
         base = std::make_unique<gen::Constant<_str>>(pattern.substr(0, 2));
      }
      else {
         base = std::make_unique<gen::LocationReference>(uro);
      }
   }
   else {
      if (isAbsolute) {
         base = std::make_unique<gen::Constant<_str>>(pattern.substr(0, separatorId));
      }
      else {
         prefix = pattern.substr(0, separatorId);
         _genptr<_str> loc(new gen::Constant<_str>(prefix));
         base = std::make_unique<gen::RelativeLocation>(loc, uro);
         prefix += OS_SEPARATOR;
      }
   }

   // the pattern contains only one asterisk
   // this is the simplest case
   // we can easily deduce the result
   if ((info & ASTERISK_INFO_ONE_ASTERISK) != 0) {
      _int separatorId2 = -1;

      for (_size i = separatorId + ((isAbsolute && separatorId == -1) ? 4 : 1); i < totalLength; i++) {
         if (pattern[i] == OS_SEPARATOR) {
            separatorId2 = static_cast<_int>(i);
            break;
         }
      }

      if (separatorId2 == -1) {
         if (separatorId == -1) {
            result = std::make_unique<gen::Uro_All>(base, uro,
               str(OS_SEPARATOR_STRING, isAbsolute ? pattern.substr(3) : pattern), isAbsolute, prefix);
         }
         else {
            const _str s = pattern.substr(separatorId);
            result = std::make_unique<gen::Uro_All>(base, uro, s, isAbsolute, prefix);
         }

         return true;
      }

      const _str suffix = str(OS_SEPARATOR_STRING, pattern.substr(separatorId2 + 1));
      const _int patternStart = separatorId == -1 ? P_PATTERN_START_ID : separatorId;
      const _int patternLength = separatorId2 - patternStart;
      const _str p = (separatorId == -1)
         ? str(OS_SEPARATOR_STRING, pattern.substr(patternStart, patternLength))
         : pattern.substr(patternStart, patternLength);

      _defptr d(new gen::Uro_Directories(base, uro, p, isAbsolute, prefix));
      result = std::make_unique<gen::DefinitionSuffix>(d, uro, suffix, isAbsolute, true);
      return true;
   }

   _size start = separatorId == -1 ? P_PATTERN_START_ID : static_cast<_size>(separatorId + 1);
   _bool hasAsterisk = false;
   _str asteriskPart;
   _str suffixPart;
   std::vector<AsteriskUnit> units;

   for (_size i = start; i < totalLength; i++) {
      switch (pattern[i]) {
         case OS_SEPARATOR: {
            addAsteriskPatternUnit(asteriskPart, suffixPart, pattern.substr(start, i - start), hasAsterisk, units);
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

   addAsteriskPatternUnit(asteriskPart, suffixPart, pattern.substr(start), hasAsterisk, units);

   if (suffixPart.empty()) {
      units.emplace_back(asteriskPart);
   }
   else {
      units.emplace_back(asteriskPart, suffixPart);
   }

   const _size ulen = units.size();

   // the pattern contains multiple asterisks
   // but they all appear within one 'path segment' (there is no separator \ / between them)
   if (ulen == 1) {
      const AsteriskUnit& u = units[0];
      const _str p = str(OS_SEPARATOR_STRING, u.asteriskPart);

      if (u.suffixPart.empty()) {
         result = std::make_unique<gen::Uro_All>(base, uro, p, isAbsolute, prefix);
      }
      else {
         _defptr d(new gen::Uro_Directories(base, uro, p, isAbsolute, prefix));
         result = std::make_unique<gen::DefinitionSuffix>(d, uro, u.suffixPart, isAbsolute, true);
      }

      return true;
   }

   // the pattern contains multiple asterisks spread around
   // but there are no 'double asterisks' **
   if ((info & ASTERISK_INFO_DOUBLE_ASTERISK) == 0) {
      const _str firstPatt = str(OS_SEPARATOR_STRING, units[0].asteriskPart);

      if (units[0].suffixPart.empty()) {
         result = std::make_unique<gen::Uro_Directories>(base, uro, firstPatt, isAbsolute, prefix);
      }
      else {
         _defptr d(new gen::Uro_Directories(base, uro, firstPatt, isAbsolute, prefix));
         result = std::make_unique<gen::DefinitionSuffix>(d, uro, units[0].suffixPart, isAbsolute, false);
      }

      for (_size i = 1; i < ulen; i++) {
         const _bool isFinal = i == (ulen - 1);
         std::unique_ptr<gen::LocationVessel> vessel(new gen::LocationVessel(isAbsolute, uro));
         gen::LocationVessel& vesselRef = *(vessel.get());
         _genptr<_str> vesselPtr = std::move(vessel);
         _defptr nextDef;
         const _str nextPatt = str(OS_SEPARATOR_STRING, units[i].asteriskPart);

         if (units[i].suffixPart.empty()) {
            if (isFinal) {
               nextDef = std::make_unique<gen::Uro_All>(vesselPtr, uro, nextPatt, isAbsolute, prefix);
            }
            else {
               nextDef = std::make_unique<gen::Uro_Directories>(vesselPtr, uro, nextPatt, isAbsolute, prefix);
            }
         }
         else {
            _defptr d(new gen::Uro_Directories(vesselPtr, uro, nextPatt, isAbsolute, prefix));
            nextDef = std::make_unique<gen::DefinitionSuffix>(d, uro, units[i].suffixPart, isAbsolute, isFinal);
         }

         _defptr prev = std::move(result);
         result = std::make_unique<gen::NestedDefiniton>(vesselRef, nextDef, prev, uro, isAbsolute, isFinal);
      }

      return true;
   }

   return false;
}

void addAsteriskPatternUnit(_str& asteriskPart, _str& suffixPart, const _str& part,
   const _bool& hasAsterisk, std::vector<AsteriskUnit>& units)
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

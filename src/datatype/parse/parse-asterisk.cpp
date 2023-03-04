/*
    This file is part of Uroboros2.
    Uroboros2 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    Uroboros2 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with Uroboros2. If not, see <http://www.gnu.org/licenses/>.
*/

#include "parse-asterisk.h"
#include "../generator/gen-string.h"
#include "../generator/gen-generic.h"
#include "../generator/gen-definition.h"
#include "../generator/gen-double-asterisk.h"
#include "../../os.h"


namespace uro::parse
{

_bool parseAsteriskPattern(_defptr& result, const _str& originPattern, const _int line, _uro& uro)
{
   const _str pattern = os_trim(originPattern);

   if (pattern == STRING_CHAR_ASTERISK) {
      _genptr<_str> loc = std::make_unique<gen::LocationReference>(uro);
      result = std::make_unique<gen::All>(loc, uro, gen::os::DEFAULT_PATTERN,
         gen::os::IS_RELATIVE_PATH, gen::os::NO_PREFIX);
      return true;
   }

   if (pattern == STRING_DOUBLE_ASTERISK) {
      _genptr<_str> loc = std::make_unique<gen::LocationReference>(uro);
      result = std::make_unique<gen::RecursiveAll>(loc, uro, gen::os::IS_RELATIVE_PATH, gen::os::NO_PREFIX);
      return true;
   }

   const _uint32 info = os_patternInfo(pattern);

   if ((info & ASTERISK_INFO_VALID) == 0) {
      throw SyntaxError::invalidAsteriskPattern(originPattern, line);
      return false;
   }

   const _bool isAbsolute = info & ASTERISK_INFO_IS_ABSOLUTE;
   const _size totalLength = pattern.size();
   const _size patternStart = isAbsolute ? 3 : 0;
   _int separatorId = -1;

   for (_size i = patternStart; i < totalLength; i++) {
      switch (pattern[i]) {
         case OS_SEPARATOR: {
            separatorId = static_cast<_int>(i);
            break;
         }
         case CHAR_ASTERISK: {
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
         _genptr<_str> loc = std::make_unique<gen::Constant<_str>>(prefix);
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
            result = std::make_unique<gen::All>(base, uro,
               str(OS_SEPARATOR_STRING, isAbsolute ? pattern.substr(3) : pattern), isAbsolute, prefix);
         }
         else {
            const _str s = pattern.substr(separatorId);
            result = std::make_unique<gen::All>(base, uro, s, isAbsolute, prefix);
         }

         return true;
      }

      const _str suffix = str(OS_SEPARATOR_STRING, pattern.substr(separatorId2 + 1));
      const _int patternStart2 = separatorId == -1 ? patternStart : separatorId;
      const _int patternLength = separatorId2 - patternStart2;
      const _str p = (separatorId == -1)
         ? str(OS_SEPARATOR_STRING, pattern.substr(patternStart2, patternLength))
         : pattern.substr(patternStart2, patternLength);

      _defptr d = std::make_unique<gen::Directories>(base, uro, p, isAbsolute, prefix);
      result = std::make_unique<gen::DefinitionSuffix>(d, uro, suffix, isAbsolute, gen::os::IS_FINAL);
      return true;
   }

   _size start = separatorId == -1 ? patternStart : static_cast<_size>(separatorId + 1);

   if ((info & ASTERISK_INFO_DOUBLE_ASTERISK) != 0) {
      return parseDoubleAsterisk(result, base, pattern, start, isAbsolute, uro);
   }

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
         case CHAR_ASTERISK: {
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
   const _bool hasDoubleAst = (info & ASTERISK_INFO_DOUBLE_ASTERISK) != 0;

   // the pattern contains multiple asterisks
   // but they all appear within one 'path segment' (there is no separator \ / between them)
   if (ulen == 1) {
      const AsteriskUnit& u = units[0];
      const _str p = str(OS_SEPARATOR_STRING, u.asteriskPart);

      if (u.suffixPart.empty()) {
         result = std::make_unique<gen::All>(base, uro, p, isAbsolute, prefix);
      }
      else {
         _defptr d = std::make_unique<gen::Directories>(base, uro, p, isAbsolute, prefix);
         result = std::make_unique<gen::DefinitionSuffix>(d, uro, u.suffixPart, isAbsolute, gen::os::IS_FINAL);
      }
      return true;
   }

   // finally we build complex asterisk patterns
   const _str firstPatt = str(OS_SEPARATOR_STRING, units[0].asteriskPart);

   if (units[0].suffixPart.empty()) {
      result = std::make_unique<gen::Directories>(base, uro, firstPatt, isAbsolute, prefix);
   }
   else {
      _defptr d = std::make_unique<gen::Directories>(base, uro, firstPatt, isAbsolute, prefix);
      result = std::make_unique<gen::DefinitionSuffix>(d, uro, units[0].suffixPart, isAbsolute, gen::os::IS_NOT_FINAL);
   }

   for (_size i = 1; i < ulen; i++) {
      const _bool isFinal = i == (ulen - 1);
      LocationContext* locContext = uro.contexts.getLocationContext();
      std::unique_ptr<gen::LocationVessel> vessel = std::make_unique<gen::LocationVessel>(isAbsolute, locContext);
      gen::LocationVessel& vesselRef = *(vessel.get());
      _genptr<_str> vesselPtr = std::move(vessel);
      _defptr nextDef;

      const _str nextPatt = str(OS_SEPARATOR_STRING, units[i].asteriskPart);

      if (units[i].suffixPart.empty()) {
         if (isFinal) {
            nextDef = std::make_unique<gen::All>(vesselPtr, uro, nextPatt, isAbsolute, gen::os::NO_PREFIX);
         }
         else {
            nextDef = std::make_unique<gen::Directories>(vesselPtr, uro, nextPatt, isAbsolute, gen::os::NO_PREFIX);
         }
      }
      else {
         _defptr d = std::make_unique<gen::Directories>(vesselPtr, uro, nextPatt, isAbsolute, gen::os::NO_PREFIX);
         nextDef = std::make_unique<gen::DefinitionSuffix>(d, uro, units[i].suffixPart, isAbsolute, isFinal);
      }

      _defptr prev = std::move(result);
      result = std::make_unique<gen::NestedDefiniton>(vesselRef, nextDef, prev, isAbsolute, isFinal);
   }

   return true;
}


void addAsteriskPatternUnit(_str& asteriskPart, _str& suffixPart, const _str& part,
   const _bool hasAsterisk, std::vector<AsteriskUnit>& units)
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


_bool parseDoubleAsterisk(_defptr& result, _genptr<_str>& base, const _str& pattern, 
   const _size start, const _bool isAbsolute, _uro& uro)
{
   enum Mode {
      m_Normal,
      m_SingleAsterisk,
      m_DoubleAsterisk
   };

   Mode mode = Mode::m_Normal;
   _str finalPattern;

   for (const _char& ch : pattern) {
      switch (mode) {
         case Mode::m_Normal: {
            switch (ch) {
               case OS_SEPARATOR: {
                  finalPattern += ch;
                  break;
               }
               case CHAR_ASTERISK: {
                  mode = Mode::m_SingleAsterisk;
                  break;
               }
               default: {
                  finalPattern += ch;
                  break;
               }
            }
            break;
         }
         case Mode::m_SingleAsterisk: {
            switch (ch) {
               case OS_SEPARATOR: {
                  finalPattern += gen::WILDCARD_SINGLE_ASTERISK;
                  finalPattern += ch;
                  mode = Mode::m_Normal;
                  break;
               }
               case CHAR_ASTERISK: {
                  mode = Mode::m_DoubleAsterisk;
                  break;
               }
               default: {
                  finalPattern += gen::WILDCARD_SINGLE_ASTERISK;
                  finalPattern += ch;
                  mode = Mode::m_Normal;
                  break;
               }
            }
            break;
         }
         case Mode::m_DoubleAsterisk: {
            switch (ch) {
               case OS_SEPARATOR: {
                  finalPattern += gen::WILDCARD_DOUBLE_ASTERISK;
                  finalPattern += ch;
                  mode = Mode::m_Normal;
                  break;
               }
               case CHAR_ASTERISK: {
                  break;
               }
               default: {
                  finalPattern += gen::WILDCARD_DOUBLE_ASTERISK;
                  finalPattern += ch;
                  mode = Mode::m_Normal;
                  break;
               }
            }
            break;
         }
      }
   }

   switch (mode) {
      case Mode::m_SingleAsterisk: {
         finalPattern += gen::WILDCARD_SINGLE_ASTERISK;
         break;
      }
      case Mode::m_DoubleAsterisk: {
         finalPattern += gen::WILDCARD_DOUBLE_ASTERISK;
         break;
      }
   }

   gen::_rallptr loc = std::make_unique<gen::RecursiveAll>(base, uro, isAbsolute, gen::os::NO_PREFIX);
   result = std::make_unique<gen::DoubleAsteriskPattern>(loc, uro, finalPattern, start);
   return true;
}

}

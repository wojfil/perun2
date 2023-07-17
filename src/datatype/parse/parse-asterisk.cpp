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

#include "parse-asterisk.h"
#include "../generator/gen-string.h"
#include "../generator/gen-generic.h"
#include "../generator/gen-definition.h"
#include "../generator/gen-double-asterisk.h"
#include "../../os.h"


namespace perun2::parse
{

p_bool parseAsteriskPattern(p_defptr& result, const p_str& originPattern, const p_int line, p_perun2& p2)
{
   const p_str trimmed = os_trim(originPattern);
   p_size retreats = 0;
   const p_str pattern = os_trimRetreats(trimmed, retreats);

   if (os_hasDotSegments(pattern)) {
      throw SyntaxError::asteriskPatternCannotContainDotSegments(originPattern, line);
      return false;
   }

   if (pattern.size() == 1 && pattern[0] == CHAR_ASTERISK) {
      _genptr<p_str> loc = std::make_unique<gen::LocationReference>(p2);
      p_str prefix;

      if (retreats > 0) {
         _genptr<p_str> prev = std::move(loc);
         loc = std::make_unique<gen::RetreatedPath>(prev, retreats);
         prefix = os_doubleDotsPrefix(retreats);
      }

      result = std::make_unique<gen::All>(loc, p2, gen::os::DEFAULT_PATTERN, gen::os::IS_RELATIVE_PATH, prefix);
      return true;
   }

   if (pattern.size() == 2 && pattern[0] == CHAR_ASTERISK && pattern[1] == CHAR_ASTERISK) {
      _genptr<p_str> loc = std::make_unique<gen::LocationReference>(p2);
      p_str prefix;

      if (retreats > 0) {
         _genptr<p_str> prev = std::move(loc);
         loc = std::make_unique<gen::RetreatedPath>(prev, retreats);
         prefix = os_doubleDotsPrefix(retreats);
      }

      result = std::make_unique<gen::RecursiveAll>(loc, p2, gen::os::IS_RELATIVE_PATH, prefix);
      return true;
   }

   const uint32_t info = os_patternInfo(pattern);

   if ((info & ASTERISK_INFO_VALID) == 0) {
      throw SyntaxError::invalidAsteriskPattern(originPattern, line);
      return false;
   }

   const p_bool isAbsolute = info & ASTERISK_INFO_IS_ABSOLUTE;
   const p_size totalLength = pattern.size();
   const p_size patternStart = isAbsolute ? 3 : 0;
   p_int separatorId = -1;

   for (p_size i = patternStart; i < totalLength; i++) {
      switch (pattern[i]) {
         case OS_SEPARATOR: {
            separatorId = static_cast<p_int>(i);
            break;
         }
         case CHAR_ASTERISK: {
            goto exitAsteriskBeginning;
         }
      }
   }

exitAsteriskBeginning:

   p_str prefix;
   _genptr<p_str> base;

   if (separatorId == -1) {
      if (isAbsolute) {
         base = std::make_unique<gen::Constant<p_str>>(pattern.substr(0, 2));
      }
      else {
         base = std::make_unique<gen::LocationReference>(p2);
         if (retreats > 0) {
            _genptr<p_str> prev = std::move(base);
            base = std::make_unique<gen::RetreatedPath>(prev, retreats);
         }
      }
   }
   else {
      if (isAbsolute) {
         base = std::make_unique<gen::Constant<p_str>>(pattern.substr(0, separatorId));
      }
      else {
         prefix = pattern.substr(0, separatorId);
         _genptr<p_str> loc = std::make_unique<gen::Constant<p_str>>(prefix);
         base = std::make_unique<gen::RelativeLocation>(loc, p2, retreats);
         prefix += OS_SEPARATOR;
      }
   }

   // the pattern contains only one asterisk
   // this is the simplest case
   // we can easily deduce the result
   if ((info & ASTERISK_INFO_ONE_ASTERISK) != 0) {
      p_int separatorId2 = -1;
      for (p_size i = separatorId + ((isAbsolute && separatorId == -1) ? 4 : 1); i < totalLength; i++) {
         if (pattern[i] == OS_SEPARATOR) {
            separatorId2 = static_cast<p_int>(i);
            break;
         }
      }

      if (separatorId2 == -1) {
         if (separatorId == -1) {
            result = std::make_unique<gen::All>(base, p2,
               str(OS_SEPARATOR, isAbsolute ? pattern.substr(3) : pattern), isAbsolute, prefix, retreats);
         }
         else {
            const p_str s = pattern.substr(separatorId);
            result = std::make_unique<gen::All>(base, p2, s, isAbsolute, prefix, retreats);
         }

         return true;
      }

      const p_str suffix = str(OS_SEPARATOR, pattern.substr(separatorId2 + 1));
      const p_int patternStart2 = separatorId == -1 ? patternStart : separatorId;
      const p_int patternLength = separatorId2 - patternStart2;
      const p_str p = (separatorId == -1)
         ? str(OS_SEPARATOR, pattern.substr(patternStart2, patternLength))
         : pattern.substr(patternStart2, patternLength);

      p_defptr d = std::make_unique<gen::Directories>(base, p2, p, isAbsolute, prefix);

      parseDefinitionSuffix(result, d, suffix, isAbsolute, gen::os::IS_FINAL, retreats, nullptr, p2);
      return true;
   }

   p_size start = separatorId == -1 ? patternStart : static_cast<p_size>(separatorId + 1);

   if ((info & ASTERISK_INFO_DOUBLE_ASTERISK) != 0) {
      return parseDoubleAsterisk(result, base, pattern, trimmed, start, isAbsolute, retreats, p2);
   }

   p_bool hasAsterisk = false;
   p_str asteriskPart;
   p_str suffixPart;
   std::vector<AsteriskUnit> units;

   for (p_size i = start; i < totalLength; i++) {
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

   const p_size ulen = units.size();

   // the pattern contains multiple asterisks
   // but they all appear within one 'path segment' (there is no separator \ / between them)
   if (ulen == 1) {
      const AsteriskUnit& u = units[0];
      const p_str p = str(OS_SEPARATOR, u.asteriskPart);

      if (u.suffixPart.empty()) {
         result = std::make_unique<gen::All>(base, p2, p, isAbsolute, prefix, retreats);
      }
      else {
         p_defptr d = std::make_unique<gen::Directories>(base, p2, p, isAbsolute, prefix);
         parseDefinitionSuffix(result, d, u.suffixPart, isAbsolute, gen::os::IS_FINAL, retreats, nullptr, p2);
      }
      return true;
   }

   // finally we build complex asterisk patterns
   const p_str firstPatt = str(OS_SEPARATOR, units[0].asteriskPart);

   if (units[0].suffixPart.empty()) {
      result = std::make_unique<gen::Directories>(base, p2, firstPatt, isAbsolute, prefix);
   }
   else {
      p_defptr d = std::make_unique<gen::Directories>(base, p2, firstPatt, isAbsolute, prefix);
      parseDefinitionSuffix(result, d, units[0].suffixPart, isAbsolute, gen::os::IS_NOT_FINAL, retreats, nullptr, p2);
   }

   for (p_size i = 1; i < ulen; i++) {
      const p_bool isFinal = i == (ulen - 1);

      _genptr<p_str> loc = std::make_unique<gen::LocationReference>(p2);
      if (!isAbsolute && retreats > 0) {
         _genptr<p_str> prev = std::move(loc);
         loc = std::make_unique<gen::RetreatedPath>(prev, retreats);
      }

      std::unique_ptr<gen::LocationVessel> vessel = std::make_unique<gen::LocationVessel>(isAbsolute, loc);
      gen::LocationVessel& vesselRef = *(vessel.get());
      _genptr<p_str> vesselPtr = std::move(vessel);
      p_defptr nextDef;

      const p_str nextPatt = str(OS_SEPARATOR, units[i].asteriskPart);

      if (units[i].suffixPart.empty()) {
         if (isFinal) {
            nextDef = std::make_unique<gen::All>(vesselPtr, p2, nextPatt, isAbsolute, p_str());
         }
         else {
            nextDef = std::make_unique<gen::Directories>(vesselPtr, p2, nextPatt, isAbsolute, p_str());
         }
      }
      else {
         p_def* def = result.get();
         p_defptr d = std::make_unique<gen::Directories>(vesselPtr, p2, nextPatt, isAbsolute, p_str());
         parseDefinitionSuffix(nextDef, d, units[i].suffixPart, isAbsolute, isFinal, retreats, def, p2);
      }

      p_defptr prev = std::move(result);
      result = std::make_unique<gen::NestedDefiniton>(vesselRef, nextDef, prev, isAbsolute, isFinal, isFinal ? retreats : 0);
   }

   return true;
}


void addAsteriskPatternUnit(p_str& asteriskPart, p_str& suffixPart, const p_str& part,
   const p_bool hasAsterisk, std::vector<AsteriskUnit>& units)
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
         suffixPart = str(suffixPart, OS_SEPARATOR, part);
      }
   }
}


p_bool parseDoubleAsterisk(p_defptr& result, _genptr<p_str>& base, const p_str& pattern, const p_str& trimmed,
   const p_size start, const p_bool isAbsolute, const p_int retreats, p_perun2& p2)
{
   enum Mode {
      m_Normal,
      m_SingleAsterisk,
      m_DoubleAsterisk
   };

   Mode mode = Mode::m_Normal;
   p_str finalPattern;

   for (const p_char ch : pattern) {
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

   p_str prefix;
   if (!isAbsolute) {
      prefix = pattern.substr(0, start);
   }

   gen::_rallptr loc = std::make_unique<gen::RecursiveAll>(base, p2, isAbsolute, p_str());
   result = std::make_unique<gen::DoubleAsteriskPattern>(loc, p2, finalPattern, prefix, retreats);
   return true;
}

p_bool parseDefinitionSuffix(p_defptr& result, p_defptr& definition, const p_str& suffix,
   const p_bool isAbsolute, const p_bool isFinal, const p_int retreats, p_def* previous, p_perun2& p2)
{
   if (isAbsolute) {
      result = std::make_unique<gen::AbsoluteDefSuffix>(definition, suffix, isFinal);
   }
   else if (retreats == 0) {
      result = std::make_unique<gen::RelativeDefSuffix>(definition, p2, suffix, isFinal, previous);
   }
   else {
      if (previous == nullptr) {
         result = std::make_unique<gen::RetreatedDefSuffix>(definition, p2, suffix, isFinal, retreats, previous);
      }
      else {
         result = std::make_unique<gen::FarRetreatedDefSuffix>(definition, p2, suffix, isFinal, retreats, previous);
      }
   }

   return true;
}

}

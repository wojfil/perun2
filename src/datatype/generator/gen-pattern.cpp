#include "gen-pattern.h"
#include "gen-string.h"
#include "../../os.h"


_boo PatternParser::parse(const _str& pattern, const _int& asteriskId, _def*& result) const
{
   const _str trimmed = os_trim(pattern);

   if (trimmed == L"*") {
      result = this->defGenerator.generatePattern(
         new LocationReference(this->uroboros), ELEM_ALL, OS_SEPARATOR_ASTERISK);
      return true;
   }

   const _size length = trimmed.size();

   if (asteriskId == length - 1) {
      const _str p = str(OS_SEPARATOR_STRING, pattern.substr(0, length - 1), L"*");
      result = this->defGenerator.generatePattern(
         new LocationReference(this->uroboros), ELEM_ALL, p);
      return true;
   }

   // todo
   return false;
};


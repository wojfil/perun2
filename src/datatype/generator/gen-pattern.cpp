#include "gen-pattern.h"
#include "gen-string.h"
#include "../../os.h"


_boo PatternParser::parse(const _str& pattern, _def*& result) const
{
   const _str trimmed = os_trim(pattern);

   if (trimmed == L"*") {
      result = this->defGenerator.generatePattern(
         new LocationReference(this->uroboros), ELEM_ALL, OS_SEPARATOR_ASTERISK);
      return true;
   }

   // todo
   return false;
};



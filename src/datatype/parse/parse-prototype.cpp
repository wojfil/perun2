#include "parse-prototype.h"
#include "../generator/gen-definition.h"


void makeWhereFilter(Generator<_boo>* boo, Attribute* attr, const _boo& hasMemory, _def*& result, Uroboros* uro)
{
   result = new Filter_WhereDef(result, boo, attr, hasMemory, uro);
}

void makeWhereFilter(Generator<_boo>* boo, _def*& result, Uroboros* uro)
{
   result = new Filter_WhereDef(result, boo, uro);
}

void makeNumericFilter(const Keyword& kw, Generator<_num>* num, _def*& result, Uroboros* uro)
{
   switch (kw) {
      case Keyword::kw_Every: {
         result = new Filter_EveryDef(result, num, uro);
         break;
      }
      case Keyword::kw_Limit: {
         result = new Filter_LimitDef(result, num, uro);
         break;
      }
      case Keyword::kw_Skip: {
         result = new Filter_SkipDef(result, num, uro);
         break;
      }
   }
}

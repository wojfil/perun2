#include "parse-prototype.h"
#include "../generator/gen-definition.h"


namespace uro::parse
{

void makeWhereFilter(Generator<_boo>* boo, Attribute* attr, const _boo& hasMemory, _def*& result, uro::Uroboros* uro)
{
   result = new gen::Filter_WhereDef(result, boo, attr, hasMemory, uro);
}

void makeWhereFilter(Generator<_boo>* boo, _def*& result, uro::Uroboros* uro)
{
   result = new gen::Filter_WhereDef(result, boo, uro);
}

void makeNumericFilter(const Keyword& kw, Generator<_num>* num, _def*& result, uro::Uroboros* uro)
{
   switch (kw) {
      case Keyword::kw_Every: {
         result = new gen::Filter_EveryDef(result, num, uro);
         break;
      }
      case Keyword::kw_Limit: {
         result = new gen::Filter_LimitDef(result, num, uro);
         break;
      }
      case Keyword::kw_Skip: {
         result = new gen::Filter_SkipDef(result, num, uro);
         break;
      }
   }
}

}

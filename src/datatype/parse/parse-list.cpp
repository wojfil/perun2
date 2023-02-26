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

#include "parse-list.h"
#include "parse-generic.h"
#include "../generator/gen-list.h"
#include "../generator/gen-generic.h"
#include "../../lexer.h"
#include "../../datatype/order.h"
#include "../parse/parse-function.h"
#include "parse-generic.h"
#include "../parse-gen.h"


namespace uro::parse
{

_bool parseList(_genptr<_list>& result, const Tokens& tks, _uro& uro)
{
   const _size len = tks.getLength();

   if (len == 1) {
      return parseOneToken(uro, tks, result);
   }
   
   if (tks.check(TI_HAS_FILTER_KEYWORD)) {
      if (parseListFilter(result, tks, uro)) {
         return true;
      }
      else {
         throw SyntaxError(L"this syntax structure cannot be resolved to any collection of values", tks.first().line);
      }
   }

   if (len >= 3) {
      if (tks.check(TI_HAS_CHAR_COMMA)) {
         return parseListed<_str>(result, tks, uro);
      }

      if (parseBinary<_list>(result, tks, uro) || parseTernary<_list>(result, tks, uro)) {
         return true;
      }
   }

   if (tks.check(TI_IS_POSSIBLE_FUNCTION)) {
      return func::listFunction(result, tks, uro);
   }

   return false;
}


static _bool parseListFilter(_genptr<_list>& result, const Tokens& tks, _uro& uro)
{
   const _size firstKeywordId = tks.getFilterKeywordId(uro);
   const Tokens tks2(tks, tks.getStart(), firstKeywordId - tks.getStart());
   _genptr<_list> base;
   if (!parse(uro, tks2, base)) {
      return false;
   }

   const _int kw = firstKeywordId - tks.getStart() + 1;
   const _int start = tks.getStart() + kw;
   const _int length = tks.getLength() - kw;
   const Tokens tks3(tks, start, length);
   std::vector<Tokens> filterTokens = tks3.splitByFiltherKeywords(uro);
   const _size flength = filterTokens.size();

   for (_size i = 0; i < flength; i++) {
      Tokens& ts = filterTokens[i];
      const Token tsf = ts.first();
      const Keyword& kw = tsf.value.keyword.k;
      ts.trimLeft();

      switch (kw) {
         case Keyword::kw_Final: {
            _genptr<_num> num;
            if (!parse(uro, ts, num)) {
               throw SyntaxError::keywordNotFollowedByNumber(tsf.getOriginString(uro), tsf.line);
            }

            _genptr<_list> prev = std::move(base);
            base = std::make_unique<gen::ListFilter_Final>(prev, num);
            break;
         }
         case Keyword::kw_Every:
         case Keyword::kw_Limit:
         case Keyword::kw_Skip: {
            if (kw == Keyword::kw_Limit) {
               checkLimitBySize(ts, uro);
            }

            _genptr<_num> num;
            if (!parse(uro, ts, num)) {
               throw SyntaxError::keywordNotFollowedByNumber(tsf.getOriginString(uro), tsf.line);
            }

            _genptr<_list> prev = std::move(base);

            switch(kw) {
               case Keyword::kw_Every: {
                  base = std::make_unique<gen::ListFilter_Every>(prev, num);
                  break;
               }
               case Keyword::kw_Limit: {
                  base = std::make_unique<gen::ListFilter_Limit>(prev, num);
                  break;
               }
               case Keyword::kw_Skip: {
                  base = std::make_unique<gen::ListFilter_Skip>(prev, num);
                  break;
               }
            }
            
            break;
         }
         case Keyword::kw_Where: {
            _fcptr context = std::make_unique<FileContext>(uro);
            uro.contexts.addFileContext(context.get());

            _genptr<_bool> boo;
            if (!parse(uro, ts, boo)) {
               throw SyntaxError::keywordNotFollowedByBool(tsf.getOriginString(uro), tsf.line);
            }

            uro.contexts.retreatFileContext();

            _genptr<_list> prev = std::move(base);
            base = std::make_unique<gen::ListFilter_Where>(boo, prev, context, uro);
            break;
         }
         case Keyword::kw_Order: {
            gen::_ordptr order;
            gen::_indptr indices = std::make_unique<gen::OrderIndices>();
            _fcptr context = std::make_unique<FileContext>(uro);
            uro.contexts.addFileContext(context.get());

            parseOrder(order, indices, ts, tsf, uro);

            uro.contexts.retreatFileContext();
            _genptr<_list> prev = std::move(base);
            base = std::make_unique<gen::OrderBy_List>(prev, context, indices, order, uro);
            break;
         }
      }
   }

   result = std::move(base);
   return true;
}

}

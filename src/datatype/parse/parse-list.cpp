/*
    This file is part of Perun2.
    Perun2 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    Perun2 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with Perun2. If not, see <http://www.gnu.org/licenses/>.
*/

#include "../../../include/perun2/datatype/parse/parse-list.hpp"
#include "../../../include/perun2/datatype/parse/parse-generic.hpp"
#include "../../../include/perun2/datatype/generator/gen-list.hpp"
#include "../../../include/perun2/datatype/generator/gen-generic.hpp"
#include "../../../include/perun2/lexer.hpp"
#include "../../../include/perun2/datatype/order.hpp"
#include "../../../include/perun2/datatype/parse/parse-function.hpp"
#include "../../../include/perun2/datatype/parse/parse-generic.hpp"
#include "../../../include/perun2/datatype/parse-gen.hpp"


namespace perun2::parse
{

p_bool parseList(p_genptr<p_list>& result, const Tokens& tks, Perun2Process& p2)
{
   const p_size len = tks.getLength();

   if (len == 1) {
      return parseOneToken(p2, tks, result);
   }

   if (tks.check(TI_HAS_FILTER_KEYWORD)) {
      if (parseListFilter(result, tks, p2)) {
         return true;
      }
      else {
         throw SyntaxError(L"this syntax structure cannot be resolved to a collection", tks.first().line);
      }
   }

   if (len >= 3) {
      if (tks.check(TI_HAS_CHAR_COMMA)) {
         return parseListed<p_str>(result, tks, p2);
      }

      if (parseBinary<p_list>(result, tks, p2) || parseTernary<p_list>(result, tks, p2)) {
         return true;
      }
   }

   if (tks.check(TI_IS_POSSIBLE_FUNCTION)) {
      return func::listFunction(result, tks, p2);
   }

   return false;
}


static p_bool parseListFilter(p_genptr<p_list>& result, const Tokens& tks, Perun2Process& p2)
{
   const p_size firstKeywordId = tks.getFilterKeywordId(p2);
   const Tokens tks2(tks, tks.getStart(), firstKeywordId - tks.getStart());
   p_genptr<p_list> base;
   if (!parse(p2, tks2, base)) {
      return false;
   }

   const p_int kw = firstKeywordId - tks.getStart() + 1;
   const p_int start = tks.getStart() + kw;
   const p_int length = tks.getLength() - kw;
   const Tokens tks3(tks, start, length);
   std::vector<Tokens> filterTokens = tks3.splitByFiltherKeywords(p2);
   const p_size flength = filterTokens.size();

   for (p_size i = 0; i < flength; i++) {
      Tokens& ts = filterTokens[i];
      const Token tsf = ts.first();
      const Keyword& kw = tsf.value.keyword;
      ts.popLeft();

      switch (kw) {
         case Keyword::kw_Final: {
            p_genptr<p_num> num;
            if (!parse(p2, ts, num)) {
               throw SyntaxError::keywordNotFollowedByNumber(tsf.origin, tsf.line);
            }

            p_genptr<p_list> prev = std::move(base);
            base = std::make_unique<gen::ListFilter_Final>(prev, num);
            break;
         }
         case Keyword::kw_Every:
         case Keyword::kw_Limit:
         case Keyword::kw_Skip: {
            if (kw == Keyword::kw_Limit) {
               checkLimitBySize(ts, p2);
            }

            p_genptr<p_num> num;
            if (!parse(p2, ts, num)) {
               throw SyntaxError::keywordNotFollowedByNumber(tsf.origin, tsf.line);
            }

            p_genptr<p_list> prev = std::move(base);

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
            p_fcptr context = std::make_unique<FileContext>(p2);
            p2.contexts.addFileContext(context.get());

            p_genptr<p_bool> boo;
            if (!parse(p2, ts, boo)) {
               throw SyntaxError::keywordNotFollowedByBool(tsf.origin, tsf.line);
            }

            p2.contexts.retreatFileContext();

            p_genptr<p_list> prev = std::move(base);
            base = std::make_unique<gen::ListFilter_Where>(boo, prev, context, p2);
            break;
         }
         case Keyword::kw_Order: {
            gen::p_ordptr order;
            gen::p_indptr indices = std::make_unique<gen::OrderIndices>();
            p_fcptr context = std::make_unique<FileContext>(p2);
            p2.contexts.addFileContext(context.get());

            parseOrder<gen::p_ordptr>(order, indices.get(), ts, tsf, p2);

            p2.contexts.retreatFileContext();
            p_genptr<p_list> prev = std::move(base);
            base = std::make_unique<gen::OrderBy_List>(prev, context, indices, order, p2);
            break;
         }
      }
   }

   result = std::move(base);
   return true;
}

}

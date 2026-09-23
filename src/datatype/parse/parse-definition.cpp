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

#include "parse-definition.h"
#include "parse-generic.h"
#include "../../lexer.h"
#include "../order.h"
#include "../generator/gen-definition.h"
#include "../cast.h"
#include "../parse/parse-function.h"
#include "parse-generic.h"
#include "../parse-gen.h"
#include "parse-order.h"


namespace perun2::parse
{

p_bool parseDefinition(p_defptr& result, const Tokens& tks, Perun2Process& p2)
{
   const p_size len = tks.getLength();

   if (len == 1) {
      return parseOneToken(p2, tks, result);
   }

   if (tks.check(TI_HAS_FILTER_KEYWORD)) {
      return parseDefFilter(result, tks, p2);
   }

   if (tks.check(TI_HAS_CHAR_COMMA) && parseDefChain(result, tks, p2)) {
      return true;
   }

   return parseDefBinary(result, tks, p2) || parseDefTernary(result, tks, p2);
}


// Definition chain is a collection of Strings, Lists and Definitions
// separated by commas, that contains at least one Definition
// as a result, multiple elements are transformed into one Definition and lazy evaluation is preserved
static p_bool parseDefChain(p_defptr& result, const Tokens& tks, Perun2Process& p2)
{
   enum ChainLink {
      cl_Definition = 0,
      cl_String,
      cl_List
   };

   const std::vector<Tokens> elements = tks.splitBySymbol(CHAR_COMMA);
   const p_size len = elements.size();

   ChainLink cl;
   p_defptr prevDef;
   p_genptr<p_str> prevStr;
   p_genptr<p_list> prevList;

   if (parse(p2, elements[0], prevDef)) {
      cl = ChainLink::cl_Definition;
   }
   else {
      if (parse(p2, elements[0], prevStr)) {
         cl = ChainLink::cl_String;
      }
      else {
         if (parse(p2, elements[0], prevList)) {
            cl = ChainLink::cl_List;
         }
         else {
            return false;
         }
      }
   }

   for (p_size i = 1; i < len; i++) {
      const Tokens& tk = elements[i];

      switch (cl) {
         case cl_Definition: {
            p_defptr def;
            if (parse(p2, tk, def)) {
               p_defptr pdef = std::move(prevDef);
               prevDef = std::make_unique<gen::Join_DefDef>(pdef, def, p2);
            }
            else {
               p_genptr<p_str> str;
               if (parse(p2, tk, str)) {
                  p_defptr pdef = std::move(prevDef);
                  prevDef = std::make_unique<gen::Join_DefStr>(pdef, str, p2);
               }
               else {
                  p_genptr<p_list> list;
                  if (parse(p2, tk, list)) {
                     p_defptr pdef = std::move(prevDef);
                     prevDef = std::make_unique<gen::Join_DefList>(pdef, list, p2);
                  }
                  else {
                     return false;
                  }
               }
            }

            break;
         }
         case cl_String: {
            p_defptr def;
            if (parse(p2, tk, def)) {
               prevDef = std::make_unique<gen::Join_StrDef>(prevStr, def, p2);
               prevStr.reset();
               cl = ChainLink::cl_Definition;
            }
            else {
               p_genptr<p_str> str;
               if (parse(p2, tk, str)) {
                  prevList = std::make_unique<gen::Join_StrStr>(prevStr, str);
                  prevStr.reset();
                  cl = ChainLink::cl_List;
               }
               else {
                  p_genptr<p_list> list;
                  if (parse(p2, tk, list)) {
                     prevList = std::make_unique<gen::Join_StrList>(prevStr, list);
                     prevStr.reset();
                     cl = ChainLink::cl_List;
                  }
                  else {
                     return false;
                  }
               }
            }

            break;
         }
         case cl_List: {
            p_defptr def;
            if (parse(p2, tk, def)) {
               prevDef = std::make_unique<gen::Join_ListDef>(prevList, def, p2);
               prevList.reset();
               cl = ChainLink::cl_Definition;
            }
            else {
               p_genptr<p_str> str;
               if (parse(p2, tk, str)) {
                  p_genptr<p_list> plist(std::move(prevList));
                  prevList = std::make_unique<gen::Join_ListStr>(plist, str);
               }
               else {
                  p_genptr<p_list> list;
                  if (parse(p2, tk, list)) {
                     p_genptr<p_list> plist(std::move(prevList));
                     prevList = std::make_unique<gen::Join_ListList>(plist, list);
                  }
                  else {
                     return false;
                  }
               }
            }
            break;
         }
      }
   }

   if (cl == cl_Definition) {
      result = std::move(prevDef);
      return true;
   }

   return false;
}


static p_bool parseDefTernary(p_defptr& result, const Tokens& tks, Perun2Process& p2)
{
   if (!tks.check(TI_IS_POSSIBLE_TERNARY)) {
      return false;
   }

   std::tuple<Tokens, Tokens, Tokens> trio = tks.divideForTernary();

   p_genptr<p_bool> condition;
   if (!parse(p2, std::get<0>(trio), condition)) {
      return false;
   }

   p_defptr left;
   if (!parse(p2, std::get<1>(trio), left)) {
      return false;
   }

   p_defptr right;
   if (!parse(p2, std::get<2>(trio), right)) {
      return false;
   }

   result = std::make_unique<gen::DefTernary>(condition, left, right);
   return true;
}


static p_bool parseDefBinary(p_defptr& result, const Tokens& tks, Perun2Process& p2)
{
   if (!tks.check(TI_IS_POSSIBLE_BINARY)) {
      return false;
   }

   std::pair<Tokens, Tokens> pair = tks.divideBySymbol(CHAR_QUESTION_MARK);

   p_genptr<p_bool> condition;
   if (!parse(p2, pair.first, condition)) {
      return false;
   }

   p_defptr value;
   if (!parse(p2, pair.second, value)) {
      return false;
   }

   result = std::make_unique<gen::DefBinary>(condition, value);
   return true;
}


static p_bool parseDefFilter(p_defptr& result, const Tokens& tks, Perun2Process& p2)
{
   const p_size firstKeywordId = tks.getFilterKeywordId(p2);
   const Tokens tks2(tks, tks.getStart(), firstKeywordId - tks.getStart());
   p_defptr base;
   if (!parse(p2, tks2, base)) {
      return false;
   }

   const p_int kw = firstKeywordId - tks.getStart() + 1;
   const p_int start = tks.getStart() + kw;
   const p_int length = tks.getLength() - kw;
   const Tokens tks3(tks, start, length);
   std::vector<Tokens> filterTokens = tks3.splitByFiltherKeywords(p2);
   const p_size flength = filterTokens.size();

   FileContext* contextPtr = base->getFileContext();
   if (contextPtr == nullptr) {
      p_defptr prev = std::move(base);
      base = std::make_unique<gen::DefWithContext>(prev, p2);
      contextPtr = base->getFileContext();
   }

   p2.contexts.addFileContext(contextPtr);

   for (p_size i = 0; i < flength; i++) {
      Tokens& ts = filterTokens[i];
      const Token tsf = ts.first();
      const Keyword& kw = tsf.value.keyword.k;
      ts.popLeft();

      switch (kw) {
         case Keyword::kw_Final: {
            p2.contexts.retreatFileContext();

            p_genptr<p_num> num;
            if (!parse(p2, ts, num)) {
               throw SyntaxError::keywordNotFollowedByNumber(tsf.getOriginString(p2), tsf.line);
            }

            p_fcptr nextContext = std::make_unique<FileContext>(p2);
            p2.contexts.addFileContext(nextContext.get());
            p_defptr prev = std::move(base);
            base = std::make_unique<gen::DefFilter_Final>(prev, num, nextContext, contextPtr, p2);
            break;
         }
         case Keyword::kw_Every:
         case Keyword::kw_Limit:
         case Keyword::kw_Skip: {
            if (kw == Keyword::kw_Limit) {
               checkLimitBySize(ts, p2);
            }

            p2.contexts.retreatFileContext();

            p_genptr<p_num> num;
            if (!parse(p2, ts, num)) {
               throw SyntaxError::keywordNotFollowedByNumber(tsf.getOriginString(p2), tsf.line);
            }

            p2.contexts.addFileContext(contextPtr);
            p_defptr prev = std::move(base);

            switch(kw) {
               case Keyword::kw_Every: {
                  base = std::make_unique<gen::DefFilter_Every>(prev, num, contextPtr, p2);
                  break;
               }
               case Keyword::kw_Limit: {
                  base = std::make_unique<gen::DefFilter_Limit>(prev, num, contextPtr, p2);
                  break;
               }
               case Keyword::kw_Skip: {
                  base = std::make_unique<gen::DefFilter_Skip>(prev, num, contextPtr, p2);
                  break;
               }
            }

            break;
         }
         case Keyword::kw_Where: {
            p_genptr<p_bool> boo;
            if (!parse(p2, ts, boo)) {
               throw SyntaxError::keywordNotFollowedByBool(tsf.getOriginString(p2), tsf.line);
            }

            p_defptr prev = std::move(base);
            base = std::make_unique<gen::DefFilter_Where>(boo, prev, contextPtr, p2);
            break;
         }
         case Keyword::kw_Order: {
            gen::p_loptr limitOne;
            gen::p_ordptr order;
            gen::p_indptr indices;

            // if Order By is followed by "limit 1"
            // we can introduce optimizations and combine these two filters into one
            if (i != flength - 1 && filterTokens[i + 1].getLength() == 2 
               && filterTokens[i + 1].first().isKeyword(Keyword::kw_Limit)
               && filterTokens[i + 1].second().isOne()) 
            {
               i++;
               parseOrder<gen::p_loptr>(limitOne, nullptr, ts, tsf, p2);
            }
            else {
               indices = std::make_unique<gen::OrderIndices>();
               parseOrder<gen::p_ordptr>(order, indices.get(), ts, tsf, p2);
            }

            // retreat previous context
            // and add a new one instead
            p2.contexts.retreatFileContext();
            p_fcptr nextContext = std::make_unique<FileContext>(p2);
            p2.contexts.addFileContext(nextContext.get());
            p_defptr prev = std::move(base);

            if (limitOne) {
               base = std::make_unique<gen::OrderByLimitOne>(prev, contextPtr, nextContext, limitOne, p2);
            }
            else {
               base = std::make_unique<gen::OrderBy_Definition>(prev, contextPtr, nextContext, indices, order, p2);
            }

            break;
         }
      }
   }

   p2.contexts.retreatFileContext();
   result = std::move(base);
   return true;
}

}

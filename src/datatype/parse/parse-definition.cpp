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

_bool parseDefinition(_defptr& result, const Tokens& tks, _p2& p2)
{
   const _size len = tks.getLength();

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
static _bool parseDefChain(_defptr& result, const Tokens& tks, _p2& p2)
{
   enum ChainLink {
      cl_Definition = 0,
      cl_String,
      cl_List
   };

   const std::vector<Tokens> elements = tks.splitBySymbol(CHAR_COMMA);
   const _size len = elements.size();

   ChainLink cl;
   _defptr prevDef;
   _genptr<_str> prevStr;
   _genptr<_list> prevList;

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

   for (_size i = 1; i < len; i++) {
      const Tokens& tk = elements[i];

      switch (cl) {
         case cl_Definition: {
            _defptr def;
            if (parse(p2, tk, def)) {
               _defptr pdef = std::move(prevDef);
               prevDef = std::make_unique<gen::Join_DefDef>(pdef, def, p2);
            }
            else {
               _genptr<_str> str;
               if (parse(p2, tk, str)) {
                  _defptr pdef = std::move(prevDef);
                  prevDef = std::make_unique<gen::Join_DefStr>(pdef, str, p2);
               }
               else {
                  _genptr<_list> list;
                  if (parse(p2, tk, list)) {
                     _defptr pdef = std::move(prevDef);
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
            _defptr def;
            if (parse(p2, tk, def)) {
               prevDef = std::make_unique<gen::Join_StrDef>(prevStr, def, p2);
               prevStr.reset();
               cl = ChainLink::cl_Definition;
            }
            else {
               _genptr<_str> str;
               if (parse(p2, tk, str)) {
                  prevList = std::make_unique<gen::Join_StrStr>(prevStr, str);
                  prevStr.reset();
                  cl = ChainLink::cl_List;
               }
               else {
                  _genptr<_list> list;
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
            _defptr def;
            if (parse(p2, tk, def)) {
               prevDef = std::make_unique<gen::Join_ListDef>(prevList, def, p2);
               prevList.reset();
               cl = ChainLink::cl_Definition;
            }
            else {
               _genptr<_str> str;
               if (parse(p2, tk, str)) {
                  _genptr<_list> plist(std::move(prevList));
                  prevList = std::make_unique<gen::Join_ListStr>(plist, str);
               }
               else {
                  _genptr<_list> list;
                  if (parse(p2, tk, list)) {
                     _genptr<_list> plist(std::move(prevList));
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


static _bool parseDefTernary(_defptr& result, const Tokens& tks, _p2& p2)
{
   if (!tks.check(TI_IS_POSSIBLE_TERNARY)) {
      return false;
   }

   std::tuple<Tokens, Tokens, Tokens> trio = tks.divideForTernary();

   _genptr<_bool> condition;
   if (!parse(p2, std::get<0>(trio), condition)) {
      return false;
   }

   _defptr left;
   if (!parse(p2, std::get<1>(trio), left)) {
      return false;
   }

   _defptr right;
   if (!parse(p2, std::get<2>(trio), right)) {
      return false;
   }

   result = std::make_unique<gen::DefTernary>(condition, left, right);
   return true;
}


static _bool parseDefBinary(_defptr& result, const Tokens& tks, _p2& p2)
{
   if (!tks.check(TI_IS_POSSIBLE_BINARY)) {
      return false;
   }

   std::pair<Tokens, Tokens> pair = tks.divideBySymbol(CHAR_QUESTION_MARK);

   _genptr<_bool> condition;
   if (!parse(p2, pair.first, condition)) {
      return false;
   }

   _defptr value;
   if (!parse(p2, pair.second, value)) {
      return false;
   }

   result = std::make_unique<gen::DefBinary>(condition, value);
   return true;
}


static _bool parseDefFilter(_defptr& result, const Tokens& tks, _p2& p2)
{
   const _size firstKeywordId = tks.getFilterKeywordId(p2);
   const Tokens tks2(tks, tks.getStart(), firstKeywordId - tks.getStart());
   _defptr base;
   if (!parse(p2, tks2, base)) {
      return false;
   }

   const _int kw = firstKeywordId - tks.getStart() + 1;
   const _int start = tks.getStart() + kw;
   const _int length = tks.getLength() - kw;
   const Tokens tks3(tks, start, length);
   std::vector<Tokens> filterTokens = tks3.splitByFiltherKeywords(p2);
   const _size flength = filterTokens.size();

   FileContext* contextPtr = base->getFileContext();
   if (contextPtr == nullptr) {
      _defptr prev = std::move(base);
      base = std::make_unique<gen::DefWithContext>(prev, p2);
      contextPtr = base->getFileContext();
   }

   p2.contexts.addFileContext(contextPtr);

   for (_size i = 0; i < flength; i++) {
      Tokens& ts = filterTokens[i];
      const Token tsf = ts.first();
      const Keyword& kw = tsf.value.keyword.k;
      ts.trimLeft();

      switch (kw) {
         case Keyword::kw_Final: {
            p2.contexts.retreatFileContext();

            _genptr<_num> num;
            if (!parse(p2, ts, num)) {
               throw SyntaxError::keywordNotFollowedByNumber(tsf.getOriginString(p2), tsf.line);
            }

            _fcptr nextContext = std::make_unique<FileContext>(p2);
            p2.contexts.addFileContext(nextContext.get());
            _defptr prev = std::move(base);
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

            _genptr<_num> num;
            if (!parse(p2, ts, num)) {
               throw SyntaxError::keywordNotFollowedByNumber(tsf.getOriginString(p2), tsf.line);
            }

            p2.contexts.addFileContext(contextPtr);
            _defptr prev = std::move(base);

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
            _genptr<_bool> boo;
            if (!parse(p2, ts, boo)) {
               throw SyntaxError::keywordNotFollowedByBool(tsf.getOriginString(p2), tsf.line);
            }

            _defptr prev = std::move(base);
            base = std::make_unique<gen::DefFilter_Where>(boo, prev, contextPtr, p2);
            break;
         }
         case Keyword::kw_Order: {
            gen::_loptr limitOne;
            gen::_ordptr order;
            gen::_indptr indices;

            // if Order By is followed by "limit 1"
            // we can introduce optimizations and combine these two filters into one
            if (i != flength - 1 && filterTokens[i + 1].getLength() == 2 
               && filterTokens[i + 1].first().isKeyword(Keyword::kw_Limit)
               && filterTokens[i + 1].second().isOne()) 
            {
               i++;
               parseOrder<gen::_loptr>(limitOne, nullptr, ts, tsf, p2);
            }
            else {
               indices = std::make_unique<gen::OrderIndices>();
               parseOrder<gen::_ordptr>(order, indices.get(), ts, tsf, p2);
            }

            // retreat previous context
            // and add a new one instead
            p2.contexts.retreatFileContext();
            _fcptr nextContext = std::make_unique<FileContext>(p2);
            p2.contexts.addFileContext(nextContext.get());
            _defptr prev = std::move(base);

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

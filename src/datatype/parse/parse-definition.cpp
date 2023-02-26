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


namespace uro::parse
{

_bool parseDefinition(_defptr& result, const Tokens& tks, _uro& uro)
{
   const _size len = tks.getLength();

   if (len == 1) {
      return parseOneToken(uro, tks, result);
   }

   if (tks.check(TI_HAS_FILTER_KEYWORD)) {
      return parseDefFilter(result, tks, uro);
   }

   if (tks.check(TI_HAS_CHAR_COMMA) && parseDefChain(result, tks, uro)) {
      return true;
   }

   return parseDefBinary(result, tks, uro) || parseDefTernary(result, tks, uro);
}


// Definition chain is a collection of Strings, Lists and Definitions
// separated by commas, that contains at least one Definition
// as a result, multiple elements are transformed into one Definition
static _bool parseDefChain(_defptr& result, const Tokens& tks, _uro& uro)
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

   if (parse(uro, elements[0], prevDef)) {
      cl = ChainLink::cl_Definition;
   }
   else {
      if (parse(uro, elements[0], prevStr)) {
         cl = ChainLink::cl_String;
      }
      else {
         if (parse(uro, elements[0], prevList)) {
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
            if (parse(uro, tk, def)) {
               _defptr pdef = std::move(prevDef);
               prevDef = std::make_unique<gen::Join_DefDef>(pdef, def, uro);
            }
            else {
               _genptr<_str> str;
               if (parse(uro, tk, str)) {
                  _defptr pdef = std::move(prevDef);
                  prevDef = std::make_unique<gen::Join_DefStr>(pdef, str, uro);
               }
               else {
                  _genptr<_list> list;
                  if (parse(uro, tk, list)) {
                     _defptr pdef = std::move(prevDef);
                     prevDef = std::make_unique<gen::Join_DefList>(pdef, list, uro);
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
            if (parse(uro, tk, def)) {
               prevDef = std::make_unique<gen::Join_StrDef>(prevStr, def, uro);
               prevStr.reset();
               cl = ChainLink::cl_Definition;
            }
            else {
               _genptr<_str> str;
               if (parse(uro, tk, str)) {
                  prevList = std::make_unique<gen::Join_StrStr>(prevStr, str);
                  prevStr.reset();
                  cl = ChainLink::cl_List;
               }
               else {
                  _genptr<_list> list;
                  if (parse(uro, tk, list)) {
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
            if (parse(uro, tk, def)) {
               prevDef = std::make_unique<gen::Join_ListDef>(prevList, def, uro);
               prevList.reset();
               cl = ChainLink::cl_Definition;
            }
            else {
               _genptr<_str> str;
               if (parse(uro, tk, str)) {
                  _genptr<_list> plist(std::move(prevList));
                  prevList = std::make_unique<gen::Join_ListStr>(plist, str);
               }
               else {
                  _genptr<_list> list;
                  if (parse(uro, tk, list)) {
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


static _bool parseDefTernary(_defptr& result, const Tokens& tks, _uro& uro)
{
   if (!tks.check(TI_IS_POSSIBLE_TERNARY)) {
      return false;
   }

   std::tuple<Tokens, Tokens, Tokens> trio = tks.divideForTernary();

   _genptr<_bool> condition;
   if (!parse(uro, std::get<0>(trio), condition)) {
      return false;
   }

   _defptr left;
   if (!parse(uro, std::get<1>(trio), left)) {
      return false;
   }

   _defptr right;
   if (!parse(uro, std::get<2>(trio), right)) {
      return false;
   }

   result = std::make_unique<gen::DefTernary>(condition, left, right);
   return true;
}


static _bool parseDefBinary(_defptr& result, const Tokens& tks, _uro& uro)
{
   if (!tks.check(TI_IS_POSSIBLE_BINARY)) {
      return false;
   }

   std::pair<Tokens, Tokens> pair = tks.divideBySymbol(CHAR_QUESTION_MARK);

   _genptr<_bool> condition;
   if (!parse(uro, pair.first, condition)) {
      return false;
   }

   _defptr value;
   if (!parse(uro, pair.second, value)) {
      return false;
   }

   result = std::make_unique<gen::DefBinary>(condition, value);
   return true;
}


static _bool parseDefFilter(_defptr& result, const Tokens& tks, _uro& uro)
{
   const _size firstKeywordId = tks.getFilterKeywordId();

   if (firstKeywordId == tks.getStart()) {
      throw SyntaxError::filterKeywordAtStart(tks.first().getOriginString(uro), tks.first().line);
   }
   else if (firstKeywordId == tks.getStart() + tks.getLength() - 1) {
      const Token& t = tks.listAt(firstKeywordId);
      throw SyntaxError::filterKeywordAtEnd(t.getOriginString(uro), t.line);
   }

   const Tokens tks2(tks, tks.getStart(), firstKeywordId - tks.getStart());
   _defptr base;
   if (!parse(uro, tks2, base)) {
      return false;
   }

   const _int kw = firstKeywordId - tks.getStart() + 1;
   const _int start = tks.getStart() + kw;
   const _int length = tks.getLength() - kw;
   const Tokens tks3(tks, start, length);
   std::vector<Tokens> filterTokens = tks3.splitByFiltherKeywords(uro);
   const _size flength = filterTokens.size();

   FileContext* contextPtr = base->getFileContext();
   if (contextPtr == nullptr) {
      _defptr prev = std::move(base);
      base = std::make_unique<gen::DefWithContext>(prev, uro);
      contextPtr = base->getFileContext();
   }

   uro.contexts.addFileContext(contextPtr);

   for (_size i = 0; i < flength; i++) {
      Tokens& ts = filterTokens[i];
      const Token tsf = ts.first();
      const Keyword& kw = tsf.value.keyword.k;
      ts.trimLeft();

      switch (kw) {
         case Keyword::kw_Final: {
            uro.contexts.retreatFileContext();

            _genptr<_num> num;
            if (!parse(uro, ts, num)) {
               throw SyntaxError::keywordNotFollowedByNumber(tsf.getOriginString(uro), tsf.line);
            }

            _fcptr nextContext = std::make_unique<FileContext>(uro);
            uro.contexts.addFileContext(nextContext.get());
            _defptr prev = std::move(base);
            base = std::make_unique<gen::Filter_FinalDef>(prev, num, nextContext, uro);
            break;
         }
         case Keyword::kw_Every:
         case Keyword::kw_Limit:
         case Keyword::kw_Skip: {
            if (kw == Keyword::kw_Limit) {
               checkLimitBySize(ts, uro);
            }

            uro.contexts.retreatFileContext();

            _genptr<_num> num;
            if (!parse(uro, ts, num)) {
               throw SyntaxError::keywordNotFollowedByNumber(tsf.getOriginString(uro), tsf.line);
            }

            uro.contexts.addFileContext(contextPtr);
            _defptr prev = std::move(base);

            switch(kw) {
               case Keyword::kw_Every: {
                  base = std::make_unique<gen::Filter_EveryDef>(prev, num, contextPtr, uro);
                  break;
               }
               case Keyword::kw_Limit: {
                  base = std::make_unique<gen::Filter_LimitDef>(prev, num, contextPtr, uro);
                  break;
               }
               case Keyword::kw_Skip: {
                  base = std::make_unique<gen::Filter_SkipDef>(prev, num, contextPtr, uro);
                  break;
               }
            }
            
            break;
         }
         case Keyword::kw_Where: {
            _genptr<_bool> boo;
            if (!parse(uro, ts, boo)) {
               throw SyntaxError::keywordNotFollowedByBool(tsf.getOriginString(uro), tsf.line);
            }

            _defptr prev = std::move(base);
            base = std::make_unique<gen::Filter_WhereDef>(boo, prev, contextPtr, uro);
            break;
         }
         case Keyword::kw_Order: {
            gen::_ordptr order;
            gen::_indptr indices = std::make_unique<gen::OrderIndices>();
            parseOrder(order, indices, ts, tsf, uro);

            // retreat previous context
            // and add a new one instead
            FileContext* prevContext = contextPtr;
            uro.contexts.retreatFileContext();
            _fcptr nextContext = std::make_unique<FileContext>(uro);
            uro.contexts.addFileContext(nextContext.get());
            _defptr prev = std::move(base);
            base = std::make_unique<gen::OrderBy_Definition>(prev, contextPtr, nextContext, indices, order, uro);
            break;
         }
      }
   }

   uro.contexts.retreatFileContext();
   result = std::move(base);
   return true;
}

}

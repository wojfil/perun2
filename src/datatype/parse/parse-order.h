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

#ifndef PARSE_ORDER_H_INCLUDED
#define PARSE_ORDER_H_INCLUDED


#include "../../perun2.h"
#include "../../tokens.h"
#include "../order-limit-one.h"
#include "../generator/gen-definition.h"
#include "../parse-gen.h"


namespace perun2::parse
{


template <typename T>
void setOrderUnit(gen::_ordptr& result, _genptr<T>& value, const _bool desc, gen::OrderIndices* indices)
{
   if (result) {
      gen::_ordptr prev = std::move(result);
      result = std::make_unique<gen::OrderUnit_Middle<T>>(value, desc, prev, indices);
   }
   else {
      result = std::make_unique<gen::OrderUnit_Final<T>>(value, desc, indices);
   }
}


template <typename T>
void setOrderUnit(gen::_loptr& result, _genptr<T>& value, const _bool desc, gen::OrderIndices* indices)
{
   if (result) {
      gen::_loptr prev = std::move(result);
      result = std::make_unique<gen::LimitOneUnit_Middle<T>>(value, desc, prev);
   }
   else {
      result = std::make_unique<gen::LimitOneUnit_Final<T>>(value, desc);
   }
}


template <typename T2>
_bool parseOrder(T2& result, gen::OrderIndices* indices, Tokens& tks, const Token& keyword, _p2& p2)
{
   const Token& first = tks.first();
   
   if (tks.getLength() == 1 && first.type == Token::t_Keyword) {
      const Keyword& kw = first.value.keyword.k;
      FileContext* fc = p2.contexts.getFileContext();

      if (kw == Keyword::kw_Asc) {
         _genptr<_str> str = std::make_unique<VariableReference<_str>>(fc->this_.get());
         setOrderUnit(result, str, false, indices);
         return true;
      }
      else if (kw == Keyword::kw_Desc) {
         _genptr<_str> str = std::make_unique<VariableReference<_str>>(fc->this_.get());
         setOrderUnit(result, str, true, indices);
         return true;
      }
   }

   if (!first.isKeyword(Keyword::kw_By)) {
      throw SyntaxError(str(L"keyword '", keyword.getOriginString(p2),
         L"' should be followed by a keyword 'by'"), first.line);
   }

   tks.trimLeft();
   if (tks.isEmpty()) {
      throw SyntaxError(str(L"declaration of '", keyword.getOriginString(p2),
         L" ", first.getOriginString(p2), L"' filter is empty"), first.line);
   }

   std::vector<Tokens> tokensList;
   if (tks.check(TI_HAS_CHAR_COMMA)) {
      tokensList = tks.splitBySymbol(CHAR_COMMA);
   }
   else {
      tokensList.emplace_back(tks);
   }

   const _int length = tokensList.size();

   for (_int i = length - 1; i >= 0; i--) {
      Tokens& tk = tokensList[i];
      _bool desc = false;
      const Token last = tk.last();

      if (last.type == Token::t_Keyword) {
         const Keyword& kw = last.value.keyword.k;
         if (kw == Keyword::kw_Asc) {
            tk.trimRight();
            if (tk.isEmpty()) {
               throw SyntaxError(str(L"keyword '", last.getOriginString(p2),
                  L"' is not preceded by a value used for order"), last.line);
            }
         }
         else if (kw == Keyword::kw_Desc) {
            desc = true;
            tk.trimRight();
            if (tk.isEmpty()) {
               throw SyntaxError(str(L"keyword '", last.getOriginString(p2),
                  L"' is not preceded by a value used for order"), last.line);
            }
         }
      }

      _genptr<_bool> uboo;
      if (parse(p2, tk, uboo)) {
         setOrderUnit(result, uboo, desc, indices);
         continue;
      }

      _genptr<_num> unum;
      if (parse(p2, tk, unum)) {
         setOrderUnit(result, unum, desc, indices);
         continue;
      }

      _genptr<_per> uper;
      if (parse(p2, tk, uper)) {
         setOrderUnit(result, uper, desc, indices);
         continue;
      }

      _genptr<_tim> utim;
      if (parse(p2, tk, utim)) {
         setOrderUnit(result, utim, desc, indices);
         continue;
      }

      _genptr<_str> ustr;
      if (parse(p2, tk, ustr)) {
         setOrderUnit(result, ustr, desc, indices);
         continue;
      }
      else {
         throw SyntaxError(L"value of this order unit cannot be resolved to any valid data type. "
            L"Hint: if you use multiple variables for order, separate them by commas",
            tk.first().line);
      }
   }

   return true;
}

}

#endif // PARSE_ORDER_H_INCLUDED

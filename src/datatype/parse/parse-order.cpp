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

#include "parse-order.h"
#include "../../var.h"


namespace uro::parse
{

_bool parseOrder(gen::_ordptr& resultOrder, gen::_indptr& indices, Tokens& tks, const Token& keyword, _uro& uro)
{
   const Token& first = tks.first();
   
   if (tks.getLength() == 1 && first.type == Token::t_Keyword) {
      const Keyword& kw = first.value.keyword.k;
      FileContext* fc = uro.contexts.getFileContext();

      if (kw == Keyword::kw_Asc) {
         _genptr<_str> str = std::make_unique<VariableReference<_str>>(fc->this_.get());
         resultOrder = std::make_unique<gen::OrderUnit_Final<_str>>(str, false, indices);
         return true;
      }
      else if (kw == Keyword::kw_Desc)
      {
         _genptr<_str> str = std::make_unique<VariableReference<_str>>(fc->this_.get());
         resultOrder = std::make_unique<gen::OrderUnit_Final<_str>>(str, true, indices);
         return true;
      }
   }

   if (!first.isKeyword(Keyword::kw_By)) {
      throw SyntaxError(str(L"keyword '", keyword.getOriginString(uro),
         L"' should be followed by a keyword 'by'"), first.line);
   }

   tks.trimLeft();
   if (tks.isEmpty()) {
      throw SyntaxError(str(L"declaration of '", keyword.getOriginString(uro),
         L" ", first.getOriginString(uro), L"' filter is empty"), first.line);
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
               throw SyntaxError(str(L"keyword '", last.getOriginString(uro),
                  L"' is not preceded by a value used for order"), last.line);
            }
         }
         else if (kw == Keyword::kw_Desc) {
            desc = true;
            tk.trimRight();
            if (tk.isEmpty()) {
               throw SyntaxError(str(L"keyword '", last.getOriginString(uro),
                  L"' is not preceded by a value used for order"), last.line);
            }
         }
      }

      _genptr<_bool> uboo;
      if (parse(uro, tk, uboo)) {
         setOrderUnit(resultOrder, uboo, desc, indices);
         continue;
      }

      _genptr<_num> unum;
      if (parse(uro, tk, unum)) {
         setOrderUnit(resultOrder, unum, desc, indices);
         continue;
      }

      _genptr<_per> uper;
      if (parse(uro, tk, uper)) {
         setOrderUnit(resultOrder, uper, desc, indices);
         continue;
      }

      _genptr<_tim> utim;
      if (parse(uro, tk, utim)) {
         setOrderUnit(resultOrder, utim, desc, indices);
         continue;
      }

      _genptr<_str> ustr;
      if (parse(uro, tk, ustr)) {
         setOrderUnit(resultOrder, ustr, desc, indices);
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

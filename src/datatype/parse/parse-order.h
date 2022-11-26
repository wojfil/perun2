/*
    This file is part of Uroboros.
    Uroboros is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    Uroboros is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with Uroboros. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef PARSE_ORDER_H_INCLUDED
#define PARSE_ORDER_H_INCLUDED


#include "../../var/var-inner.h"
#include "../../uroboros.h"
#include "../../tokens.h"
#include "../generator/gen-definition.h"
#include "../parse-gen.h"


namespace uro::parse
{

void resetOrderParseSettings(const ThisState& state, const ThisState& prevState, Uroboros& uro);
void orderUnitFailure(const Token& tk, Uroboros& uro);
void prepareOrderUnit(Tokens& tks, _bool& desc, gen::_ordptr& order, gen::_indptr& indices, Uroboros& uro);


template <typename T>
void setOrderUnit(gen::_ordptr& order, _genptr<T>& value, const _bool& desc, gen::_indptr& indices)
{
   if (order) {
      gen::_ordptr prev = std::move(order);
      order = std::make_unique<gen::OrderUnit_Middle<T>>(value, desc, prev, indices);
   }
   else {
      order = std::make_unique<gen::OrderUnit_Final<T>>(value, desc, indices);
   }
}

void setSingleOrderFilter(_attrptr& attr, const _bool& hasMemory, _defptr& result,
   gen::_indptr& indices, gen::_ordptr& order, Uroboros& uro);

template <typename T>
void setSingleOrderFilter(_attrptr& attr, const _bool& hasMemory,
   _genptr<std::vector<T>>& result, gen::_indptr& indices, gen::_ordptr& order, Uroboros& uro)
{
   _genptr<std::vector<T>> prev = std::move(result);
   result = std::make_unique<gen::OrderBy_List<T>>(prev, attr, indices, order, uro);
}


template <typename T, typename T2>
void addOrderByFilter(T& result, const ThisState& state, const Token& orderKeyword,
   Tokens& ts2, _fdata* fdata, Uroboros& uro)
{
   const ThisState prevThisState = uro.vars.inner.thisState;
   uro.vars.inner.thisState = state;
   const _bool hasMemory = uro.vc.anyAttribute();
   _attrptr attr;

   if (state == ThisState::ts_String) {
      if (fdata == nullptr) {
         attr = std::make_unique<Attribute>(uro);
      }
      else {
         attr = std::make_unique<BridgeAttribute>(uro, fdata);
      }

      uro.vc.addAttribute(attr);
   }

   const Token& first = ts2.first();

   if (ts2.getLength() == 1 && first.type == Token::t_Keyword) {
      const Keyword& kw = first.value.keyword.k;
      if (kw == Keyword::kw_Asc || kw == Keyword::kw_Desc) {
         const _bool desc = kw == Keyword::kw_Desc;
         gen::_indptr indices(new gen::OrderIndices());

         switch (state) {
            case ThisState::ts_String: {
               _genptr<_str> str;
               uro.vars.inner.createThisRef(str);
               gen::_ordptr ord(new gen::OrderUnit_Final<_str>(str, desc, indices));
               setSingleOrderFilter(attr, hasMemory, result, indices, ord, uro);
               break;
            }
            case ThisState::ts_Number: {
               _genptr<_num> num;
               uro.vars.inner.createThisRef(num);
               gen::_ordptr ord(new gen::OrderUnit_Final<_num>(num, desc, indices));
               setSingleOrderFilter(attr, hasMemory, result, indices, ord, uro);
               break;
            }
            case ThisState::ts_Time: {
               _genptr<_tim> tim;
               uro.vars.inner.createThisRef(tim);
               gen::_ordptr ord(new gen::OrderUnit_Final<_tim>(tim, desc, indices));
               setSingleOrderFilter(attr, hasMemory, result, indices, ord, uro);
               break;
            }
         }

         resetOrderParseSettings(state, prevThisState, uro);
         return;
      }
   }

   if (!first.isKeyword(Keyword::kw_By)) {
      throw SyntaxException(str(L"keyword '", orderKeyword.getOriginString(uro),
         L"' should be followed by a keyword 'by'"), first.line);
   }

   ts2.trimLeft();
   if (ts2.isEmpty()) {
      throw SyntaxException(str(L"declaration of '", orderKeyword.getOriginString(uro),
         L" ", first.getOriginString(uro), L"' filter is empty"), first.line);
   }

   std::vector<Tokens> tokensList;
   if (ts2.check(TI_HAS_CHAR_COMMA)) {
      tokensList = ts2.splitBySymbol(L',');
   }
   else {
      tokensList.emplace_back(ts2);
   }

   const _int length = tokensList.size();
   gen::_ordptr order;
   gen::_indptr indices(new gen::OrderIndices());

   for (_int i = length - 1; i >= 0; i--) {
      Tokens& tk = tokensList[i];
      _bool desc;
      prepareOrderUnit(tk, desc, order, indices, uro);

      _genptr<_bool> uboo;
      if (parse(uro, tk, uboo)) {
         setOrderUnit(order, uboo, desc, indices);
         continue;
      }

      _genptr<_num> unum;
      if (parse(uro, tk, unum)) {
         setOrderUnit(order, unum, desc, indices);
         continue;
      }

      _genptr<_per> uper;
      if (parse(uro, tk, uper)) {
         setOrderUnit(order, uper, desc, indices);
         continue;
      }

      _genptr<_tim> utim;
      if (parse(uro, tk, utim)) {
         setOrderUnit(order, utim, desc, indices);
         continue;
      }

      _genptr<_str> ustr;
      if (parse(uro, tk, ustr)) {
         setOrderUnit(order, ustr, desc, indices);
         continue;
      }
      else {
         throw SyntaxException(L"value of this order unit cannot be resolved to any valid data type. "
            L"Hint: if you use multiple variables for order, separate them by commas",
            tk.first().line);
      }
   }

   setSingleOrderFilter(attr, hasMemory, result, indices, order, uro);
   resetOrderParseSettings(state, prevThisState, uro);
}

}

#endif // PARSE_ORDER_H_INCLUDED

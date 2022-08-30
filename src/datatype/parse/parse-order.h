#ifndef PARSE_ORDER_H_INCLUDED
#define PARSE_ORDER_H_INCLUDED


#include "../../var/var-inner.h"
#include "../../uroboros.h"
#include "../../tokens.h"
#include "../generator/gen-definition.h"
#include "../parse-gen.h"


void resetOrderParseSettings(const ThisState& state, const ThisState& prevState, Uroboros* uro);

template <typename T>
void cleanAfterOrderParseFailure(T& result, Attribute* attr, const ThisState& state)
{
   if (state == ThisState::ts_String) {
      delete attr;
   }

   delete result;
}


template <typename T>
void orderUnitFailure(const Token& tk, T& result)
{
   throw SyntaxException(str(L"keyword '", *tk.value.keyword.os,
      L"' is not preceded by a value used for order"), tk.line);
}


template <typename T>
void prepareOrderUnit(Tokens& tks, _boo& desc, T& result, Attribute* attr,
   const ThisState& state, Order* order, OrderIndices* indices)
{
   desc = false;
   const Token& last = tks.last();

   if (last.type == Token::t_Keyword) {
      const Keyword& kw = last.value.keyword.k;
      if (kw == Keyword::kw_Asc) {
         tks.trimRight();
         if (tks.isEmpty()) {
            delete indices;
            if (order != nullptr) {
               delete order;
            }
            cleanAfterOrderParseFailure(result, attr, state);
            orderUnitFailure(last, result);
         }
      }
      else if (kw == Keyword::kw_Desc) {
         desc = true;
         tks.trimRight();
         if (tks.isEmpty()) {
            delete indices;
            if (order != nullptr) {
               delete order;
            }
            cleanAfterOrderParseFailure(result, attr, state);
            orderUnitFailure(last, result);
         }
      }
   }
}


template <typename T>
void setOrderUnit(Order*& order, Generator<T>* value, const _boo& desc, OrderIndices* indices)
{
   if (order == nullptr) {
      order = new OrderUnit_Final<T>(value, desc, indices);
   }
   else {
      order = new OrderUnit_Middle<T>(value, desc, order, indices);
   }
}

void setSingleOrderFilter(Attribute* attr, const _boo& hasMemory, _def*& result,
   OrderIndices* indices, Order* order, Uroboros* uro);

template <typename T>
void setSingleOrderFilter(Attribute* attr, const _boo& hasMemory,
   Generator<std::vector<T>>*& result, OrderIndices* indices, Order* order, Uroboros* uro)
{
   result = new OrderBy<T>(new OrderBase_Depthless<T>(result), attr, indices, order, uro);
}


template <typename T, typename T2>
void addOrderByFilter(T& result, const ThisState& state, const Token& orderKeyword,
   Tokens& ts2, Uroboros* uro)
{
   const ThisState prevThisState = uro->vars.inner.thisState;
   uro->vars.inner.thisState = state;
   const _boo hasMemory = uro->vc.anyAttribute();
   Attribute* attr = state == ThisState::ts_String
      ? new Attribute(uro)
      : nullptr;

   if (state == ThisState::ts_String) {
      uro->vc.addAttribute(attr);
   }

   const Token& first = ts2.first();

   if (ts2.getLength() == 1 && first.type == Token::t_Keyword) {
      const Keyword& kw = first.value.keyword.k;
      if (kw == Keyword::kw_Asc || kw == Keyword::kw_Desc) {
         const _boo desc = kw == Keyword::kw_Desc;
         OrderIndices* indices = new OrderIndices();

         switch (state) {
            case ThisState::ts_String: {
               Generator<_str>* str;
               uro->vars.inner.createThisRef(str);
               setSingleOrderFilter(attr, hasMemory, result, indices,
                  new OrderUnit_Final<_str>(str, desc, indices), uro);
               break;
            }
            case ThisState::ts_Number: {
               Generator<_num>* num;
               uro->vars.inner.createThisRef(num);
               setSingleOrderFilter(attr, hasMemory, result, indices,
                  new OrderUnit_Final<_num>(num, desc, indices), uro);
               break;
            }
            case ThisState::ts_Time: {
               Generator<_tim>* tim;
               uro->vars.inner.createThisRef(tim);
               setSingleOrderFilter(attr, hasMemory, result, indices,
                  new OrderUnit_Final<_tim>(tim, desc, indices), uro);
               break;
            }
         }

         resetOrderParseSettings(state, prevThisState, uro);
         return;
      }

      return;
   }

   if (!first.isKeyword(Keyword::kw_By)) {
      cleanAfterOrderParseFailure(result, attr, state);
      throw SyntaxException(str(L"keyword '", *orderKeyword.value.keyword.os,
         L"' should be followed by a keyword 'by'"), first.line);
   }

   ts2.trimLeft();
   if (ts2.isEmpty()) {
      cleanAfterOrderParseFailure(result, attr, state);
      throw SyntaxException(str(L"declaration of '", *orderKeyword.value.keyword.os,
         L" ", *first.value.keyword.os, L"' filter is empty"), first.line);
   }

   Tokens ts3 = prepareForGen(ts2, uro);
   if (ts3.isEmpty()) {
      cleanAfterOrderParseFailure(result, attr, state);
      throw SyntaxException(str(L"declaration of '", *orderKeyword.value.keyword.os,
         L" ", *first.value.keyword.os, L"' filter is empty"), first.line);
   }

   std::vector<Tokens> tokensList;
   if (ts3.containsSymbol(PG_CHAR_COMMA)) {
      ts3.splitBySymbol(L',', tokensList);
   }
   else {
      tokensList.push_back(ts3);
   }

   const _int length = tokensList.size();
   Order* order = nullptr;
   OrderIndices* indices = new OrderIndices();

   for (_int i = length - 1; i >= 0; i--) {
      Tokens& tk = tokensList[i];
      _boo desc;
      prepareOrderUnit(tk, desc, result, attr, state, order, indices);

      Generator<_boo>* uboo;
      if (parse(uro, tk, uboo)) {
         setOrderUnit(order, uboo, desc, indices);
         continue;
      }

      Generator<_num>* unum;
      if (parse(uro, tk, unum)) {
         setOrderUnit(order, unum, desc, indices);
         continue;
      }

      Generator<_per>* uper;
      if (parse(uro, tk, uper)) {
         setOrderUnit(order, uper, desc, indices);
         continue;
      }

      Generator<_tim>* utim;
      if (parse(uro, tk, utim)) {
         setOrderUnit(order, utim, desc, indices);
         continue;
      }

      Generator<_str>* ustr;
      if (parse(uro, tk, ustr)) {
         setOrderUnit(order, ustr, desc, indices);
         continue;
      }
      else {
         if (order != nullptr) {
            delete order;
         }
         delete indices;
         cleanAfterOrderParseFailure(result, attr, state);

         throw SyntaxException(L"value of this order unit cannot be resolved to any valid data type. "
            L"Hint: if you use multiple variables for order, separate them by commas",
            tk.first().line);
      }
   }

   setSingleOrderFilter(attr, hasMemory, result, indices, order, uro);
   resetOrderParseSettings(state, prevThisState, uro);
}

#endif // PARSE_ORDER_H_INCLUDED

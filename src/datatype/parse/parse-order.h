#ifndef PARSE_ORDER_H_INCLUDED
#define PARSE_ORDER_H_INCLUDED


#include "../../var/var-inner.h"
#include "../../uroboros.h"
#include "../../tokens.h"
#include "../generator/gen-definition.h"
#include "../parse-gen.h"


void resetOrderParseSettings(const ThisState& state, const ThisState& prevState, Uroboros* uro);



template <typename T>
void orderUnitFailure(const Token& tk, T& result)
{
   delete result;
   throw SyntaxException(str(L"keyword '", *tk.value.keyword.os,
      L"' is not preceded by a value"), tk.line);
}


template <typename T>
void prepareOrderUnit(Tokens& tks, _boo& desc, T& result)
{
   desc = false;
   const Token& last = tks.last();

   if (last.type == Token::t_Keyword) {
      const Keyword& kw = last.value.keyword.k;
      if (kw == Keyword::kw_Asc) {
         tks.trimRight();
         if (tks.isEmpty()) {
            orderUnitFailure(last, result);
         }
      }
      else if (kw == Keyword::kw_Desc) {
         desc = true;
         tks.trimRight();
         if (tks.isEmpty()) {
            orderUnitFailure(last, result);
         }
      }
   }
}


template <typename U1>
void setSingleOrderFilter(Generator<U1>* value, Attribute* attr, const _boo& hasMemory,
   const _boo& desc, _def*& result, Uroboros* uro)
{
   OrderBase_Definition* obase = new OrderBase_Definition(result, uro);
   result = new OrderByCast(new OrderBy_OneValue<_str, U1>(
      obase, value, attr, desc, uro), attr, hasMemory, obase, uro);
}


template <typename T, typename U1>
void setSingleOrderFilter(Generator<U1>* value, Attribute* attr, const _boo& hasMemory,
   const _boo& desc, Generator<std::vector<T>>*& result, Uroboros* uro)
{
   result = new OrderBy_OneValue<T, U1>(new OrderBase_Depthless<T>(result), value, attr, desc, uro);
}


template <typename T, typename T2>
void psrseTwoUnitsFilter(T& result, Tokens& tks, Uroboros* uro)
{
   Tokens left(tks);
   Tokens right(tks);
   tks.divideBySymbol(L',', left, right);

   if (left.isEmpty()) {
      delete result;
      throw SyntaxException(L"order by ", tks.first().line);

// todo
   }

}


template <typename T, typename T2>
void psrseThreeUnitsFilter(T& result, Tokens& tks, Uroboros* uro)
{


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

         switch (state) {
            case ThisState::ts_String: {
               Generator<_str>* str;
               uro->vars.inner.createThisRef(str);
               setSingleOrderFilter(str, attr, hasMemory, desc, result, uro);
               break;
            }
            case ThisState::ts_Number: {
               Generator<_num>* num;
               uro->vars.inner.createThisRef(num);
               setSingleOrderFilter(num, attr, hasMemory, desc, result, uro);
               break;
            }
            case ThisState::ts_Time: {
               Generator<_tim>* tim;
               uro->vars.inner.createThisRef(tim);
               setSingleOrderFilter(tim, attr, hasMemory, desc, result, uro);
               break;
            }
         }

         resetOrderParseSettings(state, prevThisState, uro);
         return;
      }
   }

   if (!first.isKeyword(Keyword::kw_By)) {
      delete result;
      throw SyntaxException(str(L"keyword '", *orderKeyword.value.keyword.os,
         L"' should be followed by a keyword 'by'"), first.line);
   }

   ts2.trimLeft();
   if (ts2.isEmpty()) {
      delete result;
      throw SyntaxException(str(L"declaration of '", *orderKeyword.value.keyword.os,
         L" ", *first.value.keyword.os, L"' filter is empty"), first.line);
   }

   Tokens ts3 = prepareForGen(ts2, uro);
   if (ts3.isEmpty()) {
      delete result;
      throw SyntaxException(str(L"declaration of '", *orderKeyword.value.keyword.os,
         L" ", *first.value.keyword.os, L"' filter is empty"), first.line);
   }

   const _int commas = ts3.countSymbols(L',');

   switch (commas) {
      case 0: {
         _boo desc;
         prepareOrderUnit(ts3, desc, result);

         Generator<_boo>* uboo;
         if (parse(uro, ts3, uboo)) {
            setSingleOrderFilter(uboo, attr, hasMemory, desc, result, uro);
            resetOrderParseSettings(state, prevThisState, uro);
            return;
         }

         Generator<_num>* unum;
         if (parse(uro, ts3, unum)) {
            setSingleOrderFilter(unum, attr, hasMemory, desc, result, uro);
            resetOrderParseSettings(state, prevThisState, uro);
            return;
         }

         Generator<_per>* uper;
         if (parse(uro, ts3, uper)) {
            setSingleOrderFilter(uper, attr, hasMemory, desc, result, uro);
            resetOrderParseSettings(state, prevThisState, uro);
            return;
         }

         Generator<_tim>* utim;
         if (parse(uro, ts3, utim)) {
            setSingleOrderFilter(utim, attr, hasMemory, desc, result, uro);
            resetOrderParseSettings(state, prevThisState, uro);
            return;
         }

         Generator<_str>* ustr;
         if (parse(uro, ts3, ustr)) {
            setSingleOrderFilter(ustr, attr, hasMemory, desc, result, uro);
            resetOrderParseSettings(state, prevThisState, uro);
            return;
         }
         else {
            delete result;
            throw SyntaxException(str(L"value of '", *orderKeyword.value.keyword.os, L" by' unit "
               L"cannot be resolved to any valid data type. If you use multiple variables for order, separate them by commas"),
               ts3.first().line);
         }

         break;
      }
      case 1: {
         psrseTwoUnitsFilter<T, T2>(result, ts3, uro);
         resetOrderParseSettings(state, prevThisState, uro);
         break;
      }
      case 2: {
         psrseThreeUnitsFilter<T, T2>(result, ts3, uro);
         resetOrderParseSettings(state, prevThisState, uro);
         break;
      }
      default: {
         throw SyntaxException(L"current version of Uroboros does not support order by 4 or more variables",
            ts3.first().line);
      }
   }
}


#endif // PARSE_ORDER_H_INCLUDED

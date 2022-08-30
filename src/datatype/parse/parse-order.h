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
      L"' is not preceded by a value"), tk.line);
}


template <typename T>
void prepareOrderUnit(Tokens& tks, _boo& desc, T& result, Attribute* attr, const ThisState& state)
{
   desc = false;
   const Token& last = tks.last();

   if (last.type == Token::t_Keyword) {
      const Keyword& kw = last.value.keyword.k;
      if (kw == Keyword::kw_Asc) {
         tks.trimRight();
         if (tks.isEmpty()) {
            cleanAfterOrderParseFailure(result, attr, state);
            orderUnitFailure(last, result);
         }
      }
      else if (kw == Keyword::kw_Desc) {
         desc = true;
         tks.trimRight();
         if (tks.isEmpty()) {
            cleanAfterOrderParseFailure(result, attr, state);
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


template <typename U1, typename U2>
void setDoubleOrderFilter(Generator<U1>* value_1, Generator<U2>* value_2, Attribute* attr,
   const _boo& hasMemory, const _boo& desc, _def*& result, Uroboros* uro)
{
   OrderBase_Definition* obase = new OrderBase_Definition(result, uro);
   result = new OrderByCast(new OrderBy_TwoValues<_str, U1, U2>(
      obase, value_1, value_2, attr, desc, uro), attr, hasMemory, obase, uro);
}


template <typename T, typename U1, typename U2>
void setDoubleOrderFilter(Generator<U1>* value_1, Generator<U2>* value_2, Attribute* attr,
   const _boo& hasMemory, const _boo& desc, Generator<std::vector<T>>*& result, Uroboros* uro)
{
   result = new OrderBy_TwoValues<T, U1, U2>(new OrderBase_Depthless<T>(result), value_1, value_2, attr, desc, uro);
}


template <typename U1, typename U2, typename U3>
void setTripleOrderFilter(Generator<U1>* value_1, Generator<U2>* value_2, Generator<U3>* value_3,
   Attribute* attr, const _boo& hasMemory, const _boo& desc, _def*& result, Uroboros* uro)
{
   OrderBase_Definition* obase = new OrderBase_Definition(result, uro);
   result = new OrderByCast(new OrderBy_ThreeValues<_str, U1, U2, U3>(
      obase, value_1, value_2, value_3, attr, desc, uro), attr, hasMemory, obase, uro);
}


template <typename T, typename U1, typename U2, typename U3>
void setTripleOrderFilter(Generator<U1>* value_1, Generator<U2>* value_2, Generator<U3>* value_3,
   Attribute* attr, const _boo& hasMemory, const _boo& desc, Generator<std::vector<T>>*& result, Uroboros* uro)
{
   result = new OrderBy_ThreeValues<T, U1, U2, U3>(
      new OrderBase_Depthless<T>(result), value_1, value_2, value_3, attr, desc, uro);
}


template <typename T, typename T2, typename U1>
void finishTwoUnitsOrder(T& result, Tokens& tks, Attribute* attr,
   const ThisState& state, const _boo& hasMemory, Uroboros* uro, Generator<U1>* firstUnit)
{
   _boo desc;
   prepareOrderUnit(tks, desc, result, attr, state);

   Generator<_boo>* uboo;
   if (parse(uro, tks, uboo)) {
      setDoubleOrderFilter(firstUnit, uboo, attr, hasMemory, desc, result, uro);
      return;
   }

   Generator<_num>* unum;
   if (parse(uro, tks, unum)) {
      setDoubleOrderFilter(firstUnit, unum, attr, hasMemory, desc, result, uro);
      return;
   }

   Generator<_per>* uper;
   if (parse(uro, tks, uper)) {
      setDoubleOrderFilter(firstUnit, uper, attr, hasMemory, desc, result, uro);
      return;
   }

   Generator<_tim>* utim;
   if (parse(uro, tks, utim)) {
      setDoubleOrderFilter(firstUnit, utim, attr, hasMemory, desc, result, uro);
      return;
   }

   Generator<_str>* ustr;
   if (parse(uro, tks, ustr)) {
      setDoubleOrderFilter(firstUnit, ustr, attr, hasMemory, desc, result, uro);
      return;
   }
   else {
      delete firstUnit;
      cleanAfterOrderParseFailure(result, attr, state);
      throw SyntaxException(L"value of the last order unit "
         L"cannot be resolved to any valid data type. If you use multiple variables for order, separate them by commas",
         tks.first().line);
   }
}


template <typename T, typename T2>
void parseTwoUnitsOrder(T& result, Tokens& tks, Attribute* attr,
   const ThisState& state, const _boo& hasMemory, Uroboros* uro)
{
   Tokens left(tks);
   Tokens right(tks);
   tks.divideBySymbol(L',', left, right);

   if (left.isEmpty()) {
      cleanAfterOrderParseFailure(result, attr, state);
      throw SyntaxException(L"value of the first order unit is empty", tks.first().line);
   }

   if (right.isEmpty()) {
      cleanAfterOrderParseFailure(result, attr, state);
      throw SyntaxException(L"value of the second order unit is empty", tks.first().line);
   }

   _boo desc;
   prepareOrderUnit(left, desc, result, attr, state);

   Generator<_boo>* uboo;
   if (parse(uro, left, uboo)) {
      finishTwoUnitsOrder<T, T2, _boo>(result, right, attr, state, hasMemory, uro, uboo);
      return;
   }

   Generator<_num>* unum;
   if (parse(uro, left, unum)) {
      finishTwoUnitsOrder<T, T2, _num>(result, right, attr, state, hasMemory, uro, unum);
      return;
   }

   Generator<_per>* uper;
   if (parse(uro, left, uper)) {
      finishTwoUnitsOrder<T, T2, _per>(result, right, attr, state, hasMemory, uro, uper);
      return;
   }

   Generator<_tim>* utim;
   if (parse(uro, left, utim)) {
      finishTwoUnitsOrder<T, T2, _tim>(result, right, attr, state, hasMemory, uro, utim);
      return;
   }

   Generator<_str>* ustr;
   if (parse(uro, left, ustr)) {
      finishTwoUnitsOrder<T, T2, _str>(result, right, attr, state, hasMemory, uro, ustr);
      return;
   }
   else {
      cleanAfterOrderParseFailure(result, attr, state);
      throw SyntaxException(L"value of the first order unit "
         L"cannot be resolved to any valid data type. If you use multiple variables for order, separate them by commas",
         left.first().line);
   }
}


/*template <typename T, typename T2>
void parseThreeUnitsOrder(T& result, Tokens& tks, Attribute* attr,
   const ThisState& state, const _boo& hasMemory, Uroboros* uro)
{









}*/

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

   const _int commas = ts3.countSymbols(L',');

   switch (commas) {
      case 0: {
         _boo desc;
         prepareOrderUnit(ts3, desc, result, attr, state);

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
            cleanAfterOrderParseFailure(result, attr, state);
            throw SyntaxException(L"value of the order "
               L"cannot be resolved to any valid data type. If you use multiple variables for order, separate them by commas",
               ts3.first().line);
         }

         break;
      }
      case 1: {
         parseTwoUnitsOrder<T, T2>(result, ts3, attr, state, hasMemory, uro);
         resetOrderParseSettings(state, prevThisState, uro);
         break;
      }
      /*case 2: {
         parseThreeUnitsOrder<T, T2>(result, ts3, attr, state, hasMemory, uro);
         resetOrderParseSettings(state, prevThisState, uro);
         break;
      }*/
      default: {
         cleanAfterOrderParseFailure(result, attr, state);
         throw SyntaxException(L"current version of Uroboros does not support order by 3 or more units",
            ts3.first().line);
      }
   }
}


#endif // PARSE_ORDER_H_INCLUDED

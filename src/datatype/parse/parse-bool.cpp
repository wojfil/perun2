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

#include "parse-bool.h"
#include "parse-list.h"
#include "parse-generic.h"
#include "../function.h"
#include "../generator/gen-bool.h"
#include "../../brackets.h"
#include "../../lexer.h"
#include "../generator/gen-like.h"
#include "../../var/var-runtime.h"
#include "../parse-gen.h"
#include <vector>


namespace uro::parse
{

Generator<_boo>* parseBool(const Tokens& tks, uro::Uroboros* uro)
{
   const _size len = tks.getLength();

   if (len == 1) {
      Generator<_boo>* unit = nullptr;
      parseOneToken(uro, tks, unit);
      return unit;
   }

   if (tks.check(TI_HAS_FILTER_KEYWORD)) {
      return nullptr;
   }

   const _boo possibleBinary = tks.check(TI_HAS_CHAR_QUESTION_MARK);

   if (tks.check(TI_IS_POSSIBLE_FUNCTION)) {
      Generator<_boo>* func = func::boolFunction(tks, uro);
      if (func != nullptr) {
         return func;
      }
   }
   else if (len >= 2 && !possibleBinary) {
      // build numeric expression (but only if sequence has any operator)
      BracketsInfo bi;
      const _int end = tks.getEnd();
      const _int start = tks.getStart();

      for (_int i = start; i <= end; i++) {
         const Token& t = tks.listAt(i);
         if (t.type == Token::t_Keyword && isBoolExpOperator(t) && bi.isBracketFree())
         {
            if (!(t.isKeyword(Keyword::kw_Not) && i != end && (tks.listAt(i + 1).isKeyword(Keyword::kw_In) || tks.listAt(i + 1).isKeyword(Keyword::kw_Like))))
            {
               Generator<_boo>* boo = parseBoolExp(tks, uro);
               if (boo == nullptr) {
                  throw SyntaxException(L"syntax of a boolean expression is not valid", tks.first().line);
               }
               else {
                  return boo;
               }
            }
         }
         bi.refresh(t);
      }
   }

   Generator<_boo>* cmp = parseComparisons(tks, uro);
   if (cmp != nullptr) {
      return cmp;
   }

   if (tks.check(TI_HAS_KEYWORD_IN)) {
      Generator<_boo>* boo = parseIn(tks, uro);
      if (boo != nullptr) {
         return boo;
      }
   }

   if (tks.check(TI_HAS_KEYWORD_LIKE)) {
      Generator<_boo>* boo = parseLike(tks, uro);
      if (boo != nullptr) {
         return boo;
      }
   }

   Generator<_boo>* tern = parseTernary<_boo>(tks, uro);
   if (tern != nullptr) {
      return tern;
   }

   return nullptr;
}


// build boolean expression
// multiple logic statements
// connected with keywords not, and, or, xor and brackets ()
static Generator<_boo>* parseBoolExp(const Tokens& tks, uro::Uroboros* uro)
{
   std::vector<ExpElement<_boo>*> infList; // infix notation list
   const _int start = tks.getStart();
   const _int end = tks.getEnd();
   _int sublen = 0, lv1 = 0, lv2 = 0;

   for (_int i = start; i <= end; i++) {
      const Token& t = tks.listAt(i);
      if (t.type == Token::t_Keyword) {
         const _boo free = (lv1 == 0) && (lv2 == 0);
         if (isBoolExpOperator(t)) {
            if (t.isKeyword(Keyword::kw_Not) && i != end &&
                (tks.listAt(i + 1).isKeyword(Keyword::kw_In) || tks.listAt(i + 1).isKeyword(Keyword::kw_Like)))
            {
               sublen++;
            }
            else {
               const _char ch = toBoolExpOperator(t);
               if (sublen == 0) {
                  infList.push_back(new ExpElement<_boo>(ch, t.line));
               }
               else {
                  if (free) {
                     const Tokens tks2(tks, i - sublen, sublen);
                     const _int line = tks2.first().line;

                     if (tks2.getLength() == 1 && tks2.first().isLogicConstant()) {
                        const _boo boo = tks2.first().value.keyword.k == Keyword::kw_True;
                        infList.push_back(new ExpElement<_boo>(boo, line));
                        infList.push_back(new ExpElement<_boo>(ch, line));
                        sublen = 0;
                     }
                     else {
                        Generator<_boo>* boo;
                        if (parse(uro, tks2, boo)) {
                           infList.push_back(new ExpElement<_boo>(boo, line));
                           infList.push_back(new ExpElement<_boo>(ch, line));
                           sublen = 0;
                        }
                        else {
                           langutil::deleteVector(infList);
                           return nullptr;
                        }
                     }
                  }
                  else {
                     sublen++;
                  }
               }
            }
         }
         else {
            sublen++;
         }
      }
      else if (t.type == Token::t_Symbol) {
         sublen++;

         switch (t.value.ch) {
            case L'(': {
               lv1++;
               break;
            }
            case L')': {
               lv1--;
               break;
            }
            case L'[': {
               lv2++;
               break;
            }
            case L']': {
               lv2--;
               break;
            }
            default: {
               break;
            }
         }
      }
      else {
         sublen++;
      }
   }

   if (sublen != 0) {
      const Tokens tks2(tks, 1 + end - sublen, sublen);

      if (tks2.getLength() == 1 && tks2.first().isLogicConstant()) {
         const _boo boo = (tks2.first().value.keyword.k == Keyword::kw_True);
         infList.push_back(new ExpElement<_boo>(boo, tks2.first().line));
      }
      else {
         Generator<_boo>* boo;
         if (parse(uro, tks2, boo)) {
            infList.push_back(new ExpElement<_boo>(boo, tks2.first().line));
         }
         else {
            langutil::deleteVector(infList);
            return nullptr;
         }
      }
   }

   if (!isBoolExpComputable(infList)) {
      langutil::deleteVector(infList);
      throw SyntaxException(L"syntax of a boolean expression is not valid",
         tks.first().line);
   }

   std::vector<ExpElement<_boo>*> pntList;
   Generator<_boo>* boo = boolExpTree(infList, pntList);

   langutil::deleteVector(infList);
   langutil::deleteVector(pntList);

   return boo;
}

static Generator<_boo>* boolExpTree(
   const std::vector<ExpElement<_boo>*>& infList,
   std::vector<ExpElement<_boo>*>& pntList)
{
   std::vector<ExpElement<_boo>*> elements;
   std::vector<ExpElement<_boo>*> temp;
   const _size len = infList.size();
   _int brackets = 0;
   _boo anyNot = false;

   for (_size i = 0; i < len; i++) {
      ExpElement<_boo>* e = infList[i];
      if (e->type == ElementType::et_Operator) {
         const _char op = e->_operator;
         switch (op) {
            case L'(': {
               brackets++;
               if (brackets > 1) {
                  temp.push_back(e);
               }
               break;
            }
            case L')': {
               brackets--;
               if (brackets == 0) {
                  Generator<_boo>* result = boolExpTree(temp, pntList);
                  ExpElement<_boo>* ee = new ExpElement<_boo>(result, e->line);
                  pntList.push_back(ee);
                  temp.resize(0);
                  elements.push_back(ee);
               }
               else {
                  temp.push_back(e);
               }
               break;
            }
            default: {
               if (brackets == 0) {
                  elements.push_back(e);
               }
               else {
                  temp.push_back(e);
               }
               if (!anyNot && op == L'!') {
                  anyNot = true;
               }
               break;
            }
         }
      }
      else
      {
         if (brackets == 0) {
            elements.push_back(e);
         }
         else {
            temp.push_back(e);
         }
      }
   }

   return anyNot
      ? boolExpIntegrateNegations(elements, pntList)
      : boolExpTreeMerge(elements, pntList);
}

static Generator<_boo>* boolExpIntegrateNegations(
   const std::vector<ExpElement<_boo>*>& elements,
   std::vector<ExpElement<_boo>*>& pntList)
{
   std::vector<ExpElement<_boo>*> newList;
   _boo negate = false;
   const _size len = elements.size();

   for (_size i = 0; i < len; i++) {
      ExpElement<_boo>* e = elements[i];
      if (e->type == ElementType::et_Operator) {
         if (e->_operator == L'!') {
            negate = true;
         }
         else {
            newList.push_back(e);
         }
      }
      else {
         if (negate) {
            ExpElement<_boo>* newElement;

            if (e->type == ElementType::et_Constant) {
               const _boo value = !(e->constant);
               newElement = new ExpElement<_boo>(value, e->line);
            }
            else {
               Generator<_boo>* n = e->takeValue();
               Generator<_boo>* no = new gen::Not(n);
               newElement = new ExpElement<_boo>(no, e->line);
            }

            pntList.push_back(newElement);
            newList.push_back(newElement);
            negate = false;
         }
         else {
            newList.push_back(e);
         }
      }
   }

   return boolExpTreeMerge(newList, pntList);
}

static Generator<_boo>* boolExpTreeMerge(
   const std::vector<ExpElement<_boo>*>& elements,
   std::vector<ExpElement<_boo>*>& pntList)
{
   const _size len = elements.size();
   if (len == 1) {
      return elements[0]->takeValue();
   }

   ExpElement<_boo>* firstElement = elements[0];
   ExpElement<_boo>* oper;

   for (_size i = 1; i < len; i++) {
      ExpElement<_boo>* secondElement = elements[i];
      const ElementType& type = secondElement->type;

      if (type == ElementType::et_Operator) {
         oper = secondElement;
      }
      else {
         const _char& op = oper->_operator;
         ExpElement<_boo>* newElement;

         if (type == ElementType::et_Constant
            && firstElement->type == ElementType::et_Constant) {

            const _boo v1 = firstElement->constant;
            const _boo v2 = secondElement->constant;
            _boo value;

            switch(op) {
               case L'&': {
                  value = v1 && v2;
                  break;
               }
               case L'|': {
                  value = v1 || v2;
                  break;
               }
               case L'^': {
                  value = v1 ^ v2;
                  break;
               }
            }

            newElement = new ExpElement<_boo>(value, firstElement->line);
         }
         else {
            gen::BinaryOperation<_boo>* bin;
            Generator<_boo>* first = firstElement->takeValue();
            Generator<_boo>* second = secondElement->takeValue();

            switch(op) {
               case L'&': {
                  bin = new gen::And(first, second);
                  break;
               }
               case L'|': {
                  bin = new gen::Or(first, second);
                  break;
               }
               case L'^': {
                  bin = new gen::Xor(first, second);
                  break;
               }
            }

            newElement = new ExpElement<_boo>(bin, firstElement->line);
         }
         pntList.push_back(newElement);
         firstElement = newElement;
      }
   }

   return firstElement->takeValue();
}

static _boo isBoolExpComputable(const std::vector<ExpElement<_boo>*>& infList)
{
   const _size len = infList.size();
   if (len == 0) {
      return false;
   }

   // bool expressions can start with only two symbols: not or (
   ExpElement<_boo>* prev = infList[0];
   if (prev->type == ElementType::et_Operator) {
      const _char op = prev->_operator;
      if (!(op == L'(' || op == L'!')) {
         return false;
      }
   }

   // bool expressions can end with only one symbol: )
   ExpElement<_boo>* last = infList[len - 1];
   if (last->type == ElementType::et_Operator) {
      if (last->_operator != L')') {
         return false;
      }
   }

   for (_size i = 1; i < len; i++) {
      ExpElement<_boo>* curr = infList[i];
      const _boo cop = curr->type == ElementType::et_Operator;

      if (prev->type == ElementType::et_Operator) {
         switch (prev->_operator) {
            case L'!': {
               if (cop && curr->_operator != L'(') {
                  return false;
               }
               break;
            }
            case L'(': {
               if (cop) {
                  const _char op = curr->_operator;
                  if (!(op == L'(' || op == L'!')) {
                     return false;
                  }
               }
               break;
            }
            case L')': {
               if (cop) {
                  const _char op = curr->_operator;
                  if (op == L'(' || op == L'!') {
                     return false;
                  }
               }
               else {
                  return false;
               }
               break;
            }
            default: {
               if (cop) {
                  const _char op = curr->_operator;
                  if (!(op == L'(' || op == L'!')) {
                     return false;
                  }
               }
               break;
            }
         }
      }
      else
      {
         if (cop) {
            const _char op = curr->_operator;
            if (op == L'!' || op == L'(') {
               return false;
            }
         }
         else {
            return false;
         }
      }
      prev = curr;
   }

   return true;
}

_boo isBoolExpOperator(const Token& tk)
{
   switch (tk.value.keyword.k) {
      case Keyword::kw_And:
      case Keyword::kw_Or:
      case Keyword::kw_Xor:
      case Keyword::kw_Not:
         return true;
      default:
         return false;
   }
}

static _char toBoolExpOperator(const Token& tk)
{
   switch (tk.value.keyword.k) {
      case Keyword::kw_And:
         return L'&';
      case Keyword::kw_Or:
         return L'|';
      case Keyword::kw_Xor:
         return L'^';
      case Keyword::kw_Not:
         return L'!';
      default:
         return L' ';
   }
}

template <typename T>
static Generator<_boo>* parseIn_Unit(const _boo& negated, 
   const std::pair<Tokens, Tokens>& pair, uro::Uroboros* uro)
{
   Generator<T>* valLeft;
   if (!parse(uro, pair.first, valLeft)) {
      return nullptr;
   }

   // check if the right side is a single value
   // in this special case, the whole structure is reduced to
   // a simple comparison "left == right" or "left != right"
   Generator<T>* num2;
   if (parse(uro, pair.second, num2)) {
      if (negated) {
         return new gen::NotEquals<T>(valLeft, num2);
      }
      else {
         return new gen::Equals<T>(valLeft, num2);
      }
   }

   Generator<std::vector<T>>* valRight;

   if (parse(uro, pair.second, valRight)) {
      if (valRight->isConstant()) {
         const std::vector<T> vs = valRight->getValue();
         delete valRight;
         Generator<_boo>* in = new gen::InConstList<T>(valLeft, vs);
         return negated ? new gen::Not(in) : in;
      }
      else {
         Generator<_boo>* in = new gen::InList<T>(valLeft, valRight);
         return negated ? new gen::Not(in) : in;
      }
   }
   else {
      delete valLeft;
      return nullptr;
   }
}

static Generator<_boo>* parseIn(const Tokens& tks, uro::Uroboros* uro)
{
   std::pair<Tokens, Tokens> pair = tks.divideByKeyword(Keyword::kw_In);

   if (pair.first.isEmpty()) {
      emptyOperSideException(tks.first(), true, uro);
   }
   if (pair.second.isEmpty()) {
      emptyOperSideException(tks.last(), false, uro);
   }

   _boo neg = pair.first.last().isKeyword(Keyword::kw_Not);
   if (neg) {
      pair.first.trimRight();
      if (pair.first.isEmpty()) {
         emptyOperSideException(tks.first(), true, uro);
      }
   }

   // first: try to build "Number IN NumList"
   Generator<_boo>* list = parseIn_Unit<_num>(neg, pair, uro);
   if (list != nullptr) {
      return list;
   }

   const Token& lf = pair.first.first();

   if (pair.first.getLength() == 1 && lf.type == Token::t_Word &&
      uro->hashes.HASH_GROUP_TIME_ATTR.find(lf.value.word.h) != uro->hashes.HASH_GROUP_TIME_ATTR.end())
   {
      if (pair.second.check(TI_HAS_CHAR_COMMA)) {
         const std::vector<Tokens> elements = pair.second.splitBySymbol(L',');
         const _size elen = elements.size();
         for (_size i = 0; i < elen; i++) {
            const Tokens& t = elements[i];
            if (t.getLength() == 1) {
               const Token& tf = t.first();
               if (tf.isWeekDay()) {
                  timeInNumberException(pair.first.first(), tf, L"weekDay", neg, pair.first, uro);
               }
               else if (tf.isMonth()) {
                  timeInNumberException(pair.first.first(), tf, L"month", neg, pair.first, uro);
               }

               const _boo isInteger = (tf.type == Token::t_Number) && !tf.value.num.n.isDouble;
               if (isInteger) {
                  timeInNumberException(pair.first.first(), tf, L"year", neg, pair.first, uro);
               }
            }
         }
      }
      else if (pair.second.getLength() == 1) {
         const Token& rf = pair.second.first();
         const _boo isWeek = rf.isWeekDay();
         const _boo isMonth = rf.isMonth();

         if (isWeek || isMonth) {
            timeInNumberException(pair.first.first(), pair.second.first(), 
               isWeek ? L"weekDay" : L"month", neg, pair.first, uro);
         }

         const _boo isInteger = (rf.type == Token::t_Number) && !rf.value.num.n.isDouble;
         if (isInteger) {
            timeInNumberException(pair.first.first(), pair.second.first(), L"year", neg, pair.first, uro);
         }
      }
   }

   // secondary: try to build "Time IN TimList"
   Generator<_boo>* list2 = parseInTimList(neg, pair, uro);
   if (list2 != nullptr) {
      return list2;
   }

   // finally: try to build "string IN list"
   Generator<_boo>* list3 = parseIn_Unit<_str>(neg, pair, uro);
   return list3;
}


static Generator<_boo>* parseInTimList(const bool& negated, 
   const std::pair<Tokens, Tokens>& pair, uro::Uroboros* uro)
{
   Generator<_tim>* tim;
   if (!parse(uro, pair.first, tim)) {
      return nullptr;
   }

   Generator<_tim>* tim2;
   if (parse(uro, pair.second, tim2)) {
      if (negated) {
         return new gen::NotEquals<_tim>(tim, tim2);
      }
      else {
         return new gen::Equals<_tim>(tim, tim2);
      }
   }

   Generator<_tlist>* tlist;
   if (parse(uro, pair.second, tlist)) {
      if (tlist->isConstant()) {
         const _tlist vs = tlist->getValue();
         delete tlist;
         Generator<_boo>* in = new gen::InConstTimeList(tim, vs);
         return negated ? new gen::Not(in) : in;
      }
      else {
         Generator<_boo>* in = new gen::InList<_tim>(tim, tlist);
         return negated ? new gen::Not(in) : in;
      }
   }
   else {
      delete tim;
      return nullptr;
   }
}

static void emptyOperSideException(const Token& oper, const bool& isLeft, uro::Uroboros* uro)
{
   const _str side = isLeft ? L"left" : L"right";

   throw SyntaxException(str(side, L" side of operator '", oper.getOriginString(uro), L"' is empty"),
      oper.line);
}

static void timeInNumberException(const Token& timeVar, const Token& numVar,
   const _str& timeMember, const _boo& negated, const Tokens& tks, uro::Uroboros* uro)
{
   if (timeMember == L"year") {
      if (negated) {
         throw SyntaxException(str(L"instead of '", timeVar.getOriginString(uro), L" not in ", toStr(numVar.value.num.n.value.i),
            L"', write '", timeVar.getOriginString(uro), L".year != ",
            toStr(numVar.value.num.n.value.i), L"'"), tks.first().line);
      }
      else {
         throw SyntaxException(str(L"instead of '", timeVar.getOriginString(uro), L" in ", toStr(numVar.value.num.n.value.i),
            L"', write '", timeVar.getOriginString(uro), L".year = ",
            toStr(numVar.value.num.n.value.i), L"'"), tks.first().line);
      }
   }
   else {
      if (negated) {
         throw SyntaxException(str(L"instead of '", timeVar.getOriginString(uro), L" not in ", numVar.getOriginString(uro),
            L"', write '", timeVar.getOriginString(uro), L".", timeMember,
            L" != ", numVar.getOriginString(uro), L"'"), tks.first().line);
      }
      else {
         throw SyntaxException(str(L"instead of '", timeVar.getOriginString(uro), L" in ", numVar.getOriginString(uro),
            L"', write '", timeVar.getOriginString(uro), L".", timeMember,
            L" = ", numVar.getOriginString(uro), L"'"), tks.first().line);
      }
   }
}

static Generator<_boo>* parseLike(const Tokens& tks, uro::Uroboros* uro)
{
   std::pair<Tokens, Tokens> pair = tks.divideByKeyword(Keyword::kw_Like);

   if (pair.first.isEmpty()) {
      emptyOperSideException(tks.first(), true, uro);
   }
   if (pair.second.isEmpty()) {
      emptyOperSideException(tks.last(), false, uro);
   }

   const _boo neg = pair.first.last().isKeyword(Keyword::kw_Not);
   if (neg) {
      pair.first.trimRight();
      if (pair.first.isEmpty()) {
         emptyOperSideException(tks.first(), true, uro);
      }
   }

   Generator<_str>* value;
   if (!parse(uro, pair.first, value)) {
      return nullptr;
   }

   Generator<_str>* pattern;
   if (parse(uro, pair.second, pattern)) {
      if (pattern->isConstant()) {
         const _str cnst = pattern->getValue();
         delete pattern;

         if (neg) {
            return new gen::Not(new gen::LikeConst(value, cnst));
         }
         else {
            return new gen::LikeConst(value, cnst);
         }
      }

      if (neg) {
         return new gen::Not(new gen::Like(value, pattern));
      }
      else {
         return new gen::Like(value, pattern);
      }
   }
   else {
      delete value;
      return nullptr;
   }
}

static Generator<_boo>* parseComparisons(const Tokens& tks, uro::Uroboros* uro)
{
   BracketsInfo bi;
   const _int end = tks.getEnd();

   for (_int i = tks.getStart(); i <= end; i++) {
      const Token& t = tks.listAt(i);
      if (t.type == Token::t_Symbol && bi.isBracketFree()) {
         const _char& ch = t.value.ch;
         switch (ch) {
            case L'<':
            case L'>':
            case L'!':
            case L'=': {
               return parseComparison(tks, ch, uro);
            }
         }
      }
      bi.refresh(t);
   }

   return nullptr;
}

template <typename T>
static Generator<_boo>* comparison(Generator<T>* val1,
   Generator<T>* val2, const gen::CompType& ct)
{
   switch (ct) {
      case gen::ct_Equals:
         return new gen::Equals<T>(val1, val2);
      case gen::ct_NotEquals:
         return new gen::NotEquals<T>(val1, val2);
      case gen::ct_Smaller:
         return new gen::Smaller<T>(val1, val2);
      case gen::ct_SmallerEquals:
         return new gen::SmallerEquals<T>(val1, val2);
      case gen::ct_Bigger:
         return new gen::Bigger<T>(val1, val2);
      case gen::ct_BiggerEquals:
         return new gen::BiggerEquals<T>(val1, val2);
      default:
         return nullptr;
   };
}


template <typename T>
Generator<_boo>* parseComparisonUnit(const Tokens& left, const Tokens& right, const gen::CompType& ct, uro::Uroboros* uro)
{
   Generator<T>* v1;
   Generator<T>* v2;
   const _boo parsed1 = parse(uro, left, v1);
   const _boo parsed2 = parse(uro, right, v2);
   if (parsed1 && parsed2) {
      switch (ct) {
         case gen::ct_Equals:
            return new gen::Equals<T>(v1, v2);
         case gen::ct_NotEquals:
            return new gen::NotEquals<T>(v1, v2);
         case gen::ct_Smaller:
            return new gen::Smaller<T>(v1, v2);
         case gen::ct_SmallerEquals:
            return new gen::SmallerEquals<T>(v1, v2);
         case gen::ct_Bigger:
            return new gen::Bigger<T>(v1, v2);
         case gen::ct_BiggerEquals:
            return new gen::BiggerEquals<T>(v1, v2);
         default:
            return nullptr;
      };
      return comparison<T>(v1, v2, ct);
   }

   if (parsed1) {
      delete v1;
   }
   else if (parsed2) {
      delete v2;
   }

   return nullptr;
}

static Generator<_boo>* parseComparison(const Tokens& tks, const _char& sign, uro::Uroboros* uro)
{
   gen::CompType ct;
   const std::pair<Tokens, Tokens> pair = prepareComparison(tks, sign, ct);
   const Tokens& left = pair.first;
   const Tokens& right = pair.second;

   // look for some common errors
   // and throw precise messages to the user
   if (left.getLength() == 1 && right.getLength() == 1) {
      const Token& t1 = left.first();
      const Token& t2 = right.first();

      const _boo isWeek1 = t1.isWeekDay();
      const _boo isWeek2 = t2.isWeekDay();
      const _boo isMonth1 = t1.isMonth();
      const _boo isMonth2 = t2.isMonth();
      const _boo isVar1 = t1.type == Token::t_Word &&
         uro->hashes.HASH_GROUP_TIME_ATTR.find(t1.value.word.h) != uro->hashes.HASH_GROUP_TIME_ATTR.end();
      const _boo isVar2 = t2.type == Token::t_Word &&
         uro->hashes.HASH_GROUP_TIME_ATTR.find(t2.value.word.h) != uro->hashes.HASH_GROUP_TIME_ATTR.end();

      //const _str& v1 = t1.originString;
      //const _str& v2 = t2.originString;
      const _str s = _str(1, sign);

      if (isVar1 && (isWeek2 || isMonth2)) {
         throw SyntaxException(str(L"instead of '", t1.getOriginString(uro), L" ", s, L" ", t2.getOriginString(uro),
            L"', write '", t1.getOriginString(uro), L".", (isWeek2 ? L"weekDay" : L"month"),
            L" ", s, L" ", t2.getOriginString(uro), L"'"), tks.first().line);
      }
      else if ((isWeek1 || isMonth1) && isVar2) {
         throw SyntaxException(str(L"instead of '", t1.getOriginString(uro), L" ", s, L" ", t2.getOriginString(uro),
            L"', write '", t1.getOriginString(uro), L" ", s, L" ", t2.getOriginString(uro), L".",
            (isWeek1 ? L"weekDay" : L"month"), L"'"), tks.first().line);
      }

      const _boo isInteger1 = (t1.type == Token::t_Number) && !t1.value.num.n.isDouble;
      const _boo isInteger2 = (t2.type == Token::t_Number) && !t2.value.num.n.isDouble;

      if (isVar1 && isInteger2) {
         const _nint& nm = t2.value.num.n.value.i;
         if (nm >= 1950LL && nm <= 2100LL) {
            throw SyntaxException(str(L"instead of '", t1.getOriginString(uro), L" ", s, L" ", toStr(nm),
               L"', write '", t1.getOriginString(uro), L".year ", s, L" ", toStr(nm), L"'"), tks.first().line);
         }
         else {
            throw SyntaxException(str(L"time variable '", t1.getOriginString(uro),
               L"' cannot be compared with a number"), tks.first().line);
         }
      }
      else if (isInteger1 && isVar2) {
         const _nint& nm = t1.value.num.n.value.i;
         if (nm >= 1950LL && nm <= 2100) {
            throw SyntaxException(str(L"instead of '", toStr(nm), L" ", s, L" ", t2.getOriginString(uro),
               L"', write '", toStr(nm), L" ", s, L" ", t2.getOriginString(uro), L".year'"), tks.first().line);
         }
         else {
            throw SyntaxException(str(L"time variable '", t2.getOriginString(uro),
               L"' cannot be compared with a number"), tks.first().line);
         }
      }
   }

   // try to parse comparison for every singular data type
   Generator<_boo>* boo = parseComparisonUnit<_boo>(left, right, ct, uro);
   if (boo != nullptr) {
      return boo;
   }

   Generator<_boo>* num = parseComparisonUnit<_num>(left, right, ct, uro);
   if (num != nullptr) {
      return num;
   }

   Generator<_boo>* per = parseComparisonUnit<_per>(left, right, ct, uro);
   if (per != nullptr) {
      return per;
   }

   Generator<_boo>* tim = parseComparisonUnit<_tim>(left, right, ct, uro);
   if (tim != nullptr) {
      return tim;
   }
   Generator<_boo>* str = parseComparisonUnit<_str>(left, right, ct, uro);
   if (str != nullptr) {
      return str;
   }

   // comparisons between singular values have failed, so try comparisons of collections
   return parseCollectionComparisons(left, right, ct, uro);
}

Generator<_boo>* comparisonDefList( _def* def, Generator<_list>* list, const gen::CompType& ct,
   const _boo& reversed, uro::Uroboros* uro)
{
   switch (ct) {
      case gen::ct_Equals:
         return new gen::DefinitionListEqual(def, list, uro);
      case gen::ct_NotEquals:
         return new gen::DefinitionListNotEqual(def, list, uro);
      default: {
         if (reversed) {
            switch(ct) {
               case gen::ct_Smaller:
                  return new gen::DefinitionListBigger(def, list, uro);
               case gen::ct_SmallerEquals:
                  return new gen::DefinitionListBiggerEquals(def, list, uro);
               case gen::ct_Bigger:
                  return new gen::DefinitionListSmaller(def, list, uro);
               case gen::ct_BiggerEquals:
                  return new gen::DefinitionListSmallerEquals(def, list, uro);
            }
         }
         else {
            switch(ct) {
               case gen::ct_Smaller:
                  return new gen::DefinitionListSmaller(def, list, uro);
               case gen::ct_SmallerEquals:
                  return new gen::DefinitionListSmallerEquals(def, list, uro);
               case gen::ct_Bigger:
                  return new gen::DefinitionListBigger(def, list, uro);
               case gen::ct_BiggerEquals:
                  return new gen::DefinitionListBiggerEquals(def, list, uro);
            }
         }
      }
   }

   return nullptr;
}


template <typename T>
Generator<_boo>* comparisonCollections(const Tokens& left, const Tokens& right, const gen::CompType& ct, uro::Uroboros* uro)
{
   Generator<std::vector<T>>* leftValue;
   if (parse(uro, left, leftValue)) {

      Generator<std::vector<T>>* rightValue;
      if (parse(uro, right, rightValue)) {
         switch(ct) {
            case gen::ct_Equals:
               return new gen::CollectionsEqual<T>(leftValue, rightValue);
            case gen::ct_NotEquals:
               return new gen::CollectionsNotEqual<T>(leftValue, rightValue);
            case gen::ct_Smaller:
               return new gen::CollectionsSmaller<T>(leftValue, rightValue);
            case gen::ct_SmallerEquals:
               return new gen::CollectionsSmallerEquals<T>(leftValue, rightValue);
            case gen::ct_Bigger:
               return new gen::CollectionsBigger<T>(leftValue, rightValue);
            case gen::ct_BiggerEquals:
               return new gen::CollectionsBiggerEquals<T>(leftValue, rightValue);
         }
      }
   }

   return nullptr;
}


template <typename T>
Generator<_boo>* comparisonCollectionValue(const Tokens& left, const Tokens& right,
   const gen::CompType& ct, uro::Uroboros* uro)
{
   Generator<T>* leftValue;
   if (parse(uro, left, leftValue)) {
      Generator<std::vector<T>>* rightCollection;

      if (parse(uro, right, rightCollection)) {
         switch(ct) {
            case gen::ct_Equals:
               return new gen::CollectionValueEquals<T>(rightCollection, leftValue);
            case gen::ct_NotEquals:
               return new gen::CollectionValueNotEquals<T>(rightCollection, leftValue);
            case gen::ct_Smaller:
               return new gen::CollectionValueBigger<T>(rightCollection);
            case gen::ct_SmallerEquals:
               return new gen::CollectionValueBiggerEquals<T>(rightCollection);
            case gen::ct_Bigger:
               return new gen::CollectionValueSmaller<T>(rightCollection);
            case gen::ct_BiggerEquals:
               return new gen::CollectionValueSmallerEquals<T>(rightCollection);
         }
      }
      else {
         delete leftValue;
         return nullptr;
      }
   }

   Generator<T>* rightValue;
   if (parse(uro, right, rightValue)) {
      Generator<std::vector<T>>* leftCollection;

      if (parse(uro, left, leftCollection)) {
         switch(ct) {
            case gen::ct_Equals:
               return new gen::CollectionValueEquals<T>(leftCollection, rightValue);
            case gen::ct_NotEquals:
               return new gen::CollectionValueNotEquals<T>(leftCollection, rightValue);
            case gen::ct_Smaller:
               return new gen::CollectionValueSmaller<T>(leftCollection);
            case gen::ct_SmallerEquals:
               return new gen::CollectionValueSmallerEquals<T>(leftCollection);
            case gen::ct_Bigger:
               return new gen::CollectionValueBigger<T>(leftCollection);
            case gen::ct_BiggerEquals:
               return new gen::CollectionValueBiggerEquals<T>(leftCollection);
         }
      }
      else {
         delete rightValue;
         return nullptr;
      }
   }

   return nullptr;
}

static Generator<_boo>* parseCollectionComparisons(const Tokens& left,
   const Tokens& right, const gen::CompType& ct, uro::Uroboros* uro)
{
   _def* leftDef;
   _def* rightDef;
   const _boo hasLeftDef = parse(uro, left, leftDef);
   const _boo hasRightDef = parse(uro, right, rightDef);

   // special case situations
   // when definition is compared with other data types or with another definition
   // need optimized solutions
   if (hasLeftDef || hasRightDef) {
      if (hasLeftDef && hasRightDef) {
         switch(ct) {
            case gen::ct_Equals:
               return new gen::DefinitionsEqual(leftDef, rightDef, uro);
            case gen::ct_NotEquals:
               return new gen::DefinitionsNotEqual(leftDef, rightDef, uro);
            case gen::ct_Smaller:
               return new gen::DefinitionsSmaller(leftDef, rightDef, uro);
            case gen::ct_SmallerEquals:
               return new gen::DefinitionsSmallerEquals(leftDef, rightDef, uro);
            case gen::ct_Bigger:
               return new gen::DefinitionsBigger(leftDef, rightDef, uro);
            case gen::ct_BiggerEquals:
               return new gen::DefinitionsBiggerEquals(leftDef, rightDef, uro);
         }
      }

      if (hasLeftDef) {
         Generator<_list>* rightList;
         return parse(uro, right, rightList)
            ? comparisonDefList(leftDef, rightList, ct, false, uro)
            : nullptr;
      }
      else {
         Generator<_list>* leftList;
         return parse(uro, left, leftList)
            ? comparisonDefList(rightDef, leftList, ct, true, uro)
            : nullptr;
      }
   }

   Generator<_boo>* tim = comparisonCollectionValue<_tim>(left, right, ct, uro);
   if (tim != nullptr) {
      return tim;
   }

   Generator<_boo>* num = comparisonCollectionValue<_num>(left, right, ct, uro);
   if (num != nullptr) {
      return num;
   }

   Generator<_boo>* str = comparisonCollectionValue<_str>(left, right, ct, uro);
   if (str != nullptr) {
      return str;
   }

   Generator<_boo>* tlist = comparisonCollections<_tim>(left, right, ct, uro);
   if (tlist != nullptr) {
      return tlist;
   }

   Generator<_boo>* nlist = comparisonCollections<_num>(left, right, ct, uro);
   if (nlist != nullptr) {
      return nlist;
   }

   return comparisonCollections<_str>(left, right, ct, uro);
}

static std::pair<Tokens, Tokens> prepareComparison(const Tokens& tks, const _char& sign, gen::CompType& ctype)
{
   std::pair<Tokens, Tokens> result = tks.divideBySymbol(sign);
   _boo eq = false;

   if (result.first.isEmpty()) {
      if (result.second.isEmpty()) {
         throw SyntaxException(str(L"both sides of ", charStr(sign),
            L" comparison are empty"), tks.first().line);
      }
      else {
         throw SyntaxException(str(L"left side of ", charStr(sign),
            L" comparison is empty"), tks.first().line);
      }
   }

   if (result.second.isEmpty()) {
      throw SyntaxException(str(L"right side of ", charStr(sign),
         L" comparison is empty"), tks.last().line);
   }

   if (result.second.first().isSymbol(L'=')) {
      if (result.second.getLength() == 1) {
         throw SyntaxException(str(L"right side of ", charStr(sign),
            L"= comparison is empty"), result.second.first().line);
      }

      result.second.trimLeft();
      eq = true;
   }
   else if (sign == L'!') {
      throw SyntaxException(L"expected = after exclamation mark. "
         L"For a simple negation, use keyword 'not' instead",
         result.second.first().line);
   }

   switch (sign) {
      case L'<':
         ctype = eq ? gen::CompType::ct_SmallerEquals : gen::CompType::ct_Smaller;
         break;
      case L'>':
         ctype = eq ? gen::CompType::ct_BiggerEquals : gen::CompType::ct_Bigger;
         break;
      case L'=':
         ctype = gen::CompType::ct_Equals;
         break;
      case L'!': {
         ctype = gen::CompType::ct_NotEquals;
         break;
      default:
         ctype = gen::CompType::ct_Equals;
         break;
      }
   }

   return result;
}

}

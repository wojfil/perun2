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


Generator<_boo>* parseBool(const Tokens& tks, Uroboros* uro)
{
   const _size len = tks.getLength();

   if (len == 1) {
      Generator<_boo>* unit = nullptr;
      parseOneToken(uro, tks, unit);
      return unit;
   }

   if (tks.containsFilterKeyword()) {
      return nullptr;
   }

   const _boo possibleBinary = tks.containsSymbol(PG_CHAR_QUESTION_MARK);

   if (tks.isPossibleFunction()) {
      Generator<_boo>* func = boolFunction(tks, uro);
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

   if (tks.containsKeyword(PG_KEYWORD_IN)) {
      Generator<_boo>* boo = parseIn(tks, uro);
      if (boo != nullptr) {
         return boo;
      }
   }

   if (tks.containsKeyword(PG_KEYWORD_LIKE)) {
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
static Generator<_boo>* parseBoolExp(const Tokens& tks, Uroboros* uro)
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
                     const Tokens tks2(tks.list, i - sublen, sublen);
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
                           deleteVector(infList);
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
      Tokens tks2(tks.list, 1 + end - sublen, sublen);

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
            deleteVector(infList);
            return nullptr;
         }
      }
   }

   if (!isBoolExpComputable(infList)) {
      deleteVector(infList);
      throw SyntaxException(L"syntax of a boolean expression is not valid",
         tks.first().line);
   }

   std::vector<ExpElement<_boo>*> pntList;
   Generator<_boo>* boo = boolExpTree(infList, pntList);

   deleteVector(infList);
   deleteVector(pntList);

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
               Generator<_boo>* no = new Not(n);
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
            BinaryOperation<_boo>* bin;
            Generator<_boo>* first = firstElement->takeValue();
            Generator<_boo>* second = secondElement->takeValue();

            switch(op) {
               case L'&': {
                  bin = new And(first, second);
                  break;
               }
               case L'|': {
                  bin = new Or(first, second);
                  break;
               }
               case L'^': {
                  bin = new Xor(first, second);
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
static Generator<_boo>* parseIn_Unit(const _boo& negated, const Tokens& left,
   const Tokens& right, Uroboros* uro)
{
   Generator<T>* valLeft;
   if (!parse(uro, left, valLeft)) {
      return nullptr;
   }

   // check if the right side is a single value
   // in this special case, the whole structure is reduced to
   // a simple comparison "left == right" or "left != right"
   Generator<T>* num2;
   if (parse(uro, right, num2)) {
      if (negated) {
         return new NotEquals<T>(valLeft, num2);
      }
      else {
         return new Equals<T>(valLeft, num2);
      }
   }

   Generator<std::vector<T>>* valRight;

   if (parse(uro, right, valRight)) {
      if (valRight->isConstant()) {
         const std::vector<T> vs = valRight->getValue();
         delete valRight;
         Generator<_boo>* in = new InConstList<T>(valLeft, vs);
         return negated ? new Not(in) : in;
      }
      else {
         Generator<_boo>* in = new InList<T>(valLeft, valRight);
         return negated ? new Not(in) : in;
      }
   }
   else {
      delete valLeft;
      return nullptr;
   }
}

static Generator<_boo>* parseIn(const Tokens& tks, Uroboros* uro)
{
   Tokens left(tks);
   Tokens right(tks);
   tks.divideByKeyword(Keyword::kw_In, left, right);

   if (left.isEmpty()) {
      emptyOperSideException(tks.first(), true);
   }
   if (right.isEmpty()) {
      emptyOperSideException(tks.last(), false);
   }

   _boo neg = left.last().isKeyword(Keyword::kw_Not);
   if (neg) {
      left.trimRight();
      if (left.isEmpty()) {
         emptyOperSideException(tks.first(), true);
      }
   }

   // first: try to build "Number IN NumList"
   Generator<_boo>* list = parseIn_Unit<_num>(neg, left, right, uro);
   if (list != nullptr) {
      return list;
   }

   const Token& lf = left.first();

   if (left.getLength() == 1 && lf.type == Token::t_Word &&
      uro->hashes.HASH_GROUP_TIME_ATTR.find(lf.value.word.h) != uro->hashes.HASH_GROUP_TIME_ATTR.end())
   {
      if (right.containsSymbol(PG_CHAR_COMMA)) {
         std::vector<Tokens> elements;
         right.splitBySymbol(L',', elements);
         const _size elen = elements.size();
         for (_size i = 0; i < elen; i++) {
            const Tokens& t = elements[i];
            if (t.getLength() == 1) {
               const Token& tf = t.first();
               if (tf.isWeekDay()) {
                  timeInNumberException(left.first(), tf, L"weekDay", neg, left);
               }
               else if (tf.isMonth()) {
                  timeInNumberException(left.first(), tf, L"month", neg, left);
               }

               const _boo isInteger = (tf.type == Token::t_Number) && !tf.value.num.n.isDouble;
               if (isInteger) {
                  timeInNumberException(left.first(), tf, L"year", neg, left);
               }
            }
         }
      }
      else if (right.getLength() == 1) {
         const Token& rf = right.first();
         const _boo isWeek = rf.isWeekDay();
         const _boo isMonth = rf.isMonth();

         if (isWeek || isMonth) {
            timeInNumberException(left.first(), right.first(), isWeek ? L"weekDay" : L"month", neg, left);
         }

         const _boo isInteger = (rf.type == Token::t_Number) && !rf.value.num.n.isDouble;
         if (isInteger) {
            timeInNumberException(left.first(), right.first(), L"year", neg, left);
         }
      }
   }

   // secondary: try to build "Time IN TimList"
   Generator<_boo>* list2 = parseInTimList(neg, left, right, uro);
   if (list2 != nullptr) {
      return list2;
   }

   // finally: try to build "string IN list"
   Generator<_boo>* list3 = parseIn_Unit<_str>(neg, left, right, uro);
   return list3;
}


static Generator<_boo>* parseInTimList(const bool& negated, const Tokens& left,
   const Tokens& right, Uroboros* uro)
{
   Generator<_tim>* tim;
   if (!parse(uro, left, tim)) {
      return nullptr;
   }

   Generator<_tim>* tim2;
   if (parse(uro, right, tim2)) {
      if (negated) {
         return new NotEquals<_tim>(tim, tim2);
      }
      else {
         return new Equals<_tim>(tim, tim2);
      }
   }

   Generator<_tlist>* tlist;
   if (parse(uro, right, tlist)) {
      if (tlist->isConstant()) {
         const _tlist vs = tlist->getValue();
         delete tlist;
         Generator<_boo>* in = new InConstTimeList(tim, vs);
         return negated ? new Not(in) : in;
      }
      else {
         Generator<_boo>* in = new InTimeList(tim, tlist);
         return negated ? new Not(in) : in;
      }
   }
   else {
      delete tim;
      return nullptr;
   }
}

static void emptyOperSideException(const Token& oper, const bool& isLeft)
{
   const _str side = isLeft ? L"left" : L"right";

   throw SyntaxException(str(side, L" side of operator '", *oper.value.keyword.os, L"' is empty"),
      oper.line);
}

static void timeInNumberException(const Token& timeVar, const Token& numVar,
   const _str& timeMember, const _boo& negated, const Tokens& tks)
{
   if (timeMember == L"year") {
      if (negated) {
         throw SyntaxException(str(L"instead of '", *timeVar.value.word.os, L" not in ", toStr(numVar.value.num.n.value.i),
            L"', write '", *timeVar.value.word.os, L".year != ",
            toStr(numVar.value.num.n.value.i), L"'"), tks.first().line);
      }
      else {
         throw SyntaxException(str(L"instead of '", *timeVar.value.word.os, L" in ", toStr(numVar.value.num.n.value.i),
            L"', write '", *timeVar.value.word.os, L".year = ",
            toStr(numVar.value.num.n.value.i), L"'"), tks.first().line);
      }
   }
   else {
      if (negated) {
         throw SyntaxException(str(L"instead of '", *timeVar.value.word.os, L" not in ", *numVar.value.word.os,
            L"', write '", *timeVar.value.word.os, L".", timeMember,
            L" != ", *numVar.value.word.os, L"'"), tks.first().line);
      }
      else {
         throw SyntaxException(str(L"instead of '", *timeVar.value.word.os, L" in ", *numVar.value.word.os,
            L"', write '", *timeVar.value.word.os, L".", timeMember,
            L" = ", *numVar.value.word.os, L"'"), tks.first().line);
      }
   }
}

static Generator<_boo>* parseLike(const Tokens& tks, Uroboros* uro)
{
   Tokens left(tks);
   Tokens right(tks);
   tks.divideByKeyword(Keyword::kw_Like, left, right);

   if (left.isEmpty()) {
      emptyOperSideException(tks.first(), true);
   }
   if (right.isEmpty()) {
      emptyOperSideException(tks.last(), false);
   }

   _boo neg = left.last().isKeyword(Keyword::kw_Not);
   if (neg) {
      left.trimRight();
      if (left.isEmpty()) {
         emptyOperSideException(tks.first(), true);
      }
   }

   Generator<_str>* value;
   if (!parse(uro, left, value)) {
      return nullptr;
   }

   Generator<_str>* pattern;
   if (parse(uro, right, pattern)) {
      if (pattern->isConstant()) {
         const _str cnst = pattern->getValue();
         delete pattern;

         if (!isLikePatternCorrect(cnst)) {
            throw SyntaxException(str(L"pattern '", cnst,
               L"' is not valid for the 'like' operator"), right.first().line);
         }

         if (neg) {
            return new Not(new LikeConst(value, cnst));
         }
         else {
            return new LikeConst(value, cnst);
         }
      }

      if (neg) {
         return new Not(new Like(value, pattern));
      }
      else {
         return new Like(value, pattern);
      }
   }
   else {
      delete value;
      return nullptr;
   }
}

static Generator<_boo>* parseComparisons(const Tokens& tks, Uroboros* uro)
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
   Generator<T>* val2, const CompType& ct)
{
   switch (ct) {
      case ct_Equals:
         return new Equals<T>(val1, val2);
      case ct_NotEquals:
         return new NotEquals<T>(val1, val2);
      case ct_Smaller:
         return new Smaller<T>(val1, val2);
      case ct_SmallerEquals:
         return new SmallerEquals<T>(val1, val2);
      case ct_Bigger:
         return new Bigger<T>(val1, val2);
      case ct_BiggerEquals:
         return new BiggerEquals<T>(val1, val2);
      default:
         return nullptr;
   };
}


template <typename T>
Generator<_boo>* parseComparisonUnit(const Tokens& left, const Tokens& right, const CompType& ct, Uroboros* uro)
{
   Generator<T>* v1;
   Generator<T>* v2;
   const _boo parsed1 = parse(uro, left, v1);
   const _boo parsed2 = parse(uro, right, v2);
   if (parsed1 && parsed2) {
      switch (ct) {
         case ct_Equals:
            return new Equals<T>(v1, v2);
         case ct_NotEquals:
            return new NotEquals<T>(v1, v2);
         case ct_Smaller:
            return new Smaller<T>(v1, v2);
         case ct_SmallerEquals:
            return new SmallerEquals<T>(v1, v2);
         case ct_Bigger:
            return new Bigger<T>(v1, v2);
         case ct_BiggerEquals:
            return new BiggerEquals<T>(v1, v2);
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

static Generator<_boo>* parseComparison(const Tokens& tks, const _char& sign, Uroboros* uro)
{
   Tokens left(tks);
   Tokens right(tks);
   CompType ct = prepareComparison(tks, sign, left, right);

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
         throw SyntaxException(str(L"instead of '", *t1.value.word.os, L" ", s, L" ", *t2.value.num.os,
            L"', write '", *t1.value.word.os, L".", (isWeek2 ? L"weekDay" : L"month"),
            L" ", s, L" ", *t2.value.num.os, L"'"), tks.first().line);
      }
      else if ((isWeek1 || isMonth1) && isVar2) {
         throw SyntaxException(str(L"instead of '", *t1.value.num.os, L" ", s, L" ", *t2.value.word.os,
            L"', write '", *t1.value.num.os, L" ", s, L" ", *t2.value.word.os, L".",
            (isWeek1 ? L"weekDay" : L"month"), L"'"), tks.first().line);
      }

      const _boo isInteger1 = (t1.type == Token::t_Number) && !t1.value.num.n.isDouble;
      const _boo isInteger2 = (t2.type == Token::t_Number) && !t2.value.num.n.isDouble;

      if (isVar1 && isInteger2) {
         const _nint& nm = t2.value.num.n.value.i;
         if (nm >= 1950LL && nm <= 2100LL) {
            throw SyntaxException(str(L"instead of '", *t1.value.word.os, L" ", s, L" ", toStr(nm),
               L"', write '", *t1.value.word.os, L".year ", s, L" ", toStr(nm), L"'"), tks.first().line);
         }
         else {
            throw SyntaxException(str(L"time variable '", *t1.value.word.os,
               L"' cannot be compared with a number"), tks.first().line);
         }
      }
      else if (isInteger1 && isVar2) {
         const _nint& nm = t1.value.num.n.value.i;
         if (nm >= 1950LL && nm <= 2100) {
            throw SyntaxException(str(L"instead of '", toStr(nm), L" ", s, L" ", *t2.value.word.os,
               L"', write '", toStr(nm), L" ", s, L" ", *t2.value.word.os, L".year'"), tks.first().line);
         }
         else {
            throw SyntaxException(str(L"time variable '", *t2.value.word.os,
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

Generator<_boo>* comparisonDefList( _def* def, Generator<_list>* list, const CompType& ct,
   const _boo& reversed, Uroboros* uro)
{
   switch (ct) {
      case ct_Equals:
         return new DefinitionListEqual(def, list, uro);
      case ct_NotEquals:
         return new DefinitionListNotEqual(def, list, uro);
      default: {
         if (reversed) {
            switch(ct) {
               case ct_Smaller:
                  return new DefinitionListBigger(def, list, uro);
               case ct_SmallerEquals:
                  return new DefinitionListBiggerEquals(def, list, uro);
               case ct_Bigger:
                  return new DefinitionListSmaller(def, list, uro);
               case ct_BiggerEquals:
                  return new DefinitionListSmallerEquals(def, list, uro);
            }
         }
         else {
            switch(ct) {
               case ct_Smaller:
                  return new DefinitionListSmaller(def, list, uro);
               case ct_SmallerEquals:
                  return new DefinitionListSmallerEquals(def, list, uro);
               case ct_Bigger:
                  return new DefinitionListBigger(def, list, uro);
               case ct_BiggerEquals:
                  return new DefinitionListBiggerEquals(def, list, uro);
            }
         }
      }
   }

   return nullptr;
}


template <typename T>
Generator<_boo>* comparisonCollections(const Tokens& left, const Tokens& right, const CompType& ct, Uroboros* uro)
{
   Generator<std::vector<T>>* leftValue;
   if (parse(uro, left, leftValue)) {

      Generator<std::vector<T>>* rightValue;
      if (parse(uro, right, rightValue)) {
         switch(ct) {
            case ct_Equals:
               return new CollectionsEqual<T>(leftValue, rightValue);
            case ct_NotEquals:
               return new CollectionsNotEqual<T>(leftValue, rightValue);
            case ct_Smaller:
               return new CollectionsSmaller<T>(leftValue, rightValue);
            case ct_SmallerEquals:
               return new CollectionsSmallerEquals<T>(leftValue, rightValue);
            case ct_Bigger:
               return new CollectionsBigger<T>(leftValue, rightValue);
            case ct_BiggerEquals:
               return new CollectionsBiggerEquals<T>(leftValue, rightValue);
         }
      }
   }

   return nullptr;
}


template <typename T>
Generator<_boo>* comparisonCollectionValue(const Tokens& left, const Tokens& right,
   const CompType& ct, Uroboros* uro)
{
   Generator<T>* leftValue;
   if (parse(uro, left, leftValue)) {
      Generator<std::vector<T>>* rightCollection;

      if (parse(uro, right, rightCollection)) {
         switch(ct) {
            case ct_Equals:
               return new CollectionValueEquals<T>(rightCollection, leftValue);
            case ct_NotEquals:
               return new CollectionValueNotEquals<T>(rightCollection, leftValue);
            case ct_Smaller:
               return new CollectionValueBigger<T>(rightCollection);
            case ct_SmallerEquals:
               return new CollectionValueBiggerEquals<T>(rightCollection);
            case ct_Bigger:
               return new CollectionValueSmaller<T>(rightCollection);
            case ct_BiggerEquals:
               return new CollectionValueSmallerEquals<T>(rightCollection);
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
            case ct_Equals:
               return new CollectionValueEquals<T>(leftCollection, rightValue);
            case ct_NotEquals:
               return new CollectionValueNotEquals<T>(leftCollection, rightValue);
            case ct_Smaller:
               return new CollectionValueSmaller<T>(leftCollection);
            case ct_SmallerEquals:
               return new CollectionValueSmallerEquals<T>(leftCollection);
            case ct_Bigger:
               return new CollectionValueBigger<T>(leftCollection);
            case ct_BiggerEquals:
               return new CollectionValueBiggerEquals<T>(leftCollection);
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
   const Tokens& right, const CompType& ct, Uroboros* uro)
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
            case ct_Equals:
               return new DefinitionsEqual(leftDef, rightDef, uro);
            case ct_NotEquals:
               return new DefinitionsNotEqual(leftDef, rightDef, uro);
            case ct_Smaller:
               return new DefinitionsSmaller(leftDef, rightDef, uro);
            case ct_SmallerEquals:
               return new DefinitionsSmallerEquals(leftDef, rightDef, uro);
            case ct_Bigger:
               return new DefinitionsBigger(leftDef, rightDef, uro);
            case ct_BiggerEquals:
               return new DefinitionsBiggerEquals(leftDef, rightDef, uro);
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

static CompType prepareComparison(const Tokens& tks, const _char& sign,
   Tokens& left, Tokens& right)
{
   tks.divideBySymbol(sign, left, right);
   _boo eq = false;

   if (left.isEmpty()) {
      if (right.isEmpty()) {
         throw SyntaxException(str(L"both sides of ", charStr(sign),
            L" comparison are empty"), tks.first().line);
      }
      else {
         throw SyntaxException(str(L"left side of ", charStr(sign),
            L" comparison is empty"), tks.first().line);
      }
   }

   if (right.isEmpty()) {
      throw SyntaxException(str(L"right side of ", charStr(sign),
         L" comparison is empty"), tks.last().line);
   }

   if (right.first().isSymbol(L'=')) {
      if (right.getLength() == 1) {
         throw SyntaxException(str(L"right side of ", charStr(sign),
            L"= comparison is empty"), right.first().line);
      }

      right.trimLeft();
      eq = true;
   }
   else if (sign == L'!') {
      throw SyntaxException(L"expected = after exclamation mark. "
         L"For a simple negation, use keyword 'not' instead",
         right.first().line);
   }

   switch (sign) {
      case L'<':
         return eq ? CompType::ct_SmallerEquals : CompType::ct_Smaller;
      case L'>':
         return eq ? CompType::ct_BiggerEquals : CompType::ct_Bigger;
      case L'=':
         return CompType::ct_Equals;
      case L'!': {
         return CompType::ct_NotEquals;
      default:
         return CompType::ct_Equals;
      }
   }
}

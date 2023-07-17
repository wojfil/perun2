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

#include "parse-bool.h"
#include "parse-list.h"
#include "parse-generic.h"
#include "../parse/parse-function.h"
#include "../generator/gen-bool.h"
#include "../../brackets.h"
#include "../../lexer.h"
#include "../generator/gen-like.h"
#include "../parse-gen.h"


namespace perun2::parse
{

p_bool parseBool(_genptr<p_bool>& result, const Tokens& tks, pp_perun2& p2)
{
   const p_size len = tks.getLength();

   if (len == 1) {
      return parseOneToken(p2, tks, result);
   }

   if (tks.check(TI_HAS_FILTER_KEYWORD)) {
      return false;
   }

   const p_bool possibleBinary = tks.check(TI_HAS_CHAR_QUESTION_MARK);

   if (tks.check(TI_IS_POSSIBLE_FUNCTION)) {
      return func::boolFunction(result, tks, p2);
   }
   else if (len >= 2 && !possibleBinary) {
      // build numeric expression (but only if sequence has any operator)
      BracketsInfo bi;
      const p_int end = tks.getEnd();
      const p_int start = tks.getStart();

      for (p_int i = start; i <= end; i++) {
         const Token& t = tks.listAt(i);
         if (t.type == Token::t_Keyword && isBoolExpOperator(t) && bi.isBracketFree())
         {
            if (!(t.isKeyword(Keyword::kw_Not) && i != end && tks.listAt(i + 1).isNegatableKeywordOperator()))
            {
               if (!parseBoolExp(result, tks, p2)) {
                  throw SyntaxError::syntaxOfBooleanExpressionNotValid(tks.first().line);
               }
               return true;
            }
         }
         bi.refresh(t);
      }
   }

   if (parseComparisons(result, tks, p2)) {
      return true;
   }

   if (tks.check(TI_HAS_KEYWORD_IN)) {
      if (parseIn(result, tks, p2)) {
         return true;
      }
   }

   if (tks.check(TI_HAS_KEYWORD_LIKE)) {
      if (parseLike(result, tks, p2)) {
         return true;
      }
   }

   return parseTernary<p_bool>(result, tks, p2);
}


// build boolean expression
// multiple logic statements
// connected with keywords not, and, or, xor and brackets ()
static p_bool parseBoolExp(_genptr<p_bool>& result, const Tokens& tks, pp_perun2& p2)
{
   std::vector<ExpElement<p_bool>> infList; // infix notation list
   const p_int start = tks.getStart();
   const p_int end = tks.getEnd();
   p_int sublen = 0;
   BracketsInfo bi;

   for (p_int i = start; i <= end; i++) {
      const Token& t = tks.listAt(i);
      if (t.type == Token::t_Keyword) {
         if (isBoolExpOperator(t)) {
            if (t.isKeyword(Keyword::kw_Not) && i != end && tks.listAt(i + 1).isNegatableKeywordOperator())
            {
               sublen++;
            }
            else {
               const p_char ch = toBoolExpOperator(t);
               if (sublen == 0) {
                  infList.emplace_back(ch, t.line);
               }
               else {
                  if (bi.isBracketFree()) {
                     const Tokens tks2(tks, i - sublen, sublen);
                     const p_int line = tks2.first().line;

                     if (tks2.getLength() == 1 && tks2.first().isLogicConstant()) {
                        const p_bool boo = tks2.first().value.keyword.k == Keyword::kw_True;
                        infList.emplace_back(boo, line);
                        infList.emplace_back(ch, line);
                        sublen = 0;
                     }
                     else {
                        _genptr<p_bool> boo;
                        if (parse(p2, tks2, boo)) {
                           infList.emplace_back(boo, line);
                           infList.emplace_back(ch, line);
                           sublen = 0;
                        }
                        else {
                           return false;
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
      else {
         bi.refresh(t);
         sublen++;
      }
   }

   if (sublen != 0) {
      const Tokens tks2(tks, 1 + end - sublen, sublen);

      if (tks2.getLength() == 1 && tks2.first().isLogicConstant()) {
         const p_bool boo = (tks2.first().value.keyword.k == Keyword::kw_True);
         infList.emplace_back(boo, tks2.first().line);
      }
      else {
         _genptr<p_bool> boo;
         if (parse(p2, tks2, boo)) {
            infList.emplace_back(boo, tks2.first().line);
         }
         else {
            return false;
         }
      }
   }

   if (!isBoolExpComputable(infList)) {
      throw SyntaxError::syntaxOfBooleanExpressionNotValid(tks.first().line);
   }

   return boolExpTree(result, infList);
}

static p_bool boolExpTree(_genptr<p_bool>& result, std::vector<ExpElement<p_bool>>& infList)
{
   std::vector<ExpElement<p_bool>> elements;
   std::vector<ExpElement<p_bool>> temp;
   const p_size len = infList.size();
   p_int brackets = 0;
   p_bool anyNot = false;

   for (p_size i = 0; i < len; i++) {
      ExpElement<p_bool>& e = infList[i];
      if (e.type == ElementType::et_Operator) {
         const p_char op = e.operator_;
         switch (op) {
            case CHAR_OPENING_ROUND_BRACKET: {
               brackets++;
               if (brackets > 1) {
                  temp.emplace_back(e);
               }
               break;
            }
            case CHAR_CLOSING_ROUND_BRACKET: {
               brackets--;
               if (brackets == 0) {
                  _genptr<p_bool> res;

                  if (!boolExpTree(res, temp)) {
                     return false;
                  }

                  temp.clear();
                  elements.emplace_back(res, e.line);
               }
               else {
                  temp.emplace_back(e);
               }
               break;
            }
            default: {
               if (brackets == 0) {
                  elements.emplace_back(e);
               }
               else {
                  temp.emplace_back(e);
               }
               if (!anyNot && op == CHAR_EXCLAMATION_MARK) {
                  anyNot = true;
               }
               break;
            }
         }
      }
      else
      {
         if (brackets == 0) {
            elements.emplace_back(e);
         }
         else {
            temp.emplace_back(e);
         }
      }
   }

   return anyNot
      ? boolExpIntegrateNegations(result, elements)
      : boolExpTreeMerge(result, elements);
}

static p_bool boolExpIntegrateNegations(_genptr<p_bool>& result,
   std::vector<ExpElement<p_bool>>& elements)
{
   std::vector<ExpElement<p_bool>> newList;
   p_bool negate = false;
   const p_size len = elements.size();

   for (p_size i = 0; i < len; i++) {
      ExpElement<p_bool>& e = elements[i];
      if (e.type == ElementType::et_Operator) {
         if (e.operator_ == CHAR_EXCLAMATION_MARK) {
            negate = true;
         }
         else {
            newList.emplace_back(e);
         }
      }
      else {
         if (negate) {
            if (e.type == ElementType::et_Constant) {
               const p_bool value = !(e.constant);
               newList.emplace_back(value, e.line);
            }
            else {
               _genptr<p_bool> n = std::move(e.generator);
               _genptr<p_bool> no = std::make_unique<gen::Not>(n);
               newList.emplace_back(no, e.line);
            }

            negate = false;
         }
         else {
            newList.emplace_back(e);
         }
      }
   }

   return boolExpTreeMerge(result, newList);
}

static p_bool boolExpTreeMerge(_genptr<p_bool>& result,
   std::vector<ExpElement<p_bool>>& elements)
{
   const p_size len = elements.size();
   if (len == 1) {
      result = std::move(elements[0].generator);
      return true;
   }

   _genptr<p_bool> first = std::move(elements[0].generator);
   p_bool firstIsConstant = first->isConstant();
   p_char op;

   for (p_size i = 1; i < len; i++) {
      ExpElement<p_bool>& e = elements[i];

      if (e.type == ElementType::et_Operator) {
         op = e.operator_;
         continue;
      }

      _genptr<p_bool> second(std::move(e.generator));

      if (firstIsConstant && e.type == ElementType::et_Constant) {
         p_bool value;

         switch(op) {
            case CHAR_AMPERSAND: {
               value = first->getValue() && second->getValue();
               break;
            }
            case CHAR_VERTICAL_BAR: {
               value = first->getValue() || second->getValue();;
               break;
            }
            case CHAR_CARET: {
               value = first->getValue() ^ second->getValue();;
               break;
            }
         }

         first = std::make_unique<gen::Constant<p_bool>>(value);
         continue;
      }

      _genptr<p_bool> prev = std::move(first);

      switch (op) {
         case CHAR_AMPERSAND: {
            first = std::make_unique<gen::And>(prev, second);
            break;
         }
         case CHAR_VERTICAL_BAR: {
            first = std::make_unique<gen::Or>(prev, second);
            break;
         }
         case CHAR_CARET: {
            first = std::make_unique<gen::Xor>(prev, second);
            break;
         }
      }

      firstIsConstant = false;
   }

   result = std::move(first);
   return true;
}

static p_bool isBoolExpComputable(const std::vector<ExpElement<p_bool>>& infList)
{
   const p_size len = infList.size();
   if (len == 0) {
      return false;
   }

   // bool expressions can start with only two symbols: not or (
   const ExpElement<p_bool>& first = infList[0];
   if (first.type == ElementType::et_Operator) {
      if (!(first.operator_ == CHAR_OPENING_ROUND_BRACKET || first.operator_ == CHAR_EXCLAMATION_MARK)) {
         return false;
      }
   }

   // bool expressions can end with only one symbol: )
   const ExpElement<p_bool>& last = infList[len - 1];
   if (last.type == ElementType::et_Operator) {
      if (last.operator_ != CHAR_CLOSING_ROUND_BRACKET) {
         return false;
      }
   }

   for (p_size i = 1; i < len; i++) {
      const ExpElement<p_bool>& prev = infList[i - 1];
      const ExpElement<p_bool>& curr = infList[i];
      const p_bool cop = curr.type == ElementType::et_Operator;

      if (prev.type == ElementType::et_Operator) {
         switch (prev.operator_) {
            case CHAR_EXCLAMATION_MARK: {
               if (cop && curr.operator_ != CHAR_OPENING_ROUND_BRACKET) {
                  return false;
               }
               break;
            }
            case CHAR_OPENING_ROUND_BRACKET: {
               if (cop) {
                  if (!(curr.operator_ == CHAR_OPENING_ROUND_BRACKET || curr.operator_ == CHAR_EXCLAMATION_MARK)) {
                     return false;
                  }
               }
               break;
            }
            case CHAR_CLOSING_ROUND_BRACKET: {
               if (cop) {
                  if (curr.operator_ == CHAR_OPENING_ROUND_BRACKET || curr.operator_ == CHAR_EXCLAMATION_MARK) {
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
                  if (!(curr.operator_ == CHAR_OPENING_ROUND_BRACKET || curr.operator_ == CHAR_EXCLAMATION_MARK)) {
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
            if (curr.operator_ == CHAR_EXCLAMATION_MARK || curr.operator_ == CHAR_OPENING_ROUND_BRACKET) {
               return false;
            }
         }
         else {
            return false;
         }
      }
   }

   return true;
}

p_bool isBoolExpOperator(const Token& tk)
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

static p_char toBoolExpOperator(const Token& tk)
{
   switch (tk.value.keyword.k) {
      case Keyword::kw_And:
         return CHAR_AMPERSAND;
      case Keyword::kw_Or:
         return CHAR_VERTICAL_BAR;
      case Keyword::kw_Xor:
         return CHAR_CARET;
      case Keyword::kw_Not:
         return CHAR_EXCLAMATION_MARK;
      default:
         return CHAR_SPACE;
   }
}

template <typename T>
static p_bool parseIn_Unit(_genptr<p_bool>& result, const p_bool negated,
   const std::pair<Tokens, Tokens>& pair, pp_perun2& p2)
{
   _genptr<T> valLeft;
   if (!parse(p2, pair.first, valLeft)) {
      return false;
   }

   // check if the right side is a single value
   // in this special case, the whole structure is reduced to
   // a simple comparison "left == right" or "left != right"
   _genptr<T> num2;
   if (parse(p2, pair.second, num2)) {
      if (negated) {
         result = std::make_unique<gen::NotEquals<T>>(valLeft, num2);
      }
      else {
         result = std::make_unique<gen::Equals<T>>(valLeft, num2);
      }
      return true;
   }

   _genptr<std::vector<T>> valRight;

   if (parse(p2, pair.second, valRight)) {
      if (valRight->isConstant()) {
         const std::vector<T> vs = valRight->getValue();
         _genptr<p_bool> in = std::make_unique<gen::InConstList<T>>(valLeft, vs);
         result = negated
            ? std::make_unique<gen::Not>(in)
            : std::move(in);
      }
      else {
         _genptr<p_bool> in = std::make_unique<gen::InList<T>>(valLeft, valRight);
         result = negated
            ? std::make_unique<gen::Not>(in)
            : std::move(in);
      }

      return true;
   }
   else {
      return false;
   }
}

static void leftInTimeException(const Token& tk, const p_str& varMember,
   const std::pair<Tokens, Tokens>& pair, const p_bool negated, pp_perun2& p2)
{
   const Token& first = pair.first.first();

   if (negated) {
      const p_str v1 = str(first.getOriginString(p2), L" not in ", tk.getOriginString(p2));
      const p_str v2 = str(first.getOriginString(p2),
         CHAR_DOT, varMember, L" != ", tk.getOriginString(p2));
      throw SyntaxError::insteadOfYouShouldWrite(v1, v2, first.line);
   }
   else {
      const p_str v1 = str(first.getOriginString(p2), L" in ", tk.getOriginString(p2));
      const p_str v2 = str(first.getOriginString(p2),
         CHAR_DOT, varMember, L" = ", tk.getOriginString(p2));
      throw SyntaxError::insteadOfYouShouldWrite(v1, v2, first.line);
   }
}

static void rightInTimeException(const Token& tk, const p_str& varMember,
   const std::pair<Tokens, Tokens>& pair, const p_bool negated, pp_perun2& p2)
{
   const Token& first = pair.second.first();

   if (negated) {
      const p_str v1 = str(tk.getOriginString(p2) , L" not in ", first.getOriginString(p2));
      const p_str v2 = str(tk.getOriginString(p2), L" != ", first.getOriginString(p2),
         CHAR_DOT, varMember);
      throw SyntaxError::insteadOfYouShouldWrite(v1, v2, tk.line);
   }
   else {
      const p_str v1 = str(tk.getOriginString(p2), L" in ", first.getOriginString(p2));
      const p_str v2 = str(tk.getOriginString(p2), L" = ", first.getOriginString(p2),
         CHAR_DOT, varMember);
      throw SyntaxError::insteadOfYouShouldWrite(v1, v2, tk.line);
   }
}

static void checkCommonExceptions_InTime(const std::pair<Tokens, Tokens>& pair, const p_bool negated, pp_perun2& p2)
{
   const Token& f1 = pair.first.first();
   const Token& f2 = pair.second.first();

   const p_bool leftTimeVar = pair.first.getLength() == 1 && f1.isWord(STRINGS_TIME_VAR, p2);
   const p_bool rightTimeVar = pair.second.getLength() == 1 && f2.isWord(STRINGS_TIME_VAR, p2);

   // check cases when left side is a time variable (creation, modification...)
   if (leftTimeVar)
   {
      if (pair.second.check(TI_HAS_CHAR_COMMA)) {
         const std::vector<Tokens> elements = pair.second.splitBySymbol(CHAR_COMMA);
         const p_size elen = elements.size();
         for (p_size i = 0; i < elen; i++) {
            const Tokens& t = elements[i];
            if (t.getLength() == 1) {
               const Token& tf = t.first();
               if (tf.isWeekDay()) {
                  leftInTimeException(tf, STRING_WEEKDAY_CAMELCASE, pair, negated, p2);
               }
               else if (tf.isMonth()) {
                  leftInTimeException(tf, STRING_MONTH, pair, negated, p2);
               }
               else if (tf.type == Token::t_Number && !tf.value.num.n.isDouble) {
                  leftInTimeException(tf, STRING_YEAR, pair, negated, p2);
               }
            }
         }
      }
      else if (pair.second.getLength() == 1) {
         const Token& rf = pair.second.first();

         if (rf.isWeekDay()) {
            leftInTimeException(rf, STRING_WEEKDAY_CAMELCASE, pair, negated, p2);
         }
         else if (rf.isMonth()) {
            leftInTimeException(rf, STRING_MONTH, pair, negated, p2);
         }
         else if (rf.type == Token::t_Number && !rf.value.num.n.isDouble) {
            leftInTimeException(rf, STRING_YEAR, pair, negated, p2);
         }
      }
   }

   // check cases when right side is a time attribute (creation, modification...)
   if (rightTimeVar)
   {
      if (pair.first.check(TI_HAS_CHAR_COMMA)) {
         const std::vector<Tokens> elements = pair.first.splitBySymbol(CHAR_COMMA);
         const p_size elen = elements.size();
         for (p_size i = 0; i < elen; i++) {
            const Tokens& t = elements[i];
            if (t.getLength() == 1) {
               const Token& tf = t.first();
               if (tf.isWeekDay()) {
                  rightInTimeException(tf, STRING_WEEKDAY_CAMELCASE, pair, negated, p2);
               }
               else if (tf.isMonth()) {
                  rightInTimeException(tf, STRING_MONTH, pair, negated, p2);
               }
               else if (tf.type == Token::t_Number && !tf.value.num.n.isDouble) {
                  rightInTimeException(tf, STRING_YEAR, pair, negated, p2);
               }
            }
         }
      }
      else if (pair.first.getLength() == 1) {
         const Token& rf = pair.first.first();

         if (rf.isWeekDay()) {
            rightInTimeException(rf, STRING_WEEKDAY_CAMELCASE, pair, negated, p2);
         }
         else if (rf.isMonth()) {
            rightInTimeException(rf, STRING_MONTH, pair, negated, p2);
         }
         else if (rf.type == Token::t_Number && !rf.value.num.n.isDouble) {
            rightInTimeException(rf, STRING_YEAR, pair, negated, p2);
         }
      }
   }
}


static p_bool parseIn(_genptr<p_bool>& result, const Tokens& tks, pp_perun2& p2)
{
   std::pair<Tokens, Tokens> pair = tks.divideByKeyword(Keyword::kw_In);

   if (pair.first.isEmpty()) {
      throw SyntaxError::leftSideOfOperatorIsEmpty(tks.first().getOriginString(p2), tks.first().line);
   }
   if (pair.second.isEmpty()) {
      throw SyntaxError::rightSideOfOperatorIsEmpty(tks.last().getOriginString(p2), tks.last().line);
   }

   p_bool neg = pair.first.last().isKeyword(Keyword::kw_Not);

   if (neg) {
      if (pair.first.getLength() == 1) {
         throw SyntaxError::leftSideOfOperatorIsEmpty(str(tks.first().getOriginString(p2), 
            CHAR_SPACE, tks.second().getOriginString(p2)), tks.first().line);
      }

      pair.first.trimRight();
   }
   
   // first: try to build "Number IN NumList"
   _genptr<p_bool> list;

   if (parseIn_Unit<p_num>(list, neg, pair, p2)) {
      result = std::move(list);
      return true;
   }

   checkCommonExceptions_InTime(pair, neg, p2);

   // secondary: try to build "Time IN TimList"
   _genptr<p_bool> list2;
   if (parseInTimList(list2, neg, pair, p2)) {
      result = std::move(list2);
      return true;
   }

   // finally: try to build "string IN list"
   return parseIn_Unit<p_str>(result, neg, pair, p2);;
}


static p_bool parseInTimList(_genptr<p_bool>& result, const bool& negated,
   const std::pair<Tokens, Tokens>& pair, pp_perun2& p2)
{
   _genptr<p_tim> tim;
   if (!parse(p2, pair.first, tim)) {
      return false;
   }

   _genptr<p_tim> tim2;
   if (parse(p2, pair.second, tim2)) {
      if (negated) {
         result = std::make_unique<gen::NotEquals<p_tim>>(tim, tim2);
      }
      else {
         result = std::make_unique<gen::Equals<p_tim>>(tim, tim2);
      }

      return true;
   }

   _genptr<p_tlist> tlist;
   if (parse(p2, pair.second, tlist)) {
      if (tlist->isConstant()) {
         const p_tlist vs = tlist->getValue();
         _genptr<p_bool> in = std::make_unique<gen::InConstTimeList>(tim, vs);
         result = negated
            ? std::make_unique<gen::Not>(in)
            : std::move(in);
      }
      else {
         _genptr<p_bool> in = std::make_unique<gen::InList<p_tim>>(tim, tlist);
         result = negated
            ? std::make_unique<gen::Not>(in)
            : std::move(in);
      }

      return true;
   }
   else {
      return false;
   }
}

static p_bool parseLike(_genptr<p_bool>& result, const Tokens& tks, pp_perun2& p2)
{
   std::pair<Tokens, Tokens> pair = tks.divideByKeyword(Keyword::kw_Like);

   if (pair.first.isEmpty()) {
      throw SyntaxError::leftSideOfOperatorIsEmpty(tks.first().getOriginString(p2), tks.first().line);
   }
   if (pair.second.isEmpty()) {
      throw SyntaxError::rightSideOfOperatorIsEmpty(tks.last().getOriginString(p2), tks.last().line);
   }

   const p_bool neg = pair.first.last().isKeyword(Keyword::kw_Not);
   if (neg) {
      if (pair.first.getLength() == 1) {
         throw SyntaxError::leftSideOfOperatorIsEmpty(str(tks.first().getOriginString(p2), 
            CHAR_SPACE, tks.second().getOriginString(p2)), tks.first().line);
      }

      pair.first.trimRight();
   }

   _genptr<p_str> value;
   if (!parse(p2, pair.first, value)) {
      return false;
   }

   _genptr<p_str> pattern;
   if (parse(p2, pair.second, pattern)) {
      if (pattern->isConstant()) {
         const p_str cnst = pattern->getValue();

         if (neg) {
            _genptr<p_bool> b = std::make_unique<gen::LikeConst>(value, cnst);
            result = std::make_unique<gen::Not>(b);
         }
         else {
            result = std::make_unique<gen::LikeConst>(value, cnst);
         }

         return true;
      }

      if (neg) {
         _genptr<p_bool> b = std::make_unique<gen::Like>(value, pattern);
         result = std::make_unique<gen::Not>(b);
      }
      else {
         result = std::make_unique<gen::Like>(value, pattern);
      }

      return true;
   }
   else {
      return false;
   }
}

static p_bool parseComparisons(_genptr<p_bool>& result, const Tokens& tks, pp_perun2& p2)
{
   BracketsInfo bi;
   const p_int end = tks.getEnd();

   for (p_int i = tks.getStart(); i <= end; i++) {
      const Token& t = tks.listAt(i);
      if (t.type == Token::t_Symbol && bi.isBracketFree()) {
         switch (t.value.ch) {
            case CHAR_SMALLER:
            case CHAR_GREATER:
            case CHAR_EXCLAMATION_MARK:
            case CHAR_EQUAL_SIGN: {
               return parseComparison(result, tks, t.value.ch, p2);
            }
         }
      }
      bi.refresh(t);
   }

   return false;
}

template <typename T>
static p_bool comparison(_genptr<p_bool>& result, _genptr<T>& val1,
   _genptr<T>& val2, const gen::CompType& ct)
{
   switch (ct) {
      case gen::ct_Equals: {
         result = std::make_unique<gen::Equals<T>>(val1, val2);
         break;
      }
      case gen::ct_NotEquals: {
         result = std::make_unique<gen::NotEquals<T>>(val1, val2);
         break;
      }
      case gen::ct_Smaller: {
         result = std::make_unique<gen::Smaller<T>>(val1, val2);
         break;
      }
      case gen::ct_SmallerEquals: {
         result = std::make_unique<gen::SmallerEquals<T>>(val1, val2);
         break;
      }
      case gen::ct_Bigger: {
         result = std::make_unique<gen::Bigger<T>>(val1, val2);
         break;
      }
      case gen::ct_BiggerEquals: {
         result = std::make_unique<gen::BiggerEquals<T>>(val1, val2);
         break;
      }
      default:
         return false;
   };

   return true;
}


template <typename T>
p_bool parseComparisonUnit(_genptr<p_bool>& result, const Tokens& left,
   const Tokens& right, const gen::CompType& ct, pp_perun2& p2)
{
   _genptr<T> v1;
   _genptr<T> v2;
   if (parse(p2, left, v1) && parse(p2, right, v2)) {
      return comparison<T>(result, v1, v2, ct);
   }

   return false;
}


static void checkCommonExceptions_Comparison(const Tokens& left, const Tokens& right, const p_char sign, pp_perun2& p2)
{
   const Token& t1 = left.first();
   const Token& t2 = right.first();

   const p_bool isWeek1 = t1.isWeekDay();
   const p_bool isWeek2 = t2.isWeekDay();
   const p_bool isMonth1 = t1.isMonth();
   const p_bool isMonth2 = t2.isMonth();
   const p_bool isVar1 = t1.isWord(STRINGS_TIME_VAR, p2);
   const p_bool isVar2 = t2.isWord(STRINGS_TIME_VAR, p2);
   
   if (isVar1 && (isWeek2 || isMonth2)) {
      throw SyntaxError(str(L"instead of '", t1.getOriginString(p2), L" ", toStr(sign), L" ", t2.getOriginString(p2),
         L"', you should write '", t1.getOriginString(p2), L".", (isWeek2 ? STRING_WEEKDAY_CAMELCASE : STRING_MONTH),
         L" ", toStr(sign), L" ", t2.getOriginString(p2), L"'"), t1.line);
   }
   else if ((isWeek1 || isMonth1) && isVar2) {
      throw SyntaxError(str(L"instead of '", t1.getOriginString(p2), L" ", toStr(sign), L" ", t2.getOriginString(p2),
         L"', you should write '", t1.getOriginString(p2), L" ", toStr(sign), L" ", t2.getOriginString(p2), L".",
         (isWeek1 ? STRING_WEEKDAY_CAMELCASE : STRING_MONTH), L"'"), t1.line);
   }

   const p_bool isInteger1 = (t1.type == Token::t_Number) && !t1.value.num.n.isDouble;
   const p_bool isInteger2 = (t2.type == Token::t_Number) && !t2.value.num.n.isDouble;

   if (isVar1 && isInteger2) {
      const p_nint nm = t2.value.num.n.value.i;
      if (nm >= NINT_1950 && nm <= NINT_2100) {
         throw SyntaxError(str(L"instead of '", t1.getOriginString(p2), L" ", toStr(sign), L" ", toStr(nm),
            L"', you should write '", t1.getOriginString(p2), L".year ", toStr(sign), L" ", toStr(nm), L"'"), t1.line);
      }
      else {
         throw SyntaxError(str(L"time variable '", t1.getOriginString(p2),
            L"' cannot be compared with a number"), t1.line);
      }
   }
   else if (isInteger1 && isVar2) {
      const p_nint nm = t1.value.num.n.value.i;
      if (nm >= NINT_1950 && nm <= NINT_2100) {
         throw SyntaxError(str(L"instead of '", toStr(nm), L" ", toStr(sign), L" ", t2.getOriginString(p2),
            L"', you should write '", toStr(nm), L" ", toStr(sign), L" ", t2.getOriginString(p2), L".year'"), t1.line);
      }
      else {
         throw SyntaxError(str(L"time variable '", t2.getOriginString(p2),
            L"' cannot be compared with a number"), t1.line);
      }
   }
}


static p_bool parseComparison(_genptr<p_bool>& result, const Tokens& tks, const p_char sign, pp_perun2& p2)
{
   gen::CompType ct;
   const std::pair<Tokens, Tokens> pair = prepareComparison(tks, sign, ct);
   const Tokens& left = pair.first;
   const Tokens& right = pair.second;

   // look for some common errors
   // and throw precise messages to the user
   if (left.getLength() == 1 && right.getLength() == 1) {
      checkCommonExceptions_Comparison(left, right, sign, p2);
   }

   // try to parse comparison for every singular data type
   if (parseComparisonUnit<p_bool>(result, left, right, ct, p2)
    || parseComparisonUnit<p_num>(result, left, right, ct, p2)
    || parseComparisonUnit<p_per>(result, left, right, ct, p2)
    || parseComparisonUnit<p_tim>(result, left, right, ct, p2)
    || parseComparisonUnit<p_str>(result, left, right, ct, p2)) {
      return true;
   }

   // comparisons between singular values have failed, so try comparisons of collections
   return parseCollectionComparisons(result, left, right, ct, p2);
}

p_bool comparisonDefList(_genptr<p_bool>& result, p_defptr& def, _genptr<p_list>& list, const gen::CompType& ct,
   const p_bool reversed, pp_perun2& p2)
{
   switch (ct) {
      case gen::ct_Equals: {
         result = std::make_unique<gen::DefinitionListEqual>(def, list, p2);
         break;
      }
      case gen::ct_NotEquals: {
         result = std::make_unique<gen::DefinitionListNotEqual>(def, list, p2);
         break;
    }
      default: {
         if (reversed) {
            switch(ct) {
               case gen::ct_Smaller: {
                  result = std::make_unique<gen::DefinitionListBigger>(def, list, p2);
                  break;
               }
               case gen::ct_SmallerEquals: {
                  result = std::make_unique<gen::DefinitionListBiggerEquals>(def, list, p2);
                  break;
               }
               case gen::ct_Bigger: {
                  result = std::make_unique<gen::DefinitionListSmaller>(def, list, p2);
                  break;
               }
               case gen::ct_BiggerEquals: {
                  result = std::make_unique<gen::DefinitionListSmallerEquals>(def, list, p2);
                  break;
               }
               default: {
                  return false;
               }
            }
         }
         else {
            switch(ct) {
               case gen::ct_Smaller: {
                  result = std::make_unique<gen::DefinitionListSmaller>(def, list, p2);
                  break;
               }
               case gen::ct_SmallerEquals: {
                  result = std::make_unique<gen::DefinitionListSmallerEquals>(def, list, p2);
                  break;
               }
               case gen::ct_Bigger: {
                  result = std::make_unique<gen::DefinitionListBigger>(def, list, p2);
                  break;
               }
               case gen::ct_BiggerEquals: {
                  result = std::make_unique<gen::DefinitionListBiggerEquals>(def, list, p2);
                  break;
               }
               default: {
                  return false;
               }
            }
         }
         break;
      }
   }

   return true;
}


template <typename T>
p_bool comparisonCollections(_genptr<p_bool>& result, const Tokens& left,
   const Tokens& right, const gen::CompType& ct, pp_perun2& p2)
{
   _genptr<std::vector<T>> leftValue;
   if (parse(p2, left, leftValue)) {
      _genptr<std::vector<T>> rightValue;

      if (parse(p2, right, rightValue)) {
         switch(ct) {
            case gen::ct_Equals: {
               result = std::make_unique<gen::CollectionsEqual<T>>(leftValue, rightValue);
               break;
            }
            case gen::ct_NotEquals: {
               result = std::make_unique<gen::CollectionsNotEqual<T>>(leftValue, rightValue);
               break;
            }
            case gen::ct_Smaller: {
               result = std::make_unique<gen::CollectionsSmaller<T>>(leftValue, rightValue);
               break;
            }
            case gen::ct_SmallerEquals: {
               result = std::make_unique<gen::CollectionsSmallerEquals<T>>(leftValue, rightValue);
               break;
            }
            case gen::ct_Bigger: {
               result = std::make_unique<gen::CollectionsBigger<T>>(leftValue, rightValue);
               break;
            }
            case gen::ct_BiggerEquals: {
               result = std::make_unique<gen::CollectionsBiggerEquals<T>>(leftValue, rightValue);
               break;
            }
         }
         return true;
      }
   }

   return false;
}


template <typename T>
p_bool comparisonCollectionValue(_genptr<p_bool>& result, const Tokens& left, const Tokens& right,
   const gen::CompType& ct, pp_perun2& p2)
{
   _genptr<T> leftValue;
   if (parse(p2, left, leftValue)) {
      _genptr<std::vector<T>> rightCollection;

      if (parse(p2, right, rightCollection)) {
         switch(ct) {
            case gen::ct_Equals: {
               result = std::make_unique<gen::CollectionValueEquals<T>>(rightCollection, leftValue);
               break;
            }
            case gen::ct_NotEquals: {
               result = std::make_unique<gen::CollectionValueNotEquals<T>>(rightCollection, leftValue);
               break;
            }
            case gen::ct_Smaller: {
               result = std::make_unique<gen::CollectionValueBigger<T>>(rightCollection);
               break;
            }
            case gen::ct_SmallerEquals: {
               result = std::make_unique<gen::CollectionValueBiggerEquals<T>>(rightCollection);
               break;
            }
            case gen::ct_Bigger: {
               result = std::make_unique<gen::CollectionValueSmaller<T>>(rightCollection);
               break;
            }
            case gen::ct_BiggerEquals: {
               result = std::make_unique<gen::CollectionValueSmallerEquals<T>>(rightCollection);
               break;
            }
         }
         return true;
      }
      else {
         return false;
      }
   }

   _genptr<T> rightValue;
   if (parse(p2, right, rightValue)) {
      _genptr<std::vector<T>> leftCollection;

      if (parse(p2, left, leftCollection)) {
         switch(ct) {
            case gen::ct_Equals: {
               result = std::make_unique<gen::CollectionValueEquals<T>>(leftCollection, rightValue);
               break;
            }
            case gen::ct_NotEquals: {
               result = std::make_unique<gen::CollectionValueNotEquals<T>>(leftCollection, rightValue);
               break;
            }
            case gen::ct_Smaller: {
               result = std::make_unique<gen::CollectionValueSmaller<T>>(leftCollection);
               break;
            }
            case gen::ct_SmallerEquals: {
               result = std::make_unique<gen::CollectionValueSmallerEquals<T>>(leftCollection);
               break;
            }
            case gen::ct_Bigger: {
               result = std::make_unique<gen::CollectionValueBigger<T>>(leftCollection);
               break;
            }
            case gen::ct_BiggerEquals: {
               result = std::make_unique<gen::CollectionValueBiggerEquals<T>>(leftCollection);
               break;
            }
         }

         return true;
      }
      else {
         return false;
      }
   }

   return false;
}

static p_bool parseCollectionComparisons(_genptr<p_bool>& result, const Tokens& left,
   const Tokens& right, const gen::CompType& ct, pp_perun2& p2)
{
   p_defptr leftDef;
   p_defptr rightDef;
   const p_bool hasLeftDef = parse(p2, left, leftDef);
   const p_bool hasRightDef = parse(p2, right, rightDef);

   // special case situations
   // when definition is compared with other data types or with another definition
   // need optimized solutions
   if (hasLeftDef || hasRightDef) {
      if (hasLeftDef && hasRightDef) {
         switch(ct) {
            case gen::ct_Equals: {
               result = std::make_unique<gen::DefinitionsEqual>(leftDef, rightDef, p2);
               break;
            }
            case gen::ct_NotEquals: {
               result = std::make_unique<gen::DefinitionsNotEqual>(leftDef, rightDef, p2);
               break;
            }
            case gen::ct_Smaller: {
               result = std::make_unique<gen::DefinitionsSmaller>(leftDef, rightDef, p2);
               break;
            }
            case gen::ct_SmallerEquals: {
               result = std::make_unique<gen::DefinitionsSmallerEquals>(leftDef, rightDef, p2);
               break;
            }
            case gen::ct_Bigger: {
               result = std::make_unique<gen::DefinitionsBigger>(leftDef, rightDef, p2);
               break;
            }
            case gen::ct_BiggerEquals: {
               result = std::make_unique<gen::DefinitionsBiggerEquals>(leftDef, rightDef, p2);
               break;
            }
         }

         return true;
      }

      if (hasLeftDef) {
         _genptr<p_list> rightList;
         return parse(p2, right, rightList)
            ? comparisonDefList(result, leftDef, rightList, ct, false, p2)
            : false;
      }
      else {
         _genptr<p_list> leftList;
         return parse(p2, left, leftList)
            ? comparisonDefList(result, rightDef, leftList, ct, true, p2)
            : false;
      }
   }

   return comparisonCollectionValue<p_tim>(result, left, right, ct, p2)
       || comparisonCollectionValue<p_num>(result, left, right, ct, p2)
       || comparisonCollectionValue<p_str>(result, left, right, ct, p2)
       || comparisonCollections<p_tim>(result, left, right, ct, p2)
       || comparisonCollections<p_num>(result, left, right, ct, p2)
       || comparisonCollections<p_str>(result, left, right, ct, p2);
}

static std::pair<Tokens, Tokens> prepareComparison(const Tokens& tks, const p_char sign, gen::CompType& ctype)
{
   std::pair<Tokens, Tokens> result = tks.divideBySymbol(sign);
   p_bool eq = false;

   if (result.first.isEmpty()) {
      if (result.second.isEmpty()) {
         throw SyntaxError(str(L"both sides of ", toStr(sign),
            L" comparison are empty"), tks.first().line);
      }
      else {
         throw SyntaxError(str(L"left side of ", toStr(sign),
            L" comparison is empty"), tks.first().line);
      }
   }

   if (result.second.isEmpty()) {
      throw SyntaxError(str(L"right side of ", toStr(sign),
         L" comparison is empty"), tks.last().line);
   }

   if (result.second.first().isSymbol(CHAR_EQUAL_SIGN)) {
      if (result.second.getLength() == 1) {
         throw SyntaxError(str(L"right side of ", toStr(sign),
            L"= comparison is empty"), result.second.first().line);
      }

      result.second.trimLeft();
      eq = true;
   }
   else if (sign == CHAR_EXCLAMATION_MARK) {
      throw SyntaxError(L"expected = after exclamation mark. "
         L"For a simple negation, use keyword 'not' instead",
         result.second.first().line);
   }

   switch (sign) {
      case CHAR_SMALLER:
         ctype = eq ? gen::CompType::ct_SmallerEquals : gen::CompType::ct_Smaller;
         break;
      case CHAR_GREATER:
         ctype = eq ? gen::CompType::ct_BiggerEquals : gen::CompType::ct_Bigger;
         break;
      case CHAR_EQUAL_SIGN:
         ctype = gen::CompType::ct_Equals;
         break;
      case CHAR_EXCLAMATION_MARK: {
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

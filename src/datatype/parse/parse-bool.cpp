/*
    This file is part of Perun2.
    Perun2 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    Perun2 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with Perun2. If not, see <http://www.gnu.org/licenses/>.
*/

#include "parse-bool.h"
#include "parse-list.h"
#include "parse-generic.h"
#include "parse-function.h"
#include "../parse/parse-function.h"
#include "../generator/gen-bool.h"
#include "../generator/gen-bool-constr.h"
#include "../../brackets.h"
#include "../../lexer.h"
#include "../text/like.h"
#include "../text/resemblance.h"
#include "../text/regexp.h"
#include "../parse-gen.h"
#include "../incr-constr.h"
#include "../between.h"
#include <stack>


namespace perun2::parse
{

p_bool parseBool(p_genptr<p_bool>& result, const Tokens& tks, Perun2Process& p2)
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
      if (tryToParseBoolExp(result, tks, p2)) {
         return true;
      }
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

   if (tks.check(TI_HAS_KEYWORD_RESEMBLES)) {
      if (parseResembles(result, tks, p2)) {
         return true;
      }
   }

   if (tks.check(TI_HAS_KEYWORD_BETWEEN)) {
      if (parseBetween(result, tks, p2)) {
         return true;
      }
   }

   if (tks.check(TI_HAS_KEYWORD_REGEXP)) {
      if (parseRegexp(result, tks, p2)) {
         return true;
      }
   }

   if (parseComparisons(result, tks, p2)) {
      return true;
   }

   return parseBinary<p_bool>(result, tks, p2) || parseTernary<p_bool>(result, tks, p2);
}

static p_bool tryToParseBoolExp(p_genptr<p_bool>& result, const Tokens& tks, Perun2Process& p2)
{
   BracketsInfo bi;
   const p_int end = tks.getEnd();
   const p_int start = tks.getStart();
   p_int betweens = 0;

   for (p_int i = start; i <= end; i++) {
      const Token& t = tks.listAt(i);

      if (t.type != Token::t_Keyword) {
         bi.refresh(t);
         continue;
      }

      if (! bi.isBracketFree()) {
         continue;
      }

      if (t.isKeyword(Keyword::kw_Between)) {
         betweens++;
      }
      else if (isBoolExpOperator(t)) {
         if (t.isKeyword(Keyword::kw_Not) && i != end && tks.listAt(i + 1).isNegatableKeywordOperator()) {
            continue;
         }

         if (t.isKeyword(Keyword::kw_And)) {
            if (betweens > 0) {
               betweens--;
               continue;
            }
         }

         if (!parseBoolExp(result, tks, p2)) {
            throw SyntaxError::syntaxOfBooleanExpressionNotValid(tks.first().line);
         }

         return true;
      }
   }

   return false;
}

// build boolean expression
// multiple logic statements
// connected with keywords not, and, or, xor and brackets ()
static p_bool parseBoolExp(p_genptr<p_bool>& result, const Tokens& tks, Perun2Process& p2)
{
   std::vector<ExpElement<p_bool>> infList; // infix notation list
   const p_int start = tks.getStart();
   const p_int end = tks.getEnd();
   p_int sublen = 0;
   BracketsInfo bi;

   // the Between expression is problematic as it disables the preceding And boolean operator
   // we have to keep track of Betweens
   std::stack<p_int> countBetweens;
   countBetweens.push(0);

   for (p_int i = start; i <= end; i++) {
      const Token& t = tks.listAt(i);

      if (t.type == Token::t_Symbol) {
         bi.refresh(t);

         switch (t.value.ch) {
            case CHAR_OPENING_ROUND_BRACKET:
            case CHAR_OPENING_SQUARE_BRACKET: {
               countBetweens.push(0);
               break;
            }
            case CHAR_CLOSING_ROUND_BRACKET:
            case CHAR_CLOSING_SQUARE_BRACKET: {
               countBetweens.pop();
               break;
            }
         }

         sublen++;
         continue;
      }

      if (t.isKeyword(Keyword::kw_Between)) {
         countBetweens.top()++;
         sublen++;
         continue;
      }

      if (t.type != Token::t_Keyword) {
         sublen++;
         continue;
      }

      if (! isBoolExpOperator(t)) {
         sublen++;
         continue;
      }

      if (t.isKeyword(Keyword::kw_And) && countBetweens.top() != 0) {
         countBetweens.top()--;
         sublen++;
         continue;
      }

      if (t.isKeyword(Keyword::kw_Not) && i != end && tks.listAt(i + 1).isNegatableKeywordOperator()) {
         sublen++;
         continue;
      }
      
      const p_char ch = toBoolExpOperator(t);
      if (sublen == 0) {
         infList.emplace_back(ch, t.line);
         continue;
      }

      if (! bi.isBracketFree()) {
         sublen++;
         continue;
      }
   
      const Tokens tks2(tks, i - sublen, sublen);
      const p_int line = tks2.first().line;

      if (tks2.getLength() == 1 && tks2.first().isLogicConstant()) {
         const p_bool boo = tks2.first().value.keyword.k == Keyword::kw_True;
         infList.emplace_back(boo, line);
      }
      else {
         p_genptr<p_bool> boo;

         if (! parse(p2, tks2, boo)) {
            return false;
         }

         infList.emplace_back(boo, line);
      }

      infList.emplace_back(ch, line);
      sublen = 0;
   }

   if (sublen != 0) {
      const Tokens tks2(tks, 1 + end - sublen, sublen);

      if (tks2.getLength() == 1 && tks2.first().isLogicConstant()) {
         const p_bool boo = (tks2.first().value.keyword.k == Keyword::kw_True);
         infList.emplace_back(boo, tks2.first().line);
      }
      else {
         p_genptr<p_bool> boo;
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

static p_bool boolExpTree(p_genptr<p_bool>& result, std::vector<ExpElement<p_bool>>& infList)
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
                  p_genptr<p_bool> res;

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

static p_bool boolExpIntegrateNegations(p_genptr<p_bool>& result,
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
               p_genptr<p_bool> n = std::move(e.generator);
               p_genptr<p_bool> no = std::make_unique<gen::Not>(n);
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

static p_bool boolExpTreeMerge(p_genptr<p_bool>& result,
   std::vector<ExpElement<p_bool>>& elements)
{
   const p_size len = elements.size();
   if (len == 1) {
      result = std::move(elements[0].generator);
      return true;
   }

   p_genptr<p_bool> first = std::move(elements[0].generator);
   p_bool firstIsConstant = first->isConstant();
   p_char op;

   for (p_size i = 1; i < len; i++) {
      ExpElement<p_bool>& e = elements[i];

      if (e.type == ElementType::et_Operator) {
         op = e.operator_;
         continue;
      }

      p_genptr<p_bool> second(std::move(e.generator));

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

      p_genptr<p_bool> prev = std::move(first);

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
static p_bool parseIn_Unit(p_genptr<p_bool>& result, const p_bool negated,
   const std::pair<Tokens, Tokens>& pair, Perun2Process& p2)
{
   p_genptr<T> valLeft;
   if (!parse(p2, pair.first, valLeft)) {
      return false;
   }

   // check if the right side is a single value
   // in this special case, the whole structure is reduced to
   // a simple comparison "left == right" or "left != right"
   p_genptr<T> num2;
   if (parse(p2, pair.second, num2)) {
      if (negated) {
         result = std::make_unique<gen::NotEquals<T>>(valLeft, num2);
      }
      else {
         result = std::make_unique<gen::Equals<T>>(valLeft, num2);
      }
      return true;
   }

   p_genptr<std::vector<T>> valRight;

   if (parse(p2, pair.second, valRight)) {
      if (valRight->isConstant()) {
         const std::vector<T> vs = valRight->getValue();
         p_genptr<p_bool> in = std::make_unique<gen::InConstList<T>>(valLeft, vs);
         result = negated
            ? std::make_unique<gen::Not>(in)
            : std::move(in);
      }
      else {
         p_genptr<p_bool> in = std::make_unique<gen::InList<T>>(valLeft, valRight);
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
   const std::pair<Tokens, Tokens>& pair, const p_bool negated, Perun2Process& p2)
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
   const std::pair<Tokens, Tokens>& pair, const p_bool negated, Perun2Process& p2)
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

static void checkCommonExceptions_InTime(const std::pair<Tokens, Tokens>& pair, const p_bool negated, Perun2Process& p2)
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
               else if (tf.type == Token::t_Number && tf.value.num.n.state == NumberState::Int) {
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
         else if (rf.type == Token::t_Number && rf.value.num.n.state == NumberState::Int) {
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
               else if (tf.type == Token::t_Number && tf.value.num.n.state == NumberState::Int) {
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
         else if (rf.type == Token::t_Number && rf.value.num.n.state == NumberState::Int) {
            rightInTimeException(rf, STRING_YEAR, pair, negated, p2);
         }
      }
   }
}

static p_bool parseResembles(p_genptr<p_bool>& result, const Tokens& tks, Perun2Process& p2)
{
   std::pair<Tokens, Tokens> pair = tks.divideByKeyword(Keyword::kw_Resembles);

   if (pair.first.isEmpty()) {
      throw SyntaxError::leftSideOfOperatorIsEmpty(tks.first().getOriginString(p2), tks.first().line);
   }
   if (pair.second.isEmpty()) {
      throw SyntaxError::rightSideOfOperatorIsEmpty(tks.last().getOriginString(p2), tks.last().line);
   }
   if (pair.first.hasBinaryBoolKeyword()) {
      return false;
   }

   const p_bool neg = pair.first.last().isKeyword(Keyword::kw_Not);
   if (neg) {
      if (pair.first.getLength() == 1) {
         throw SyntaxError::leftSideOfOperatorIsEmpty(str(tks.first().getOriginString(p2), 
            CHAR_SPACE, tks.second().getOriginString(p2)), tks.first().line);
      }

      pair.first.popRight();
   }

   p_genptr<p_str> value;
   if (! parse(p2, pair.first, value)) {
      return false;
   }

   p_genptr<p_str> pattern;
   if (! parse(p2, pair.second, pattern)) {
      return false;
   }

   if (pattern->isConstant()) {
      const p_str cnst = pattern->getValue();

      if (cnst.empty()) {
         const p_bool value = ! neg;
         result = std::make_unique<gen::Constant<p_bool>>(value);
         return true;
      }

      if (neg) {
         p_genptr<p_bool> b = std::make_unique<gen::ResemblesConst>(value, cnst);
         result = std::make_unique<gen::Not>(b);
      }
      else {
         result = std::make_unique<gen::ResemblesConst>(value, cnst);
      }

      return true;
   }

   if (neg) {
      p_genptr<p_bool> b = std::make_unique<gen::Resembles>(value, pattern);
      result = std::make_unique<gen::Not>(b);
   }
   else {
      result = std::make_unique<gen::Resembles>(value, pattern);
   }

   return true;
}


static p_bool parseRegexp(p_genptr<p_bool>& result, const Tokens& tks, Perun2Process& p2)
{
   std::pair<Tokens, Tokens> pair = tks.divideByKeyword(Keyword::kw_Regexp);

   if (pair.first.isEmpty()) {
      throw SyntaxError::leftSideOfOperatorIsEmpty(tks.first().getOriginString(p2), tks.first().line);
   }
   if (pair.second.isEmpty()) {
      throw SyntaxError::rightSideOfOperatorIsEmpty(tks.last().getOriginString(p2), tks.last().line);
   }
   if (pair.first.hasBinaryBoolKeyword()) {
      return false;
   }

   const p_bool neg = pair.first.last().isKeyword(Keyword::kw_Not);
   if (neg) {
      if (pair.first.getLength() == 1) {
         throw SyntaxError::leftSideOfOperatorIsEmpty(str(tks.first().getOriginString(p2), 
            CHAR_SPACE, tks.second().getOriginString(p2)), tks.first().line);
      }

      pair.first.popRight();
   }

   p_genptr<p_str> value;
   if (! parse(p2, pair.first, value)) {
      return false;
   }

   p_genptr<p_str> pattern;
   if (! parse(p2, pair.second, pattern)) {
      return false;
   }

   if (pattern->isConstant()) {
      const p_str cnst = pattern->getValue();

      if (cnst.empty()) {
         result = std::make_unique<gen::Constant<p_bool>>(! neg);
         return true;
      }

      if (neg) {
         p_genptr<p_bool> b = std::make_unique<gen::RegexpConst>(value, cnst);
         result = std::make_unique<gen::Not>(b);
      }
      else {
         result = std::make_unique<gen::RegexpConst>(value, cnst);
      }

      return true;
   }

   if (neg) {
      p_genptr<p_bool> b = std::make_unique<gen::Regexp>(value, pattern);
      result = std::make_unique<gen::Not>(b);
   }
   else {
      result = std::make_unique<gen::Regexp>(value, pattern);
   }

   return true;
}


template <typename T>
static p_bool parseBetweenGeneric(p_genptr<p_bool>& result, p_genptr<T>& value, 
   const Tokens& left, const Tokens& right, const p_bool negated, Perun2Process& p2)
{
   p_genptr<T> leftGen;

   if (! parse(p2, left, leftGen)) {
      return false;
   }

   p_genptr<T> rightGen;

   if (! parse(p2, right, rightGen)) {
      return false;
   }

   const bool leftConst = leftGen->isConstant();
   const bool rightConst = rightGen->isConstant();

   if (leftConst) {
      const T leftBound = leftGen->getValue();

      if (rightConst) {
         const T rightBound = rightGen->getValue();
         result = std::make_unique<gen::BetweenConst<T>>(value, leftBound, rightBound);

         if (negated) {
            result = std::make_unique<gen::Not>(result);
         }
         return true;
      }

      result = std::make_unique<gen::BetweenHalfConst<T>>(value, rightGen, leftBound);
      if (negated) {
         result = std::make_unique<gen::Not>(result);
      }
      return true;
   }

   if (rightConst) {
      const T rightBound = rightGen->getValue();
      result = std::make_unique<gen::BetweenHalfConst<T>>(value, leftGen, rightBound);

      if (negated) {
         result = std::make_unique<gen::Not>(result);
      }
      return true;
   }

   result = std::make_unique<gen::Between<T>>(value, leftGen, rightGen);
   if (negated) {
      result = std::make_unique<gen::Not>(result);
   }
   return true;
}


static void checkCommonExceptions_Between(const Tokens& value, const Tokens& left, 
   const Tokens& right, const Token& betweenKeyword, Perun2Process& p2)
{
   const Token& t1 = value.first();
   const Token& t2 = left.first();
   const Token& t3 = right.first();

   if (! t1.isWord(STRINGS_TIME_VAR, p2)) {
      return;
   }
   
   if (t2.isWeekDay() || t2.isMonth()) {
         throw SyntaxError(str(L"instead of \"", t1.getOriginString(p2), L" ", betweenKeyword.getOriginString(p2), L" ", 
            t2.getOriginString(p2), L"\", you should write \"", 
            t1.getOriginString(p2), L".", (t2.isWeekDay() ? STRING_WEEKDAY_CAMELCASE : STRING_MONTH),
            L" ", betweenKeyword.getOriginString(p2), L" ", t2.getOriginString(p2), L"\""), t1.line);   
   }

   if (t3.isWeekDay() || t3.isMonth()) {
         throw SyntaxError(str(L"instead of \"", t1.getOriginString(p2), L" ", betweenKeyword.getOriginString(p2), L" ", 
            t3.getOriginString(p2), L"\", you should write \"", 
            t1.getOriginString(p2), L".", (t3.isWeekDay() ? STRING_WEEKDAY_CAMELCASE : STRING_MONTH),
            L" ", betweenKeyword.getOriginString(p2), L" ", t3.getOriginString(p2), L"\""), t1.line);   
   }

   if (t2.isIntegerLiteral()) {
      const p_nint nm = t2.value.num.n.value.i;

      if (nm >= NINT_1950 && nm <= NINT_2100) {
         throw SyntaxError(str(L"instead of \"", 
            t1.getOriginString(p2), L" ", betweenKeyword.getOriginString(p2), L" ", toStr(nm),
            L"\", you should write \"", 
            t1.getOriginString(p2), L".year ", betweenKeyword.getOriginString(p2), L" ", toStr(nm), L"\""), t1.line);
      }
      else {
         throw SyntaxError(str(L"the time variable \"", t1.getOriginString(p2),
            L"\" cannot be compared to a number"), t1.line);
      }
   }

   if (t3.isIntegerLiteral()) {
      const p_nint nm = t3.value.num.n.value.i;

      if (nm >= NINT_1950 && nm <= NINT_2100) {
         throw SyntaxError(str(L"instead of \"", 
            t1.getOriginString(p2), L" ", betweenKeyword.getOriginString(p2), L" ", toStr(nm),
            L"\", you should write \"", 
            t1.getOriginString(p2), L".year ", betweenKeyword.getOriginString(p2), L" ", toStr(nm), L"\""), t1.line);
      }
      else {
         throw SyntaxError(str(L"the time variable \"", t1.getOriginString(p2),
            L"\" cannot be compared to a number"), t1.line);
      }
   }
}


static p_bool parseBetween(p_genptr<p_bool>& result, const Tokens& tks, Perun2Process& p2)
{
   std::pair<Tokens, Tokens> firstDivision = tks.divideByKeyword(Keyword::kw_Between);

   if (firstDivision.first.isEmpty()) {
      throw SyntaxError::leftSideOfOperatorIsEmpty(tks.first().getOriginString(p2), tks.first().line);
   }

   const p_bool neg = firstDivision.first.last().isKeyword(Keyword::kw_Not);
   if (neg) {
      if (firstDivision.first.getLength() == 1) {
         throw SyntaxError::leftSideOfOperatorIsEmpty(str(tks.first().getOriginString(p2), 
            CHAR_SPACE, tks.second().getOriginString(p2)), tks.first().line);
      }

      firstDivision.first.popRight();
   }

   if (firstDivision.second.isEmpty()) {
      throw SyntaxError::rightSideOfOperatorIsEmpty(tks.last().getOriginString(p2), tks.last().line);
   }

   const Token& betweenToken = tks.at(firstDivision.first.getLength());

   if (! firstDivision.second.check(TI_HAS_KEYWORD_AND)) {
      const p_str between = betweenToken.getOriginString(p2);
      throw SyntaxError::operatorBetweenShouldBeFollowedByAnd(between, betweenToken.line);
   }

   std::pair<Tokens, Tokens> secondDivision = firstDivision.second.divideByKeyword(Keyword::kw_And);

   if (secondDivision.first.isEmpty()) {
      throw SyntaxError::leftSideOfOperatorIsEmpty(
         firstDivision.second.first().getOriginString(p2), firstDivision.second.first().line);
   }

   if (secondDivision.second.isEmpty()) {
      throw SyntaxError::rightSideOfOperatorIsEmpty(
         firstDivision.second.last().getOriginString(p2), firstDivision.second.last().line);
   }

   const Tokens& valueTokens = firstDivision.first;
   const Tokens& left = secondDivision.first;
   const Tokens& right = secondDivision.second;

   if (valueTokens.getLength() == 1 && left.getLength() == 1 && right.getLength() == 1) {
      checkCommonExceptions_Between(valueTokens, left, right, betweenToken, p2);
   }

   p_genptr<p_num> number;
   if (parse(p2, valueTokens, number)) {
      if (parseBetweenNumbers(result, number, left, right, neg, p2)) {
         return true;
      }
   }

   p_genptr<p_tim> time;
   if (parse(p2, valueTokens, time)) {
      if (parseBetweenTimes(result, time, left, right, neg, p2)) {
         return true;
      }
   }

   p_genptr<p_per> period;
   if (parse(p2, valueTokens, period)) {
      if (parseBetweenGeneric(result, period, left, right, neg, p2)) {
         return true;
      }
   }

   p_genptr<p_str> string;
   if (parse(p2, valueTokens, string)) {
      if (parseBetweenGeneric(result, string, left, right, neg, p2)) {
         return true;
      }
   }

   return false;
}


static p_bool parseBetweenNumbers(p_genptr<p_bool>& result, p_genptr<p_num>& value, 
   const Tokens& left, const Tokens& right, const p_bool negated, Perun2Process& p2)
{
   p_genptr<p_num> leftGen;

   if (! parse(p2, left, leftGen)) {
      return false;
   }

   p_genptr<p_num> rightGen;

   if (! parse(p2, right, rightGen)) {
      return false;
   }

   const bool leftConst = leftGen->isConstant();
   const bool rightConst = rightGen->isConstant();

   if (leftConst) {
      const p_num leftBound = leftGen->getValue();
      if (leftBound.isNaN()) {
         result = std::make_unique<gen::Constant<p_bool>>(false);
         return true;
      }

      if (rightConst) {
         const p_num rightBound = rightGen->getValue();
         if (rightBound.isNaN()) {
            result = std::make_unique<gen::Constant<p_bool>>(false);
            return true;
         }

         result = std::make_unique<gen::BetweenNumbersConst>(value, leftBound, rightBound, negated);
         return true;
      }

      result = std::make_unique<gen::BetweenNumbersHalfConst>(value, rightGen, leftBound, negated);
      return true;
   }
   else {
      if (rightConst) {
         const p_num rightBound = rightGen->getValue();
         if (rightBound.isNaN()) {
            result = std::make_unique<gen::Constant<p_bool>>(false);
            return true;
         }

         result = std::make_unique<gen::BetweenNumbersHalfConst>(value, leftGen, rightBound, negated);
         return true;
      }
   }

   result = std::make_unique<gen::BetweenNumbers>(value, leftGen, rightGen, negated);
   return true;
}


static p_bool parseBetweenTimes(p_genptr<p_bool>& result, p_genptr<p_tim>& value, 
   const Tokens& left, const Tokens& right, const p_bool negated, Perun2Process& p2)
{
   p_genptr<p_tim> leftGen;

   if (! parse(p2, left, leftGen)) {
      return false;
   }

   p_genptr<p_tim> rightGen;

   if (! parse(p2, right, rightGen)) {
      return false;
   }

   const bool leftConst = leftGen->isConstant();
   const bool rightConst = rightGen->isConstant();

   if (leftConst) {
      const p_tim leftBound = leftGen->getValue();
      if (leftBound.isNever()) {
         result = std::make_unique<gen::Constant<p_bool>>(false);
         return true;
      }

      if (rightConst) {
         const p_tim rightBound = rightGen->getValue();
         if (rightBound.isNever() || ! leftBound.isComparableWith(rightBound)) {
            result = std::make_unique<gen::Constant<p_bool>>(false);
            return true;
         }

         result = std::make_unique<gen::BetweenTimesConst>(value, leftBound, rightBound, negated);
         return true;
      }

      result = std::make_unique<gen::BetweenTimesHalfConst>(value, rightGen, leftBound, negated);
      return true;
   }
   else {
      if (rightConst) {
         const p_tim rightBound = rightGen->getValue();
         if (rightBound.isNever()) {
            result = std::make_unique<gen::Constant<p_bool>>(false);
            return true;
         }

         result = std::make_unique<gen::BetweenTimesHalfConst>(value, leftGen, rightBound, negated);
         return true;
      }
   }

   result = std::make_unique<gen::BetweenTimes>(value, leftGen, rightGen, negated);
   return true;
}


static p_bool parseIn(p_genptr<p_bool>& result, const Tokens& tks, Perun2Process& p2)
{
   std::pair<Tokens, Tokens> pair = tks.divideByKeyword(Keyword::kw_In);

   if (pair.first.isEmpty()) {
      throw SyntaxError::leftSideOfOperatorIsEmpty(tks.first().getOriginString(p2), tks.first().line);
   }
   if (pair.second.isEmpty()) {
      throw SyntaxError::rightSideOfOperatorIsEmpty(tks.last().getOriginString(p2), tks.last().line);
   }
   
   if (pair.first.hasBinaryBoolKeyword()) {
      return false;
   }

   p_bool neg = pair.first.last().isKeyword(Keyword::kw_Not);

   if (neg) {
      if (pair.first.getLength() == 1) {
         throw SyntaxError::leftSideOfOperatorIsEmpty(str(tks.first().getOriginString(p2), 
            CHAR_SPACE, tks.second().getOriginString(p2)), tks.first().line);
      }

      pair.first.popRight();
   }
   
   // first: try to build "Number IN NumList"
   p_genptr<p_bool> list;

   if (parseIn_Unit<p_num>(list, neg, pair, p2)) {
      result = std::move(list);
      return true;
   }

   checkCommonExceptions_InTime(pair, neg, p2);

   // secondary: try to build "Time IN TimList"
   p_genptr<p_bool> list2;
   if (parseInTimList(list2, neg, pair, p2)) {
      result = std::move(list2);
      return true;
   }

   // finally: try to build "string IN list"
   return parseIn_Unit<p_str>(result, neg, pair, p2);;
}


static p_bool parseInTimList(p_genptr<p_bool>& result, const bool& negated,
   const std::pair<Tokens, Tokens>& pair, Perun2Process& p2)
{
   p_genptr<p_tim> tim;
   if (!parse(p2, pair.first, tim)) {
      return false;
   }

   p_genptr<p_tim> tim2;
   if (parse(p2, pair.second, tim2)) {
      if (negated) {
         result = std::make_unique<gen::NotEquals<p_tim>>(tim, tim2);
      }
      else {
         result = std::make_unique<gen::Equals<p_tim>>(tim, tim2);
      }

      return true;
   }

   p_genptr<p_tlist> tlist;
   if (parse(p2, pair.second, tlist)) {
      if (tlist->isConstant()) {
         const p_tlist vs = tlist->getValue();
         p_genptr<p_bool> in = std::make_unique<gen::InConstTimeList>(tim, vs);
         result = negated
            ? std::make_unique<gen::Not>(in)
            : std::move(in);
      }
      else {
         p_genptr<p_bool> in = std::make_unique<gen::InList<p_tim>>(tim, tlist);
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

static p_bool parseLike(p_genptr<p_bool>& result, const Tokens& tks, Perun2Process& p2)
{
   std::pair<Tokens, Tokens> pair = tks.divideByKeyword(Keyword::kw_Like);

   if (pair.first.isEmpty()) {
      throw SyntaxError::leftSideOfOperatorIsEmpty(tks.first().getOriginString(p2), tks.first().line);
   }
   if (pair.second.isEmpty()) {
      throw SyntaxError::rightSideOfOperatorIsEmpty(tks.last().getOriginString(p2), tks.last().line);
   }
   if (pair.first.hasBinaryBoolKeyword()) {
      return false;
   }

   const p_bool neg = pair.first.last().isKeyword(Keyword::kw_Not);
   if (neg) {
      if (pair.first.getLength() == 1) {
         throw SyntaxError::leftSideOfOperatorIsEmpty(str(tks.first().getOriginString(p2), 
            CHAR_SPACE, tks.second().getOriginString(p2)), tks.first().line);
      }

      pair.first.popRight();
   }

   p_genptr<p_str> value;
   if (! parse(p2, pair.first, value)) {
      return false;
   }

   p_genptr<p_str> pattern;
   if (! parse(p2, pair.second, pattern)) {
      if (pair.second.getLength() == 1 && pair.second.first().type == Token::t_Pattern) {
         const p_str pattern = pair.second.first().getOriginString(p2);
         throw SyntaxError::asteriskIsNotWildcardInLikeOperator(pattern, pair.second.first().line);
      }

      return false;
   }

   if (pattern->isConstant()) {
      const p_str cnst = pattern->getValue();

      if (neg) {
         p_genptr<p_bool> b = std::make_unique<gen::LikeConst>(value, cnst);
         result = std::make_unique<gen::Not>(b);
      }
      else {
         result = std::make_unique<gen::LikeConst>(value, cnst);
      }

      return true;
   }

   if (neg) {
      p_genptr<p_bool> b = std::make_unique<gen::Like>(value, pattern);
      result = std::make_unique<gen::Not>(b);
   }
   else {
      result = std::make_unique<gen::Like>(value, pattern);
   }

   return true;
}

static p_bool parseComparisons(p_genptr<p_bool>& result, const Tokens& tks, Perun2Process& p2)
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
static p_bool comparison(p_genptr<p_bool>& result, p_genptr<T>& val1,
   p_genptr<T>& val2, const CompType& ct)
{
   switch (ct) {
      case CompType::ct_Equals: {
         result = std::make_unique<gen::Equals<T>>(val1, val2);
         break;
      }
      case CompType::ct_NotEquals: {
         result = std::make_unique<gen::NotEquals<T>>(val1, val2);
         break;
      }
      case CompType::ct_Smaller: {
         result = std::make_unique<gen::Smaller<T>>(val1, val2);
         break;
      }
      case CompType::ct_SmallerEquals: {
         result = std::make_unique<gen::SmallerEquals<T>>(val1, val2);
         break;
      }
      case CompType::ct_Bigger: {
         result = std::make_unique<gen::Bigger<T>>(val1, val2);
         break;
      }
      case CompType::ct_BiggerEquals: {
         result = std::make_unique<gen::BiggerEquals<T>>(val1, val2);
         break;
      }
      default:
         return false;
   };

   return true;
}


template <typename T>
p_bool parseComparisonUnit(p_genptr<p_bool>& result, const Tokens& left,
   const Tokens& right, const CompType& ct, Perun2Process& p2)
{
   p_genptr<T> v1;
   p_genptr<T> v2;
   if (parse(p2, left, v1) && parse(p2, right, v2)) {
      return comparison<T>(result, v1, v2, ct);
   }

   return false;
}


static void checkCommonExceptions_Comparison(const Tokens& left, const Tokens& right, const p_char sign, Perun2Process& p2)
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
      throw SyntaxError(str(L"instead of \"", t1.getOriginString(p2), L" ", toStr(sign), L" ", t2.getOriginString(p2),
         L"\", you should write \"", t1.getOriginString(p2), L".", (isWeek2 ? STRING_WEEKDAY_CAMELCASE : STRING_MONTH),
         L" ", toStr(sign), L" ", t2.getOriginString(p2), L"\""), t1.line);
   }
   else if ((isWeek1 || isMonth1) && isVar2) {
      throw SyntaxError(str(L"instead of \"", t1.getOriginString(p2), L" ", toStr(sign), L" ", t2.getOriginString(p2),
         L"\", you should write \"", t1.getOriginString(p2), L" ", toStr(sign), L" ", t2.getOriginString(p2), L".",
         (isWeek1 ? STRING_WEEKDAY_CAMELCASE : STRING_MONTH), L"\""), t1.line);
   }

   if (isVar1 && t2.isIntegerLiteral()) {
      const p_nint nm = t2.value.num.n.value.i;
      if (nm >= NINT_1950 && nm <= NINT_2100) {
         throw SyntaxError(str(L"instead of \"", t1.getOriginString(p2), L" ", toStr(sign), L" ", toStr(nm),
            L"\", you should write \"", t1.getOriginString(p2), L".year ", toStr(sign), L" ", toStr(nm), L"\""), t1.line);
      }
      else {
         throw SyntaxError(str(L"the time variable \"", t1.getOriginString(p2),
            L"\" cannot be compared to a number"), t1.line);
      }
   }
   else if (t1.isIntegerLiteral() && isVar2) {
      const p_nint nm = t1.value.num.n.value.i;
      if (nm >= NINT_1950 && nm <= NINT_2100) {
         throw SyntaxError(str(L"instead of \"", toStr(nm), L" ", toStr(sign), L" ", t2.getOriginString(p2),
            L"\", you should write \"", toStr(nm), L" ", toStr(sign), L" ", t2.getOriginString(p2), L".year\""), t1.line);
      }
      else {
         throw SyntaxError(str(L"the time variable \"", t2.getOriginString(p2),
            L"\" cannot be compared to a number"), t1.line);
      }
   }
}


static p_bool functionIncrConstr(p_genptr<p_bool>& result, const Tokens& tokens, 
   p_genptr<p_num>& rightSide, const CompType ct, Perun2Process& p2)
{
   const Token& word = tokens.first();
   const std::vector<Tokens> args = func::toFunctionArgs(tokens);
   const p_size argsCount = args.size();

   if (word.isWord(STRING_COUNTINSIDE, p2)) {
      if (argsCount != 1) {
         if (argsCount != 0) {
            func::checkInOperatorCommaAmbiguity(word, args[0], p2);
         }
         func::functionArgNumberException(argsCount, word, p2);
      }

      func::checkFunctionAttribute(word, p2);

      FileContext* fctx = p2.contexts.getFileContext();
      fctx->attribute->setCoreCommandBase();

      if (fctx->isInside) {
         p_defptr def;
         if (!parse::parse(p2, args[0], def)) {
            func::functionArgException(0, STRING_DEFINITION, word, p2);
         }

         result = std::make_unique<gen::CountConstraint>(rightSide, ct, def, p2);
         return true;
      }

      p_lcptr lctx;
      p2.contexts.makeLocationContext(lctx);
      p2.contexts.addLocationContext(lctx.get());

      p_defptr def;
      if (!parse::parse(p2, args[0], def)) {
         func::functionArgException(0, STRING_DEFINITION, word, p2);
      }

      p2.contexts.retreatLocationContext();
      result = std::make_unique<gen::CountInsideConstraint>(rightSide, ct, def, lctx, *fctx, p2);
      return true;
   }
   else if (word.isWord(STRING_COUNT, p2)) {
      if (argsCount != 1) {
         if (argsCount != 0) {
            func::checkInOperatorCommaAmbiguity(word, args[0], p2);
         }
         func::functionArgNumberException(argsCount, word, p2);
      }

      p_defptr def;
      if (parse::parse(p2, args[0], def)) {
         result = std::make_unique<gen::CountConstraint>(rightSide, ct, def, p2);
         return true;
      }

      return false;
   }
   else if (word.isWord(STRING_SIZE, p2)) {
      if (argsCount != 1) {
         if (argsCount != 0) {
            func::checkInOperatorCommaAmbiguity(word, args[0], p2);
         }
         func::functionArgNumberException(argsCount, word, p2);
      }

      p_defptr def;
      if (parse::parse(p2, args[0], def)) {
         result = std::make_unique<gen::SizeConstraint_Def>(rightSide, def, ct, p2);
         return true;
      }

      p_genptr<p_list> list;
      if (parse::parse(p2, args[0], list)) {
         result = std::make_unique<gen::SizeConstraint_List>(rightSide, list, ct, p2);
         return true;
      }

      return false;
   }

   return false;
}

static p_bool sizeIncrConstr(p_genptr<p_bool>& result, const Token& varToken, 
   p_genptr<p_num>& rightSide, const CompType ct, Perun2Process& p2)
{
   if (! p2.contexts.hasFileContext()) {
      throw SyntaxError::undefinedVarValue(varToken.getOriginString(p2), varToken.line);
   }

   FileContext& context = *p2.contexts.getFileContext();
   context.attribute->setCoreCommandBase();
   context.attribute->set(ATTR_SIZE_FILE_ONLY);
   result = std::make_unique<gen::SizeConstraint>(rightSide, ct, context, isAlterableAttribute(varToken, p2), p2);
   return true;
}


static p_bool parseIncrConstr(p_genptr<p_bool>& result, const Tokens& left, 
   const Tokens& right, const CompType ct, Perun2Process& p2)
{
   if (left.getLength() == 1 && left.first().isWord(STRING_SIZE, p2)) {
      p_genptr<p_num> rightSide;
      if (!parse(p2, right, rightSide)) {
         return false;
      }

      return sizeIncrConstr(result, left.first(), rightSide, ct, p2);
   }

   if (left.check(TI_IS_POSSIBLE_FUNCTION)) {
      p_genptr<p_num> rightSide;
      if (!parse(p2, right, rightSide)) {
         return false;
      }

      return functionIncrConstr(result, left, rightSide, ct, p2);
   }

   return false;
}


static p_bool parseComparison(p_genptr<p_bool>& result, const Tokens& tks, const p_char sign, Perun2Process& p2)
{
   CompType ct;
   const std::pair<Tokens, Tokens> pair = prepareComparison(tks, sign, ct);
   const Tokens& left = pair.first;
   const Tokens& right = pair.second;

   // look for some common errors
   // and throw precise messages to the user
   if (left.getLength() == 1 && right.getLength() == 1) {
      checkCommonExceptions_Comparison(left, right, sign, p2);
   }

   // try some rare optimizations
   if (parseIncrConstr(result, left, right, ct, p2) || parseIncrConstr(result, right, left, mirrorCompType(ct), p2))
   {
      return true;
   }

   // try to parse comparison for every singular data type
   if (parseComparisonUnit<p_bool>(result, left, right, ct, p2)
    || parseComparisonUnit<p_num>(result, left, right, ct, p2)
    || parseComparisonUnit<p_per>(result, left, right, ct, p2)
    || parseComparisonUnit<p_tim>(result, left, right, ct, p2)
    || parseComparisonUnit<p_str>(result, left, right, ct, p2)) 
   {
      return true;
   }

   // comparisons between singular values have failed, so try comparisons of collections
   return parseCollectionComparisons(result, left, right, ct, p2);
}

p_bool comparisonDefList(p_genptr<p_bool>& result, p_defptr& def, p_genptr<p_list>& list, const CompType& ct,
   const p_bool reversed, Perun2Process& p2)
{
   switch (ct) {
      case CompType::ct_Equals: {
         result = std::make_unique<gen::DefinitionListEqual>(def, list, p2);
         break;
      }
      case CompType::ct_NotEquals: {
         result = std::make_unique<gen::DefinitionListNotEqual>(def, list, p2);
         break;
    }
      default: {
         if (reversed) {
            switch(ct) {
               case CompType::ct_Smaller: {
                  result = std::make_unique<gen::DefinitionListBigger>(def, list, p2);
                  break;
               }
               case CompType::ct_SmallerEquals: {
                  result = std::make_unique<gen::DefinitionListBiggerEquals>(def, list, p2);
                  break;
               }
               case CompType::ct_Bigger: {
                  result = std::make_unique<gen::DefinitionListSmaller>(def, list, p2);
                  break;
               }
               case CompType::ct_BiggerEquals: {
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
               case CompType::ct_Smaller: {
                  result = std::make_unique<gen::DefinitionListSmaller>(def, list, p2);
                  break;
               }
               case CompType::ct_SmallerEquals: {
                  result = std::make_unique<gen::DefinitionListSmallerEquals>(def, list, p2);
                  break;
               }
               case CompType::ct_Bigger: {
                  result = std::make_unique<gen::DefinitionListBigger>(def, list, p2);
                  break;
               }
               case CompType::ct_BiggerEquals: {
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
p_bool comparisonCollections(p_genptr<p_bool>& result, const Tokens& left,
   const Tokens& right, const CompType ct, Perun2Process& p2)
{
   p_genptr<std::vector<T>> leftValue;
   if (parse(p2, left, leftValue)) {
      p_genptr<std::vector<T>> rightValue;

      if (parse(p2, right, rightValue)) {
         switch(ct) {
            case CompType::ct_Equals: {
               result = std::make_unique<gen::CollectionsEqual<T>>(leftValue, rightValue);
               break;
            }
            case CompType::ct_NotEquals: {
               result = std::make_unique<gen::CollectionsNotEqual<T>>(leftValue, rightValue);
               break;
            }
            case CompType::ct_Smaller: {
               result = std::make_unique<gen::CollectionsSmaller<T>>(leftValue, rightValue);
               break;
            }
            case CompType::ct_SmallerEquals: {
               result = std::make_unique<gen::CollectionsSmallerEquals<T>>(leftValue, rightValue);
               break;
            }
            case CompType::ct_Bigger: {
               result = std::make_unique<gen::CollectionsBigger<T>>(leftValue, rightValue);
               break;
            }
            case CompType::ct_BiggerEquals: {
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
p_bool comparisonCollectionValue(p_genptr<p_bool>& result, const Tokens& left, const Tokens& right,
   const CompType& ct, Perun2Process& p2)
{
   p_genptr<T> leftValue;
   if (parse(p2, left, leftValue)) {
      p_genptr<std::vector<T>> rightCollection;

      if (parse(p2, right, rightCollection)) {
         switch(ct) {
            case CompType::ct_Equals: {
               result = std::make_unique<gen::CollectionValueEquals<T>>(rightCollection, leftValue);
               break;
            }
            case CompType::ct_NotEquals: {
               result = std::make_unique<gen::CollectionValueNotEquals<T>>(rightCollection, leftValue);
               break;
            }
            case CompType::ct_Smaller: {
               result = std::make_unique<gen::CollectionValueBigger<T>>(rightCollection);
               break;
            }
            case CompType::ct_SmallerEquals: {
               result = std::make_unique<gen::CollectionValueBiggerEquals<T>>(rightCollection);
               break;
            }
            case CompType::ct_Bigger: {
               result = std::make_unique<gen::CollectionValueSmaller<T>>(rightCollection);
               break;
            }
            case CompType::ct_BiggerEquals: {
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

   p_genptr<T> rightValue;
   if (parse(p2, right, rightValue)) {
      p_genptr<std::vector<T>> leftCollection;

      if (parse(p2, left, leftCollection)) {
         switch(ct) {
            case CompType::ct_Equals: {
               result = std::make_unique<gen::CollectionValueEquals<T>>(leftCollection, rightValue);
               break;
            }
            case CompType::ct_NotEquals: {
               result = std::make_unique<gen::CollectionValueNotEquals<T>>(leftCollection, rightValue);
               break;
            }
            case CompType::ct_Smaller: {
               result = std::make_unique<gen::CollectionValueSmaller<T>>(leftCollection);
               break;
            }
            case CompType::ct_SmallerEquals: {
               result = std::make_unique<gen::CollectionValueSmallerEquals<T>>(leftCollection);
               break;
            }
            case CompType::ct_Bigger: {
               result = std::make_unique<gen::CollectionValueBigger<T>>(leftCollection);
               break;
            }
            case CompType::ct_BiggerEquals: {
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

static p_bool parseCollectionComparisons(p_genptr<p_bool>& result, const Tokens& left,
   const Tokens& right, const CompType ct, Perun2Process& p2)
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
            case CompType::ct_Equals: {
               result = std::make_unique<gen::DefinitionsEqual>(leftDef, rightDef, p2);
               break;
            }
            case CompType::ct_NotEquals: {
               result = std::make_unique<gen::DefinitionsNotEqual>(leftDef, rightDef, p2);
               break;
            }
            case CompType::ct_Smaller: {
               result = std::make_unique<gen::DefinitionsSmaller>(leftDef, rightDef, p2);
               break;
            }
            case CompType::ct_SmallerEquals: {
               result = std::make_unique<gen::DefinitionsSmallerEquals>(leftDef, rightDef, p2);
               break;
            }
            case CompType::ct_Bigger: {
               result = std::make_unique<gen::DefinitionsBigger>(leftDef, rightDef, p2);
               break;
            }
            case CompType::ct_BiggerEquals: {
               result = std::make_unique<gen::DefinitionsBiggerEquals>(leftDef, rightDef, p2);
               break;
            }
         }

         return true;
      }

      if (hasLeftDef) {
         p_genptr<p_list> rightList;
         return parse(p2, right, rightList)
            ? comparisonDefList(result, leftDef, rightList, ct, false, p2)
            : false;
      }
      else {
         p_genptr<p_list> leftList;
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

static std::pair<Tokens, Tokens> prepareComparison(const Tokens& tks, const p_char sign, CompType& ctype)
{
   std::pair<Tokens, Tokens> result = tks.divideBySymbol(sign);
   p_bool eq = false;

   if (result.first.isEmpty()) {
      if (result.second.isEmpty()) {
         throw SyntaxError(str(L"both sides of the ", toStr(sign),
            L" comparison are empty"), tks.first().line);
      }
      else {
         throw SyntaxError(str(L"the left side of the ", toStr(sign),
            L" comparison is empty"), tks.first().line);
      }
   }

   if (result.second.isEmpty()) {
      throw SyntaxError(str(L"the right side of the ", toStr(sign),
         L" comparison is empty"), tks.last().line);
   }

   if (result.second.first().isSymbol(CHAR_EQUAL_SIGN)) {
      if (result.second.getLength() == 1) {
         throw SyntaxError(str(L"the right side of the ", toStr(sign),
            L"= comparison is empty"), result.second.first().line);
      }

      result.second.popLeft();
      eq = true;
   }
   else if (sign == CHAR_EXCLAMATION_MARK) {
      throw SyntaxError(L"expected = after exclamation mark. "
         L"For a simple negation, use keyword \"not\" instead",
         result.second.first().line);
   }

   switch (sign) {
      case CHAR_SMALLER:
         ctype = eq ? CompType::ct_SmallerEquals : CompType::ct_Smaller;
         break;
      case CHAR_GREATER:
         ctype = eq ? CompType::ct_BiggerEquals : CompType::ct_Bigger;
         break;
      case CHAR_EQUAL_SIGN:
         ctype = CompType::ct_Equals;
         break;
      case CHAR_EXCLAMATION_MARK: {
         ctype = CompType::ct_NotEquals;
         break;
      default:
         ctype = CompType::ct_Equals;
         break;
      }
   }

   return result;
}

}

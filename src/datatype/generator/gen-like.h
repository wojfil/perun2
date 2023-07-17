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

#ifndef GEN_LIKE_H
#define GEN_LIKE_H

#include "../datatype.h"
#include "../wildcard.h"
#include <unordered_set>
#include <unordered_map>


namespace perun2::gen
{

// these two non-printable chars are used internally for the Like operator
// they symbolize:
// 1) lack of char
// 2) there is a set of characters at this position
p_constexpr p_char WILDCARD_NONE = CHAR_NULL;
p_constexpr p_char WILDCARD_SET = CHAR_NULL_2;

p_constexpr p_char WILDCARD_ONE_CHAR = CHAR_UNDERSCORE;
p_constexpr p_char WILDCARD_MULTIPLE_CHARS = CHAR_PERCENT;
p_constexpr p_char WILDCARD_ONE_DIGIT = CHAR_HASH;
p_constexpr p_char WILDCARD_SET_RANGE = CHAR_MINUS;
p_constexpr p_char WILDCARD_SET_START = CHAR_OPENING_SQUARE_BRACKET;
p_constexpr p_char WILDCARD_SET_END = CHAR_CLOSING_SQUARE_BRACKET;
p_constexpr p_char WILDCARD_SET_EXCLUSION = CHAR_CARET;


struct LikeSet
{
public:
   LikeSet() = delete;
   LikeSet(const std::unordered_set<p_char>& vals, const p_bool neg);
   p_bool contains(const p_char ch) const;

private:
   const std::unordered_set<p_char> values;
   const p_bool negated;
};


struct LikeComparer
{
   virtual p_bool compareToPattern(const p_str& value) = 0;
};


typedef std::unique_ptr<LikeComparer> _likeptr;


static LikeSet makeLikeSet(const p_str& pattern, p_size startId, const p_size endId);
static void defaultLikeCmp(_likeptr& result, const p_str& pattern);
void parseLikeCmp(_likeptr& result, const p_str& pattern);


// operator LIKE with pattern initialized with a string literal
struct LikeConst : Generator<p_bool>
{
public:
   LikeConst() = delete;
   LikeConst(_genptr<p_str>& val, const p_str& pattern);
   p_bool getValue() override;

private:
   _genptr<p_str> value;
   _likeptr comparer;
};


// operator LIKE with pattern initialized with any string expression
struct Like : Generator<p_bool>
{
public:
   Like(_genptr<p_str>& val, _genptr<p_str>& pat);
   p_bool getValue() override;

private:
   _genptr<p_str> value;
   _genptr<p_str> pattern;
   _likeptr comparer;
   p_str prevPattern;
};


//  %exa[m-v]__pl_e%            complex pattern like this
struct LC_Default : LikeComparer, WildcardComparer
{
public:
   LC_Default() = delete;
   LC_Default(const p_str& pat);
   LC_Default(const p_str& pat, const std::unordered_map<p_size, LikeSet>& cs);
   p_bool compareToPattern(const p_str& value) override;

protected:
   WildcardCharState checkState(const p_size n, const p_size m) override;
   p_size getMinLength(const p_str& pat) const override;

private:
   const std::unordered_map<p_size, LikeSet> charSets;
};


//  example%
struct LC_StartsWith : LikeComparer
{
public:
   LC_StartsWith() = delete;
   LC_StartsWith(const p_str& pat);
   p_bool compareToPattern(const p_str& value) override;

private:
   p_size length;
   p_str start;
};


//  %example
struct LC_EndsWith : LikeComparer
{
public:
   LC_EndsWith() = delete;
   LC_EndsWith(const p_str& pat);
   p_bool compareToPattern(const p_str& value) override;

private:
   const p_size length;
   const p_str end;
};


//  %example%
struct LC_Contains : LikeComparer
{
public:
   LC_Contains() = delete;
   LC_Contains(const p_str& pat);
   p_bool compareToPattern(const p_str& value) override;

private:
   const p_size length;
   const p_str string;
};


//  e%
struct LC_StartsWithChar : LikeComparer
{
public:
   LC_StartsWithChar() = delete;
   LC_StartsWithChar(const p_str& pat) : ch(pat[0]) {};
   p_bool compareToPattern(const p_str& value) override;

private:
   const p_char ch;
};


//  %e
struct LC_EndsWithChar : LikeComparer
{
public:
   LC_EndsWithChar() = delete;
   LC_EndsWithChar(const p_str& pat) : ch(pat[1]) {};
   p_bool compareToPattern(const p_str& value) override;

private:
   const p_char ch;
};


//  %e%
struct LC_ContainsChar : LikeComparer
{
public:
   LC_ContainsChar() = delete;
   LC_ContainsChar(const p_str& pat) : ch(pat[1]) {};
   p_bool compareToPattern(const p_str& value) override;

private:
   const p_char ch;
};


//  _example
struct LC_UnderscoreStart : LikeComparer
{
public:
   LC_UnderscoreStart() = delete;
   LC_UnderscoreStart(const p_str& pat) : pattern(pat),
      length(pat.size()) {};
   p_bool compareToPattern(const p_str& value) override;

private:
   const p_str pattern;
   const p_size length;
};


//  example_
struct LC_UnderscoreEnd : LikeComparer
{
public:
   LC_UnderscoreEnd() = delete;
   LC_UnderscoreEnd(const p_str& pat) : pattern(pat), length(pat.size()) {};
   p_bool compareToPattern(const p_str& value) override;

private:
   const p_str pattern;
   const p_size length;
};


//  _example_
struct LC_UnderscoreStartEnd : LikeComparer
{
public:
   LC_UnderscoreStartEnd() = delete;
   LC_UnderscoreStartEnd(const p_str& pat) : pattern(pat), length(pat.size()) {};
   p_bool compareToPattern(const p_str& value) override;

private:
   const p_str pattern;
   const p_size length;
};


//  example
struct LC_Equals : LikeComparer
{
public:
   LC_Equals() = delete;
   LC_Equals(const p_str& pat) : pattern(pat) {};
   p_bool compareToPattern(const p_str& value) override;

private:
   const p_str pattern;
};


//  case that is always true / always false
struct LC_Constant : LikeComparer
{
public:
   LC_Constant() = delete;
   LC_Constant(const p_bool cnst) : constant(cnst) {};
   p_bool compareToPattern(const p_str& value) override;

private:
   const p_bool constant;
};



//  string has to contain certain amount of characters
struct LC_ConstantLength : LikeComparer
{
public:
   LC_ConstantLength() = delete;
   LC_ConstantLength(const p_size len) : length(len) {};
   p_bool compareToPattern(const p_str& value) override;

private:
   const p_size length;
};


//  _example%
struct LC_UnderscorePercent : LikeComparer
{
public:
   LC_UnderscorePercent() = delete;
   LC_UnderscorePercent(const p_str& pat);
   p_bool compareToPattern(const p_str& value) override;

private:
   const p_size length;
   const p_str start;
};


//  %example_
struct LC_PercentUnderscore : LikeComparer
{
public:
   LC_PercentUnderscore() = delete;
   LC_PercentUnderscore(const p_str& pat);
   p_bool compareToPattern(const p_str& value) override;

private:
   const p_size length;
   const p_str end;
};


//  string has to contain certain amount of characters and they are all digits
struct LC_OnlyDigits : LikeComparer
{
public:
   LC_OnlyDigits() = delete;
   LC_OnlyDigits(const p_size len) : length(len) {};
   p_bool compareToPattern(const p_str& value) override;

private:
   const p_size length;
};


//  _exa_mpl_e
struct LC_Field_U : LikeComparer
{
public:
   LC_Field_U() = delete;
   LC_Field_U(const p_str& pat);
   p_bool compareToPattern(const p_str& value) override;

private:
   const p_size length;
   const p_str pattern;
   std::vector<p_bool> isUnderscore;
};


//  #exa#mpl#e
struct LC_Field_H : LikeComparer
{
public:
   LC_Field_H() = delete;
   LC_Field_H(const p_str& pat);
   p_bool compareToPattern(const p_str& value) override;

private:
   const p_size length;
   const p_str pattern;
   std::vector<p_bool> isHash;
};


//  #exa__mpl#e_e
struct LC_Field_UH : LikeComparer
{
public:
   LC_Field_UH() = delete;
   LC_Field_UH(const p_str& pat);
   p_bool compareToPattern(const p_str& value) override;

private:
   const p_size length;
   const p_str pattern;
};

}

#endif /* GEN_LIKE_H */

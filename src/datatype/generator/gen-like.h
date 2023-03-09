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
#include "../../hash.h"
#include <unordered_set>
#include <unordered_map>


namespace perun2::gen
{

// these two non-printable chars are used internally for the Like operator
// they symbolize:
// 1) lack of char
// 2) there is a set of characters at this position
inline constexpr _char WILDCARD_NONE = CHAR_NULL;
inline constexpr _char WILDCARD_SET = CHAR_NULL_2;

inline constexpr _char WILDCARD_ONE_CHAR = CHAR_UNDERSCORE;
inline constexpr _char WILDCARD_MULTIPLE_CHARS = CHAR_PERCENT;
inline constexpr _char WILDCARD_ONE_DIGIT = CHAR_HASH;
inline constexpr _char WILDCARD_SET_RANGE = CHAR_MINUS;
inline constexpr _char WILDCARD_SET_START = CHAR_OPENING_SQUARE_BRACKET;
inline constexpr _char WILDCARD_SET_END = CHAR_CLOSING_SQUARE_BRACKET;
inline constexpr _char WILDCARD_SET_EXCLUSION = CHAR_CARET;


struct LikeSet
{
public:
   LikeSet() = delete;
   LikeSet(const std::unordered_set<_char>& vals, const _bool neg);
   _bool contains(const _char ch) const;

private:
   const std::unordered_set<_char> values;
   const _bool negated;
};


struct LikeComparer
{
   virtual _bool compareToPattern(const _str& value) = 0;
};


typedef std::unique_ptr<LikeComparer> _likeptr;


static LikeSet makeLikeSet(const _str& pattern, _size startId, const _size endId);
static void defaultLikeCmp(_likeptr& result, const _str& pattern);
void parseLikeCmp(_likeptr& result, const _str& pattern);


// operator LIKE with pattern initialized with a string literal
struct LikeConst : Generator<_bool>
{
public:
   LikeConst() = delete;
   LikeConst(_genptr<_str>& val, const _str& pattern);
   _bool getValue() override;

private:
   _genptr<_str> value;
   _likeptr comparer;
};


// operator LIKE with pattern initialized with any string expression
struct Like : Generator<_bool>
{
public:
   Like(_genptr<_str>& val, _genptr<_str>& pat);
   _bool getValue() override;

private:
   _genptr<_str> value;
   _genptr<_str> pattern;
   _likeptr comparer;
   _hash prevHash = 0;
};


//  %exa[m-v]__pl_e%            complex pattern like this
struct LC_Default : LikeComparer, WildcardComparer
{
public:
   LC_Default() = delete;
   LC_Default(const _str& pat);
   LC_Default(const _str& pat, const std::unordered_map<_size, LikeSet>& cs);
   _bool compareToPattern(const _str& value) override;

protected:
   WildcardCharState checkState(const _size n, const _size m) override;
   _size getMinLength(const _str& pat) const override;

private:
   const std::unordered_map<_size, LikeSet> charSets;
};


//  example%
struct LC_StartsWith : LikeComparer
{
public:
   LC_StartsWith() = delete;
   LC_StartsWith(const _str& pat);
   _bool compareToPattern(const _str& value) override;

private:
   _size length;
   _str start;
};


//  %example
struct LC_EndsWith : LikeComparer
{
public:
   LC_EndsWith() = delete;
   LC_EndsWith(const _str& pat);
   _bool compareToPattern(const _str& value) override;

private:
   const _size length;
   const _str end;
};


//  %example%
struct LC_Contains : LikeComparer
{
public:
   LC_Contains() = delete;
   LC_Contains(const _str& pat);
   _bool compareToPattern(const _str& value) override;

private:
   const _size length;
   const _str string;
};


//  e%
struct LC_StartsWithChar : LikeComparer
{
public:
   LC_StartsWithChar() = delete;
   LC_StartsWithChar(const _str& pat) : ch(pat[0]) {};
   _bool compareToPattern(const _str& value) override;

private:
   const _char ch;
};


//  %e
struct LC_EndsWithChar : LikeComparer
{
public:
   LC_EndsWithChar() = delete;
   LC_EndsWithChar(const _str& pat) : ch(pat[1]) {};
   _bool compareToPattern(const _str& value) override;

private:
   const _char ch;
};


//  %e%
struct LC_ContainsChar : LikeComparer
{
public:
   LC_ContainsChar() = delete;
   LC_ContainsChar(const _str& pat) : ch(pat[1]) {};
   _bool compareToPattern(const _str& value) override;

private:
   const _char ch;
};


//  _example
struct LC_UnderscoreStart : LikeComparer
{
public:
   LC_UnderscoreStart() = delete;
   LC_UnderscoreStart(const _str& pat) : pattern(pat),
      length(pat.size()) {};
   _bool compareToPattern(const _str& value) override;

private:
   const _str pattern;
   const _size length;
};


//  example_
struct LC_UnderscoreEnd : LikeComparer
{
public:
   LC_UnderscoreEnd() = delete;
   LC_UnderscoreEnd(const _str& pat) : pattern(pat), length(pat.size()) {};
   _bool compareToPattern(const _str& value) override;

private:
   const _str pattern;
   const _size length;
};


//  _example_
struct LC_UnderscoreStartEnd : LikeComparer
{
public:
   LC_UnderscoreStartEnd() = delete;
   LC_UnderscoreStartEnd(const _str& pat) : pattern(pat), length(pat.size()) {};
   _bool compareToPattern(const _str& value) override;

private:
   const _str pattern;
   const _size length;
};


//  example
struct LC_Equals : LikeComparer
{
public:
   LC_Equals() = delete;
   LC_Equals(const _str& pat) : pattern(pat) {};
   _bool compareToPattern(const _str& value) override;

private:
   const _str pattern;
};


//  case that is always true / always false
struct LC_Constant : LikeComparer
{
public:
   LC_Constant() = delete;
   LC_Constant(const _bool cnst) : constant(cnst) {};
   _bool compareToPattern(const _str& value) override;

private:
   const _bool constant;
};



//  string has to contain certain amount of characters
struct LC_ConstantLength : LikeComparer
{
public:
   LC_ConstantLength() = delete;
   LC_ConstantLength(const _size len) : length(len) {};
   _bool compareToPattern(const _str& value) override;

private:
   const _size length;
};


//  _example%
struct LC_UnderscorePercent : LikeComparer
{
public:
   LC_UnderscorePercent() = delete;
   LC_UnderscorePercent(const _str& pat);
   _bool compareToPattern(const _str& value) override;

private:
   const _size length;
   const _str start;
};


//  %example_
struct LC_PercentUnderscore : LikeComparer
{
public:
   LC_PercentUnderscore() = delete;
   LC_PercentUnderscore(const _str& pat);
   _bool compareToPattern(const _str& value) override;

private:
   const _size length;
   const _str end;
};


//  string has to contain certain amount of characters and they are all digits
struct LC_OnlyDigits : LikeComparer
{
public:
   LC_OnlyDigits() = delete;
   LC_OnlyDigits(const _size len) : length(len) {};
   _bool compareToPattern(const _str& value) override;

private:
   const _size length;
};


//  _exa_mpl_e
struct LC_Field_U : LikeComparer
{
public:
   LC_Field_U() = delete;
   LC_Field_U(const _str& pat);
   _bool compareToPattern(const _str& value) override;

private:
   const _size length;
   const _str pattern;
   std::vector<_bool> isUnderscore;
};


//  #exa#mpl#e
struct LC_Field_H : LikeComparer
{
public:
   LC_Field_H() = delete;
   LC_Field_H(const _str& pat);
   _bool compareToPattern(const _str& value) override;

private:
   const _size length;
   const _str pattern;
   std::vector<_bool> isHash;
};


//  #exa__mpl#e_e
struct LC_Field_UH : LikeComparer
{
public:
   LC_Field_UH() = delete;
   LC_Field_UH(const _str& pat);
   _bool compareToPattern(const _str& value) override;

private:
   const _size length;
   const _str pattern;
};

}

#endif /* GEN_LIKE_H */

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

#ifndef GEN_LIKE_H
#define GEN_LIKE_H

#include "../generator.h"
#include "../datatype.h"
#include <unordered_set>


struct LikeComparer
{
   virtual _boo compareToPattern(const _str& value) const = 0;
};


_boo isLikePatternCorrect(const _str& pattern);
LikeComparer* defaultLikeComparer(const _str& pattern);
LikeComparer* parseLikeComparer(const _str& pattern);


// operator LIKE with pattern initialized with a string literal
struct LikeConst : Generator<_boo>
{
public:
   LikeConst(Generator<_str>* val, const _str& pattern);

   ~LikeConst() {
      delete value;
      delete comparer;
   }

   _boo getValue() override;

private:
   Generator<_str>* value;
   const LikeComparer* comparer;
};


// operator LIKE with pattern initialized with any string expression
struct Like : Generator<_boo>
{
public:
   Like(Generator<_str>* val, Generator<_str>* pat) : value(val), pattern(pat),
      comparer(nullptr), hasPrev(false), prevHash(0) { };

   ~Like() {
      delete value;
      delete pattern;
      if (hasPrev) {
         delete comparer;
      }
   }

   _boo getValue() override;

private:
   Generator<_str>* value;
   Generator<_str>* pattern;

   LikeComparer* comparer;
   _boo hasPrev;
   _size prevHash;
};


//  %exa[m-v]__pl_e%
struct LC_Default : LikeComparer
{
public:
   LC_Default(const _str& pat);
   _boo compareToPattern(const _str& value) const override;

private:
   const _str pattern;
};


//  %exam__pl_e%
struct LC_DefaultButNoBrackets : LikeComparer
{
public:
   LC_DefaultButNoBrackets(const _str& pat);
   _boo compareToPattern(const _str& value) const override;

private:
   const _str pattern;
};


//  example%
struct LC_StartsWith : LikeComparer
{
public:
   LC_StartsWith(const _str& pat);
   _boo compareToPattern(const _str& value) const override;

private:
   _size length;
   _str start;
};


//  %example
struct LC_EndsWith : LikeComparer
{
public:
   LC_EndsWith(const _str& pat);
   _boo compareToPattern(const _str& value) const override;

private:
   _size length;
   _str end;
};


//  %example%
struct LC_Contains : LikeComparer
{
public:
   LC_Contains(const _str& pat);
   _boo compareToPattern(const _str& value) const override;

private:
   _size length;
   _str string;
};


//  e%
struct LC_StartsWithChar : LikeComparer
{
public:
   LC_StartsWithChar(const _str& pat) : ch(pat[0]) {};
   _boo compareToPattern(const _str& value) const override;

private:
   const _char ch;
};


//  %e
struct LC_EndsWithChar : LikeComparer
{
public:
   LC_EndsWithChar(const _str& pat) : ch(pat[1]) {};
   _boo compareToPattern(const _str& value) const override;

private:
   const _char ch;
};


//  %e%
struct LC_ContainsChar : LikeComparer
{
public:
   LC_ContainsChar(const _str& pat) : ch(pat[1]) {};
   _boo compareToPattern(const _str& value) const override;

private:
   const _char ch;
};


//  _example
struct LC_UnderscoreStart : LikeComparer
{
public:
   LC_UnderscoreStart(const _str& pat) : pattern(pat),
      length(pat.size()) {};
   _boo compareToPattern(const _str& value) const override;

private:
   const _str pattern;
   const _size length;
};


//  example_
struct LC_UnderscoreEnd : LikeComparer
{
public:
   LC_UnderscoreEnd(const _str& pat) : pattern(pat),
      length(pat.size()), lengthMinusOne(pat.size() - 1) {};
   _boo compareToPattern(const _str& value) const override;

private:
   const _str pattern;
   const _size length;
   const _size lengthMinusOne;
};


//  _example_
struct LC_UnderscoreStartEnd : LikeComparer
{
public:
   LC_UnderscoreStartEnd(const _str& pat) : pattern(pat),
      length(pat.size()), lengthMinusOne(pat.size() - 1) {};
   _boo compareToPattern(const _str& value) const override;

private:
   const _str pattern;
   const _size length;
   const _size lengthMinusOne;
};


//  example
struct LC_Equals : LikeComparer
{
public:
   LC_Equals(const _str& pat) : pattern(pat) {};
   _boo compareToPattern(const _str& value) const override;

private:
   const _str pattern;
};


//  case that is always true / always false
struct LC_Constant : LikeComparer
{
public:
   LC_Constant(const _boo& cnst) : constant(cnst) {};
   _boo compareToPattern(const _str& value) const override;

private:
   const _boo constant;
};



//  string has to contain certain amount of characters
struct LC_ConstantLength : LikeComparer
{
public:
   LC_ConstantLength(const _size& len) : length(len) {};
   _boo compareToPattern(const _str& value) const override;

private:
   const _size length;
};


//  _example%
struct LC_UnderscorePercent : LikeComparer
{
public:
   LC_UnderscorePercent(const _str& pat);
   _boo compareToPattern(const _str& value) const override;

private:
   _size length;
   _str start;
};


//  %example_
struct LC_PercentUnderscore : LikeComparer
{
public:
   LC_PercentUnderscore(const _str& pat);
   _boo compareToPattern(const _str& value) const override;

private:
   _size length;
   _str end;
};


//  string has to contain certain amount of characters and they are all digits
struct LC_OnlyDigits : LikeComparer
{
public:
   LC_OnlyDigits(const _size& len) : length(len) {};
   _boo compareToPattern(const _str& value) const override;

private:
   const _size length;
};


//  _exa_mpl_e
struct LC_Field_U : LikeComparer
{
public:
   LC_Field_U(const _str& pat);
   _boo compareToPattern(const _str& value) const override;

private:
   _size length;
   _str pattern;
   std::vector<_boo> isUnderscore;
};


//  #exa#mpl#e
struct LC_Field_H : LikeComparer
{
public:
   LC_Field_H(const _str& pat);
   _boo compareToPattern(const _str& value) const override;

private:
   _size length;
   _str pattern;
   std::vector<_boo> isHash;
};


//  #exa__mpl#e_e
struct LC_Field_UH : LikeComparer
{
public:
   LC_Field_UH(const _str& pat);
   _boo compareToPattern(const _str& value) const override;

private:
   _size length;
   _str pattern;
   std::vector<_boo> isUnderscore;
   std::vector<_boo> isHash;
};


//  [abc]
struct LC_SingleSet : LikeComparer
{
public:
   LC_SingleSet(const _str& pat);
   _boo compareToPattern(const _str& value) const override;

private:
   std::unordered_set<_char> chars;
};


//  [a-h]
struct LC_SingleRange : LikeComparer
{
public:
   LC_SingleRange(const _str& pat);
   _boo compareToPattern(const _str& value) const override;

private:
   _char first;
   _char last;
};




#endif /* GEN_LIKE_H */

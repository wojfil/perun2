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


_boo correctLikePattern(const _str& pattern);
LikeComparer* parseLikeComparer(const _str& pattern);


// operator LIKE with pattern initialized with a string literal
struct LikeConst : Generator<_boo>
{
public:
   LikeConst(Generator<_str>* val, const _str pattern);

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


//  %exam__pl_e%
struct LikeComparer_Default : LikeComparer
{
public:
   LikeComparer_Default(const _str& pat) : pattern(pat) { };
   _boo compareToPattern(const _str& value) const override;

private:
   const _str pattern;
};


//  example%
struct LikeComparer_StartsWith : LikeComparer
{
public:
   LikeComparer_StartsWith(const _str& pat);
   _boo compareToPattern(const _str& value) const override;

private:
   _size length;
   _str start;
};


//  %example
struct LikeComparer_EndsWith : LikeComparer
{
public:
   LikeComparer_EndsWith(const _str& pat);
   _boo compareToPattern(const _str& value) const override;

private:
   _size length;
   _str end;
};


//  %example%
struct LikeComparer_Contains : LikeComparer
{
public:
   LikeComparer_Contains(const _str& pat);
   _boo compareToPattern(const _str& value) const override;

private:
   _size length;
   _str string;
};


//  e%
struct LikeComparer_StartsWithChar : LikeComparer
{
public:
   LikeComparer_StartsWithChar(const _str& pat) : ch(pat[0]) {};
   _boo compareToPattern(const _str& value) const override;

private:
   const _char ch;
};


//  %e
struct LikeComparer_EndsWithChar : LikeComparer
{
public:
   LikeComparer_EndsWithChar(const _str& pat) : ch(pat[1]) {};
   _boo compareToPattern(const _str& value) const override;

private:
   const _char ch;
};


//  %e%
struct LikeComparer_ContainsChar : LikeComparer
{
public:
   LikeComparer_ContainsChar(const _str& pat) : ch(pat[1]) {};
   _boo compareToPattern(const _str& value) const override;

private:
   const _char ch;
};


//  _example
struct LikeComparer_UnderscoreStart : LikeComparer
{
public:
   LikeComparer_UnderscoreStart(const _str& pat) : pattern(pat),
      length(pat.size()) {};
   _boo compareToPattern(const _str& value) const override;

private:
   const _str pattern;
   const _size length;
};


//  example_
struct LikeComparer_UnderscoreEnd : LikeComparer
{
public:
   LikeComparer_UnderscoreEnd(const _str& pat) : pattern(pat),
      length(pat.size()), lengthMinusOne(pat.size() - 1) {};
   _boo compareToPattern(const _str& value) const override;

private:
   const _str pattern;
   const _size length;
   const _size lengthMinusOne;
};


//  _example_
struct LikeComparer_UnderscoreStartEnd : LikeComparer
{
public:
   LikeComparer_UnderscoreStartEnd(const _str& pat) : pattern(pat),
      length(pat.size()), lengthMinusOne(pat.size() - 1) {};
   _boo compareToPattern(const _str& value) const override;

private:
   const _str pattern;
   const _size length;
   const _size lengthMinusOne;
};


//  example
struct LikeComparer_Equals : LikeComparer
{
public:
   LikeComparer_Equals(const _str& pat) : pattern(pat) {};
   _boo compareToPattern(const _str& value) const override;

private:
   const _str pattern;
};


//  case that is always true / always false
struct LikeComparer_Constant : LikeComparer
{
public:
   LikeComparer_Constant(const _boo& cnst) : constant(cnst) {};
   _boo compareToPattern(const _str& value) const override;

private:
   const _boo constant;
};



//  string has to contain certain amount of characters
struct LikeComparer_ConstantLength : LikeComparer
{
public:
   LikeComparer_ConstantLength(const _size& len) : length(len) {};
   _boo compareToPattern(const _str& value) const override;

private:
   const _size length;
};


//  _example%
struct LikeComparer_UnderscorePercent : LikeComparer
{
public:
   LikeComparer_UnderscorePercent(const _str& pat);
   _boo compareToPattern(const _str& value) const override;

private:
   _size length;
   _str start;
};


//  %example_
struct LikeComparer_PercentUnderscore : LikeComparer
{
public:
   LikeComparer_PercentUnderscore(const _str& pat);
   _boo compareToPattern(const _str& value) const override;

private:
   _size length;
   _str end;
};


//  string has to contain cetrain amount of characters and they are all digits
struct LikeComparer_OnlyDigits : LikeComparer
{
public:
   LikeComparer_OnlyDigits(const _size& len) : length(len) {};
   _boo compareToPattern(const _str& value) const override;

private:
   const _size length;
};


//  _exa_mpl_e
struct LikeComparer_Field_U : LikeComparer
{
public:
   LikeComparer_Field_U(const _str& pat);
   _boo compareToPattern(const _str& value) const override;

private:
   _size length;
   _str pattern;
   std::vector<_boo> isUnderscore;
};


//  #exa#mpl#e
struct LikeComparer_Field_H : LikeComparer
{
public:
   LikeComparer_Field_H(const _str& pat);
   _boo compareToPattern(const _str& value) const override;

private:
   _size length;
   _str pattern;
   std::vector<_boo> isHash;
};


//  #exa__mpl#e_e
struct LikeComparer_Field_UH : LikeComparer
{
public:
   LikeComparer_Field_UH(const _str& pat);
   _boo compareToPattern(const _str& value) const override;

private:
   _size length;
   _str pattern;
   std::vector<_boo> isUnderscore;
   std::vector<_boo> isHash;
};


//  [abc]
struct LikeComparer_SingleSet : LikeComparer
{
public:
   LikeComparer_SingleSet(const _str& pat);
   _boo compareToPattern(const _str& value) const override;

private:
   std::unordered_set<_char> chars;
};


//  [a-h]
struct LikeComparer_SingleRange : LikeComparer
{
public:
   LikeComparer_SingleRange(const _str& pat);
   _boo compareToPattern(const _str& value) const override;

private:
   _char first;
   _char last;
};




#endif /* GEN_LIKE_H */

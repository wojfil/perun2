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

#pragma once

#include "../datatype.h"
#include "../generator.h"


namespace perun2::gen
{

// for every 3 characters in the pattern, allow 1 mistake
p_constexpr p_int RESEMBLANCE_MISTAKES_ALLOWED = 3;

p_constexpr p_ndouble RESEMBLANCE_RATIO = 
   NDOUBLE_ONE - (NDOUBLE_ONE / static_cast<p_ndouble>(RESEMBLANCE_MISTAKES_ALLOWED));


// operator RESEMBLES with pattern initialized with a string literal
struct ResemblesConst : Generator<p_bool>
{
public:
   ResemblesConst(p_genptr<p_str>& val, const p_str& pat);
   p_bool getValue() override;

private:
   p_genptr<p_str> value;
   p_str pattern;
   const p_int mistakesAllowed;
};


// operator RESEMBLES with pattern initialized with any string expression
struct Resembles : Generator<p_bool>
{
public:
   Resembles(p_genptr<p_str>& val, p_genptr<p_str>& pat);
   p_bool getValue() override;

private:
   p_genptr<p_str> value;
   p_genptr<p_str> pattern;
};


// before the Resemblance algorithm starts
// its arguments should be transformed to raw lowercase
void prepareForResemblance(p_str& value);

// the main Resemblance algorithm
p_ndouble str_resemblance(const p_str& value, const p_str& pattern);
static p_int minOfThree(p_int a, p_int b, p_int c);

// this implementation of Damerau-Levenshtein Distance calculates
// minimum of the distances between str2 and any substring of str1 that starts at index 0
// so, for str1='abcd', these substrings would be 'a', 'ab', 'abc' and 'abcd'
static p_int multiDamerauLevenshteinDistance(const p_str& str1, const p_str& str2);

static p_int resemblanceMistakesAllowed(const p_str& pattern);

}

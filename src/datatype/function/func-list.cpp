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

#include "func-list.h"
#include "../../lexer.h"
#include "../../unicode/convert.h"


namespace perun2::func
{


p_list F_Characters::getValue()
{
   const p_str v = arg1->getValue();

   if (v.empty()) {
      return p_list();
   }

   const p_list graphemes = toGraphemes(v, *(perun2.graphemeIterator.get()));
   return graphemes;
}


p_list F_Split::getValue()
{
   p_str v1 = arg1->getValue();

   if (v1.empty()) {
      return p_list();
   }

   const p_str v2 = arg2->getValue();
   const p_list graphemes = toGraphemes(v1, *(perun2.graphemeIterator.get()));
   const p_list graphemes2 = toGraphemes(v2, *(perun2.graphemeIterator.get()));

   if (graphemes2.empty()) {
      return graphemes;
   }

   if (graphemes2.size() == 1) {
      const p_str& sep = graphemes2[0];
      
      p_str temp;
      p_list r;

      for (const p_str& g : graphemes) {
         if (g == sep) {
            r.emplace_back(temp);

            if (! temp.empty()) {
               temp.clear();
            }
         }
         else {
            temp += g;
         }
      }

      r.emplace_back(temp);
      return r;
   }

   const p_list& sep = graphemes2;
   p_str temp;
   p_list r;

   for (size_t i = 0; i < graphemes.size(); i++) {
      const p_str& g = graphemes[i];

      if (g == sep[0] && (i + sep.size()) <= graphemes.size()) {
         p_bool fit = true;

         for (size_t j = 1; j < sep.size(); j++) {
            if (graphemes[i + j] != sep[j]) {
               fit = false;
               break;
            }
         }

         if (fit) {
            r.emplace_back(temp);

            if (! temp.empty()) {
               temp.clear();
            }

            i += sep.size() - 1;
            continue;
         }
      }

      temp += g;
   }

   r.emplace_back(temp);
   return r;
}


p_list F_Words::getValue()
{
   p_str value = arg1->getValue();

   if (value.empty()) {
      return p_list();
   }

   if (value.size() == 1) {
      return char_isAlpha(value[0]) ? p_list{value} : p_list();
   }

   if (hasOnlyOneCharGraphemes(value)) {
      if (value.size() == 2) {
         if (char_isAlpha(value[0])) {
            if (char_isAlpha(value[1])) {
               return p_list{value};
            }
            else {
               value.pop_back();
               return p_list{value};
            }
         }
         else {
            if (char_isAlpha(value[1])) {
               value.erase(value.begin());
               return p_list{value};
            }
            else {
               return p_list();
            }
         }
      }

      p_list words;
      p_bool prevLetter = false;
      p_size start = 0;

      for (p_size i = 0; i < value.size(); i++) {
         const p_bool isLetter = char_isAlpha(value[i]);
         if (isLetter) {
            if (!prevLetter) {
               start = i;
            }
         }
         else {
            if (prevLetter) {
               words.emplace_back(value.substr(start, i - start));
            }
         }
         prevLetter = isLetter;
      }

      if (prevLetter) {
         words.emplace_back(value.substr(start));
      }

      return words;
   }

   const p_list graphemes = toGraphemes(value, *(perun2.graphemeIterator.get()));
   p_list words;
   p_str tempWord;

   for (const p_str& graph : graphemes) {
      const p_bool isLetter = isLetterGrapheme(graph);

      if (isLetter) {
         tempWord += graph;
      }
      else {
         if (! tempWord.empty()) {
            words.emplace_back(tempWord);
            tempWord.clear();
         }
      }
   }

   if (! tempWord.empty()) {
      words.emplace_back(tempWord);
   }

   return words;
}

inline p_nint F_Numbers::fromChar(const p_char ch)
{
   return static_cast<p_nint>(ch - CHAR_0);
}

p_nlist F_Numbers::getValue()
{
   p_str value = arg1->getValue();

   if (value.empty()) {
      return p_nlist();
   }

   if (value.size() == 1) {
      return char_isDigit(value[0])
         ? p_nlist{fromChar(value[0])}
         : p_nlist();
   }

   p_nlist numbers;
   p_bool prevDigit = false;
   p_size start = 0;

   for (p_size i = 0; i < value.size(); i++) {
      const p_bool isDigit = char_isDigit(value[i]);
      if (isDigit) {
         if (!prevDigit) {
            start = i;
         }
      }
      else {
         if (prevDigit) {
            try {
               const p_nint ii = std::stoll(utf32_to_utf8(value.substr(start, i - start)));
               numbers.emplace_back(ii);
            }
            catch (...) { }
         }
      }
      prevDigit = isDigit;
   }

   if (prevDigit) {
      try {
         const p_nint ii = std::stoll(utf32_to_utf8(value.substr(start)));
         numbers.emplace_back(ii);
      }
      catch (...) { }
   }

   return numbers;
}

}

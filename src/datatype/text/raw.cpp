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

#include "raw.h"


namespace perun2
{

void str_toRaw(p_str& value)
{
   p_constexpr uint32_t DOUBLECHARS_NONE               = 0;
   p_constexpr uint32_t DOUBLECHARS_LOWER_OE           = 1 << 0;
   p_constexpr uint32_t DOUBLECHARS_LOWER_AE           = 1 << 1;
   p_constexpr uint32_t DOUBLECHARS_LOWER_AE_ACCENT    = 1 << 2;
   p_constexpr uint32_t DOUBLECHARS_LOWER_AE_MACRON    = 1 << 3;
   p_constexpr uint32_t DOUBLECHARS_UPPER_OE           = 1 << 4;
   p_constexpr uint32_t DOUBLECHARS_UPPER_AE           = 1 << 5;
   p_constexpr uint32_t DOUBLECHARS_UPPER_AE_ACCENT    = 1 << 6;
   p_constexpr uint32_t DOUBLECHARS_UPPER_AE_MACRON    = 1 << 7;

   uint32_t doubleChars = DOUBLECHARS_NONE;

   for (p_char& ch : value) {
      switch (ch) {
         case L'œ':
         {
            doubleChars |= DOUBLECHARS_LOWER_OE;
            break;
         }
         case L'æ':
         {
            doubleChars |= DOUBLECHARS_LOWER_AE;
            break;
         }
         case L'ǽ':
         {
            doubleChars |= DOUBLECHARS_LOWER_AE_ACCENT;
            break;
         }
         case L'ǣ':
         {
            doubleChars |= DOUBLECHARS_LOWER_AE_MACRON;
            break;
         }
         case L'Œ':
         {
            doubleChars |= DOUBLECHARS_UPPER_OE;
            break;
         }
         case L'Æ':
         {
            doubleChars |= DOUBLECHARS_UPPER_AE;
            break;
         }
         case L'Ǽ':
         {
            doubleChars |= DOUBLECHARS_UPPER_AE_ACCENT;
            break;
         }
         case L'Ǣ':
         {
            doubleChars |= DOUBLECHARS_UPPER_AE_MACRON;
            break;
         }
         case L'Ą':
         //case L'A̧':
         //case L'Ą̊': 
         case L'Á':
         case L'À':
         case L'Ȧ':
         case L'Â':
         case L'Ä':
         case L'Ǟ':
         case L'Ǎ':
         case L'Ă':
         case L'Ā':
         case L'Ã':
         case L'Å':
         case L'Ǻ':
         {
            ch = L'A';
            break;
         }
         case L'Ɓ':
         case L'Ḅ':
         {
            ch = L'B';
            break;
         }
         case L'Ƈ':
         case L'Ç':
         case L'Ć':
         case L'Ċ':
         case L'Ĉ':
         case L'Č':
         {
            ch = L'C';
            break;
         }
         case L'Đ':
         case L'Ɗ':
         case L'Ɖ':
         case L'Ď':
         case L'Ḍ':
         case L'Ḑ':
         case L'Ḓ':
         {
            ch = L'D';
            break;
         }
         case L'Ę':
         case L'Ȩ':
         //case L'Ə̧':
         //case L'Ɛ̧':
         case L'É':
         case L'È':
         case L'Ė':
         case L'Ê':
         case L'Ë':
         case L'Ě':
         case L'Ĕ':
         case L'Ē':
         case L'Ẽ':
         //case L'E̊':
         case L'Ẹ':
         {
            ch = L'E';
            break;
         }
         case L'Ƒ':
         {
            ch = L'F';
            break;
         }
         case L'Ǥ':
         case L'Ɠ':
         case L'Ǵ':
         case L'Ġ':
         case L'Ĝ':
         case L'Ǧ':
         case L'Ğ':
         //case L'G̃':
         case L'Ģ':
         {
            ch = L'G';
            break;
         }
         case L'Ħ':
         case L'Ɦ':
         case L'Ĥ':
         case L'Ḥ':
         {
            ch = L'H';
            break;
         }
         case L'Į':
         //case L'I̧':
         case L'Ɨ':
         //case L'Ɨ̧':
         case L'Í':
         case L'Ì':
         case L'İ':
         case L'Î':
         case L'Ï':
         case L'Ǐ':
         case L'Ĭ':
         case L'Ī':
         case L'Ĩ':
         case L'Ị':
         {
            ch = L'I';
            break;
         }
         case L'Ĵ':
         {
            ch = L'J';
            break;
         }
         case L'Ƙ':
         case L'Ķ':
         case L'Ǩ':
         {
            ch = L'K';
            break;
         }
         case L'Ł':
         case L'Ĺ':
         case L'Ļ':
         case L'Ľ':
         case L'Ŀ':
         case L'Ḷ':
         case L'Ḽ':
         {
            ch = L'L';
            break;
         }
         //case L'M̧':
         //case L'M̂':
         //case L'M̄':
         //{
         //   ch = L'M';
         //   break;
         //}
         case L'Ɲ':
         // notice that there is no ʼN character here
         // the N-apostrophe character from Afrikaans has only lower case variant
         // upper case has never been included in any international keyboard
         case L'Ń':
         //case L'N̂':
         case L'Ṅ':
         //case L'N̈':
         case L'Ň':
         //case L'N̄':
         case L'Ñ':
         case L'Ņ':
         case L'Ṋ':
         {
            ch = L'N';
            break;
         }
         case L'Ǫ':
         //case L'O̧':
         case L'Ø':
         case L'Ơ':
         //case L'Ɔ̧':
         case L'Ó':
         case L'Ò':
         case L'Ȯ':
         case L'Ȱ':
         case L'Ô':
         case L'Ö':
         case L'Ȫ':
         case L'Ǒ':
         case L'Ŏ':
         case L'Ō':
         case L'Õ':
         case L'Ȭ':
         case L'Ő':
         case L'Ọ':
         case L'Ǿ':
         {
            ch = L'O';
            break;
         }
         case L'Ƥ':
         //case L'P̄':
         {
            ch = L'P';
            break;
         }
         case L'Ɍ':
         case L'Ŕ':
         case L'Ř':
         case L'Ŗ':
         case L'Ṛ':
         {
            ch = L'R';
            break;
         }
         case L'Ş':
         case L'Ś':
         case L'Ŝ':
         case L'Ṡ':
         case L'Š':
         case L'Ș':
         case L'Ṣ':
         {
            ch = L'S';
            break;
         }
         case L'Ƭ':
         case L'Ţ':
         case L'Ŧ':
         case L'Ť':
         case L'Ț':
         case L'Ṭ':
         case L'Ṱ':
         {
            ch = L'T';
            break;
         }
         case L'Ų':
         //case L'U̧':
         case L'Ư':
         case L'Ʉ':
         case L'Ú':
         case L'Ù':
         case L'Û':
         case L'Ü':
         case L'Ǔ':
         case L'Ŭ':
         case L'Ū':
         case L'Ũ':
         case L'Ű':
         case L'Ů':
         case L'Ụ':
         {
            ch = L'U';
            break;
         }
         case L'Ẃ':
         case L'Ẁ':
         case L'Ŵ':
         case L'Ẅ':
         {
            ch = L'W';
            break;
         }
         case L'Ẋ':
         {
            ch = L'X';
            break;
         }

         //case L'Y̨':
         case L'Ƴ':
         case L'Ý':
         case L'Ỳ':
         case L'Ŷ':
         case L'Ÿ':
         case L'Ȳ':
         case L'Ỹ':
         {
            ch = L'Y';
            break;
         }
         case L'Ź':	
         case L'Ż':
         case L'Ž':
         case L'Ẓ':
         {
            ch = L'Z';
            break;
         }
         case L'ą':
         //case L'a̧':
         //case L'ą̊':
         case L'á':
         case L'à':
         case L'ȧ':
         case L'â':
         case L'ä':
         case L'ǟ':
         case L'ǎ':
         case L'ă':
         case L'ā':
         case L'ã':
         case L'å':
         case L'ǻ': {
            ch = L'a';
            break;
         }
         case L'ɓ':
         case L'ḅ': {
            ch = L'b';
            break;
         }
         case L'ƈ':
         case L'ç':
         case L'ć':
         case L'ċ':
         case L'ĉ':
         case L'č': {
            ch = L'c';
            break;
         }
         case L'đ':
         case L'ɗ':
         case L'ɖ':
         case L'ď':
         case L'ḍ':
         case L'ḑ':
         case L'ḓ': {
            ch = L'd';
            break;
         }
         case L'ę':
         case L'ȩ':
         //case L'ə̧':
         //case L'ɛ̧':
         case L'é':
         case L'è':
         case L'ė':
         case L'ê':
         case L'ë':
         case L'ě':
         case L'ĕ':
         case L'ē':
         case L'ẽ':
         //case L'e̊':
         case L'ẹ': {
            ch = L'e';
            break;
         }
         case L'ƒ': {
            ch = L'f';
            break;
         }
         case L'ǥ':
         case L'ɠ':
         case L'ǵ':
         case L'ġ':
         case L'ĝ':
         case L'ǧ':
         case L'ğ':
         //case L'g̃':
         case L'ģ': {
            ch = L'g';
            break;
         }
         case L'ħ':
         case L'ɦ':
         case L'ĥ':
         case L'ḥ': {
            ch = L'h';
            break;
         }
         case L'į':
         //case L'i̧':
         case L'ɨ':
         //case L'ɨ̧':
         case L'í':
         case L'ì':
         //case L'i':
         case L'î':
         case L'ï':
         case L'ǐ':
         case L'ĭ':
         case L'ī':
         case L'ĩ':
         case L'ị': {
            ch = L'i';
            break;
         }
         case L'ĵ': {
            ch = L'j';
            break;
         }
         case L'ƙ':
         case L'ķ':
         case L'ǩ': {
            ch = L'k';
            break;
         }
         case L'ł':
         case L'ĺ':
         case L'ļ':
         case L'ľ':
         case L'ŀ':
         case L'ḷ':
         case L'ḽ': {
            ch = L'l';
            break;
         }
         //case L'm̧':
         //case L'm̂':
         //case L'm̄': {
         //   ch = L'm';
         //   break;
         //}
         case L'ɲ':
         case L'ŉ':
         case L'ń':
         //case L'n̂':
         case L'ṅ':
         //case L'n̈':
         case L'ň':
         //case L'n̄':
         case L'ñ':
         case L'ņ':
         case L'ṋ': {
            ch = L'n';
            break;
         }
         case L'ǫ':
         //case L'o̧':
         case L'ø':
         case L'ơ':
         //case L'ɔ̧':
         case L'ó':
         case L'ò':
         case L'ȯ':
         case L'ȱ':
         case L'ô':
         case L'ö':
         case L'ȫ':
         case L'ǒ':
         case L'ŏ':
         case L'ō':
         case L'õ':
         case L'ȭ':
         case L'ő':
         case L'ọ':
         case L'ǿ': {
            ch = L'o';
            break;
         }
         case L'ƥ':
         //case L'p̄': 
         {
            ch = L'p';
            break;
         }
         case L'ɍ':
         case L'ŕ':
         case L'ř':
         case L'ŗ':
         case L'ṛ': {
            ch = L'r';
            break;
         }
         case L'ş':
         case L'ś':
         case L'ŝ':
         case L'ṡ':
         case L'š':
         case L'ș':
         case L'ṣ': {
            ch = L's';
            break;
         }
         case L'ƭ':
         case L'ţ':
         case L'ŧ':
         case L'ť':
         case L'ț':
         case L'ṭ':
         case L'ṱ': {
            ch = L't';
            break;
         }
         case L'ų':
         //case L'u̧':
         case L'ư':
         case L'ʉ':
         case L'ú':
         case L'ù':
         case L'û':
         case L'ü':
         case L'ǔ':
         case L'ŭ':
         case L'ū':
         case L'ũ':
         case L'ű':
         case L'ů':
         case L'ụ': {
            ch = L'u';
            break;
         }
         case L'ẃ':
         case L'ẁ':
         case L'ŵ':
         case L'ẅ': {
            ch = L'w';
            break;
         }
         case L'ẋ': {
            ch = L'x';
            break;
         }
         //case L'y̨':
         case L'ƴ':
         case L'ý':
         case L'ỳ':
         case L'ŷ':
         case L'ÿ':
         case L'ȳ':
         case L'ỹ': {
            ch = L'y';
            break;
         }
         case L'ź':
         case L'ż':
         case L'ž':
         case L'ẓ': {
            ch = L'z';
            break;
         }
      }
   }


   if (doubleChars == DOUBLECHARS_NONE) {
      return;
   }

   if (doubleChars & DOUBLECHARS_LOWER_OE) {
      replaceCharWithTwoChars(value, L'œ', L'o', L'e');
   }

   if (doubleChars & DOUBLECHARS_LOWER_AE) {
      replaceCharWithTwoChars(value, L'æ', L'a', L'e');
   }

   if (doubleChars & DOUBLECHARS_LOWER_AE_ACCENT) {
      replaceCharWithTwoChars(value, L'ǽ', L'a', L'e');
   }

   if (doubleChars & DOUBLECHARS_LOWER_AE_MACRON) {
      replaceCharWithTwoChars(value, L'ǣ', L'a', L'e');
   }

   if (doubleChars & DOUBLECHARS_UPPER_OE) {
      replaceCharWithTwoChars(value, L'Œ', L'O', L'E');
   }

   if (doubleChars & DOUBLECHARS_UPPER_AE) {
      replaceCharWithTwoChars(value, L'Æ', L'A', L'E');
   }

   if (doubleChars & DOUBLECHARS_UPPER_AE_ACCENT) {
      replaceCharWithTwoChars(value, L'Ǽ', L'A', L'E');
   }

   if (doubleChars & DOUBLECHARS_UPPER_AE_MACRON) {
      replaceCharWithTwoChars(value, L'Ǣ', L'A', L'E');
   }
}


inline static void replaceCharWithTwoChars(p_str& value,
   const p_char old, const p_char new1, const p_char new2)
{
   p_size pos = value.find(old);

   while (pos != p_str::npos) {
      value += value[value.size() - 1];

      for (p_int i = value.size() - 2; i > pos + 1; i--) {
         value[i] = value[i - 1];
      }

      value[pos] = new1;
      value[pos + 1] = new2;
      pos = value.find(old, pos + 2);
   }
}

}

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

#include "../../../include/perun2/datatype/text/raw.hpp"


namespace perun2
{

void str_toRaw(p_str& value)
{
   uint32_t doubleChars = TO_RAW_DOUBLECHARS_NONE;

   for (p_int i = 0; i < static_cast<p_int>(value.size()); i++) {
      switch (value[i]) {
         case 0x0153:
         {
            doubleChars |= TO_RAW_DOUBLECHARS_LOWER_OE;
            break;
         }
         case 0x00E6:
         {
            doubleChars |= TO_RAW_DOUBLECHARS_LOWER_AE;
            break;
         }
         case 0x01FD:
         {
            doubleChars |= TO_RAW_DOUBLECHARS_LOWER_AE_ACCENT;
            break;
         }
         case 0x01E3:
         {
            doubleChars |= TO_RAW_DOUBLECHARS_LOWER_AE_MACRON;
            break;
         }
         case 0x0152:
         {
            doubleChars |= TO_RAW_DOUBLECHARS_UPPER_OE;
            break;
         }
         case 0x00C6:
         {
            doubleChars |= TO_RAW_DOUBLECHARS_UPPER_AE;
            break;
         }
         case 0x01FC:
         {
            doubleChars |= TO_RAW_DOUBLECHARS_UPPER_AE_ACCENT;
            break;
         }
         case 0x01E2:
         {
            doubleChars |= TO_RAW_DOUBLECHARS_UPPER_AE_MACRON;
            break;
         }
         case 0x00DF: {
            doubleChars |= TO_RAW_DOUBLECHARS_ESZETT;
            break;
         }
         case 0x0104:
         case 0x00C1:
         case 0x00C0:
         case 0x0226:
         case 0x00C2:
         case 0x00C4:
         case 0x01DE:
         case 0x01CD:
         case 0x0102:
         case 0x0100:
         case 0x00C3:
         case 0x00C5:
         case 0x01FA:
         {
            value[i] = L'A';
            break;
         }
         case 0x0181:
         case 0x1E04:
         {
            value[i] = L'B';
            break;
         }
         case 0x0187:
         case 0x00C7:
         case 0x0106:
         case 0x010A:
         case 0x0108:
         case 0x010C:
         {
            value[i] = L'C';
            break;
         }
         case 0x0110:
         case 0x018A:
         case 0x0189:
         case 0x010E:
         case 0x1E0C:
         case 0x1E10:
         case 0x1E12:
         {
            value[i] = L'D';
            break;
         }
         case 0x0118:
         case 0x0228:
         case 0x00C9:
         case 0x00C8:
         case 0x0116:
         case 0x00CA:
         case 0x00CB:
         case 0x011A:
         case 0x0114:
         case 0x0112:
         case 0x1EBC:
         case 0x1EB8:
         {
            value[i] = L'E';
            break;
         }
         case 0x0191:
         {
            value[i] = L'F';
            break;
         }
         case 0x01E4:
         case 0x0193:
         case 0x01F4:
         case 0x0120:
         case 0x011C:
         case L'Ǧ':
         case L'Ğ':
         case L'Ģ':
         {
            value[i] = L'G';
            break;
         }
         case L'Ħ':
         case L'Ɦ':
         case L'Ĥ':
         case L'Ḥ':
         {
            value[i] = L'H';
            break;
         }
         case L'Į':
         case L'Ɨ':
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
            value[i] = L'I';
            break;
         }
         case L'Ĵ':
         {
            value[i] = L'J';
            break;
         }
         case L'Ƙ':
         case L'Ķ':
         case L'Ǩ':
         {
            value[i] = L'K';
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
            value[i] = L'L';
            break;
         }
         case L'Ɲ':
         // notice that there is no ʼN character here
         // the N-apostrophe character from Afrikaans has only lower case variant
         // upper case has never been included in any international keyboard
         case L'Ń':
         case L'Ṅ':
         case L'Ň':
         case L'Ñ':
         case L'Ņ':
         case L'Ṋ':
         {
            value[i] = L'N';
            break;
         }
         case L'Ǫ':
         case L'Ø':
         case L'Ơ':
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
            value[i] = L'O';
            break;
         }
         case L'Ƥ':
         {
            value[i] = L'P';
            break;
         }
         case L'Ɍ':
         case L'Ŕ':
         case L'Ř':
         case L'Ŗ':
         case L'Ṛ':
         {
            value[i] = L'R';
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
            value[i] = L'S';
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
            value[i] = L'T';
            break;
         }
         case L'Ų':
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
            value[i] = L'U';
            break;
         }
         case L'Ẃ':
         case L'Ẁ':
         case L'Ŵ':
         case L'Ẅ':
         {
            value[i] = L'W';
            break;
         }
         case L'Ẋ':
         {
            value[i] = L'X';
            break;
         }
         case L'Ƴ':
         case L'Ý':
         case L'Ỳ':
         case L'Ŷ':
         case L'Ÿ':
         case L'Ȳ':
         case L'Ỹ':
         {
            value[i] = L'Y';
            break;
         }
         case L'Ź':	
         case L'Ż':
         case L'Ž':
         case L'Ẓ':
         {
            value[i] = L'Z';
            break;
         }
         case L'ą':
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
            value[i] = L'a';
            break;
         }
         case L'ɓ':
         case L'ḅ': {
            value[i] = L'b';
            break;
         }
         case L'ƈ':
         case L'ç':
         case L'ć':
         case L'ċ':
         case L'ĉ':
         case L'č': {
            value[i] = L'c';
            break;
         }
         case L'đ':
         case L'ɗ':
         case L'ɖ':
         case L'ď':
         case L'ḍ':
         case L'ḑ':
         case L'ḓ': {
            value[i] = L'd';
            break;
         }
         case L'ę':
         case L'ȩ':
         case L'é':
         case L'è':
         case L'ė':
         case L'ê':
         case L'ë':
         case L'ě':
         case L'ĕ':
         case L'ē':
         case L'ẽ':
         case L'ẹ': {
            value[i] = L'e';
            break;
         }
         case L'ƒ': {
            value[i] = L'f';
            break;
         }
         case L'ǥ':
         case L'ɠ':
         case L'ǵ':
         case L'ġ':
         case L'ĝ':
         case L'ǧ':
         case L'ğ':
         case L'ģ': {
            value[i] = L'g';
            break;
         }
         case L'ħ':
         case L'ɦ':
         case L'ĥ':
         case L'ḥ': {
            value[i] = L'h';
            break;
         }
         case L'į':
         case L'ɨ':
         case L'í':
         case L'ì':
         case L'î':
         case L'ï':
         case L'ǐ':
         case L'ĭ':
         case L'ī':
         case L'ĩ':
         case L'ị': {
            value[i] = L'i';
            break;
         }
         case L'ĵ': {
            value[i] = L'j';
            break;
         }
         case L'ƙ':
         case L'ķ':
         case L'ǩ': {
            value[i] = L'k';
            break;
         }
         case L'ł':
         case L'ĺ':
         case L'ļ':
         case L'ľ':
         case L'ŀ':
         case L'ḷ':
         case L'ḽ': {
            value[i] = L'l';
            break;
         }
         case L'ɲ':
         case L'ŉ':
         case L'ń':
         case L'ṅ':
         case L'ň':
         case L'ñ':
         case L'ņ':
         case L'ṋ': {
            value[i] = L'n';
            break;
         }
         case L'ǫ':
         case L'ø':
         case L'ơ':
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
            value[i] = L'o';
            break;
         }
         case L'ƥ':
         {
            value[i] = L'p';
            break;
         }
         case L'ɍ':
         case L'ŕ':
         case L'ř':
         case L'ŗ':
         case L'ṛ': {
            value[i] = L'r';
            break;
         }
         case L'ş':
         case L'ś':
         case L'ŝ':
         case L'ṡ':
         case L'š':
         case L'ș':
         case L'ṣ': {
            value[i] = L's';
            break;
         }
         case L'ƭ':
         case L'ţ':
         case L'ŧ':
         case L'ť':
         case L'ț':
         case L'ṭ':
         case L'ṱ': {
            value[i] = L't';
            break;
         }
         case L'ų':
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
            value[i] = L'u';
            break;
         }
         case L'ẃ':
         case L'ẁ':
         case L'ŵ':
         case L'ẅ': {
            value[i] = L'w';
            break;
         }
         case L'ẋ': {
            value[i] = L'x';
            break;
         }
         case L'ƴ':
         case L'ý':
         case L'ỳ':
         case L'ŷ':
         case L'ÿ':
         case L'ȳ':
         case L'ỹ': {
            value[i] = L'y';
            break;
         }
         case L'ź':
         case L'ż':
         case L'ž':
         case L'ẓ': {
            value[i] = L'z';
            break;
         }
         default: {
            if (isCombiningMark(value[i])) {
               value.erase(i, 1);
               i--;
            }
            break;
         }
      }
   }

   if (doubleChars == TO_RAW_DOUBLECHARS_NONE) {
      return;
   }

   if (doubleChars & TO_RAW_DOUBLECHARS_LOWER_OE) {
      replaceCharWithTwoChars(value, 0x0153, L'o', L'e');
   }

   if (doubleChars & TO_RAW_DOUBLECHARS_LOWER_AE) {
      replaceCharWithTwoChars(value, 0x00E6, L'a', L'e');
   }

   if (doubleChars & TO_RAW_DOUBLECHARS_LOWER_AE_ACCENT) {
      replaceCharWithTwoChars(value, 0x01FD, L'a', L'e');
   }

   if (doubleChars & TO_RAW_DOUBLECHARS_LOWER_AE_MACRON) {
      replaceCharWithTwoChars(value, 0x01E3, L'a', L'e');
   }

   if (doubleChars & TO_RAW_DOUBLECHARS_UPPER_OE) {
      replaceCharWithTwoChars(value, 0x0152, L'O', L'E');
   }

   if (doubleChars & TO_RAW_DOUBLECHARS_UPPER_AE) {
      replaceCharWithTwoChars(value, 0x00C6, L'A', L'E');
   }

   if (doubleChars & TO_RAW_DOUBLECHARS_UPPER_AE_ACCENT) {
      replaceCharWithTwoChars(value, 0x01FC, L'A', L'E');
   }

   if (doubleChars & TO_RAW_DOUBLECHARS_UPPER_AE_MACRON) {
      replaceCharWithTwoChars(value, 0x01E2, L'A', L'E');
   }

   if (doubleChars & TO_RAW_DOUBLECHARS_ESZETT) {
      replaceCharWithTwoChars(value, 0x00DF, L's', L's');
   }
}


inline static void replaceCharWithTwoChars(p_str& value,
   const p_char old, const p_char new1, const p_char new2)
{
   p_size pos = value.find(old);

   while (pos != p_str::npos) {
      value += value[value.size() - 1];

      for (p_size i = value.size() - 2; i > pos + 1; i--) {
         value[i] = value[i - 1];
      }

      value[pos] = new1;
      value[pos + 1] = new2;
      pos = value.find(old, pos + 2);
   }
}


inline static p_bool isCombiningMark(const p_char ch)
{
   return (ch >= 0x0300 && ch <= 0x036F)       // Combining Diacritical Marks
       || (ch >= 0x1AB0 && ch <= 0x1AFF)       // Combining Diacritical Marks Extended
       || (ch >= 0x1DC0 && ch <= 0x1DFF)       // Combining Diacritical Marks Supplement
       || (ch >= 0x20D0 && ch <= 0x20FF)       // Combining Diacritical Marks for Symbols
       || (ch >= 0xFE20 && ch <= 0xFE2F);      // Combining Half Marks
}

}

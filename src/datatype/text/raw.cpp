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
   uint32_t doubleChars = RAW_DOUBLECHAR_NONE;

   for (p_char& ch : value) {
      switch (ch) {
         case L'œ':
         {
            doubleChars |= RAW_DOUBLECHAR_LOWER_OE;
            break;
         }
         case L'æ':
         {
            doubleChars |= RAW_DOUBLECHAR_LOWER_AE;
            break;
         }
         case L'ǽ':
         {
            doubleChars |= RAW_DOUBLECHAR_LOWER_AE_ACCENT;
            break;
         }
         case L'ǣ':
         {
            doubleChars |= RAW_DOUBLECHAR_LOWER_AE_MACRON;
            break;
         }
         case L'Œ':
         {
            doubleChars |= RAW_DOUBLECHAR_UPPER_OE;
            break;
         }
         case L'Æ':
         {
            doubleChars |= RAW_DOUBLECHAR_UPPER_AE;
            break;
         }
         case L'Ǽ':
         {
            doubleChars |= RAW_DOUBLECHAR_UPPER_AE_ACCENT;
            break;
         }
         case L'Ǣ':
         {
            doubleChars |= RAW_DOUBLECHAR_UPPER_AE_MACRON;
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
         case L'Ə̧':
         case L'Ɛ̧':
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
         case L'Ɔ̧':
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



// todo


// ą	a̧	ą̊	ɓ	ƈ	ç	đ	ɗ	ɖ	ę	ȩ	ə̧	ɛ̧	ƒ	ǥ	ɠ	ħ	ɦ	į	i̧	ɨ	ɨ̧	ƙ	ł	m̧	ɲ	ǫ	o̧	ø	ơ	ɔ̧	ƥ	ɍ	ş	ƭ	ţ	ŧ	ų	u̧	ư	ʉ	y̨	ƴ
// á	à	ȧ	â	ä	ǟ	ǎ	ă	ā	ã	å	ǻ			ḅ	ć	ċ	ĉ	č	ď	ḍ	ḑ	ḓ	é	è	ė	ê	ë	ě	ĕ	ē	ẽ	e̊	ẹ	ǵ	ġ	ĝ	ǧ	ğ	g̃	ģ	ĥ	ḥ
// í	ì	i	î	ï	ǐ	ĭ	ī	ĩ	ị	ĵ	ķ	ǩ	ĺ	ļ	ľ	ŀ	ḷ	ḽ	m̂	m̄	ŉ	ń	n̂	ṅ	n̈	ň	n̄	ñ	ņ	ṋ	ó	ò	ȯ	ȱ	ô	ö	ȫ	ǒ	ŏ	ō	õ	ȭ	ő	ọ	ǿ	ơ
// p̄	ŕ	ř	ŗ	ṛ	ś	ŝ	ṡ	š	ș	ṣ	ť	ț	ṭ	ṱ	ú	ù	û	ü	ǔ	ŭ	ū	ũ	ű	ů	ụ	ẃ	ẁ	ŵ	ẅ	ẋ	ý	ỳ	ŷ	ÿ	ȳ	ỹ	ź	ż	ž	ẓ



      }
   }


   if (doubleChars != RAW_DOUBLECHAR_NONE) {
      if (doubleChars & RAW_DOUBLECHAR_LOWER_OE) {
         replaceCharWithTwoChars(value, L'œ', L'o', L'e');
      }

      if (doubleChars & RAW_DOUBLECHAR_LOWER_AE) {
         replaceCharWithTwoChars(value, L'æ', L'a', L'e');
      }

      if (doubleChars & RAW_DOUBLECHAR_LOWER_AE_ACCENT) {
         replaceCharWithTwoChars(value, L'ǽ', L'a', L'e');
      }

      if (doubleChars & RAW_DOUBLECHAR_LOWER_AE_MACRON) {
         replaceCharWithTwoChars(value, L'ǣ', L'a', L'e');
      }

      if (doubleChars & RAW_DOUBLECHAR_UPPER_OE) {
         replaceCharWithTwoChars(value, L'Œ', L'O', L'E');
      }

      if (doubleChars & RAW_DOUBLECHAR_UPPER_AE) {
         replaceCharWithTwoChars(value, L'Æ', L'A', L'E');
      }

      if (doubleChars & RAW_DOUBLECHAR_UPPER_AE_ACCENT) {
         replaceCharWithTwoChars(value, L'Ǽ', L'A', L'E');
      }

      if (doubleChars & RAW_DOUBLECHAR_UPPER_AE_MACRON) {
         replaceCharWithTwoChars(value, L'Ǣ', L'A', L'E');
      }
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

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

void toRaw(p_str& value)
{
   for (p_char& ch : value) {
      switch (ch) {
         case L'Ą':	
         //case L'A̧':
         //case L'Ą̊': 
         {
            ch = L'A';
            break;
         }
         case L'Ɓ':
         {
            ch = L'B';
            break;
         }
         case L'Ƈ':
         case L'Ç':
         {
            ch = L'C';
            break;
         }
         case L'Đ':
         case L'Ɗ':
         case L'Ɖ':
         {
            ch = L'D';
            break;
         }
         case L'Ę':
         case L'Ȩ':
         case L'Ə̧':
         case L'Ɛ̧':
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
         {
            ch = L'G';
            break;
         }
         case L'Ħ':
         case L'Ɦ':
         {
            ch = L'H';
            break;
         }
         case L'Į':
         //case L'I̧':
         case L'Ɨ':
         //case L'Ɨ̧':
         {
            ch = L'I';
            break;
         }
         case L'Ƙ':
         {
            ch = L'K';
            break;
         }
         case L'Ł':
         {
            ch = L'L';
            break;
         }
         //case L'M̧':
         //{
         //   ch = L'M';
         //   break;
         //}
         case L'Ɲ':
         {
            ch = L'N';
            break;
         }
         case L'Ǫ':
         //case L'O̧':
         case L'Ø':
         case L'Ơ':
         case L'Ɔ̧':
         {
            ch = L'O';
            break;
         }
         case L'Ƥ':
         {
            ch = L'P';
            break;
         }
         case L'Ɍ':
         {
            ch = L'R';
            break;
         }
         case L'Ş':
         {
            ch = L'S';
            break;
         }
         case L'Ƭ':
         case L'Ţ':
         case L'Ŧ':
         {
            ch = L'T';
            break;
         }
         case L'Ų':
         //case L'U̧':
         case L'Ư':
         case L'Ʉ':
         {
            ch = L'U';
            break;
         }
         //case L'Y̨':
         case L'Ƴ':
         {
            ch = L'Y';
            break;
         }




// todo

// Á	À	Ȧ	Â	Ä	Ǟ	Ǎ	Ă	Ā	Ã	Å	Ǻ	Ǽ	Ǣ	Ḅ	Ć	Ċ	Ĉ	Č	Ď	Ḍ	Ḑ	Ḓ	É	È	Ė	Ê	Ë	Ě	Ĕ	Ē	Ẽ	E̊	Ẹ	Ǵ	Ġ	Ĝ	Ǧ	Ğ	G̃	Ģ	Ĥ	Ḥ
// Í	Ì	İ	Î	Ï	Ǐ	Ĭ	Ī	Ĩ	Ị	Ĵ	Ķ	Ǩ	Ĺ	Ļ	Ľ	Ŀ	Ḷ	Ḽ	M̂	M̄	ʼN	Ń	N̂	Ṅ	N̈	Ň	N̄	Ñ	Ņ	Ṋ	Ó	Ò	Ȯ	Ȱ	Ô	Ö	Ȫ	Ǒ	Ŏ	Ō	Õ	Ȭ	Ő	Ọ	Ǿ	Ơ
// P̄	Ŕ	Ř	Ŗ	Ṛ	Ś	Ŝ	Ṡ	Š	Ș	Ṣ	Ť	Ț	Ṭ	Ṱ	Ú	Ù	Û	Ü	Ǔ	Ŭ	Ū	Ũ	Ű	Ů	Ụ	Ẃ	Ẁ	Ŵ	Ẅ	Ẋ	Ý	Ỳ	Ŷ	Ÿ	Ȳ	Ỹ	Ź	Ż	Ž	Ẓ 



// ą	a̧	ą̊	ɓ	ƈ	ç	đ	ɗ	ɖ	ę	ȩ	ə̧	ɛ̧	ƒ	ǥ	ɠ	ħ	ɦ	į	i̧	ɨ	ɨ̧	ƙ	ł	m̧	ɲ	ǫ	o̧	ø	ơ	ɔ̧	ƥ	ɍ	ş	ƭ	ţ	ŧ	ų	u̧	ư	ʉ	y̨	ƴ
// á	à	ȧ	â	ä	ǟ	ǎ	ă	ā	ã	å	ǻ	ǽ	ǣ	ḅ	ć	ċ	ĉ	č	ď	ḍ	ḑ	ḓ	é	è	ė	ê	ë	ě	ĕ	ē	ẽ	e̊	ẹ	ǵ	ġ	ĝ	ǧ	ğ	g̃	ģ	ĥ	ḥ
// í	ì	i	î	ï	ǐ	ĭ	ī	ĩ	ị	ĵ	ķ	ǩ	ĺ	ļ	ľ	ŀ	ḷ	ḽ	m̂	m̄	ŉ	ń	n̂	ṅ	n̈	ň	n̄	ñ	ņ	ṋ	ó	ò	ȯ	ȱ	ô	ö	ȫ	ǒ	ŏ	ō	õ	ȭ	ő	ọ	ǿ	ơ
// p̄	ŕ	ř	ŗ	ṛ	ś	ŝ	ṡ	š	ș	ṣ	ť	ț	ṭ	ṱ	ú	ù	û	ü	ǔ	ŭ	ū	ũ	ű	ů	ụ	ẃ	ẁ	ŵ	ẅ	ẋ	ý	ỳ	ŷ	ÿ	ȳ	ỹ	ź	ż	ž	ẓ








      }
   }





}

}

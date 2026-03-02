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
         case 0x01E6:
         case 0x011E:
         case 0x0122:
         {
            value[i] = L'G';
            break;
         }
         case 0x0126:
         case 0xA7AA:
         case 0x0124:
         case 0x1E24:
         {
            value[i] = L'H';
            break;
         }
         case 0x012E:
         case 0x0197:
         case 0x00CD:
         case 0x00CC:
         case 0x0130:
         case 0x00CE:
         case 0x00CF:
         case 0x01CF:
         case 0x012C:
         case 0x012A:
         case 0x0128:
         case 0x1ECA:
         {
            value[i] = L'I';
            break;
         }
         case 0x0134:
         {
            value[i] = L'J';
            break;
         }
         case 0x0198:
         case 0x0136:
         case 0x01E8:
         {
            value[i] = L'K';
            break;
         }
         case 0x0141:
         case 0x0139:
         case 0x013B:
         case 0x013D:
         case 0x013F:
         case 0x1E36:
         case 0x1E3C:
         {
            value[i] = L'L';
            break;
         }
         case 0x019D:
         // notice that there is no ʼN character here
         // the N-apostrophe character from Afrikaans has only lower case variant
         // upper case has never been included in any international keyboard
         case 0x0143:
         case 0x1E44:
         case 0x0147:
         case 0x00D1:
         case 0x0145:
         case 0x1E4A:
         {
            value[i] = L'N';
            break;
         }
         case 0x01EA:
         case 0x00D8:
         case 0x01A0:
         case 0x00D3:
         case 0x00D2:
         case 0x022E:
         case 0x0230:
         case 0x00D4:
         case 0x00D6:
         case 0x022A:
         case 0x01D1:
         case 0x014E:
         case 0x014C:
         case 0x00D5:
         case 0x022C:
         case 0x0150:
         case 0x1ECC:
         case 0x01FE:
         {
            value[i] = L'O';
            break;
         }
         case 0x01A4:
         {
            value[i] = L'P';
            break;
         }
         case 0x024C:
         case 0x0154:
         case 0x0158:
         case 0x0156:
         case 0x1E5A:
         {
            value[i] = L'R';
            break;
         }
         case 0x015E:
         case 0x015A:
         case 0x015C:
         case 0x1E60:
         case 0x0160:
         case 0x0218:
         case 0x1E62:
         {
            value[i] = L'S';
            break;
         }
         case 0x01AC:
         case 0x0162:
         case 0x0166:
         case 0x0164:
         case 0x021A:
         case 0x1E6C:
         case 0x1E70:
         {
            value[i] = L'T';
            break;
         }
         case 0x0172:
         case 0x01AF:
         case 0x0244:
         case 0x00DA:
         case 0x00D9:
         case 0x00DB:
         case 0x00DC:
         case 0x01D3:
         case 0x016C:
         case 0x016A:
         case 0x0168:
         case 0x0170:
         case 0x016E:
         case 0x1EE4:
         {
            value[i] = L'U';
            break;
         }
         case 0x1E82:
         case 0x1E80:
         case 0x0174:
         case 0x1E84:
         {
            value[i] = L'W';
            break;
         }
         case 0x1E8A:
         {
            value[i] = L'X';
            break;
         }
         case 0x01B3:
         case 0x00DD:
         case 0x1EF2:
         case 0x0176:
         case 0x0178:
         case 0x0232:
         case 0x1EF8:
         {
            value[i] = L'Y';
            break;
         }
         case 0x0179:  
         case 0x017B:
         case 0x017D:
         case 0x1E92:
         {
            value[i] = L'Z';
            break;
         }
         case 0x0105:
         case 0x00E1:
         case 0x00E0:
         case 0x0227:
         case 0x00E2:
         case 0x00E4:
         case 0x01DF:
         case 0x01CE:
         case 0x0103:
         case 0x0101:
         case 0x00E3:
         case 0x00E5:
         case 0x01FB: {
            value[i] = L'a';
            break;
         }
         case 0x0253:
         case 0x1E05: {
            value[i] = L'b';
            break;
         }
         case 0x0188:
         case 0x00E7:
         case 0x0107:
         case 0x010B:
         case 0x0109:
         case 0x010D: {
            value[i] = L'c';
            break;
         }
         case 0x0111:
         case 0x0257:
         case 0x0256:
         case 0x010F:
         case 0x1E0D:
         case 0x1E11:
         case 0x1E13: {
            value[i] = L'd';
            break;
         }
         case 0x0119:
         case 0x0229:
         case 0x00E9:
         case 0x00E8:
         case 0x0117:
         case 0x00EA:
         case 0x00EB:
         case 0x011B:
         case 0x0115:
         case 0x0113:
         case 0x1EBD:
         case 0x1EB9: {
            value[i] = L'e';
            break;
         }
         case 0x0192: {
            value[i] = L'f';
            break;
         }
         case 0x01E5:
         case 0x0260:
         case 0x01F5:
         case 0x0121:
         case 0x011D:
         case 0x01E7:
         case 0x011F:
         case 0x0123: {
            value[i] = L'g';
            break;
         }
         case 0x0127:
         case 0x0266:
         case 0x0125:
         case 0x1E25: {
            value[i] = L'h';
            break;
         }
         case 0x012F:
         case 0x0268:
         case 0x00ED:
         case 0x00EC:
         case 0x00EE:
         case 0x00EF:
         case 0x01D0:
         case 0x012D:
         case 0x012B:
         case 0x0129:
         case 0x1ECB: {
            value[i] = L'i';
            break;
         }
         case 0x0135: {
            value[i] = L'j';
            break;
         }
         case 0x0199:
         case 0x0137:
         case 0x01E9: {
            value[i] = L'k';
            break;
         }
         case 0x0142:
         case 0x013A:
         case 0x013C:
         case 0x013E:
         case 0x0140:
         case 0x1E37:
         case 0x1E3D: {
            value[i] = L'l';
            break;
         }
         case 0x0272:
         case 0x0149:
         case 0x0144:
         case 0x1E45:
         case 0x0148:
         case 0x00F1:
         case 0x0146:
         case 0x1E4B: {
            value[i] = L'n';
            break;
         }
         case 0x01EB:
         case 0x00F8:
         case 0x01A1:
         case 0x00F3:
         case 0x00F2:
         case 0x022F:
         case 0x0231:
         case 0x00F4:
         case 0x00F6:
         case 0x022B:
         case 0x01D2:
         case 0x014F:
         case 0x014D:
         case 0x00F5:
         case 0x022D:
         case 0x0151:
         case 0x1ECD:
         case 0x01FF: {
            value[i] = L'o';
            break;
         }
         case 0x01A5:
         {
            value[i] = L'p';
            break;
         }
         case 0x024D:
         case 0x0155:
         case 0x0159:
         case 0x0157:
         case 0x1E5B: {
            value[i] = L'r';
            break;
         }
         case 0x015F:
         case 0x015B:
         case 0x015D:
         case 0x1E61:
         case 0x0161:
         case 0x0219:
         case 0x1E63: {
            value[i] = L's';
            break;
         }
         case 0x01AD:
         case 0x0163:
         case 0x0167:
         case 0x0165:
         case 0x021B:
         case 0x1E6D:
         case 0x1E71: {
            value[i] = L't';
            break;
         }
         case 0x0173:
         case 0x01B0:
         case 0x0289:
         case 0x00FA:
         case 0x00F9:
         case 0x00FB:
         case 0x00FC:
         case 0x01D4:
         case 0x016D:
         case 0x016B:
         case 0x0169:
         case 0x0171:
         case 0x016F:
         case 0x1EE5: {
            value[i] = L'u';
            break;
         }
         case 0x1E83:
         case 0x1E81:
         case 0x0175:
         case 0x1E85: {
            value[i] = L'w';
            break;
         }
         case 0x1E8B: {
            value[i] = L'x';
            break;
         }
         case 0x01B4:
         case 0x00FD:
         case 0x1EF3:
         case 0x0177:
         case 0x00FF:
         case 0x0233:
         case 0x1EF9: {
            value[i] = L'y';
            break;
         }
         case 0x017A:
         case 0x017C:
         case 0x017E:
         case 0x1E93: {
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

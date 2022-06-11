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

#include "var-inner.h"


ThisState g_thisstate;
Variable<_tim> g_access;
Variable<_boo> g_archive;
Variable<_tim> g_change;
Variable<_boo> g_compressed;
Variable<_tim> g_creation;
Variable<_num> g_depth;
Variable<_str> g_drive;
Variable<_boo> g_empty;
Variable<_boo> g_encrypted;
Variable<_boo> g_exists;
Variable<_str> g_extension;
Variable<_str> g_fullname;
Variable<_boo> g_hidden;
Variable<_num> g_index;
Variable<_boo> g_isdirectory;
Variable<_boo> g_isfile;
Variable<_per> g_lifetime;
Variable<_str> g_location;
Variable<_tim> g_modification;
Variable<_str> g_name;
Variable<_str> g_parent;
Variable<_str> g_path;
Variable<_boo> g_readonly;
Variable<_num> g_size;
Variable<_num> g_this_n;
Variable<_str> g_this_s;
Variable<_tim> g_this_t;

Variable<_boo> g_success;

_str g_trimmed;
_str g_urocom;

_list vinit_getAlphabet()
{
   _list a(26);

   for (_size i = 0; i < 26; i++) {
      a[i] = L'a' + i;
   }

   return a;
}

_list vinit_getAscii()
{
   return {
      L" ", L" ", L" ", L" ", L" ", L" ", L" ", L" ", L" ", L" ", L" ", L" ", L" ", L" ", L" ", L" ",
      L" ", L" ", L" ", L" ", L" ", L" ", L" ", L" ", L" ", L" ", L" ", L" ", L" ", L" ", L" ", L" ",
      L" ", L"!", L"\"", L"#", L"$", L"%", L"&", L"'", L"(", L")", L"*", L"+", L",", L"-", L".", L"/",
      L"0", L"1", L"2", L"3", L"4", L"5", L"6", L"7", L"8", L"9", L":", L";", L"<", L"=", L">", L"?",
      L"@", L"A", L"B", L"C", L"D", L"E", L"F", L"G", L"H", L"I", L"J", L"K", L"L", L"M", L"N", L"O",
      L"P", L"Q", L"R", L"S", L"T", L"U", L"V", L"W", L"X", L"Y", L"Z", L"[", L"\\", L"]", L"^", L"_",
      L"`", L"a", L"b", L"c", L"d", L"e", L"f", L"g", L"h", L"i", L"j", L"k", L"l", L"m", L"n", L"o",
      L"p", L"q", L"r", L"s", L"t", L"u", L"v", L"w", L"x", L"y", L"z", L"{", L"|", L"}", L"~", L" "
   };
}

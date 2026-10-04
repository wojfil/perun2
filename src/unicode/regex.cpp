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

#include "regex.h"
#include "convert.h"


namespace perun2 
{

UnicodeRegex::UnicodeRegex(const UnicodeRegex& other)
   : UnicodeRegex(other.m_originText) { }

   
UnicodeRegex::UnicodeRegex(const p_str& pattern) 
   : m_originText(pattern)
{
   UErrorCode status = U_ZERO_ERROR;
   m_pattern = icu::RegexPattern::compile(utf32_to_unicode(pattern), 0, status);

   if (U_FAILURE(status) || m_pattern == nullptr) {
      m_pattern = nullptr;
      return;
   }

   m_matcher = m_pattern->matcher(icu::UnicodeString(), status);

   if (U_FAILURE(status) || m_matcher == nullptr) {
      delete m_pattern;
      m_pattern = nullptr;
      m_matcher = nullptr;
      return;
   }

   m_good = true;
}


UnicodeRegex::~UnicodeRegex()
{
   if (m_matcher != nullptr) {
      delete m_matcher;
   }

   if (m_pattern != nullptr) {
      delete m_pattern;
   }
}


bool UnicodeRegex::isGood() const 
{
   return m_good;
}


bool UnicodeRegex::check(const p_str& value)
{
   if (! m_good) {
      return false;
   }

   icu::UnicodeString ustr = utf32_to_unicode(value);
   UErrorCode status = U_ZERO_ERROR;
   m_matcher->reset(ustr);
   bool result = m_matcher->find(status);
   return ! U_FAILURE(status) && result;
}


}


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

#include "func-bool.h"
#include "..\..\os/os.h"


namespace perun2::func
{



p_bool F_AnyInside::getValue()
{
   if (!this->fileContext->v_exists->value || !this->fileContext->v_isdirectory->value) {
      return false;
   }

   this->locContext->loadData(this->fileContext->trimmed);
   const p_bool any = this->definition->hasNext();

   if (any) {
      this->definition->reset();
   }

   return any;
}


p_bool F_ContainsStr::getValue()
{
   const p_str phrase = this->arg2->getValue();
   if (phrase.empty()) {
      return true;
   }

   const p_str base =  this->arg1->getValue();
   if (base.empty()) {
      return false;
   }

   return base.find(phrase) != p_str::npos;
}


p_bool F_ContainsDef::getValue()
{
   const p_str v = this->value->getValue();

   while (this->definition->hasNext()) {
      if (this->perun2.isNotRunning()) {
         this->definition->reset();
         break;
      }

      if (v == this->definition->getValue()) {
         this->definition->reset();
         return true;
      }
   }

   return false;
}


p_bool F_IsLower::getValue()
{
   const p_str value = this->arg1->getValue();
   if (value.empty()) {
      return false;
   }

   p_bool anyLetter = false;

   for (const p_char ch : value) {
      if (char_isAlpha(ch)) {
         if (char_isUpper(ch)) {
            return false;
         }
         anyLetter = true;
      }
   }

   return anyLetter;
}


p_bool F_IsUpper::getValue()
{
   const p_str value = this->arg1->getValue();
   if (value.empty()) {
      return false;
   }

   p_bool anyLetter = false;

   for (const p_char ch : value) {
      if (char_isAlpha(ch)) {
         if (char_isLower(ch)) {
            return false;
         }
         anyLetter = true;
      }
   }

   return anyLetter;
}


p_bool F_IsNumber::getValue()
{
   return isNumber(this->arg1->getValue());
}


p_bool isNumber(const p_str& value)
{
   if (value.empty()) {
      return false;
   }

   const p_size len = value.size();
   p_bool hasDot = false;

   for (p_size i = 0; i < len; i++) {
      const p_char ch = value[i];
      
      if (ch == CHAR_DOT) {
         if (hasDot) {
            return false;
         }
         else {
            hasDot = true;
         }
      }
      else if (ch == CHAR_MINUS) {
         if (i != 0) {
            return false;
         }
      }
      else if (!char_isDigit(ch)) {
         return false;
      }
   }

   return true;
}


p_bool F_StartsWith::getValue()
{
   const p_str phrase = this->arg2->getValue();
   if (phrase.empty()) {
      return true;
   }

   const p_str base =  this->arg1->getValue();
   if (base.empty()) {
      return false;
   }

   const p_size blen = base.size();
   const p_size plen = phrase.size();

   if (plen > blen) {
      return false;
   }

   for (p_size i = 0; i < plen; i++) {
      if (phrase[i] != base[i]) {
         return false;
      }
   }

   return true;
}


p_bool F_EndsWith::getValue()
{
   const p_str phrase = this->arg2->getValue();
   if (phrase.empty()) {
      return true;
   }

   const p_str base =  this->arg1->getValue();
   if (base.empty()) {
      return false;
   }

   const p_size blen = base.size();
   const p_size plen = phrase.size();

   if (plen > blen) {
      return false;
   }

   for (p_size i = 0; i < plen; i++) {
      if (phrase[plen - 1 - i] != base[blen - 1 - i]) {
         return false;
      }
   }

   return true;
}


p_bool F_ExistsInside::getValue()
{
   if (!this->fileContext->v_exists->value || !this->fileContext->v_isdirectory->value) {
      return false;
   }

   const p_str base = os_leftJoin(this->locContext->location->value, this->fileContext->trimmed);
   const p_str value = os_trim(this->arg1->getValue());

   if (os_isInvalid(value)) {
      return false;
   }

   return os_exists(os_leftJoin(base, value));
}


p_bool F_ExistInside::getValue()
{
   if (!this->fileContext->v_exists->value || !this->fileContext->v_isdirectory->value) {
      return false;
   }

   const p_str base = os_leftJoin(this->locContext->location->value, this->fileContext->trimmed);
   const p_list values = this->arg1->getValue();
   const p_size len = values.size();

   for (p_size i = 0; i < len; i++) {
      const p_str& v = os_trim(values[i]);

      if (os_isInvalid(v)) {
          return false;
      }

      const p_str path = os_leftJoin(base, v);
      if (!os_exists(path)) {
         return false;
      }
   }

   return true;
}


p_bool F_StartsWithChar::getValue()
{
   const p_str v = value->getValue();

   return v.empty()
      ? false
      : (v[0] == character);
}


p_bool F_EndsWithChar::getValue()
{
   const p_str v = value->getValue();

   return v.empty()
      ? false
      : (v[v.size() - 1] == character);
}


p_bool F_FindText::getValue()
{
   if (!this->context->v_exists->value || !this->context->v_isfile->value) {
      return false;
   }

   const p_str value = this->arg1->getValue();
   if (value.empty()) {
      return true;
   }

   return os_findText(this->context->v_path->value, value);
}


p_bool F_StartsWithConst::getValue()
{
   const p_str v = value->getValue();
   const p_size len = v.size();
   if (len < constantLength) {
      return false;
   }

   for (p_size i = 0; i < constantLength; i++) {
      if (v[i] != constant[i]) {
         return false;
      }
   }

   return true;
}


p_bool F_EndsWithConst::getValue()
{
   const p_str v = value->getValue();
   const p_size len = v.size();
   if (len < constantLength) {
      return false;
   }

   for (p_size i = 0; i < constantLength; i++) {
      if (v[len - constantLength + i] != constant[i]) {
         return false;
      }
   }

   return true;
}


p_bool F_IsLetter::getValue()
{
   const p_str value = this->arg1->getValue();
   if (value.empty()) {
      return false;
   }

   for (const p_char ch : value) {
      if (!char_isAlpha(ch)) {
         return false;
      }
   }

   return true;
}


p_bool F_IsDigit::getValue()
{
   const p_str value = this->arg1->getValue();
   if (value.empty()) {
      return false;
   }

   for (const p_char ch : value) {
      if (!char_isDigit(ch)) {
         return false;
      }
   }

   return true;
}


p_bool F_IsBinary::getValue()
{
   const p_str value = this->arg1->getValue();
   if (value.empty()) {
      return false;
   }

   for (const p_char ch : value) {
      switch (ch) {
         case CHAR_0:
         case CHAR_1:{
            break;
         }
         default: {
            return false;
         }
      }
   }

   return true;
}


p_bool F_IsHex::getValue()
{
   const p_str value = this->arg1->getValue();
   if (value.empty()) {
      return false;
   }

   for (const p_char ch : value) {
      switch (ch) {
         case CHAR_0: case CHAR_1: case CHAR_2: case CHAR_3: case CHAR_4:
         case CHAR_5: case CHAR_6: case CHAR_7: case CHAR_8: case CHAR_9:
         case CHAR_a: case CHAR_b: case CHAR_c: case CHAR_d: case CHAR_e: case CHAR_f:
         case CHAR_A: case CHAR_B: case CHAR_C: case CHAR_D: case CHAR_E: case CHAR_F: {
            break;
         }
         default: {
            return false;
         }
      }
   }

   return true;
}

p_bool F_Any::getValue()
{
   const p_bool result = this->definition->hasNext();

   if (result) {
      this->definition->reset();
   }

   return result;
}

F_Exists::F_Exists(p_genptr<p_str>& a1, Perun2Process& perun2)
   : Func_1(a1), context(perun2.contexts.getLocationContext()) { };

p_bool F_Exists::getValue()
{
   const p_str value = os_trim(this->arg1->getValue());

   if (os_isInvalid(value)) {
      return false;
   }

   return os_exists(os_leftJoin(this->context->location->value, value));
}

F_Exist::F_Exist(p_genptr<p_list>& a1, Perun2Process& perun2)
   : Func_1(a1), context(perun2.contexts.getLocationContext())  { };

p_bool F_Exist::getValue()
{
   const p_list values = this->arg1->getValue();

   for (const p_str& val : values) {
      const p_str& v = os_trim(val);

      if (os_isInvalid(v)) {
          return false;
      }

      const p_str path = os_leftJoin(this->context->location->value, v);
      if (!os_exists(path)) {
         return false;
      }
   }

   return true;
}

p_bool F_IsNan::getValue()
{
   return this->arg1->getValue().isNaN();
}

p_bool F_IsNever::getValue()
{
   return this->arg1->getValue().type == Time::TimeType::tt_Never;
}

F_AskPython3::F_AskPython3(p_genptr<p_str>& a1, FileContext* fctx, Perun2Process& p2) 
   : Func_1(a1), 
     fileContext(fctx),
     perun2(p2) { };

p_bool F_AskPython3::getValue()
{
   if (! fileContext->v_exists->getValue()) {
      return false;
   }

   const p_str python3 = os_trim(this->arg1->getValue());

   if (os_isAbsolute(python3)) {
      return false;
   }

   return this->perun2.python3Processes.askPython3(fileContext->this_->getValue(), python3);
}

}

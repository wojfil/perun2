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

#include "gen-bool-compare.h"
#include "../../uroboros.h"


namespace uro::gen
{

DefComparison::~DefComparison()
{
   delete value1;
   delete value2;
}

_bool DefinitionsEqual::getValue()
{
   while (true) {
      if (!this->uroboros.state == State::s_Running) {
         this->value1->reset();
         this->value2->reset();
         break;
      }

      if (this->value1->hasNext()) {
         if (this->value2->hasNext()) {
            if (this->value1->getValue() != this->value2->getValue()) {
               this->value1->reset();
               this->value2->reset();
               return false;
            }
         }
         else {
            this->value1->reset();
            return false;
         }
      }
      else {
         if (this->value2->hasNext()) {
            this->value2->reset();
            return false;
         }
         else {
            return true;
         }
      }
   }

   return false;
}

_bool DefinitionsNotEqual::getValue()
{
   while (true) {
      if (!this->uroboros.state == State::s_Running) {
         this->value1->reset();
         this->value2->reset();
         break;
      }

      if (this->value1->hasNext()) {
         if (this->value2->hasNext()) {
            if (this->value1->getValue() != this->value2->getValue()) {
               this->value1->reset();
               this->value2->reset();
               return true;
            }
         }
         else {
            this->value1->reset();
            return true;
         }
      }
      else {
         if (this->value2->hasNext()) {
            this->value2->reset();
            return true;
         }
         else {
            return false;
         }
      }
   }

   return false;
}

_bool DefinitionsSmaller::getValue()
{
   while (true) {
      if (!this->uroboros.state == State::s_Running) {
         this->value1->reset();
         this->value2->reset();
         break;
      }

      if (this->value1->hasNext()) {
         if (!this->value2->hasNext()) {
            this->value1->reset();
            return false; // no more on the right
         }
      }
      else {
         if (this->value2->hasNext()) {
            this->value2->reset();
            return true; // no more more on the left
         }
         else {
            return false; // no more in both
         }
      }
   }

   return false;
}

_bool DefinitionsSmallerEquals::getValue()
{
   while (true) {
      if (!this->uroboros.state == State::s_Running) {
         this->value1->reset();
         this->value2->reset();
         break;
      }

      if (this->value1->hasNext()) {
         if (!this->value2->hasNext()) {
            this->value1->reset();
            return false; // no more on the right
         }
      }
      else {
         if (this->value2->hasNext()) {
            this->value2->reset();
            return true; // no more more on the left
         }
         else {
            return true; // no more in both
         }
      }
   }

   return false;
}

_bool DefinitionsBigger::getValue()
{
   while (true) {
      if (!this->uroboros.state == State::s_Running) {
         this->value1->reset();
         this->value2->reset();
         break;
      }

      if (this->value1->hasNext()) {
         if (!this->value2->hasNext()) {
            this->value1->reset();
            return true; // no more on the right
         }
      }
      else {
         if (this->value2->hasNext()) {
            this->value2->reset();
            return false; // no more more on the left
         }
         else {
            return false; // no more in both
         }
      }
   }

   return false;
}

_bool DefinitionsBiggerEquals::getValue()
{
   while (true) {
      if (!this->uroboros.state == State::s_Running) {
         this->value1->reset();
         this->value2->reset();
         break;
      }

      if (this->value1->hasNext()) {
         if (!this->value2->hasNext()) {
            this->value1->reset();
            return true; // no more on the right
         }
      }
      else {
         if (this->value2->hasNext()) {
            this->value2->reset();
            return false; // no more more on the left
         }
         else {
            return true; // no more in both
         }
      }
   }

   return false;
}

DefListComparison::~DefListComparison()
{
   delete definition;
   delete list;
}

_bool DefinitionListEqual::getValue()
{
   const _list v = this->list->getValue();
   const _size len = v.size();

   if (len == 0) {
      if (this->definition->hasNext()) {
         this->definition->reset();
         return false;
      }
      else {
         return true;
      }
   }

   _size i = 0;

   while (this->definition->hasNext()) {
      if (!this->uroboros.state == State::s_Running) {
         this->definition->reset();
         break;
      }

      if (this->definition->getValue() != v[i]) {
         this->definition->reset();
         return false;
      }

      i++;
      if (i == len) {
         if (this->definition->hasNext()) {
            this->definition->reset();
            return false;
         }
         else {
            return true;
         }
      }
   }

   return false;
}

_bool DefinitionListNotEqual::getValue()
{
   const _list v = this->list->getValue();
   const _size len = v.size();

   if (len == 0) {
      if (this->definition->hasNext()) {
         this->definition->reset();
         return true;
      }
      else {
         return false;
      }
   }

   _size i = 0;

   while (this->definition->hasNext()) {
      if (!this->uroboros.state == State::s_Running) {
         this->definition->reset();
         break;
      }

      if (this->definition->getValue() != v[i]) {
         this->definition->reset();
         return true;
      }

      i++;
      if (i == len) {
         if (this->definition->hasNext()) {
            this->definition->reset();
            return true;
         }
         else {
            return false;
         }
      }
   }

   return false;
}

_bool DefinitionListSmaller::getValue()
{
   const _list v = this->list->getValue();
   const _size len = v.size();

   if (len == 0) {
      return false;
   }

   _size i = 0;

   while (this->definition->hasNext()) {
      if (!this->uroboros.state == State::s_Running) {
         this->definition->reset();
         return false;
      }

      i++;
      if (i == len) {
         this->definition->reset();
         return false;
      }
   }

   return true;
}

_bool DefinitionListSmallerEquals::getValue()
{
   const _list v = this->list->getValue();
   const _size len = v.size();

   if (len == 0) {
      if (this->definition->hasNext()) {
         this->definition->reset();
         return false;
      }
      else {
         return true;
      }
   }

   _size i = 0;

   while (this->definition->hasNext()) {
      if (!this->uroboros.state == State::s_Running) {
         this->definition->reset();
         return false;
      }

      i++;
      if (i == len) {
         if (this->definition->hasNext()) {
            this->definition->reset();
            return false;
         }
         else {
            return true;
         }
      }
   }

   return true;
}

_bool DefinitionListBigger::getValue()
{
   const _list v = this->list->getValue();
   const _size len = v.size();

   if (len == 0) {
      if (this->definition->hasNext()) {
         this->definition->reset();
         return true;
      }
      else {
         return false;
      }
   }

   _size i = 0;

   while (this->definition->hasNext()) {
      if (!this->uroboros.state == State::s_Running) {
         this->definition->reset();
         return false;
      }

      i++;
      if (i == len) {
         if (this->definition->hasNext()) {
            this->definition->reset();
            return true;
         }
         else {
            return false;
         }
      }
   }

   return false;
}

_bool DefinitionListBiggerEquals::getValue()
{
   const _list v = this->list->getValue();
   const _size len = v.size();

   if (len == 0) {
      return true;
   }

   _size i = 0;

   while (this->definition->hasNext()) {
      if (!this->uroboros.state == State::s_Running) {
         this->definition->reset();
         return false;
      }

      i++;
      if (i == len) {
         this->definition->reset();
         return true;
      }
   }

   return false;
}

}

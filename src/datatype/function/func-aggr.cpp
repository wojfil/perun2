/*
    This file is part of Perun2.
    Perun2 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    Peruns2 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with Perun2. If not, see <http://www.gnu.org/licenses/>.
*/

#include "func-aggr.h"
#include "../../util.h"
#include <algorithm>


namespace perun2::func
{

_num F_Average::getValue()
{
   _num sum;
   p_int count = countSingle;

   for (_genptr<_num>& sv : this->singleValues) {
      sum += sv->getValue();
   }

   for (_genptr<_nlist>& mv : this->multiValues) {
      _nlist nlist = mv->getValue();
      count += nlist.size();

      for (const _num& n : nlist) {
         sum += n;
      }
   }

   if (count != 0) {
      sum /= _num(count);
   }

   return sum;
}


_num F_Max::getValue()
{
   _num max;
   p_bool init = false;

   if (countSingle != 0) {
      init = true;
      max = singleValues[0]->getValue();

      for (p_size i = 1; i < countSingle; i++) {
         const _num v = singleValues[i]->getValue();
         if (v > max) {
            max = v;
         }
      }
   }

   for (_genptr<_nlist>& mv : this->multiValues) {
      _nlist nlist = mv->getValue();
      if (!nlist.empty()) {
         const p_size len = nlist.size();

         if (init) {
            for (const _num& n : nlist) {
               if (n > max) {
                  max = n;
               }
            }
         }
         else {
            init = true;
            max = nlist[0];
            for (p_size j = 1; j < len; j++) {
               if (nlist[j] > max) {
                  max = nlist[j];
               }
            }
         }
      }
   }

   return init ? max : _num();
}


_num F_Median::getValue()
{
   _nlist elements;
   elements.reserve(countSingle);

   for (_genptr<_num>& sv : this->singleValues) {
      elements.emplace_back(sv->getValue());
   }

   for (_genptr<_nlist>& mv : this->multiValues) {
      _nlist nlist = mv->getValue();
      if (!nlist.empty()) {
         langutil::appendVector(elements, nlist);
      }
   }

   std::sort(elements.begin(), elements.end());
   const p_size len = elements.size();
   const p_size half = len / 2;

   if (len % 2 == 0) {
      _num result = elements[half] + elements[half - 1];
      result /= _num(NINT_TWO);
      return result;
   }
   else {
      return elements[half];
   }
}


_num F_Min::getValue()
{
   _num min;
   p_bool init = false;

   if (countSingle != 0) {
      init = true;
      min = singleValues[0]->getValue();

      for (p_size i = 1; i < countSingle; i++) {
         const _num v = singleValues[i]->getValue();
         if (v < min) {
            min = v;
         }
      }
   }

   for (_genptr<_nlist>& mv : this->multiValues) {
      _nlist nlist = mv->getValue();
      if (!nlist.empty()) {
         if (init) {
            for (const _num& n : nlist) {
               if (n < min) {
                  min = n;
               }
            }
         }
         else {
            init = true;
            min = nlist[0];
            for (p_size j = 1; j < nlist.size(); j++) {
               if (nlist[j] < min) {
                  min = nlist[j];
               }
            }
         }
      }
   }

   return init ? min : _num();
}


_num F_Sum::getValue()
{
   _num sum;

   for (_genptr<_num>& sv : this->singleValues) {
      sum += sv->getValue();
   }

   for (_genptr<_nlist>& mv : this->multiValues) {
      for (const _num n : mv->getValue()) {
         sum += n;
      }
   }

   return sum;
}

}

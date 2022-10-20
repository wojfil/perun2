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

#include "func-aggr.h"
#include "../../util.h"
#include <algorithm>


namespace uro::func
{

FuncAggr::~FuncAggr()
{
   langutil::deleteVectorPtr(singleValues);
   langutil::deleteVectorPtr(multiValues);
}


_num F_Average::getValue()
{
   _num sum(0LL);
   _int count = countSingle;

   for (_size i = 0; i < countSingle; i++) {
      sum += (*singleValues)[i]->getValue();
   }

   for (_size i = 0; i < countMulti; i++) {
      _nlist nlist = (*multiValues)[i]->getValue();
      if (!nlist.empty()) {
         const _size len = nlist.size();
         count += len;
         for (_size j = 0; j < len; j++) {
            sum += nlist[j];
         }
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
   _bool init = false;

   if (countSingle != 0) {
      init = true;
      max = (*singleValues)[0]->getValue();

      for (_size i = 1; i < countSingle; i++) {
         const _num v = (*singleValues)[i]->getValue();
         if (v > max) {
            max = v;
         }
      }
   }

   for (_size i = 0; i < countMulti; i++) {
      _nlist nlist = (*multiValues)[i]->getValue();
      if (!nlist.empty()) {
         const _size len = nlist.size();

         if (init) {
            for (_size j = 0; j < len; j++) {
               const _num& v = nlist[j];
               if (v > max) {
                  max = v;
               }
            }
         }
         else {
            init = true;
            max = nlist[0];
            for (_size j = 1; j < len; j++) {
               const _num& v = nlist[j];
               if (v > max) {
                  max = v;
               }
            }
         }
      }
   }

   return init ? max : _num(0LL);
}


_num F_Median::getValue()
{
   // use normal vector
   // std::multiset would be more efficient, but it does not support taking elements at index
   _nlist elements;
   elements.reserve(countSingle);

   for (_size i = 0; i < countSingle; i++) {
      elements.emplace_back((*singleValues)[i]->getValue());
   }

   for (_size i = 0; i < countMulti; i++) {
      _nlist nlist = (*multiValues)[i]->getValue();
      if (!nlist.empty()) {
         langutil::appendVector(elements, nlist);
      }
   }

   std::sort(elements.begin(), elements.end());
   const _size len = elements.size();
   const _size half = len / 2;

   return len % 2 == 0
      ? _num((elements[half] + elements[half - 1]) / 2)
      : elements[half];
}


_num F_Min::getValue()
{
   _num min;
   _bool init = false;

   if (countSingle != 0) {
      init = true;
      min = (*singleValues)[0]->getValue();

      for (_size i = 1; i < countSingle; i++) {
         const _num v = (*singleValues)[i]->getValue();
         if (v < min) {
            min = v;
         }
      }
   }

   for (_size i = 0; i < countMulti; i++) {
      _nlist nlist = (*multiValues)[i]->getValue();
      if (!nlist.empty()) {
         const _size len = nlist.size();

         if (init) {
            for (_size j = 0; j < len; j++) {
               const _num& v = nlist[j];
               if (v < min) {
                  min = v;
               }
            }
         }
         else {
            init = true;
            min = nlist[0];
            for (_size j = 1; j < len; j++) {
               const _num& v = nlist[j];
               if (v < min) {
                  min = v;
               }
            }
         }
      }
   }

   return init ? min : _num(0LL);
}


_num F_Sum::getValue()
{
   _num sum(0LL);

   for (_size i = 0; i < countSingle; i++) {
      sum += (*singleValues)[i]->getValue();
   }

   for (_size i = 0; i < countMulti; i++) {
      _nlist nlist = (*multiValues)[i]->getValue();
      if (!nlist.empty()) {
         const _size len = nlist.size();
         for (_size j = 0; j < len; j++) {
            sum += nlist[j];
         }
      }
   }

   return sum;
}

}

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

p_num F_Average::getValue()
{
   p_num sum;
   p_nint count = countSingle;

   for (p_genptr<p_num>& sv : this->singleValues) {
      const p_num n = sv->getValue();

      if (n.state == NumberState::NaN) {
         count--;
      }
      else {
         sum += n;
      }
   }

   for (p_genptr<p_nlist>& mv : this->multiValues) {
      p_nlist nlist = mv->getValue();
      count += nlist.size();

      for (const p_num& n : nlist) {
         if (n.state == NumberState::NaN) {
            count--;
         }
         else {
            sum += n;
         }
      }
   }

   if (count == 0) {
      return P_NaN;
   }

   sum /= count;
   return sum;
}


p_num F_Max::getValue()
{
   p_num max;
   p_bool init = false;

   if (countSingle != 0) {
      init = true;
      max = singleValues[0]->getValue();

      for (p_size i = 1; i < countSingle; i++) {
         const p_num n = singleValues[i]->getValue();
         if (n.state != NumberState::NaN && n > max) {
            max = n;
         }
      }
   }

   for (p_genptr<p_nlist>& mv : this->multiValues) {
      p_nlist nlist = mv->getValue();
      if (!nlist.empty()) {
         if (init) {
            for (const p_num& n : nlist) {
               if (n.state != NumberState::NaN && n > max) {
                  max = n;
               }
            }
         }
         else {
            init = true;
            max = nlist[0];
            for (p_size j = 1; j < nlist.size(); j++) {
               const p_num& n = nlist[j];
               if (n.state != NumberState::NaN && n > max) {
                  max = n;
               }
            }
         }
      }
   }

   return init ? max : P_NaN;
}


p_num F_Median::getValue()
{
   p_nlist elements;
   elements.reserve(countSingle);

   for (p_genptr<p_num>& sv : this->singleValues) {
      elements.emplace_back(sv->getValue());
   }

   for (p_genptr<p_nlist>& mv : this->multiValues) {
      p_nlist nlist = mv->getValue();
      if (!nlist.empty()) {
         langutil::appendVector(elements, nlist);
      }
   }

   std::sort(elements.begin(), elements.end());
   const p_size len = elements.size();
   const p_size half = len / 2;

   if (len % 2 == 0) {
      p_num result = elements[half] + elements[half - 1];
      result /= p_num(NINT_TWO);
      return result;
   }
   else {
      return elements[half];
   }
}


p_num F_Min::getValue()
{
   p_num min;
   p_bool init = false;

   if (countSingle != 0) {
      init = true;
      min = singleValues[0]->getValue();

      for (p_size i = 1; i < countSingle; i++) {
         const p_num v = singleValues[i]->getValue();
         if (v < min) {
            min = v;
         }
      }
   }

   for (p_genptr<p_nlist>& mv : this->multiValues) {
      p_nlist nlist = mv->getValue();
      if (!nlist.empty()) {
         if (init) {
            for (const p_num& n : nlist) {
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

   return init ? min : p_num();
}


p_num F_Sum::getValue()
{
   p_num sum;

   for (p_genptr<p_num>& sv : this->singleValues) {
      sum += sv->getValue();
   }

   for (p_genptr<p_nlist>& mv : this->multiValues) {
      for (const p_num n : mv->getValue()) {
         sum += n;
      }
   }

   return sum;
}

}

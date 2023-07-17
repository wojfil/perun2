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

#include "gen-list.h"
#include "../../perun2.h"
#include "../../util.h"


namespace perun2::gen
{

Join_StrStr::Join_StrStr(_genptr<p_str>& lef, _genptr<p_str>& rig)
   : left(std::move(lef)), right(std::move(rig)) { };

Join_StrList::Join_StrList(_genptr<p_str>& lef, _genptr<p_list>& rig)
   : left(std::move(lef)), right(std::move(rig)) { };

Join_ListStr::Join_ListStr(_genptr<p_list>& lef, _genptr<p_str>& rig)
   : left(std::move(lef)), right(std::move(rig)) { };

Join_ListList::Join_ListList(_genptr<p_list>& lef, _genptr<p_list>& rig)
   : left(std::move(lef)), right(std::move(rig)) { };

ListFilter_Where::ListFilter_Where(_genptr<p_bool>& cond, _genptr<p_list>& li, _fcptr& ctx, pp_perun2& p2)
   : condition(std::move(cond)), list(std::move(li)), context(std::move(ctx)), perun2(p2) { };

ListFilter_Limit::ListFilter_Limit(_genptr<p_list>& li, _genptr<p_num>& num)
   : list(std::move(li)), number(std::move(num)) { };

ListFilter_Skip::ListFilter_Skip(_genptr<p_list>& li, _genptr<p_num>& num)
   : list(std::move(li)), number(std::move(num)) { };

ListFilter_Every::ListFilter_Every(_genptr<p_list>& li, _genptr<p_num>& num)
   : list(std::move(li)), number(std::move(num)) { };

ListFilter_Final::ListFilter_Final(_genptr<p_list>& li, _genptr<p_num>& num)
   : list(std::move(li)), number(std::move(num)) { };


p_list Join_StrStr::getValue()
{
   return { left->getValue(), right->getValue() };
};


p_list Join_StrList::getValue()
{
   p_list v = right->getValue();
   v.insert(v.begin(), left->getValue());
   return v;
};


p_list Join_ListStr::getValue()
{
   p_list v = left->getValue();
   v.emplace_back(right->getValue());
   return v;
};


p_list Join_ListList::getValue()
{
   p_list v = left->getValue();
   langutil::appendVector(v, right->getValue());
   return v;
};


p_list ListFilter_Where::getValue() 
{
   const p_list values = list->getValue();
   p_list result;
   result.reserve(values.size());
   const p_nint length = static_cast<p_nint>(values.size());

   this->context->resetIndex();
   p_nint index = NINT_ZERO;

   while (this->perun2.state == State::s_Running && index != length) {
      const p_str& unit = values[static_cast<p_size>(index)];
      this->context->index->value.value.i = index;
      this->context->loadData(unit);

      if (condition->getValue()) {
         result.emplace_back(unit);
      }

      index++;
   }

   return result;
};


p_list ListFilter_Limit::getValue() 
{
   const p_nint n = number->getValue().toInt();

   if (n <= NINT_ZERO) {
      return p_list();
   }

   const p_list lst = list->getValue();

   return n >= static_cast<p_nint>(lst.size())
      ? lst
      : p_list(lst.begin(), lst.begin() + n);
};


p_list ListFilter_Skip::getValue()
{
   const p_nint n = number->getValue().toInt();
   const p_list lst = list->getValue();

   if (n <= NINT_ZERO) {
      return lst;
   }

   return n >= static_cast<p_nint>(lst.size())
      ? p_list()
      : p_list(lst.begin() + n, lst.end());
};


p_list ListFilter_Every::getValue()
{
   const p_nint n = number->getValue().toInt();
   const p_list lst = list->getValue();

   if (n <= NINT_ONE) {
      return lst;
   }

   const p_size baseSize = lst.size();
   const p_size newSize = (baseSize / n) + ((baseSize % n == 0) ? 0 : 1);
   p_list result(newSize);

   for (p_size i = 0; i < newSize; i++) {
      result[i] = lst[i * n];
   }

   return result;
};


p_list ListFilter_Final::getValue()
{
   const p_nint n = number->getValue().toInt();

   if (n <= NINT_ZERO) {
      return p_list();
   }

   const p_list lst = list->getValue();

   return n >= static_cast<p_nint>(lst.size())
      ? lst
      : p_list(lst.end() - n, lst.end());
};


}

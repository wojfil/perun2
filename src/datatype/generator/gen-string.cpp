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

#include "gen-string.h"
#include "../../os.h"
#include "../../perun2.h"


namespace perun2::gen
{


ConcatString_2::ConcatString_2(p_genptr<p_str>& v1, p_genptr<p_str>& v2) 
   : value1(std::move(v1)), value2(std::move(v2)) { };

ConcatString_3::ConcatString_3(p_genptr<p_str>& v1, p_genptr<p_str>& v2, p_genptr<p_str>& v3) 
   : value1(std::move(v1)), value2(std::move(v2)), value3(std::move(v3)) { };

ConcatString_4::ConcatString_4(p_genptr<p_str>& v1, p_genptr<p_str>& v2, p_genptr<p_str>& v3, p_genptr<p_str>& v4) 
   : value1(std::move(v1)), value2(std::move(v2)), value3(std::move(v3)), value4(std::move(v4)) { };

ConcatString_5::ConcatString_5(p_genptr<p_str>& v1, p_genptr<p_str>& v2, p_genptr<p_str>& v3, p_genptr<p_str>& v4, p_genptr<p_str>& v5) 
   : value1(std::move(v1)), value2(std::move(v2)), value3(std::move(v3)), value4(std::move(v4)), value5(std::move(v5)) { };

ConcatString_6::ConcatString_6(p_genptr<p_str>& v1, p_genptr<p_str>& v2, p_genptr<p_str>& v3, p_genptr<p_str>& v4, p_genptr<p_str>& v5, p_genptr<p_str>& v6) 
   : value1(std::move(v1)), value2(std::move(v2)), value3(std::move(v3)), value4(std::move(v4)), value5(std::move(v5)), value6(std::move(v6)) { };


p_str ConcatString_2::getValue()
{
   return str(this->value1->getValue(), this->value2->getValue());
};

p_str ConcatString_3::getValue()
{
   return str(this->value1->getValue(), this->value2->getValue(), this->value3->getValue());
};

p_str ConcatString_4::getValue()
{
   return str(this->value1->getValue(), this->value2->getValue(), this->value3->getValue(),
              this->value4->getValue());
};

p_str ConcatString_5::getValue()
{
   return str(this->value1->getValue(), this->value2->getValue(), this->value3->getValue(),
              this->value4->getValue(), this->value5->getValue());
};

p_str ConcatString_6::getValue()
{
   return str(this->value1->getValue(), this->value2->getValue(), this->value3->getValue(),
              this->value4->getValue(), this->value5->getValue(), this->value6->getValue());
};


ConcatString_Multi::ConcatString_Multi(std::vector<p_genptr<p_str>>& val)
{
   langutil::transferUniquePtrs(val, this->values);
};

p_str ConcatString_Multi::getValue()
{
   p_stream ss;

   for (const p_genptr<p_str>& val : this->values) {
      ss << val->getValue();
   }

   return ss.str();
}

p_str StringBinary::getValue()
{
   return condition->getValue()
      ? value->getValue()
      : p_str();
}

LocationReference::LocationReference(p_perun2& p2)
   : context(*p2.contexts.getLocationContext()) { };

p_str LocationReference::getValue()
{
   return this->context.location->value;
}

RelativeLocation::RelativeLocation(p_genptr<p_str>& val, p_perun2& p2, const p_int retr)
   : value(std::move(val)), context(*p2.contexts.getLocationContext()), retreats(retr) { };

p_str RelativeLocation::getValue()
{
   if (this->retreats == 0) {
      return str(this->context.location->value, OS_SEPARATOR, this->value->getValue());
   }

   p_str base = this->context.location->value;
   os_retreatPath(base, this->retreats);

   if (base.empty()) {
      return base;
   }

   return str(base, OS_SEPARATOR, this->value->getValue());
}

RetreatedPath::RetreatedPath(p_genptr<p_str>& val, const p_int retr)
   : value(std::move(val)), reatreats(retr) { };

p_str RetreatedPath::getValue()
{
   p_str v = this->value->getValue();
   os_retreatPath(v, this->reatreats);
   return v;
}

p_str CharAtIndex::getValue()
{
   const p_str v = value->getValue();

   if (v.empty()) {
      return v;
   }

   p_nint n = index->getValue().toInt();

   if (n < NINT_ZERO) {
      n += v.size();
   }

   return (n >= NINT_ZERO && n < static_cast<p_nint>(v.size()))
      ? toStr(v[static_cast<p_size>(n)])
      : p_str();
}

p_str DefinitionElement::getValue()
{
   p_nint n = index->getValue().toInt();

   if (n < NINT_ZERO) {
      return p_str();
   }

   while (this->definition->hasNext()) {
      if (this->perun2.isNotRunning()) {
         this->definition.reset();
         return p_str();
      }

      if (n == NINT_ZERO) {
         const p_str result = this->definition->getValue();
         this->definition.reset();
         return result;
      }

      n--;
   }

   return p_str();
}

}

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

#include "gen-bool-constr.h"
#include "../../context.h"
#include "../../os.h"
#include "../../perun2.h"


namespace perun2::gen
{

SizeConstraint::SizeConstraint(p_genptr<p_num>& limit, const CompType cmptype, FileContext& ctx, p_perun2& p2)
   : constraint(limit, cmptype), context(ctx), perun2(p2) { };


p_bool SizeConstraint::getValue()
{
   if (! this->context.v_exists->value || this->context.v_isfile->value) {
      this->constraint.setValue(this->context.vp_size->value);
      return this->constraint.getFinalResult();
   }

   return os_constr_sizeDirectory(this->context.v_path->value, this->constraint, this->perun2);
}


}

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

ContextConstraint::ContextConstraint(p_genptr<p_num>& limit, const CompType cmptype, FileContext& ctx, p_perun2& p2)
   : constraint(limit, cmptype), context(ctx), perun2(p2) { };


SizeConstraint::SizeConstraint(p_genptr<p_num>& limit, const CompType cmptype, FileContext& ctx, p_perun2& p2)
   : ContextConstraint(limit, cmptype, ctx, p2) { };


CountInsideConstraint::CountInsideConstraint(p_genptr<p_num>& limit, const CompType cmptype, p_defptr& def, 
      p_lcptr& lctx, FileContext& ctx, p_perun2& p2)
   : ContextConstraint(limit, cmptype, ctx, p2), definition(std::move(def)), locContext(std::move(lctx)) { };


p_bool SizeConstraint::getValue()
{
   if (! this->context.v_exists->value || this->context.v_isfile->value) {
      this->constraint.setValue(this->context.vp_size->value);
      return this->constraint.getFinalResult();
   }

   return os_constr_sizeDirectory(this->context.v_path->value, this->constraint, this->perun2);
}


p_bool CountInsideConstraint::getValue()
{
   this->constraint.reset();

   if (! this->context.v_exists->value || this->context.v_isfile->value) {
      return this->constraint.getFinalResult();
   }

   this->constraint.reset();
   IC_State state = this->constraint.getState();
   if (state != IC_State::Unknown) {
      return state == IC_State::True;
   }

   this->locContext->loadData(this->context.trimmed);

   while (this->definition->hasNext()) {
      if (this->perun2.isNotRunning()) {
         this->definition->reset();
         return false;
      }
      
      this->constraint.incrementByOne();
      state = this->constraint.getState();
      if (state != IC_State::Unknown) {
         this->definition->reset();
         return state == IC_State::True;
      }
   }

   return this->constraint.getFinalResult();
}


}

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


CountConstraint::CountConstraint(p_genptr<p_num>& limit, const CompType cmptype, p_defptr& def, p_perun2& p2)
   : constraint(limit, cmptype), definition(std::move(def)), perun2(p2) { };


CountInsideConstraint::CountInsideConstraint(p_genptr<p_num>& limit, const CompType cmptype, p_defptr& def, 
      p_lcptr& lctx, FileContext& ctx, p_perun2& p2)
   : ContextConstraint(limit, cmptype, ctx, p2), definition(std::move(def)), locContext(std::move(lctx)) { };


SizeConstraint_Def::SizeConstraint_Def(p_genptr<p_num>& limit, p_defptr& def, const CompType cmptype, p_perun2& p2)
   : definition(std::move(def)), constraint(limit, cmptype), context(*p2.contexts.getLocationContext()), perun2(p2) { };


SizeConstraint_List::SizeConstraint_List(p_genptr<p_num>& limit, p_genptr<p_list>& lst, const CompType cmptype, p_perun2& p2)
   : list(std::move(lst)), constraint(limit, cmptype), context(*p2.contexts.getLocationContext()), perun2(p2) { };


p_bool SizeConstraint::getValue()
{
   if (! this->context.v_exists->value || this->context.v_isfile->value) {
      this->constraint.setValue(this->context.vp_size->value);
      return this->constraint.getFinalResult();
   }

   return os_constr_sizeDirectory(this->context.v_path->value, this->constraint, this->perun2);
}


p_bool CountConstraint::getValue()
{
   this->constraint.reset();
   IC_State state = this->constraint.getState();
   if (state != IC_State::Unknown) {
      return state == IC_State::True;
   }
   
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


p_bool CountInsideConstraint::getValue()
{
   this->constraint.reset();

   if (! this->context.v_exists->value || this->context.v_isfile->value) {
      return this->constraint.getFailureResult();
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


p_bool SizeConstraint_Def::getValue()
{
   this->constraint.reset();
   IC_State state = this->constraint.getState();
   if (state != IC_State::Unknown) {
      return state == IC_State::True;
   }

   p_nint total = NINT_ZERO;

   while (this->definition->hasNext()) {
      if (this->perun2.isNotRunning()) {
         this->definition->reset();
         return false;
      }

      const p_str v = this->definition->getValue();
      const p_nint s = os_size(os_leftJoin(this->context.location->value, v), this->perun2);

      if (s != NINT_MINUS_ONE) {
         this->constraint.increment(s);
         state = this->constraint.getState();
         if (state != IC_State::Unknown) {
            this->definition->reset();
            return state == IC_State::True;
         }
      }
   }

   return this->constraint.getFinalResult();
}


p_bool SizeConstraint_List::getValue()
{
   this->constraint.reset();
   IC_State state = this->constraint.getState();
   if (state != IC_State::Unknown) {
      return state == IC_State::True;
   }

   p_nint total = NINT_ZERO;
   const p_list vs = list->getValue();
   const p_size len = vs.size();

   if (len == 0) {
      return this->constraint.getFinalResult();
   }

   for (p_size i = 0; i < len; i++) {
      if (this->perun2.isNotRunning()) {
         return false;
      }

      const p_str v = os_trim(vs[i]);
      if (!v.empty() && !os_isInvalid(v)) {
         const p_nint s = os_size(os_leftJoin(this->context.location->value, v), this->perun2);
         if (s != NINT_MINUS_ONE) {
            this->constraint.increment(s);
         }
      }
   }

   return this->constraint.getFinalResult();
}


}

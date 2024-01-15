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

#include "gen-bool-constr.h"
#include "../../context/ctx-main.h"
#include "../../os/os.h"
#include "../../perun2.h"


namespace perun2::gen
{

#define P_INCR_CONSTR_START this->constraint.loadLimit(); \
   if (this->constraint.limitIsNaN()) { \
      return false;\
   } \
   this->constraint.setValueToZero();


ContextConstraint::ContextConstraint(p_genptr<p_num>& limit, const CompType cmptype, FileContext& ctx, p_perun2& p2)
   : constraint(limit, cmptype), context(ctx), perun2(p2) { };


SizeConstraint::SizeConstraint(p_genptr<p_num>& limit, const CompType cmptype, FileContext& ctx, const p_bool reread, p_perun2& p2)
   : ContextConstraint(limit, cmptype, ctx, p2), rereadFileSize(reread) { };


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
   if (! this->context.v_exists->value) {
      return false;
   }

   if (this->context.v_isfile->value) {
      if (this->rereadFileSize)  {
         const p_num n = os_sizeFile(this->context.v_path->value);

         if (n.state == NumberState::NaN) {
            return false;
         }

         this->constraint.setValue(n);
      }
      else {
         this->constraint.setValue(this->context.v_size->value);
      }

      return this->constraint.getFinalResult();
   }

   return os_sizeDirectorySatisfies(this->context.v_path->value, this->constraint, this->perun2);
}


p_bool CountConstraint::getValue()
{
   P_INCR_CONSTR_START;

   Logic state = this->constraint.getState();
   if (state != Logic::Unknown) {
      return state == Logic::True;
   }
   
   while (this->definition->hasNext()) {
      if (this->perun2.isNotRunning()) {
         this->definition->reset();
         return false;
      }

      this->constraint.incrementByOne();
      state = this->constraint.getState();
      
      if (state != Logic::Unknown) {
         this->definition->reset();
         return state == Logic::True;
      }
   }

   return this->constraint.getFinalResult();
}


p_bool CountInsideConstraint::getValue()
{
   P_INCR_CONSTR_START;

   if (! this->context.v_exists->value || this->context.v_isfile->value) {
      return false;
   }

   Logic state = this->constraint.getState();
   if (state != Logic::Unknown) {
      return state == Logic::True;
   }

   this->locContext->loadData(this->context.trimmed);

   while (this->definition->hasNext()) {
      if (this->perun2.isNotRunning()) {
         this->definition->reset();
         return false;
      }
      
      this->constraint.incrementByOne();
      state = this->constraint.getState();

      if (state != Logic::Unknown) {
         this->definition->reset();
         return state == Logic::True;
      }
   }

   return this->constraint.getFinalResult();
}


p_bool SizeConstraint_Def::getValue()
{
   P_INCR_CONSTR_START;

   Logic state = this->constraint.getState();
   if (state != Logic::Unknown) {
      return state == Logic::True;
   }

   while (this->definition->hasNext()) {
      if (this->perun2.isNotRunning()) {
         this->definition->reset();
         return false;
      }

      const p_str v = this->definition->getValue();
      const p_num n = os_size(os_leftJoin(this->context.location->value, v), this->perun2);

      if (n.state == NumberState::NaN) {
         return false;
      }

      this->constraint.increment(n);
      state = this->constraint.getState();

      if (state != Logic::Unknown) {
         this->definition->reset();
         return state == Logic::True;
      }
   }

   return this->constraint.getFinalResult();
}


p_bool SizeConstraint_List::getValue()
{
   P_INCR_CONSTR_START;

   const p_list vs = list->getValue();
   const p_size len = vs.size();
   p_bool any = false;

   if (len == 0) {
      return false;
   }

   for (p_size i = 0; i < len; i++) {
      if (this->perun2.isNotRunning()) {
         return false;
      }

      const p_str v = os_trim(vs[i]);
      if (!v.empty() && !os_isInvalid(v)) {
         const p_num n = os_size(os_leftJoin(this->context.location->value, v), this->perun2);

         if (n.state == NumberState::NaN) {
            return false;
         }

         this->constraint.increment(n);
         any = true;
         const Logic state = this->constraint.getState();

         if (state != Logic::Unknown) {
            return state == Logic::True;
         }
      }
   }
   
   return any
      ? this->constraint.getFinalResult()
      : false;
}


}

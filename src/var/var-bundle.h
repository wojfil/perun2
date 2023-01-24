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

#ifndef VAR_BUNDLE_H_INCLUDED
#define VAR_BUNDLE_H_INCLUDED

#include "var.h"
#include "../datatype/gen-ref.h"
#include "../command/com-var.h"
#include "../token.h"
#include "var-context.h"
#include "var-inner.h"


namespace uro::vars
{

template <typename T>
struct VarBundle
{
public:
   VarBundle(const std::map<_size, Variable<T>*>& iVars, uro::_uro& uro)
    : internalVars(iVars), uroboros(uro) { };

   void levelUp()
   {
      for (auto& kv : this->userVars) {
         kv.second.bracketsUp();
      }
   };

   void levelDown()
   {
      for (auto& kv : this->userVars) {
         kv.second.bracketsDown();
      }
   };

   _bool hasVar(const _size& hash)
   {
      return this->userVars.find(hash) != this->userVars.end()
          || this->internalVars.find(hash) != this->internalVars.end()
          || this->specialVars.find(hash) != this->specialVars.end();
   }

   _bool getValue(const Token& tk, _genptr<T>& result)
   {
      if (this->userVars.find(tk.value.word.h) != this->userVars.end()) {
         ParseVariable<T>& pv = this->userVars[tk.value.word.h];
         if (pv.isReachable()) {
            result = std::make_unique<gen::GeneratorRef<T>>(pv.getVarRef());
            return true;
         }
      }
      else if (this->internalVars.find(tk.value.word.h) != this->internalVars.end()) {
         this->uroboros.vc.setAttribute(tk, this->uroboros);
         const ThisState& state = this->uroboros.vars.inner.thisState;

         if (state != ThisState::ts_String) {
            const _str name = tk.getOriginString(this->uroboros);
            throw SyntaxError(str(L"the value of variable '", name,
               L"' is undefined here. Right there we are iterating over ",
               (state == ThisState::ts_Number) ? L"numbers. " : L"times. ",
               L"You should assign the value of '", name, L"' to a new temporary variable somewhere before"), tk.line);
         }

         Variable<T>& v = *this->internalVars[tk.value.word.h];
         result = std::make_unique<gen::GeneratorRef<T>>(v);
         return true;
      }
      else if (this->specialVars.find(tk.value.word.h) != this->specialVars.end()) {
         result = std::make_unique<gen::GeneratorPtrRef<T>>(this->specialVars[tk.value.word.h]);
         return true;
      }

      return false;
   }

   _bool getVarPtr(const Token& tk, ParseVariable<T>*& result)
   {
      if (this->userVars.find(tk.value.word.h) == this->userVars.end()) {
         return false;
      }
      else {
         result = &(this->userVars.find(tk.value.word.h)->second);
         return true;
      }
   }

   _bool makeVariableAssignment(_comptr& result, const Token& token, ParseVariable<T>* varPtr,
      _genptr<T>& valuePtr, const _bool& isConstant)
   {
      if (varPtr == nullptr) {
         const _size& hash = token.value.word.h;
         userVars.insert(std::make_pair(hash, ParseVariable<T>()));
         userVars[hash].var.isConstant_ = isConstant;
         if (isConstant) {
            userVars[hash].var.value = valuePtr->getValue();
         }
         result = std::make_unique<comm::VarAssignment<T>>(userVars[hash].getVarRef(), valuePtr);
         return true;
      }
      else {
         varPtr->resurrect(isConstant);
         result = std::make_unique<comm::VarAssignment<T>>(varPtr->getVarRef(), valuePtr);
         return true;
      }

      return false;
   }

   void makeNotConstant()
   {
      for (auto& kv : userVars) {
         kv.second.var.isConstant_ = false;
      }
   }

   void addSpecialVar(const _size& key, _genptr<T>& value)
   {
      specialVars.insert(std::make_pair(key, std::move(value)));
   }

private:
   std::map<_size, ParseVariable<T>> userVars;
   std::map<_size, Variable<T>*> internalVars; // dangling pointers to internal variables
   std::map<_size, _genptr<T>> specialVars;
   uro::_uro& uroboros;

};

}

#endif // VAR_BUNDLE_H_INCLUDED

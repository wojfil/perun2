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
   VarBundle(const std::map<_size, Variable<T>*>& iVars,
      const std::map<_size, Generator<T>*>& sVars, uro::Uroboros& uro)
    : userVars(), internalVars(iVars), specialVars(sVars), uroboros(uro) { };

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

   _bool getValue(VariablesContext* vc, const Token& tk, Generator<T>*& result, const InnerVariables& inner)
   {
      if (this->userVars.find(tk.value.word.h) != this->userVars.end()) {
         ParseVariable<T>* pv = &this->userVars[tk.value.word.h];
         if (pv->isReachable()) {
            result = new gen::GeneratorRef<T>(pv->getVarPtr());
            return true;
         }
      }
      else if (this->internalVars.find(tk.value.word.h) != this->internalVars.end()) {
         vc->setAttribute(tk, this->uroboros);

         if (inner.thisState != ThisState::ts_String) {
            const _str name = tk.getOriginString(this->uroboros);
            throw SyntaxException(str(L"the value of variable '", name,
               L"' is undefined here. Right there we are iterating over ",
               (inner.thisState == ThisState::ts_Number) ? L"numbers. " : L"times. ",
               L"You should assign the value of '", name, L"' to a new temporary variable somewhere before"), tk.line);
         }

         result = new gen::GeneratorRef<T>(this->internalVars[tk.value.word.h]);
         return true;
      }
      else if (this->specialVars.find(tk.value.word.h) != this->specialVars.end()) {
         result = new gen::GeneratorRef<T>(this->specialVars[tk.value.word.h]);
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

   Command* makeVariableAssignment(const Token& token, ParseVariable<T>* varPtr,
      Generator<T>* valuePtr, const _bool& isConstant)
   {
      if (varPtr == nullptr) {
         const _size& hash = token.value.word.h;
         userVars.insert(std::make_pair(hash, ParseVariable<T>()));
         userVars[hash].var.isConstant_ = isConstant;
         if (isConstant) {
            userVars[hash].var.value = valuePtr->getValue();
         }
         return new comm::VarAssignment<T>(userVars[hash].getVarPtr(), valuePtr);
      }
      else {
         varPtr->resurrect(isConstant);
         return new comm::VarAssignment<T>(varPtr->getVarPtr(), valuePtr);
      }
   }

   void makeNotConstant()
   {
      for (auto& kv : userVars) {
         kv.second.var.isConstant_ = false;
      }
   }

private:
   std::map<_size, ParseVariable<T>> userVars;
   std::map<_size, Variable<T>*> internalVars;
   std::map<_size, Generator<T>*> specialVars;
   uro::Uroboros& uroboros;

};

}

#endif // VAR_BUNDLE_H_INCLUDED

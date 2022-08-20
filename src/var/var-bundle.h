#ifndef VAR_BUNDLE_H_INCLUDED
#define VAR_BUNDLE_H_INCLUDED

#include "var.h"
#include "../datatype/gen-ref.h"
#include "../command/com-var.h"
#include "../token.h"
#include "var-context.h"
#include "var-inner.h"


template <typename T>
struct VarBundle
{
public:
   VarBundle(const std::map<_size, Variable<T>*>& iVars,
      const std::map<_size, Generator<T>*>& sVars)
    : userVars(), internalVars(iVars), specialVars(sVars) { };

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

   _boo hasVar(const _size& hash)
   {
      return this->userVars.find(hash) != this->userVars.end()
          || this->internalVars.find(hash) != this->internalVars.end()
          || this->specialVars.find(hash) != this->specialVars.end();
   }

   _boo getValue(VariablesContext* vc, const Token& tk, Generator<T>*& result, const InnerVariables& inner)
   {
      if (this->userVars.find(tk.value.word.h) != this->userVars.end()) {
         ParseVariable<T>* pv = &this->userVars[tk.value.word.h];
         if (pv->isReachable()) {
            result = new GeneratorRef<T>(pv->getVarPtr());
            return true;
         }
      }
      else if (this->internalVars.find(tk.value.word.h) != this->internalVars.end()) {
         vc->setAttribute(tk);

         if (inner.thisState != ThisState::ts_String) {
            const _str& name = *tk.value.word.os;
            throw SyntaxException(str(L"the value of variable '", name,
               L"' is undefined here. Right there we are iterating over ",
               (inner.thisState == ThisState::ts_Number) ? L"numbers. " : L"times. ",
               L"You should assign the value of '", name, L"' to a new temporary variable somewhere before"), tk.line);
         }

         result = new GeneratorRef<T>(this->internalVars[tk.value.word.h]);
         return true;
      }
      else if (this->specialVars.find(tk.value.word.h) != this->specialVars.end()) {
         result = new GeneratorRef<T>(this->specialVars[tk.value.word.h]);
         return true;
      }

      return false;
   }

   _boo getVarPtr(const Token& tk, ParseVariable<T>*& result)
   {
      if (this->userVars.find(tk.value.word.h) == this->userVars.end()) {
         return false;
      }
      else {
         result = &(this->userVars.find(tk.value.word.h)->second);
         return true;
      }
   }

   Command* makeVariableAssignment(const Token& token, ParseVariable<T>* varPtr, Generator<T>* valuePtr)
   {
      if (varPtr == nullptr) {
         const _size& hash = token.value.word.h;
         userVars.insert(std::make_pair(hash, ParseVariable<T>()));
         return new VarAssignment<T>(userVars[hash].getVarPtr(), valuePtr);
      }
      else {
         varPtr->resurrect();
         return new VarAssignment<T>(varPtr->getVarPtr(), valuePtr);
      }
   }

private:
   std::map<_size, ParseVariable<T>> userVars;
   std::map<_size, Variable<T>*> internalVars;
   std::map<_size, Generator<T>*> specialVars;

};

#endif // VAR_BUNDLE_H_INCLUDED

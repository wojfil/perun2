/*
    This file is part of Uroboros2.
    Uroboros2 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    Uroboros2 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with Uroboros2. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef CONTEXT_H_INCLUDED
#define CONTEXT_H_INCLUDED

#include "datatype/datatype.h"
#include "command/com-aggregate.h"
#include "datatype/generator/gen-os-gen.h"
#include "var/var.h"
#include "attribute.h"
#include <unordered_map>
#include <memory>
#include <iostream>


namespace uro
{
   struct _uro;
   struct Hashes;

   template <typename T>
   using _varptr = std::unique_ptr<vars::Variable<T>>;

   template <typename T>
   using _varptrs = std::unordered_map<_size, _varptr<T>>;


   struct VarsContext
   {
   public:

      void takeVarsPtr(_varptrs<_bool>*& result) { result = &this->bools; };
      void takeVarsPtr(_varptrs<_tim>*& result) { result = &this->times; };
      void takeVarsPtr(_varptrs<_per>*& result) { result = &this->periods; };
      void takeVarsPtr(_varptrs<_str>*& result) { result = &this->strings; };
      void takeVarsPtr(_varptrs<_num>*& result) { result = &this->numbers; };
      void takeVarsPtr(_varptrs<_tlist>*& result) { result = &this->timeLists; };
      void takeVarsPtr(_varptrs<_nlist>*& result) { result = &this->numLists; };
      void takeVarsPtr(_varptrs<_list>*& result) { result = &this->lists; };

      template <typename T2>
      _bool takeVar(const _size& var, vars::Variable<T2>*& result)
      {
         _varptrs<T2>* vars;
         this->takeVarsPtr(vars);
         auto v = vars->find(var);
         if (v != vars->end()) {
            result = v->second.get();
            return true;
         }

         return false;
      }

      template <typename T2>
      void addVar(const _size& var, const vars::VarType& type)
      {
         _varptrs<T2>* vars;
         this->takeVarsPtr(vars);
         //_varptr<T2> ptr = std::make_unique<vars::Variable<T2>>(type);
         //vars->insert(std::make_pair<_size, vars::Variable<T2>>(var, std::move(ptr)));

         vars->insert(std::make_pair(var, std::make_unique<vars::Variable<T2>>(type)));
      }

      _varptrs<_bool> bools;
      _varptrs<_tim> times;
      _varptrs<_per> periods;
      _varptrs<_str> strings;
      _varptrs<_num> numbers;
      _varptrs<_tlist> timeLists;
      _varptrs<_nlist> numLists;
      _varptrs<_list> lists;
   };

   struct UserVarsContext
   {
      VarsContext userVars;
   };

   struct AggregateContext
   {
      AggregateContext() = delete;
      AggregateContext(_uro& uro);
      void runAggregate() { this->aggregate.run(); }

      comm::Aggregate aggregate;
   };

   struct IndexContext : AggregateContext
   {
   public:
      IndexContext() = delete;
      IndexContext(_uro& uro);
      void resetIndex();
      void incrementIndex();

      _varptr<_num> index;
   };

   template <typename T>
   struct ThisContext : IndexContext
   {
   public:
      ThisContext<T>(_uro& uro)
         : IndexContext(uro),
           this_(std::make_unique<vars::Variable<T>>(vars::VarType::vt_Special)) { };

      _varptr<T> this_;
   };

   struct NumericContext : ThisContext<_num>
   {
   public:
      NumericContext() = delete;
      NumericContext(_uro& uro);
   };

   struct TimeContext : ThisContext<_tim>
   {
   public:
      TimeContext() = delete;
      TimeContext(_uro& uro);
   };

   struct FileContext : ThisContext<_str>
   {
   public:
      FileContext() = delete;
      FileContext(_uro& uro);
      FileContext(_attrptr& attr, _uro& uro);

      virtual void loadData(const _str& newThis);
      void reloadData();
      void loadAttributes();
      void resetIndexAndDepth();

      _attrptr attribute;
      VarsContext fileVars;
      _str trimmed;

      vars::Variable<_bool>* v_archive;
      vars::Variable<_bool>* v_compressed;
      vars::Variable<_bool>* v_empty;
      vars::Variable<_bool>* v_encrypted;
      vars::Variable<_bool>* v_exists;
      vars::Variable<_bool>* v_hidden;
      vars::Variable<_bool>* v_isdirectory;
      vars::Variable<_bool>* v_isfile;
      vars::Variable<_bool>* v_readonly;
      vars::Variable<_tim>* v_access;
      vars::Variable<_tim>* v_change;
      vars::Variable<_tim>* v_creation;
      vars::Variable<_tim>* v_modification;
      vars::Variable<_per>* v_lifetime;
      vars::Variable<_num>* v_size;
      vars::Variable<_num>* v_depth;
      vars::Variable<_str>* v_drive;
      vars::Variable<_str>* v_extension;
      vars::Variable<_str>* v_fullname;
      vars::Variable<_str>* v_name;
      vars::Variable<_str>* v_parent;
      vars::Variable<_str>* v_path;

   private:

      void initVars(_uro& uro);

      template <typename T>
      void addVar(const _size& hsh)
      {
         this->fileVars.addVar<T>(hsh, vars::VarType::vt_Attribute);
      }

   };

   struct LocationContext
   {
   public:
      LocationContext();
      LocationContext(LocationContext* prev);
      void loadData(const _str& newThis);

      _varptr<_str> location;

   private:
      LocationContext* prevLocation;
   };

   struct GlobalContext
   {
   public:
      GlobalContext() = delete;
      GlobalContext(_uro& uro);

      VarsContext globalVars;

   private:

      template <typename T>
      void addVar(const _size& hsh)
      {
         this->globalVars.addVar<T>(hsh, vars::VarType::vt_Special);
      }
   };


   typedef std::unique_ptr<UserVarsContext>        _ucptr;
   typedef std::unique_ptr<AggregateContext>       _acptr;
   typedef std::unique_ptr<IndexContext>           _icptr;
   typedef std::unique_ptr<NumericContext>         _ncptr;
   typedef std::unique_ptr<TimeContext>            _tcptr;
   typedef std::unique_ptr<FileContext>            _fcptr;
   typedef std::unique_ptr<LocationContext>        _lcptr;


   struct Contextes : GlobalContext
   {
   public:
      Contextes() = delete;
      Contextes(_uro& uro);

      _bool getVariable(const Token& tk, vars::Variable<_bool>*& result, _uro& uro);
      _bool getVariable(const Token& tk, vars::Variable<_tim>*& result, _uro& uro);
      _bool getVariable(const Token& tk, vars::Variable<_num>*& result, _uro& uro);
      _bool getVariable(const Token& tk, vars::Variable<_str>*& result, _uro& uro);
      _bool getVariable(const Token& tk, _defptr& result, _uro& uro);

      template <typename T>
      _bool getVariable(const Token& tk, _genptr<T>& result, _uro& uro)
      {
         vars::Variable<T>* varPtr;
         if (findVar(tk, varPtr, uro)) {
            result = std::make_unique<vars::VariableReference<T>>(varPtr);
            return true;
         }

         return false;
      }

      template <typename T>
      _bool getVariable(const Token& tk, vars::Variable<T>*& result, _uro& uro)
      {
         return findVar(tk, result, uro);
      }

      // context of a user variables - every structure of { } brackets
      void addUserVarsContext(UserVarsContext* ctx);
      void retreatUserVarsContext();

      // context of a raw block of commands: { print 2+2 }
      void addAggregateContext(AggregateContext* ctx);
      void retreatAggregateContext();

      // context of a repeat loop: 10 times { print 2+2 }
      void addIndexContext(IndexContext* ctx);
      void retreatIndexContext();

      // context of iteration over numbers: 2,3,4 { print this * 2 }
      void addNumericContext(NumericContext* ctx);
      void retreatNumericContext();

      // context of iteration over times: 2 june 2004, 12 april 2011 { print this.year }
      void addTimeContext(TimeContext* ctx);
      void retreatTimeContext();

      // context of iteration over file system elements or strings: files where extension = 'pdf'
      void addFileContext(FileContext* ctx);
      void retreatFileContext();

      // context of some functions: select directories where any(files)
      void addLocationContext(LocationContext* ctx);
      void retreatLocationContext();


      void markAllAttributesToRun();

      // for aggregates
      _bool nextContextWillBeAggregate();
      _bool hasAggregate();
      comm::Aggregate* getAggregate();

      LocationContext* getLocationContext();
      AggregateContext* getAggregateContext();
      _bool hasIterationContext() const;
      _bool hasFileContext() const;
      FileContext* getFileContext();
      std::vector<FileContext*>& getFileContexts();
      TimeContext* getTimeContext();
      NumericContext* getNumericContext();
      _bool hasIndexContext() const;
      void makeLocationContext(_lcptr& result);
      UserVarsContext* getUserVarsContext();

      _varptr<_bool> success;

   private:
      const Hashes& hashes;

      void retreatIterationContext();


      template <typename T>
      _bool findVar(const Token& tk, vars::Variable<T>*& result, _uro& uro)
      {
         const _size& var = tk.value.word.h;

         // look in global variables
         if (this->globalVars.takeVar(var, result)) {
            return true;
         }

         // look in user variables
         for (UserVarsContext* uvc : this->userVarsContexts) {
            if (uvc->userVars.takeVar(var, result)) {
               return true;
            }
         }

         // look in file attributes
         auto fw = this->hashes.HASH_GROUP_ATTR.find(var);

         if (fw != this->hashes.HASH_GROUP_ATTR.end()) {
            if (this->fileContexts.empty()) {
               throw SyntaxError::undefinedVarValue(tk.getOriginString(uro), tk.line);
            }

            FileContext* lastFileCtx = this->fileContexts.back();
            if (lastFileCtx != nullptr) {
               return lastFileCtx->fileVars.takeVar(var, result);
            }

            NumericContext* lastNumericCtx = this->numericContexts.back();
            if (lastNumericCtx == nullptr) {
               throw SyntaxError::timeIterationHere(tk.getOriginString(uro), tk.line);
            }
            else {
               throw SyntaxError::numericIterationHere(tk.getOriginString(uro), tk.line);
            }
         }

         return false;
      }

      void addOsGen(const _size& hash, const gen::OsElement& element, _uro& uro);

      LocationContext rootLocation;
      std::unordered_map<_size, gen::DefinitionGenerator> osGenerators;

      std::vector<UserVarsContext*> userVarsContexts;
      std::vector<AggregateContext*> aggregateContexts;
      std::vector<LocationContext*> locationContexts;
      std::vector<IndexContext*> indexContexts;

      std::vector<NumericContext*> numericContexts;
      std::vector<TimeContext*> timeContexts;
      std::vector<FileContext*> fileContexts;

   };


}

#endif // CONTEXT_H_INCLUDED

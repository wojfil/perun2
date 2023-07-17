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

#ifndef CONTEXT_H_INCLUDED
#define CONTEXT_H_INCLUDED

#include "datatype/datatype.h"
#include "command/com-aggregate.h"
#include "datatype/generator/gen-os-gen.h"
#include "var.h"
#include "attribute.h"
#include <unordered_map>
#include <memory>
#include <iostream>


namespace perun2
{
   struct p_perun2;

   template <typename T>
   using _varptr = std::unique_ptr<Variable<T>>;

   template <typename T>
   using _varptrs = std::unordered_map<p_str, _varptr<T>>;


   struct VarsContext
   {
   public:

      void takeVarsPtr(_varptrs<p_bool>*& result) { result = &this->bools; };
      void takeVarsPtr(_varptrs<_tim>*& result) { result = &this->times; };
      void takeVarsPtr(_varptrs<_per>*& result) { result = &this->periods; };
      void takeVarsPtr(_varptrs<p_str>*& result) { result = &this->strings; };
      void takeVarsPtr(_varptrs<_num>*& result) { result = &this->numbers; };
      void takeVarsPtr(_varptrs<_tlist>*& result) { result = &this->timeLists; };
      void takeVarsPtr(_varptrs<_nlist>*& result) { result = &this->numLists; };
      void takeVarsPtr(_varptrs<p_list>*& result) { result = &this->lists; };

      template <typename T>
      p_bool takeVar(const p_str& var, Variable<T>*& result)
      {
         _varptrs<T>* vars;
         this->takeVarsPtr(vars);
         auto v = vars->find(var);
         if (v != vars->end()) {
            result = v->second.get();
            return true;
         }

         return false;
      }

      template <typename T>
      Variable<T>* insertVar(const p_str& var, const VarType type)
      {
         _varptrs<T>* vars;
         this->takeVarsPtr(vars);
         auto a = vars->insert(std::make_pair(var, std::make_unique<Variable<T>>(type)));
         return a.first->second.get();
      }

      _varptrs<p_bool> bools;
      _varptrs<_tim> times;
      _varptrs<_per> periods;
      _varptrs<p_str> strings;
      _varptrs<_num> numbers;
      _varptrs<_tlist> timeLists;
      _varptrs<_nlist> numLists;
      _varptrs<p_list> lists;
   };

   struct UserVarsContext
   {
      VarsContext userVars;
   };

   struct AggregateContext
   {
      AggregateContext() = delete;
      AggregateContext(p_perun2& p2);
      void runAggregate() { this->aggregate.run(); }

      comm::Aggregate aggregate;
   };

   struct IndexContext : AggregateContext
   {
   public:
      IndexContext() = delete;
      IndexContext(p_perun2& p2);
      void resetIndex();
      void incrementIndex();

      _varptr<_num> index;
   };

   struct FileContext : IndexContext
   {
   public:
      FileContext() = delete;
      FileContext(p_perun2& p2);
      FileContext(_attrptr& attr, p_perun2& p2);

      void loadData(const p_str& newThis);
      void loadData(const p_str& newThis, const p_fdata& data);
      void reloadData();
      void loadAttributes();
      void loadAttributes(const p_fdata& data);

      p_bool attributeScope = true;
      _varptr<p_str> this_;
      _attrptr attribute;
      VarsContext fileVars;
      LocationContext* const locContext;
      p_str trimmed; // this is the value of this_, but with trimmed spaces and with proper path separators... used only internally
      p_bool invalid; // the value of this_ is invalid (has not allowed characters, is empty string etc.)
      p_bool isInside = false; // this is file context of the Inside structure

      Variable<p_bool>* v_archive;
      Variable<p_bool>* v_compressed;
      Variable<p_bool>* v_empty;
      Variable<p_bool>* v_encrypted;
      Variable<p_bool>* v_exists;
      Variable<p_bool>* v_hidden;
      Variable<p_bool>* v_isdirectory;
      Variable<p_bool>* v_isfile;
      Variable<p_bool>* v_readonly;
      Variable<_tim>* v_access;
      Variable<_tim>* v_change;
      Variable<_tim>* v_creation;
      Variable<_tim>* v_modification;
      Variable<_per>* v_lifetime;
      Variable<_num>* vp_size;
      Variable<_num>* v_depth;
      Variable<p_str>* v_drive;
      Variable<p_str>* v_extension;
      Variable<p_str>* v_fullname;
      Variable<p_str>* v_name;
      Variable<p_str>* v_parent;
      Variable<p_str>* v_path;

   private:
      void initVars(p_perun2& p2);

      template <typename T>
      Variable<T>* insertVar(const p_str& name)
      {
         return this->fileVars.insertVar<T>(name, VarType::vt_Attribute);
      }
   };

   struct LocationContext
   {
   public:
      LocationContext();
      LocationContext(LocationContext* prev);
      void loadData(const p_str& trimmedValue);

      _varptr<p_str> location;

   private:
      LocationContext* prevLocation;
   };

   struct GlobalContext
   {
   public:
      GlobalContext() = delete;
      GlobalContext(p_perun2& p2);

      template <typename T>
      void insertConstant(const p_str& name)
      {
         Variable<T>* v = this->globalVars.insertVar<T>(name, VarType::vt_Special);
         v->isConstant_ = true;
      }

      VarsContext globalVars;

   private:
      template <typename T>
      Variable<T>* insertVar(const p_str& name)
      {
         return this->globalVars.insertVar<T>(name, VarType::vt_Special);
      }
   };

   typedef std::unique_ptr<UserVarsContext>        _ucptr;
   typedef std::unique_ptr<AggregateContext>       _acptr;
   typedef std::unique_ptr<IndexContext>           _icptr;
   typedef std::unique_ptr<FileContext>            _fcptr;
   typedef std::unique_ptr<LocationContext>        _lcptr;


   struct Contexts : GlobalContext
   {
   public:
      Contexts() = delete;
      Contexts(p_perun2& p2);

      p_bool getVar(const Token& tk, Variable<p_bool>*& result, p_perun2& p2);
      p_bool getVar(const Token& tk, Variable<_num>*& result, p_perun2& p2);
      p_bool getVar(const Token& tk, Variable<p_str>*& result, p_perun2& p2);

      template <typename T>
      p_bool getVar(const Token& tk, Variable<T>*& result, p_perun2& p2)
      {
         return findVar(tk, result, p2);
      };

      // context of user variables - every structure of { } brackets
      void addUserVarsContext(UserVarsContext* ctx);
      void retreatUserVarsContext();

      // context of a raw block of commands: { print 2+2 }
      void addAggregateContext(AggregateContext* ctx);
      void retreatAggregateContext();

      // context of a repeat loop: 10 times { print 2+2 }
      void addIndexContext(IndexContext* ctx);
      void retreatIndexContext();

      // context of iteration over file system elements or strings: files where extension = 'pdf'
      void addFileContext(FileContext* ctx);
      void retreatFileContext();

      // working location context
      void addLocationContext(LocationContext* ctx);
      void retreatLocationContext();

      // for aggregates
      p_bool hasAggregate();
      comm::Aggregate* getAggregate();

      LocationContext* getLocationContext();
      AggregateContext* getAggregateContext();
      p_bool hasIterationContext() const;
      p_bool hasFileContext() const;
      FileContext* getFileContext();
      p_bool hasIndexContext() const;
      void makeLocationContext(_lcptr& result);
      UserVarsContext* getUserVarsContext();
      p_bool varExists(const Token& tk, p_perun2& p2);
      void closeAttributeScope();
      void closeDeepAttributeScope();

      _varptr<p_bool> success;
      std::unordered_map<p_str, gen::DefinitionGenerator> osGenerators;

   private:

      template <typename T>
      p_bool findVar(const Token& tk, Variable<T>*& result, p_perun2& p2)
      {
         const p_str name = tk.toLowerString(p2);

         // look in global variables
         if (this->globalVars.takeVar(name, result)) {
            p2.cache.actualize(tk);
            return true;
         }

         // look in user variables
         for (UserVarsContext* uvc : this->userVarsContexts) {
            if (uvc->userVars.takeVar(name, result)) {
               return true;
            }
         }

         // look in file attributes
         if (tk.isVariable(STRINGS_ATTR, p2)) {
            if (this->fileContexts.empty()) {
               throw SyntaxError::undefinedVarValue(tk.getOriginString(p2), tk.line);
            }

            FileContext* lastFileCtx = this->fileContexts.back();
            if (lastFileCtx->fileVars.takeVar(name, result)) {
               lastFileCtx->attribute->add(tk);
               return true;
            }
         }

         return false;
      }

      void addOsGen(const p_str& name, const gen::OsElement element, p_perun2& p2);

      LocationContext rootLocation;

      std::vector<UserVarsContext*> userVarsContexts;
      std::vector<AggregateContext*> aggregateContexts;
      std::vector<LocationContext*> locationContexts;
      std::vector<IndexContext*> indexContexts;
      std::vector<FileContext*> fileContexts;
   };
}

#endif // CONTEXT_H_INCLUDED

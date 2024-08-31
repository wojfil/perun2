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

#pragma once

#include "ctx-global.h"
#include "ctx-vars.h"
#include "ctx-file.h"
#include "ctx-aggr.h"
#include "../datatype/generator/gen-os.h"
#include "../datatype/generator/gen-os-gen.h"


namespace perun2
{
   struct Perun2Process;


   struct Contexts : GlobalContext
   {
   public:
      Contexts() = delete;
      Contexts(Perun2Process& p2);

      p_bool getVar(const Token& tk, Variable<p_bool>*& result, Perun2Process& p2);
      p_bool getVar(const Token& tk, Variable<p_num>*& result, Perun2Process& p2);
      p_bool getVar(const Token& tk, Variable<p_str>*& result, Perun2Process& p2);

      template <typename T>
      p_bool getVar(const Token& tk, Variable<T>*& result, Perun2Process& p2)
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
      void makeLocationContext(p_lcptr& result);
      UserVarsContext* getUserVarsContext();
      p_bool varExists(const Token& tk, Perun2Process& p2);
      void closeAttributeScope();
      void closeDeepAttributeScope();

      p_varptr<p_bool> success;
      std::unordered_map<p_str, gen::DefinitionGenerator> osGenerators;

   private:

      template <typename T>
      p_bool findVar(const Token& tk, Variable<T>*& result, Perun2Process& p2)
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

      void addOsGen(const p_str& name, const gen::OsElement element, Perun2Process& p2);

      LocationContext rootLocation;

      std::vector<UserVarsContext*> userVarsContexts;
      std::vector<AggregateContext*> aggregateContexts;
      std::vector<LocationContext*> locationContexts;
      std::vector<IndexContext*> indexContexts;
      std::vector<FileContext*> fileContexts;
   };

}

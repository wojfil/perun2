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

#include "arguments.h"
#include "print.h"
#include "cmd.h"
#include "os.h"


namespace uro
{

Arguments::Arguments(const _str& loc, const _str& cod)
   : location(loc), code(cod), args(_list()), flags(FLAG_NULL),
   parseState(ArgsParseState::aps_Ok) { };

Arguments::Arguments(const _str& loc, const _str& cod, const _uint32& fls)
   : location(loc), code(cod), args(_list()), flags(fls),
   parseState(ArgsParseState::aps_Ok) { };

Arguments::Arguments(const _int& argc, _char** const argv[])
{
   _bool options = true;
   _bool nextParseLocation = false;
   _bool hasCode = false;
   _bool hasValue = false;
   _bool here = false;
   _str value;
   _bool d_has = false;
   _str d_value;

   this->flags = FLAG_NULL;
   this->args = _list();
   this->parseState = ArgsParseState::aps_Failed;

   if (argc == 1) {
      rawPrint(L"Command-line error: missing arguments. Run 'uro --help' for command-line tips.");
      return;
   }

   for (_int i = 1; i < argc; i++) {
      const _str arg = _str((*argv)[i]);
      const _size len = arg.size();

      if (options && len >= 2 && arg[0] == L'-') {
         if (arg[1] == L'-') {
            if (len == 2) {
               options = false;
               continue;
            }

            _str lowerArg = arg;
            toLower(lowerArg);

            if (lowerArg == L"--version") {
               this->parseState = ArgsParseState::aps_PrintInfo;
               cmd_version();
            }
            else if (lowerArg == L"--docs") {
               this->parseState = ArgsParseState::aps_PrintInfo;
               cmd_docs();
            }
            else if (lowerArg == L"--website") {
               this->parseState = ArgsParseState::aps_PrintInfo;
               cmd_website();
            }
            else if (lowerArg == L"--help") {
               this->parseState = ArgsParseState::aps_PrintInfo;
               cmd_help();
            }
            else {
               rawPrint(str(L"Command-line error: unknown option '", arg.substr(2), L"'."));
            }

            return;
         }
         else {
            for (_size j = 1; j < len; j++) {
               switch (arg[j]) {
                  case L'c': case L'C': {
                     hasCode = true;
                     break;
                  }
                  case L'd': case L'D': {
                     nextParseLocation = true;
                     break;
                  }
                  case L'n': case L'N': {
                     if (!(this->flags & FLAG_NOOMIT)) {
                        this->flags |= FLAG_NOOMIT;
                     }
                     break;
                  }
                  case L's': case L'S': {
                     if (!(this->flags & FLAG_SILENT)) {
                        this->flags |= FLAG_SILENT;
                     }
                     break;
                  }
                  case L'g': case L'G': {
                     if (!(this->flags & FLAG_GUI)) {
                        this->flags |= FLAG_GUI;
                     }
                     break;
                  }
                  case L'h': case L'H': {
                     here = true;
                     break;
                  }
                  default: {
                     rawPrint(str(L"Command-line error: unknown option '", toStr(arg[j]), L"'."));
                     return;
                  }
               }
            }
         }
         continue;
      }

      if (nextParseLocation) {
         const _str v = os_trim(arg);

         if (v.empty()) {
            continue;
         }

         d_value = v;
         d_has = true;
         nextParseLocation = false;
         continue;
      }

      if (hasValue) {
         this->args.emplace_back(arg);
      }
      else {
         hasValue = true;
         value = arg;
      }
   }

   if (nextParseLocation) {
      rawPrint(L"Command-line error: destination directory has not been defined.");
      return;
   }

   if (!hasValue) {
      rawPrint(L"Command-line error: an argument is missing.");
      return;
   }

   const _str cdLocation = os_currentPath();

   if (hasCode) {
      this->code = value;
      if (d_has) {
         if (os_isAbsolute(d_value)) {
            this->location = d_value;
         }
         else {
            this->location = str(cdLocation, OS_SEPARATOR_STRING, d_value);
         }
      }
      else {
         this->location = cdLocation;
      }
   }
   else {
      _str filePath = os_trim(value);
      if (filePath.empty()) {
         rawPrint(L"Command-line error: no input file.");
         return;
      }

      if (!os_isAbsolute(filePath)) {
         filePath = str(cdLocation, OS_SEPARATOR_STRING, filePath);
      }

      if (os_hasExtension(filePath)) {
         if (os_extension(filePath) != OS_UROEXT) {
            rawPrint(str(L"Command-line error: wrong input file extension. Only '", OS_UROEXT, L"' is allowed."));
            return;
         }
      }
      else {
         filePath = str(filePath, OS_DOT_UROEXT);
      }

      if (!os_exists(filePath)) {
         rawPrint(str(L"Command-line error: input file '", os_fullname(filePath), L"' does not exist."));
         return;
      }

      _bool result = true;
      this->code = os_readFile(filePath, result);

      if (!result) {
         rawPrint(L"Command-line error: input file cannot be opened.");
         return;
      }

      const _str basisLocation = here ? cdLocation : os_parent(filePath);

      if (d_has) {
         if (os_isAbsolute(d_value)) {
            this->location = d_value;
         }
         else {
            this->location = str(basisLocation, OS_SEPARATOR_STRING, d_value);
         }
      }
      else {
         this->location = basisLocation;
      }
   }

   this->parseState = ArgsParseState::aps_Ok;
}

_uint32 Arguments::getFlags() const
{
   return this->flags;
}

_list Arguments::getArgs() const
{
   return this->args;
}

_str Arguments::getLocation() const
{
   return this->location;
}

const _str& Arguments::getCode() const
{
   return this->code;
}

ArgsParseState Arguments::getParseState() const
{
   return this->parseState;
}

}

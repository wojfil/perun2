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

#include "arguments.h"
#include "cmd.h"
#include "os.h"


namespace uro
{

Arguments::Arguments(const _str& loc, const _str& cod)
   : location(loc), code(cod), parseState(ArgsParseState::aps_Ok) { };

Arguments::Arguments(const _str& loc, const _str& cod, const _uint32 fls)
   : location(loc), code(cod), flags(fls), parseState(ArgsParseState::aps_Ok) { };

Arguments::Arguments(const _int argc, _char* const argv[])
{
   _bool options = true;
   _bool nextParseLocation = false;
   _bool hasCode = false;
   _bool hasValue = false;
   _bool here = false;
   _str value;
   _bool d_has = false;
   _str d_value;

   if (argc == 1) {
      cmd::error::noArguments();
      return;
   }

   for (_int i = 1; i < argc; i++) {
      const _str arg = _str(argv[i]);
      const _size len = arg.size();

      if (options && len >= 2 && arg[0] == CHAR_MINUS) {
         if (arg[1] == CHAR_MINUS) {
            if (len == 2) {
               options = false;
               continue;
            }

            _str lowerArg = arg;
            toLower(lowerArg);

            if (lowerArg == STRING_ARG_VERSION) {
               this->parseState = ArgsParseState::aps_PrintInfo;
               cmd::version();
            }
            else if (lowerArg == STRING_ARG_DOCS) {
               this->parseState = ArgsParseState::aps_PrintInfo;
               cmd::docs();
            }
            else if (lowerArg == STRING_ARG_WEBSITE) {
               this->parseState = ArgsParseState::aps_PrintInfo;
               cmd::website();
            }
            else if (lowerArg == STRING_ARG_HELP) {
               this->parseState = ArgsParseState::aps_PrintInfo;
               cmd::help();
            }
            else {
               cmd::error::unknownOption(arg.substr(2));
            }

            return;
         }
         else {
            for (_size j = 1; j < len; j++) {
               switch (arg[j]) {
                  case CHAR_FLAG_CODE: 
                  case CHAR_FLAG_CODE_UPPER: {
                     hasCode = true;
                     break;
                  }
                  case CHAR_FLAG_DIRECTORY: 
                  case CHAR_FLAG_DIRECTORY_UPPER: {
                     nextParseLocation = true;
                     break;
                  }
                  case CHAR_FLAG_NOOMIT: 
                  case CHAR_FLAG_NOOMIT_UPPER: {
                     if (!(this->flags & FLAG_NOOMIT)) {
                        this->flags |= FLAG_NOOMIT;
                     }
                     break;
                  }
                  case CHAR_FLAG_SILENT: 
                  case CHAR_FLAG_SILENT_UPPER: {
                     if (!(this->flags & FLAG_SILENT)) {
                        this->flags |= FLAG_SILENT;
                     }
                     break;
                  }
                  case CHAR_FLAG_GUI: 
                  case CHAR_FLAG_GUI_UPPER: {
                     if (!(this->flags & FLAG_GUI)) {
                        this->flags |= FLAG_GUI;
                     }
                     break;
                  }
                  case CHAR_FLAG_HERE: 
                  case CHAR_FLAG_HERE_UPPER: {
                     here = true;
                     break;
                  }
                  default: {
                     cmd::error::unknownOption(toStr(arg[j]));
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
      cmd::error::noDestination();
      return;
   }

   if (!hasValue) {
      cmd::error::noMainArgument();
      return;
   }

   const _str cdLocation = os_currentPath();

   if (hasCode) {
      this->code = value;
      if (d_has) {
         this->location = os_isAbsolute(d_value)
            ? d_value
            : str(cdLocation, OS_SEPARATOR_STRING, d_value);
      }
      else {
         this->location = cdLocation;
      }
   }
   else {
      _str filePath = os_trim(value);
      if (filePath.empty()) {
         cmd::error::noInput();
         return;
      }

      if (!os_isAbsolute(filePath)) {
         filePath = str(cdLocation, OS_SEPARATOR_STRING, filePath);
      }

      if (os_hasExtension(filePath)) {
         const _str extension = os_extension(filePath);

         if (extension != OS_UROEXT) {
            cmd::error::wrongFileExtension(OS_UROEXT);
            return;
         }
      }
      else {
         filePath = str(filePath, OS_DOT_UROEXT);
      }

      if (!os_exists(filePath)) {
         cmd::error::fileNotFound(os_fullname(filePath));
         return;
      }

      if (!os_readFile(this->code, filePath)) {
         cmd::error::fileReadFailure(os_fullname(filePath));
         return;
      }

      const _str basisLocation = here ? cdLocation : os_parent(filePath);

      if (d_has) {
         this->location = os_isAbsolute(d_value)
            ? d_value
            : str(basisLocation, OS_SEPARATOR_STRING, d_value);
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

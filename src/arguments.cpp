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

#include "arguments.h"
#include "cmd.h"
#include "metadata.h"
#include "os/os.h"


namespace perun2
{

Arguments::Arguments(const p_str& loc, const p_str& cod)
   : location(loc), code(cod), parseState(ArgsParseState::aps_Ok) { };

Arguments::Arguments(const p_str& loc, const p_str& cod, const p_flags fls)
   : location(loc), code(cod), flags(fls), parseState(ArgsParseState::aps_Ok) { };

Arguments::Arguments(const p_int argc, p_char* const argv[])
{
   enum NextArg {
      Null,
      Location,
      Code
   };

   NextArg nextArg = NextArg::Null;
   p_bool options = true;
   p_bool hasCode = false;
   p_bool hasValue = false;
   p_bool here = false;
   p_str value;
   p_bool d_has = false;
   p_str d_value;

   if (argc == 1) {
      cmd::error::noArguments();
      return;
   }

   for (p_int i = 1; i < argc; i++) {
      const p_str arg = p_str(argv[i]);
      const p_size len = arg.size();

      if (options && nextArg == NextArg::Location) {
         const p_str v = os_trim(arg);

         if (v.empty()) {
            continue;
         }

         d_value = v;
         d_has = true;
         nextArg = NextArg::Null;
         continue;
      }
      else if (options && nextArg == NextArg::Code) {
         value = arg;
         hasCode = true;
         hasValue = true;
         nextArg = NextArg::Null;
         continue;
      }

      if (options && len >= 2 && arg[0] == CHAR_MINUS) {
         if (arg[1] == CHAR_MINUS) {
            if (len == 2) {
               options = false;
               continue;
            }

            p_str lowerArg = arg;
            str_toLower(lowerArg);

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
            for (p_size j = 1; j < len; j++) {
               switch (arg[j]) {
                  case CHAR_FLAG_CODE: 
                  case CHAR_FLAG_CODE_UPPER: {
                     nextArg = NextArg::Code;
                     break;
                  }
                  case CHAR_FLAG_DIRECTORY: 
                  case CHAR_FLAG_DIRECTORY_UPPER: {
                     nextArg = NextArg::Location;
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
                  case CHAR_FLAG_HERE: 
                  case CHAR_FLAG_HERE_UPPER: {
                     here = true;
                     break;
                  }
                  case CHAR_FLAG_GUI:
                  case CHAR_FLAG_GUI_UPPER: {
                     if (!(this->flags & FLAG_GUI)) {
                        this->flags |= FLAG_GUI;
                     }
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

      if (hasValue) {
         this->args.emplace_back(arg);
      }
      else {
         hasValue = true;
         value = arg;
      }
   }

   if (nextArg == NextArg::Location) {
      cmd::error::noDestination();
      return;
   }

   if (nextArg == NextArg::Code) {
      cmd::error::noCode();
      return;
   }

   if (!hasValue) {
      cmd::error::noMainArgument();
      return;
   }

   const p_str cdLocation = os_currentPath();

   if (hasCode) {
      this->code = value;
      if (d_has) {
         this->location = os_isAbsolute(d_value)
            ? d_value
            : os_join(cdLocation, d_value);
      }
      else {
         this->location = cdLocation;
      }
      
      this->parseState = ArgsParseState::aps_Ok;
      return;
   }

   p_str filePath = os_trim(value);
   if (filePath.empty()) {
      cmd::error::noInput();
      return;
   }

   if (!os_isAbsolute(filePath)) {
      filePath = os_join(cdLocation, filePath);
   }

   if (os_hasExtension(filePath)) {
      if (! os_isPerun2Extension(filePath)) {
         cmd::error::wrongFileExtension();
         return;
      }
   }
   else {
      filePath = str(filePath, CHAR_DOT, metadata::EXTENSION);
   }

   if (!os_fileExists(filePath)) {
      cmd::error::fileNotFound(os_fullname(filePath));
      return;
   }

   if (!os_readFile(this->code, filePath)) {
      cmd::error::fileReadFailure(os_fullname(filePath));
      return;
   }

   const p_str basisLocation = here ? cdLocation : os_parent(filePath);

   if (d_has) {
      this->location = os_isAbsolute(d_value)
         ? d_value
         : os_join(basisLocation, d_value);
   }
   else {
      this->location = basisLocation;
   }

   this->parseState = ArgsParseState::aps_Ok;
}

p_bool Arguments::areGood() const
{
   return this->parseState == ArgsParseState::aps_Ok;
}

p_flags Arguments::getFlags() const
{
   return this->flags;
}

p_list Arguments::getArgs() const
{
   return this->args;
}

p_str Arguments::getLocation() const
{
   return this->location;
}

const p_str& Arguments::getCode() const
{
   return this->code;
}

ArgsParseState Arguments::getParseState() const
{
   return this->parseState;
}

p_bool Arguments::hasFlag(const p_flags flag) const
{
   return this->flags & flag;
}

}

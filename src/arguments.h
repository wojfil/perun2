#ifndef ARGUMENTS_H_INCLUDED
#define ARGUMENTS_H_INCLUDED

#include "datatype/datatype.h"


enum ArgsParseState {
   aps_Ok = 0,
   aps_PrintInfo,
   aps_Failed
};


struct Arguments
{
public:

   Arguments(const _str& location, const _str& code);
   Arguments(_int* argc, _char** argv[]);

   _uint32 getFlags() const;
   _list getArgs() const;
   _str getLocation() const;
   _str getCode() const;
   ArgsParseState getParseState() const;

private:

   _uint32 flags;
   _list args;
   _str location;
   _str code;
   ArgsParseState parseState;
};


#endif // ARGUMENTS_H_INCLUDED

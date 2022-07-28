#ifndef GEN_PATTERN_H_INCLUDED
#define GEN_PATTERN_H_INCLUDED

#include "gen-os.h"


struct PatternParser
{
public:
   PatternParser(Uroboros* uro)
      : uroboros(uro), defGenerator(ELEM_NONE, uro) { };

   _boo parse(const _str& pattern, const _int& asteriskId, _def*& result) const;

private:
   Uroboros* uroboros;
   const DefinitionGenerator defGenerator;
};


#endif // GEN_PATTERN_H_INCLUDED

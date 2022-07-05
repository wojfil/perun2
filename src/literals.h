#ifndef LITERALS_H_INCLUDED
#define LITERALS_H_INCLUDED

#include "datatype/primitives.h"
#include <vector>


struct Literals
{
public:
   Literals();
   ~Literals();
   void addValue(_str* value);

private:
   std::vector<_str*> values;
};


#endif // LITERALS_H_INCLUDED

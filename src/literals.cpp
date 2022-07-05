#include "literals.h"
#include "util.h"


Literals::Literals()
   : values(std::vector<_str*>()) { };

Literals::~Literals()
{
   deleteVector(this->values);
};

void Literals::addValue(_str* value)
{
   this->values.push_back(value);
};

#ifndef PARSE_UNIT_H_INCLUDED
#define PARSE_UNIT_H_INCLUDED

#include "../../tokens.h"


// turn one token into a generator
// overloading is applied, so these functions can fit into template parsing in other places
_boo parseOneToken(Uroboros* uro, const Token& tk, Generator<_boo>*& result);
_boo parseOneToken(Uroboros* uro, const Token& tk, Generator<_num>*& result);
_boo parseOneToken(Uroboros* uro, const Token& tk, Generator<_str>*& result);
_boo parseOneToken(Uroboros* uro, const Token& tk, Generator<_nlist>*& result);
_boo parseOneToken(Uroboros* uro, const Token& tk, Generator<_tlist>*& result);
_boo parseOneToken(Uroboros* uro, const Token& tk, Generator<_list>*& result);
_boo parseOneToken(Uroboros* uro, const Token& tk, Generator<_tim>*& result);
_boo parseOneToken(Uroboros* uro, const Token& tk, Generator<_per>*& result);
_boo parseOneToken(Uroboros* uro, const Token& tk, _def*& result);


#endif // PARSE_UNIT_H_INCLUDED

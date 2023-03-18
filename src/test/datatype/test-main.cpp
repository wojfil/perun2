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

#include <iostream>
#include <locale>
#include <clocale>
#include <fcntl.h>
#include <io.h>
#include <stdio.h>
#include "../../datatype/generator/gen-like.h"
#include "../../datatype/primitives.h"

#include "test-like.h"
#include "test-in.h"
#include "test-const.h"
#include "test-time.h"

using namespace perun2;

_int wmain(_int argc, _char* argv[], _char* envp[])
{
    std::setlocale(LC_CTYPE, "");
    _setmode(_fileno(stdout), _O_U8TEXT);
    _cout << L"DATA TYPE TESTS START" << std::endl;
	
	test_like();
	test_in();
	test_const();
	test_time();
	
    _cout << L"DATA TYPE TESTS END" << std::endl;
    _cout << L"All tests have passed successfully if there is no error message above." << std::endl;
	
    return 0;
}

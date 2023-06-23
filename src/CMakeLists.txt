cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_COMPILER g++)
set(CMAKE_CXX_STANDARD ${USED_CPP_STANDARD})
SET(CMAKE_CXX_COMPILER C:/MinGW/bin/gcc.exe)
project(perun2)

enable_language(RC)
set(CMAKE_RC_COMPILER windres)

add_compile_options(
    -Wall 
    -Wno-reorder
    -Wno-unused-function
    -O2 
    -ftemplate-backtrace-limit=0
    -fomit-frame-pointer
    -fexpensive-optimizations
    -ffunction-sections 
    -fdata-sections
    -fvisibility=hidden
    -flto
    -std=c++17
    -std=gnu++17
    -ftemplate-backtrace-limit=0
    -s
    -static-libstdc++
    -static-libgcc
    -static
    -lole32
    -loleaut32
)

add_executable(
    perun2 
    main.cpp 
    wndres.rc

    arguments.cpp
    attribute.cpp
    brackets.cpp
    cache.cpp
    cmd.cpp
    console.cpp
    context.cpp
    exception.cpp
    keyword.cpp
    lexer.cpp
    logger.cpp
    os.cpp
    perun2.cpp
    terminator.cpp
    token.cpp
    tokens.cpp
    command/com.cpp
    command/com-aggregate.cpp
    command/com-arg.cpp
    datatype/cast.cpp
    datatype/chars.cpp
    datatype/concat.cpp
    datatype/math.cpp
    datatype/number.cpp
    datatype/order-limit-one.cpp
    datatype/order.cpp
    datatype/parse-gen.cpp
    datatype/period.cpp
    datatype/strings.cpp
    datatype/time.cpp
    datatype/wildcard.cpp
    command/com-condition.cpp
    command/com-core-aggr.cpp
    command/com-core.cpp
    command/com-create.cpp
    command/com-misc.cpp
    command/com-parse-kw.cpp
    command/com-parse-unit.cpp
    command/com-parse.cpp
    command/com-struct.cpp
    command/com-var.cpp
    datatype/function/func-aggr.cpp
    datatype/function/func-attr.cpp
    datatype/function/func-bool.cpp
    datatype/function/func-list.cpp
    datatype/parse/parse-asterisk.cpp
    datatype/parse/parse-bool.cpp
    datatype/parse/parse-definition.cpp
    datatype/parse/parse-function.cpp
    datatype/parse/parse-generic.cpp
    datatype/parse/parse-list.cpp
    datatype/parse/parse-number.cpp
    datatype/parse/parse-numlist.cpp
    datatype/parse/parse-period.cpp
    datatype/parse/parse-string.cpp
    datatype/parse/parse-time.cpp
    datatype/parse/parse-timlist.cpp
    datatype/parse/parse-unit.cpp
    datatype/parse/parse-var.cpp
    datatype/function/func-number.cpp
    datatype/function/func-string.cpp
    datatype/function/func-time.cpp
    datatype/generator/gen-bool-compare.cpp
    datatype/generator/gen-bool.cpp
    datatype/generator/gen-definition.cpp
    datatype/generator/gen-double-asterisk.cpp
    datatype/generator/gen-like.cpp
    datatype/generator/gen-list.cpp
    datatype/generator/gen-number.cpp
    datatype/generator/gen-os-gen.cpp
    datatype/generator/gen-os.cpp
    datatype/generator/gen-period.cpp
    datatype/generator/gen-string.cpp
    datatype/generator/gen-time.cpp
)

target_link_libraries(perun2
    PRIVATE
    -lstdc++
)


#
#    This file is part of Perun2.
#    Perun2 is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#    Perun2 is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
#    GNU General Public License for more details.
#    You should have received a copy of the GNU General Public License
#    along with Perun2. If not, see <http://www.gnu.org/licenses/>.
#

cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_COMPILER g++)
set(CMAKE_CXX_STANDARD ${USED_CPP_STANDARD})
set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++ -static")


project(perun2)

enable_language(RC)
set(CMAKE_RC_COMPILER windres)

add_compile_options(
    -O2 
    -Wall 
    -Wno-reorder
    -Wno-switch
    -Wno-unused-function
    -Wreturn-type
    -fdata-sections
    -fexpensive-optimizations
    -ffunction-sections 
    -flto
    -fno-rtti
    -fomit-frame-pointer
    -ftemplate-backtrace-limit=0
    -fvisibility=hidden
    -s 
)

add_executable(
    perun2 

    main.cpp 
    wndres.rc

    arguments.cpp
    attribute.cpp
    brackets.cpp
    cmd.cpp
    console.cpp
    post-parse-data.cpp
    side-process.cpp
    exception.cpp
    keyword.cpp
    lexer.cpp
    logger.cpp
    perun2.cpp
    terminator.cpp
    token.cpp
    tokens.cpp
    command/com-aggregate.cpp
    command/com-arg.cpp
    command/com-condition.cpp
    command/com-copyto.cpp
    command/com-core-aggr.cpp
    command/com-core.cpp
    command/com-create.cpp
    command/com-def-action.cpp
    command/com-misc.cpp
    command/com-moveto.cpp
    command/com-parse-kw.cpp
    command/com-parse-unit.cpp
    command/com-parse.cpp
    command/com-python3.cpp
    command/com-renameto.cpp
    command/com-struct.cpp
    command/com-time.cpp
    command/com-var.cpp
    context/ctx-aggr.cpp
    context/ctx-file.cpp
    context/ctx-global.cpp
    context/ctx-index.cpp
    context/ctx-location.cpp
    context/ctx-main.cpp
    datatype/between.cpp
    datatype/cast.cpp
    datatype/comparison.cpp
    datatype/definition.cpp
    datatype/incr-constr.cpp
    datatype/math.cpp
    datatype/number.cpp
    datatype/order-limit-one.cpp
    datatype/order.cpp
    datatype/parse-gen.cpp
    datatype/period.cpp
    datatype/time.cpp
    datatype/function/func-aggr.cpp
    datatype/function/func-attr.cpp
    datatype/function/func-bool.cpp
    datatype/function/func-list.cpp
    datatype/function/func-number.cpp
    datatype/function/func-string.cpp
    datatype/function/func-time.cpp
    datatype/generator/gen-bool-compare.cpp
    datatype/generator/gen-bool-constr.cpp
    datatype/generator/gen-bool.cpp
    datatype/generator/gen-definition.cpp
    datatype/generator/gen-double-asterisk.cpp
    datatype/generator/gen-list.cpp
    datatype/generator/gen-number.cpp
    datatype/generator/gen-os-gen.cpp
    datatype/generator/gen-os.cpp
    datatype/generator/gen-period.cpp
    datatype/generator/gen-string.cpp
    datatype/generator/gen-time.cpp
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
    datatype/text/chars.cpp
    datatype/text/concat.cpp
    datatype/text/like.cpp
    datatype/text/raw.cpp
    datatype/text/regexp.cpp
    datatype/text/resemblance.cpp
    datatype/text/strings.cpp
    datatype/text/text-parsing.cpp
    datatype/text/wildcard.cpp
    os/os-common.cpp
    os/os-linux.cpp
    os/os-mac.cpp
    os/os-posix.cpp
    os/os-windows.cpp
    programs/linux/linux-programs.cpp
    programs/mac/mac-programs.cpp
    programs/windows/pattern.cpp
    programs/windows/programs-data.cpp
    programs/windows/registry.cpp
    programs/windows/start-menu.cpp
    programs/windows/win-programs.cpp
)


set(FFMPEG_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/../external/ffmpeg/include)

if(EXISTS ${FFMPEG_INCLUDE_DIR})
    message(STATUS "FFmpeg include directory found: ${FFMPEG_INCLUDE_DIR}")
    target_include_directories(perun2 PRIVATE ${FFMPEG_INCLUDE_DIR})
else()
    message(FATAL_ERROR "FFmpeg include directory not found: ${FFMPEG_INCLUDE_DIR}")
endif()


set(FFMPEG_LIBS
    ${CMAKE_SOURCE_DIR}/../external/ffmpeg/lib/avcodec.lib
    ${CMAKE_SOURCE_DIR}/../external/ffmpeg/lib/avformat.lib
    ${CMAKE_SOURCE_DIR}/../external/ffmpeg/lib/avutil.lib
)

foreach(lib ${FFMPEG_LIBS})
    if(EXISTS ${lib})
        message(STATUS "FFmpeg library found: ${lib}")
    else()
        message(FATAL_ERROR "FFmpeg library not found: ${lib}")
    endif()
endforeach()

find_library(AVCODEC_LIB NAMES avcodec PATHS ${CMAKE_SOURCE_DIR}/../external/ffmpeg/lib)
find_library(AVFORMAT_LIB NAMES avformat PATHS ${CMAKE_SOURCE_DIR}/../external/ffmpeg/lib)
find_library(AVUTIL_LIB NAMES avutil PATHS ${CMAKE_SOURCE_DIR}/../external/ffmpeg/lib)

target_link_libraries(
    
    perun2 PRIVATE

    stdc++
    ole32 
    oleaut32
    shell32
    ${FFMPEG_LIBS}
)

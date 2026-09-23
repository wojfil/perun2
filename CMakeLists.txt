cmake_minimum_required(VERSION 3.21)
project(perun2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(ICU REQUIRED COMPONENTS uc i18n)
find_package(FFMPEG REQUIRED)

add_subdirectory(src)

cmake_minimum_required(VERSION 3.10)

project(SpaceShell CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(include)

add_executable(
spaceshell
src/main.cpp
src/shell.cpp
src/parser.cpp
src/builtins.cpp
src/signals.cpp
src/prompt.cpp
)

find_package(Threads REQUIRED)
target_link_libraries(spaceshell PRIVATE Threads::Threads)

find_program(CLANG_FORMAT_EXE NAMES clang-format)

if (CLANG_FORMAT_EXE)
  file(GLOB_RECURSE ALL_SOURCE_FILES
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_SOURCE_DIR}/include/*.hpp"
  )

  add_custom_target(format
    COMMAND ${CLANG_FORMAT_EXE} -i ${ALL_SOURCE_FILES}
    COMMENT "[ClANG] Formateando el código fuente con clang-format..."
  )
endif()

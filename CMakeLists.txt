#########################################################
# CMAKE_C_COMPILER and CMAKE_CXX_COMPILER must be defined
# before the project statement; However, the project
# statement will clear CMAKE_BUILD_TYPE. CACHE variables,
# along with the order of this code, are used to work
# around these constraints.
#
# Don't put any code above or in this block, unless it
# has similar constraints.
cmake_minimum_required(VERSION 3.1.0)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/Builds/CMake")
include(CMakeFuncs)
set(openssl_min 1.0.2)
parse_target()
project(ripple-graph)
#########################################################

if("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
  set(dir "build")
  set(cmd "cmake")
  if (target)
    set(dir "${dir}/${target}")
    set(cmd "${cmd} -Dtarget=${target}")
  elseif(CMAKE_BUILD_TYPE)
    set(dir "${dir}/${CMAKE_BUILD_TYPE}")
    set(cmd "${cmd} -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}")
  else()
    set(dir "${dir}/default")
  endif()
  set(cmd "${cmd} ${CMAKE_SOURCE_DIR}")

  message(FATAL_ERROR "Builds are not allowed in ${CMAKE_SOURCE_DIR}.\n"
    "Instead:\n"
    "1) Remove the CMakeCache.txt file and CMakeFiles directory "
   "from ${CMAKE_SOURCE_DIR}.\n"
    "2) Create a directory to hold your build files, for example: ${dir}.\n"
    "3) Change to that directory.\n"
    "4) Run cmake targetting ${CMAKE_SOURCE_DIR}, for example: ${cmd}")
endif()
if("${CMAKE_GENERATOR}" MATCHES "Visual Studio" AND
    NOT ("${CMAKE_GENERATOR}" MATCHES .*Win64.*))
  message(FATAL_ERROR "Visual Studio 32-bit build is unsupported. Use
    -G\"${CMAKE_GENERATOR} Win64\"")
endif()

setup_build_cache()

if(nonunity)
    get_cmake_property(allvars VARIABLES)
    string(REGEX MATCHALL "[^;]*(DEBUG|RELEASE)[^;]*" matchvars "${allvars}")
    foreach(var IN LISTS matchvars)
        string(REGEX REPLACE "(DEBUG|RELEASE)" "\\1CLASSIC" newvar ${var})
        set(${newvar} ${${var}})
    endforeach()

    get_cmake_property(allvars CACHE_VARIABLES)
    string(REGEX MATCHALL "[^;]*(DEBUG|RELEASE)[^;]*" matchvars "${allvars}")
    foreach(var IN LISTS matchvars)
        string(REGEX REPLACE "(DEBUG|RELEASE)" "\\1CLASSIC" newvar ${var})
        set(${newvar} ${${var}} CACHE STRING "Copied from ${var}")
    endforeach()
endif()

determine_build_type()

check_gcc4_abi()

#########################################################

include_directories(
  src
  src/beast
  src/beast/include
  src/beast/extras)
#  src/nudb/include
#  src/soci/src
#  src/soci/include)

special_build_flags()

#########################################################

use_boost(
  # resist the temptation to alphabetize these. coroutine
  # must come before context.
  chrono
  coroutine
  context
  date_time
  filesystem
  program_options
  regex
  system
  thread)

use_pthread()

use_openssl(${openssl_min})

#use_protobuf()

setup_build_boilerplate()

#########################################################
#########################################################

# Unity sources
prepend(beast_unity_srcs
        src/ripple/beast/unity/
#        beast_insight_unity.cpp
#        beast_net_unity.cpp
#        beast_utility_unity.cpp)
        )

prepend(ripple_unity_srcs
        src/ripple/unity/
#        basics.cpp
#        beast.cpp)
        )

prepend(test_unity_srcs
        src/test/unity/
        )

list(APPEND unity_srcs ${beast_unity_srcs} ${ripple_unity_srcs} ${test_unity_srcs})
#########################################################
prepend(graph_srcs src/ripple/graph/
        graph.cpp)

include_directories("${CMAKE_SOURCE_DIR}"/src)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I${CMAKE_SOURCE_DIR}/src -std=c++14")

add_executable(ripple-graph ${graph_srcs} ${unity_srcs})
############################################################
list(APPEND targets "ripple-graph")
find_program(
        B2_EXE
        NAMES b2
        HINTS ${BOOST_ROOT}
        PATHS ${BOOST_ROOT}
        DOC "Location of the b2 build executable from Boost")
if(${B2_EXE} STREQUAL "B2_EXE-NOTFOUND")
    message(WARNING
            "Boost b2 executable not found. docs target will not be buildable")
elseif(NOT BOOST_ROOT)
    if(Boost_INCLUDE_DIRS)
        set(BOOST_ROOT ${Boost_INCLUDE_DIRS})
    else()
        get_filename_component(BOOST_ROOT ${B2_EXE} DIRECTORY)
    endif()
endif()
# The value for BOOST_ROOT will be determined based on
#   1) The environment BOOST_ROOT
#   2) The Boost_INCLUDE_DIRS found by `get_boost`
#   3) The folder the `b2` executable is found in.
# If those checks don't yield the correct path, BOOST_ROOT
# can be defined on the cmake command line:
#    cmake <path> -DBOOST_ROOT=<boost_path>
if(BOOST_ROOT)
    set(B2_PARAMS "-sBOOST_ROOT=${BOOST_ROOT}")
endif()

# Find bash to help Windows avoid file association problems
find_program(
        BASH_EXE
        NAMES bash sh
        DOC "Location of the bash shell executable"
)
if(${BASH_EXE} STREQUAL "BASH_EXE-NOTFOUND")
    message(WARNING
            "Unable to find bash executable. docs target may not be buildable")
    set(BASH_EXE "")
endif()

set_startup_project(ripple-graph)

foreach(target IN LISTS targets)
    target_link_libraries(${target}
            ${OPENSSL_LIBRARIES} ${PROTOBUF_LIBRARIES} ${SANITIZER_LIBRARIES})

    link_common_libraries(${target})
endforeach()

if (NOT CMAKE_SIZEOF_VOID_P EQUAL 8)
    message(WARNING "Rippled requires a 64 bit target architecture.\n"
            "The most likely cause of this warning is trying to build rippled with a 32-bit OS.")
endif()



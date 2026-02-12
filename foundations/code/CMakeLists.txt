# The minimum required version of cmake
cmake_minimum_required(VERSION 3.28)

# Set required C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# The name of the project
project(foundations)

# Use Debug as default build
if(NOT DEFINED CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# Configure static linking
option(BUILD_SHARED_LIBS "Build a shared (ON) or static (OFF) executable" ON)
if(APPLE)
    option(USE_LIBCPP "Use libc++ instead of libstdc++" ON)
    option(USE_CXXABI "Use cxxabi for clang" ON)
else()
    option(USE_LIBCPP "Use libc++ instead of libstdc++" OFF)
    option(USE_CXXABI "Use cxxabi for clang" OFF)
endif()

if(NOT ${BUILD_SHARED_LIBS})
    #disable -rdynamic
    set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "")
    set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")
    # prefer static libraries
    if(WIN32)
        set(CMAKE_FIND_LIBRARY_SUFFIXES .lib .a ${CMAKE_FIND_LIBRARY_SUFFIXES})
    else()
        set(CMAKE_FIND_LIBRARY_SUFFIXES .a ${CMAKE_FIND_LIBRARY_SUFFIXES})
    endif()
endif()

# Compiler specific additional flags (e.g. {GCC,CLANG,MSVC}_CXX_FLAGS_{DEBUG,RELWITHDEBINFO,RELEASE})
set(GCC_CXX_FLAGS_DEBUG " -pedantic -ftemplate-depth=1024 -Wall -Wextra -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-declarations -Wmissing-include-dirs -Wnoexcept -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=5 -Wundef -Wno-unused -Wdouble-promotion -Wimplicit-fallthrough=1 -Wswitch-default -Wsuggest-override -Wduplicated-branches -Wduplicated-cond")
set(CLANG_CXX_FLAGS_DEBUG " -Weverything -Wno-c++98-compat -Wno-documentation -Wno-unknown-pragmas -Wno-global-constructors -Wno-exit-time-destructors -Wno-padded -Wno-c++98-compat-pedantic -Wno-disabled-macro-expansion")
set(MSVC_CXX_FLAGS_DEBUG " /W4 /FS /MP ")
set(MSVC_CXX_FLAGS_RELEASE " /MP ")

if(__COMPILER_GNU AND NOT "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang") # GCC, MINGW
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${GCC_CXX_FLAGS_DEBUG}")
    if(NOT ${BUILD_SHARED_LIBS})
      set(CMAKE_EXE_LINKER_FLAGS "-static")
    endif()
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")
    if(${USE_LIBCPP})
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libstdc++")
    endif()
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${CLANG_CXX_FLAGS_DEBUG}")
    if(NOT ${BUILD_SHARED_LIBS})
      set(CMAKE_EXE_LINKER_FLAGS "-static")
    endif()
elseif(MSVC)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${MSVC_CXX_DEBUG_FLAGS}")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${MSVC_CXX_RELEASE_FLAGS}")
endif()

# Executables 

add_executable(a1_code_correctness_refactor_a code_correctness_refactor_a.cpp)
add_executable(a1_code_correctness_refactor_b code_correctness_refactor_b.cpp)
add_executable(a1_hamming_distance_statistics hamming_distance_statistics.cpp)
add_executable(a1_statistical_metrics_engine statistical_metrics_engine.cpp)
add_executable(a1_matrix_distance_analyzer matrix_distance_analyzer.cpp)
add_executable(a1_fasta_next_permutation fasta_next_permutation.cpp)

# link with libraries
if(NOT WIN32)
    if(${CMAKE_SYSTEM_NAME} MATCHES "Linux" AND ${USE_CXXABI})
        set(CXX_ABI c++abi)
    endif()
    if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        target_link_libraries(a1_code_correctness_refactor_a ${CXX_ABI})
        target_link_libraries(a1_code_correctness_refactor_b ${CXX_ABI})
        target_link_libraries(a1_hamming_distance_statistics ${CXX_ABI})
        target_link_libraries(a1_statistical_metrics_engine ${CXX_ABI})
        target_link_libraries(a1_matrix_distance_analyzer ${CXX_ABI})
        target_link_libraries(a1_fasta_next_permutation ${CXX_ABI})
   endif()
endif()

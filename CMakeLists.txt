#
# F E T C H   L E D G E R   P R O J E C T
#
cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
project(fetch)

# ------------------------------------------------------------------------------
# Configuration Options
# ------------------------------------------------------------------------------

# cmake-format: off
option(FETCH_ENABLE_TESTS               "Enable test targets"                   ON)
option(FETCH_ENABLE_BENCHMARKS          "Enable benchmarking targets"           ON)
option(FETCH_ENABLE_EXAMPLES            "Enable example targets"                ON)
option(FETCH_ENABLE_PYTHON_BINDINGS     "Build Python examples"                 ON)
option(FETCH_ENABLE_CCACHE              "Speed build times with ccache"         ON)
option(FETCH_WARNINGS_AS_ERRORS         "Enable warnings as errors"             ON)
option(FETCH_ENABLE_DEADLOCK_DETECTION  "Enable deadlock detection on mutexes"  OFF)
option(FETCH_VERBOSE_CMAKE              "Enable verbose cmake output"           OFF)
option(FETCH_ENABLE_CLANG_TIDY          "Enable clang tidy checking"            OFF)
option(FETCH_ENABLE_COVERAGE            "Enable coverage generation"            OFF)
option(FETCH_DISABLE_COLOUR_LOG         "Disable the colour logging"            OFF)
option(FETCH_STATIC_LINK                "Enable static linking"                 OFF)
option(FETCH_ENABLE_LTO                 "Enable link-time optimisation"         ON)
option(FETCH_ENABLE_BACKTRACE           "Enable backtrace on errors"            ON)
option(FETCH_FIXEDPOINT_DEBUG_HEX       "Print Hex value of FixedPoint numbers" OFF)
option(FETCH_FORCE_COLORED_OUTPUT       "Always produce ANSI-colored output."   ON)
option(FETCH_PROFILE_BUILD_TIME         "Enable build time profiling"           OFF)
# cmake-format: on

# custom string options
set(FETCH_DEBUG_SANITIZER "" CACHE STRING "The clang based sanitizer to be enabled")
set(FETCH_COMPILE_LOGGING_LEVEL "info" CACHE STRING "The precompiled logging level for the project")

# cmake-format: off
# target architecture (to be replaced with automatic detection) sysctl -a | grep machdep.cpu.features
option(FETCH_ARCH_AVX   "Architecture maximally supports AVX"     OFF)
option(FETCH_ARCH_FMA   "Architecture maximally supports FMA"     OFF)
option(FETCH_ARCH_AVX2  "Architecture maximally supports AVX2"    ON)
# cmake-format: on

# advanced options
mark_as_advanced(FETCH_DISABLE_COLOUR_LOG)

# ------------------------------------------------------------------------------
# Top Level Setup
# ------------------------------------------------------------------------------

set(FETCH_ROOT_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR})
set(FETCH_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(FETCH_ROOT_CMAKE_DIR ${FETCH_ROOT_DIR}/cmake)
set(FETCH_ROOT_VENDOR_DIR ${FETCH_ROOT_DIR}/vendor)

# Default to a release build
if (NOT CMAKE_BUILD_TYPE)
  message(STATUS "Defaulting to Release build")
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING "The build configuration" FORCE)
endif (NOT CMAKE_BUILD_TYPE)

# Global CMake Configuration
include(${FETCH_ROOT_CMAKE_DIR}/BuildTools.cmake)
include(${FETCH_ROOT_CMAKE_DIR}/BuildTargets.cmake)

# detect the current environment
detect_environment()

if (FETCH_STATIC_LINK)
  set(OPENSSL_USE_STATIC_LIBS TRUE)
endif (FETCH_STATIC_LINK)

# Enable CTest support
if (FETCH_ENABLE_TESTS)
  enable_testing()
endif (FETCH_ENABLE_TESTS)

if (FETCH_DEBUG_SANITIZER)
  fetch_info("Running with sanitizer: ${FETCH_DEBUG_SANITIZER}")
endif (FETCH_DEBUG_SANITIZER)

if (FETCH_PROFILE_BUILD_TIME)
  set(time_profiler "${FETCH_ROOT_DIR}/scripts/build-profile-gen.py --")

  set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${time_profiler}")
  set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK "${time_profiler}")
elseif (FETCH_ENABLE_CCACHE)
  # Enable CCACHE - compiler caching to reduce build times
  find_program(SCCACHE_FOUND sccache)
  find_program(CCACHE_FOUND ccache)

  if (SCCACHE_FOUND)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE sccache)
    fetch_info("Found sccache on system - including")
  elseif (CCACHE_FOUND)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
    fetch_info("Found ccache on system - including")
  else ()
    fetch_warning(
      "ccache is enabled but wasn't found on your system - strongly advise installing ccache for reduced build times"
      )
  endif ()
endif (FETCH_PROFILE_BUILD_TIME)

conditionally_enable_lto()

# ------------------------------------------------------------------------------
# System / Vendor Targets
# ------------------------------------------------------------------------------

configure_vendor_targets()

# ------------------------------------------------------------------------------
# Project Targets
# ------------------------------------------------------------------------------

configure_library_targets()

add_subdirectory(apps)

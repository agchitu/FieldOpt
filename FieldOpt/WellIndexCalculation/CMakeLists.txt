cmake_minimum_required(VERSION 2.8)
project(wellindexcalculator)

# Dbg ==================================================================
message("\n============================================================")
message(STATUS "PROJECT_NAME => ${PROJECT_NAME}")
message(STATUS "PROJECT_SOURCE_DIR: ${PROJECT_SOURCE_DIR}")
message(STATUS "CMAKE_BINARY_DIR: ${CMAKE_BINARY_DIR}")

# Options ==============================================================
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(WIC_LIB_TARGET wicalc_lib)
set(WIC_APP_TARGET wicalc_app)
set(WIC_LIB_TARGET_ADG _wic-rixx)

# ResInsight ===========================================================
set(CVF_ENABLE_ASSERTS 1)
set(CVF_ENABLE_TIGHT_ASSERTS 1)
include(${CMAKE_CURRENT_SOURCE_DIR}/resinxx.cmake)

# Include: Qt ==========================================================
include_directories(${Qt5Core_INCLUDE_DIRS})

# Compile options ======================================================
set(CMAKE_CXX_STANDARD 11)
add_compile_options(-std=c++11)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/resinxx/rixx_app_fwk
    ${CMAKE_CURRENT_SOURCE_DIR}/resinxx/rixx_core_geom
    ${CMAKE_CURRENT_SOURCE_DIR}/resinxx/rixx_res_mod
    ${CMAKE_CURRENT_SOURCE_DIR}/resinxx/rixx_grid
)

if (NOT BUILD_WIC_ADGPRS)
message(".............................................................")
message("configure wicalc library...")

# controls ricasedata delete
add_definitions(-DFIELDOPT_BUILD)

add_library(${WIC_LIB_TARGET} SHARED
    intersected_cell.cpp
    #
    wicalc_rixx.cpp
    ${RIXX_ALL_CPP_FILES}
    #
        WellDefinition.cpp)

#add_library(${WIC_LIB_TARGET}_static STATIC
#    intersected_cell.cpp
#    #
#    wicalc_rixx.cpp
#    ${RIXX_ALL_CPP_FILES}
#    #
#        WellDefinition.cpp)

#add_library(fieldopt::${WIC_LIB_TARGET}
#    ALIAS ${WIC_LIB_TARGET})

message(STATUS "Set WIC_LIB_TARGET: ${WIC_LIB_TARGET}")

set_target_properties(${WIC_LIB_TARGET}
    PROPERTIES COMPILE_FLAGS "-fPIC")

target_link_libraries(${WIC_LIB_TARGET}
    PUBLIC fieldopt::reservoir
    PUBLIC ${Boost_LIBRARIES}
    PUBLIC Qt5::Core
    PUBLIC ri:ert_ecl
    )

message(".............................................................")
message("configure TNO wicalc standalone application...")
add_executable(${WIC_APP_TARGET}
    wicalc_main.cpp
    intersected_cell.cpp
    wicalc_rixx.cpp
    ${RIXX_ALL_CPP_FILES}
    WellDefinition.cpp
)

target_link_libraries(${WIC_APP_TARGET}
      ${WIC_LIB_TARGET}
    )

install(TARGETS ${WIC_LIB_TARGET} ${WIC_APP_TARGET}
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib/static
    )

endif()

# Testing ==============================================================
if (BUILD_TESTING AND NOT BUILD_WIC_ADGPRS AND NOT USE_SANITIZING)

  # Unit tests
#  find_package(GTest REQUIRED)

  include_directories(${GTEST_INCLUDE_DIRS}
      ${EIGEN3_INCLUDE_DIR} tests)

  add_executable(test_wellindexcalculator
      tests/test_intersected_cells.cpp
      tests/test_single_cell_wellindex.cpp)

  target_link_libraries(test_wellindexcalculator
      ${WIC_LIB_TARGET}
      ${GTEST_BOTH_LIBRARIES}
      ${CMAKE_THREAD_LIBS_INIT}
          )

  add_test(NAME test_wellindexcalculator
      COMMAND $<TARGET_FILE:test_wellindexcalculator>)
endif()


# Debug: include dirs ==================================================
include(../InclDirDbg.cmake)

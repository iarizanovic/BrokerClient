# Author: Ivan Arizanovic <ivanarizanovic@yahoo.com>

cmake_minimum_required (VERSION 3.8)
project ("BrokerClient")

#------------------------------------------------------------------------------
# Enable Hot Reload for MSVC compilers if supported.
#------------------------------------------------------------------------------
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

#------------------------------------------------------------------------------
# Include the libraries and sources
#------------------------------------------------------------------------------
find_library(GTEST_LIBRARY            NAMES gtest                 HINTS lib/)
find_library(GTEST_MAIN_LIBRARY       NAMES gtest_main            HINTS lib/)
find_library(GMOCK_LIBRARY            NAMES gmock                 HINTS lib/)
find_library(GMOCK_MAIN_LIBRARY       NAMES gmock_main            HINTS lib/)
find_library(SQLite3_LIBRARY          NAMES sqlite3               HINTS lib/)
find_library(PROTON_CORE_LIBRARY      NAMES qpid-proton-core      HINTS lib/)
find_library(PROTON_CPP_LIBRARY       NAMES qpid-proton-cpp       HINTS lib/)
find_library(PROTON_PROACTOR_LIBRARY  NAMES qpid-proton-proactor  HINTS lib/)
find_library(PROTON_LIBRARY           NAMES qpid-proton           HINTS lib/)
include_directories(${CMAKE_SOURCE_DIR}/include)

#------------------------------------------------------------------------------
# Copy the dynamic-link libraries
#------------------------------------------------------------------------------
file(
  COPY
    bin/gtest_main.dll
    bin/gtest.dll
    bin/gmock_main.dll
    bin/gmock.dll
    bin/jsoncpp.dll
    bin/sqlite3.dll
    bin/qpid-proton-cpp.dll
    bin/qpid-proton-proactor.dll
    bin/qpid-proton.dll
    bin/qpid-proton-core.dll
  DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/
)

#------------------------------------------------------------------------------
# Create Database library
#------------------------------------------------------------------------------
add_library(Database STATIC "Database.cpp" "Database.h")

target_link_libraries(
  Database PUBLIC
  ${SQLite3_LIBRARY}
)

#------------------------------------------------------------------------------
# Create BrokerClient library
#------------------------------------------------------------------------------
add_library (BrokerClient STATIC "BrokerClient.cpp" "BrokerClient.h")

target_link_libraries(
  BrokerClient PUBLIC
  Database
  ${PROTON_CORE_LIBRARY}
  ${PROTON_CPP_LIBRARY}
  ${PROTON_PROACTOR_LIBRARY}
  ${PROTON_LIBRARY}
)

#------------------------------------------------------------------------------
# Create Test executable
#------------------------------------------------------------------------------
add_executable (Test "Test.cpp")

target_link_libraries(
  Test PUBLIC
  BrokerClient
  ${GTEST_LIBRARY}
  ${GTEST_MAIN_LIBRARY}
  ${GMOCK_LIBRARY}
  ${GMOCK_MAIN_LIBRARY}
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Test PROPERTY CXX_STANDARD 20)
endif()

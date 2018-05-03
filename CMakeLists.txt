################################################################################
#                                                                              #
#             NemaTode  TriBITS Project and Package CMakeLists.txt File           #
#                                                                              #
################################################################################

IF (NOT TRIBITS_PROCESSING_PACKAGE)
  # This CMakeLists.txt file is being processed as the NemaTode TriBITS projects's base
  # CMakeLists.txt file!  (See comments at bottom of this file.)
  CMAKE_MINIMUM_REQUIRED(VERSION 2.8.12 FATAL_ERROR)
  INCLUDE("${CMAKE_CURRENT_SOURCE_DIR}/ProjectName.cmake")
  PROJECT(${PROJECT_NAME} NONE)
  # enable C++11 by default
  SET(NemaTode_ENABLE_CXX11 ON CACHE BOOL "Compile using the C++11 standard" FORCE)

  # disable generating HTML dependencies webpage and xml files
  SET(${PROJECT_NAME}_DEPS_XML_OUTPUT_FILE OFF CACHE BOOL "")
  # disable TriBITS export system to save time configuring
  SET(${PROJECT_NAME}_ENABLE_INSTALL_CMAKE_CONFIG_FILES OFF CACHE BOOL "")
  SET(${PROJECT_NAME}_ENABLE_EXPORT_MAKEFILES OFF CACHE BOOL "")
  SET(${PROJECT_NAME}_TRIBITS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/TriBITS/tribits" CACHE PATH "")
  INCLUDE("${${PROJECT_NAME}_TRIBITS_DIR}/TriBITS.cmake")
  SET(${PROJECT_NAME}_ENABLE_TESTS ON CACHE BOOL "Enable tests by default.")

  TRIBITS_PROJECT_ENABLE_ALL()
ELSE()
  # This CMakeLists.txt file is being processed as the TriBITS package file.
  INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
  # include binary directory for configured includes
  INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
  INCLUDE(CMakeLists.package.txt)
ENDIF()

# NOTE: In order to allow the `NemaTode/` directory to be both a TriBITS
# package (for inclusion in other TriBITS projects) and to be a TriBITS
# project itself, you only have to put in a simple if statement in this
# top-level CMakeLists.txt file.  That is all!

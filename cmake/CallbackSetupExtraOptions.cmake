##---------------------------------------------------------------------------##
## NemaTode/cmake/CallbackSetupExtraOptions.cmake
## Jordan Lefebvre
##---------------------------------------------------------------------------##

IF (NemaTode_SOURCE_DIR)
  #
  # CMake Macros
  SET(CMAKE_MODULE_PATH  ${CMAKE_MODULE_PATH} "${NemaTode_SOURCE_DIR}/cmake")
ENDIF()

##---------------------------------------------------------------------------##

MACRO(TRIBITS_REPOSITORY_SETUP_EXTRA_OPTIONS)
  #
  # Include the TestFramework setup
  #
  ADD_SUBDIRECTORY(${NemaTode_SOURCE_DIR}/testframework/setup)
  ########################################
  # C++11
  ########################################

  # Set CXX11 to be enabled by default.
  SET(${PROJECT_NAME}_ENABLE_CXX11_DEFAULT TRUE)
  ########################################
  # STATIC/SHARED BUILD SETUP
  ########################################

  # Add install RPATH when building shared
  IF(BUILD_SHARED_LIBS AND CMAKE_INSTALL_PREFIX)
    SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
    SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
    IF (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
      SET(CMAKE_INSTALL_RPATH "@loader_path/../lib")
      SET(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
      SET(CMAKE_SKIP_BUILD_RPATH FALSE)
      SET(CMAKE_MACOSX_RPATH ON)
    ELSEIF("${CMAKE_SYSTEM_NAME}" STREQUAL "Linux")
       # use, i.e. don't skip the full RPATH for the build tree
       SET(CMAKE_SKIP_BUILD_RPATH  FALSE)

       # when building, don't use the install RPATH already
       # (but later on when installing)
       SET(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)

       # the RPATH to be used when installing
       SET(CMAKE_INSTALL_RPATH "\$ORIGIN/../lib")

       # don't add the automatically determined parts of the RPATH
       # which point to directories outside the build tree to the install RPATH
       SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
    ENDIF()
  ENDIF()

  ########################################
  # NemaTode PACKAGE SETUP
  ########################################

  #
  # Default MPI to OFF
  #
  SET(TPL_ENABLE_MPI OFF CACHE BOOL "" FORCE)
  #
  # Check if the environment has changed whether we want fortran enabled
  #
  IF(NOT "$ENV{${PROJECT_NAME}_ENABLE_Fortran}" STREQUAL "" )
     SET(${PROJECT_NAME}_ENABLE_Fortran $ENV{${PROJECT_NAME}_ENABLE_Fortran})
  ELSE()
     # ensure fortran compiler is set on
     SET(${PROJECT_NAME}_ENABLE_Fortran OFF CACHE BOOL "" FORCE)
  ENDIF()
  # Set up NemaTode cmake directory, used by default option scripts
  SET(NemaTode_CMAKE_DIR "${NemaTode_SOURCE_DIR}/cmake" CACHE PATH "")

ENDMACRO()

##---------------------------------------------------------------------------##
##          end of NemaTode/cmake/CallbackSetupExtraOptions.cmake
##---------------------------------------------------------------------------##

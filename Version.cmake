##---------------------------------------------------------------------------##
## FIT/Version.cmake
## Jordan P. Lefebvre, lefebvrejp@ornl.gov
##---------------------------------------------------------------------------##
# Single file that needs to be changed on a release branch
# or on the development branch in order to configure FIT
# for release mode and set the version.

SET(FIT_NAME "FIT")
SET(FIT_MAJOR_VERSION "0") # must quote as zero is 'undefined'
SET(FIT_MINOR_VERSION "0") # must quote as zero is 'undefined'
SET(FIT_MICRO_VERSION "0") # must quote as zero is 'undefined'
SET(FIT_VERSION ${FIT_MAJOR_VERSION}.${FIT_MINOR_VERSION})
SET(FIT_FULL_VERSION "alpha ${FIT_MAJOR_VERSION}.${FIT_MINOR_VERSION}.${FIT_MICRO_VERSION}")
SET(FIT_VERSION_STRING "${FIT_MAJOR_VERSION}.${FIT_MINOR_VERSION}.${FIT_MICRO_VERSION} (aplha)")
SET(FIT_MAJOR_MINOR_VERSION ${FIT_MAJOR_VERSION}${FIT_MINOR_VERSION}${FIT_MICRO_VERSION})
SET(FIT_ENABLE_DEVELOPMENT_MODE_DEFAULT ON) # Change to 'OFF' for a release (beta or final)

##---------------------------------------------------------------------------##
##                    end of FIT/Version.cmake
##---------------------------------------------------------------------------##

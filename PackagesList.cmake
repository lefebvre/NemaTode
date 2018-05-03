##---------------------------------------------------------------------------##
## NemaTode/PackagesList.cmake
## Jordan Lefebvre, lefebvrejp@ornl.gov
##---------------------------------------------------------------------------##
#
# See documentation in TriBITS https://tribits.org


##---------------------------------------------------------------------------##
## PACKAGES PROVIDED
##---------------------------------------------------------------------------##

TRIBITS_REPOSITORY_DEFINE_PACKAGES(
  googletest    googletest/googletest ST
  testframework testframework         ST
  NemaTode      .                     PT
)

TRIBITS_ALLOW_MISSING_EXTERNAL_PACKAGES(
   googletest
   testframework
)

##---------------------------------------------------------------------------##
##                     end of NemaTode/PackagesList.cmake
##---------------------------------------------------------------------------##

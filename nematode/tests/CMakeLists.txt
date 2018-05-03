INCLUDE(GoogleTest)

ADD_GOOGLE_TEST(tstNumberConversion.cc NP 1)

TRIBITS_ADD_EXECUTABLE(demo_simple
   NOEXEPREFIX
   SOURCES demo_simple.cpp
)
TRIBITS_ADD_EXECUTABLE(demo_advanced
   NOEXEPREFIX
   SOURCES demo_advanced.cpp
)

#include "gtest/gtest.h"

#include <nematode/NumberConversion.h>
#include <nematode/nmea.h>

using namespace std;
using namespace nmea;
TEST(NemaTode, NumberConversion)
{
  string s;
  float f;
  long long k;

  try
  {
    s = "-1.345";
    f = parseDouble(s);
    cout << s << ": " << f << endl;
  }
  catch (NumberConversionError& ex)
  {
    cout << ex.message << endl;
  }

  try
  {
    s = "-1.23e-2";
    f = parseDouble(s);
    cout << s << ": " << f << endl;
  }
  catch (NumberConversionError& ex)
  {
    cout << ex.message << endl;
  }

  try
  {
    s = "";
    f = parseDouble(s);
    cout << s << ": " << f << endl;
  }
  catch (NumberConversionError& ex)
  {
    cout << ex.message << endl;
  }

  {
    // -- fails, ok
    s = "asd";
    EXPECT_THROW(parseDouble(s), NumberConversionError);
  }

  {
    s = "-1234.123";
    EXPECT_THROW(parseInt(s), NumberConversionError);
  }

  try
  {
    s = "01234";
    k = parseInt(s);
    cout << s << ": " << k << endl;
  }
  catch (NumberConversionError& ex)
  {
    cout << ex.message << endl;
  }

  try
  {
    // -- converts to 0
    s = "";
    k = parseInt(s);
    cout << s << ": " << k << endl;
  }
  catch (NumberConversionError& ex)
  {
    cout << ex.message << endl;
  }

  {
    // -- fails, ok
    s = "asd";
    EXPECT_THROW(parseInt(s), NumberConversionError);
  }

  try
  {
    // -- fails, ok
    s = "-16";
    k = parseInt(s);
    cout << s << ": " << k << endl;
  }
  catch (NumberConversionError& ex)
  {
    cout << ex.message << endl;
  }
}

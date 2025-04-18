/*
 * GPSFix.cpp
 *
 *  Created on: Jul 23, 2014
 *      Author: Cameron Karlsson
 *
 *  See the license file included with this source.
 */

#include <nematode/GPSFix.h>
#include <chrono>
#include <cmath>
#include <iomanip>
#include <sstream>
#include <string>

using namespace std;
using namespace std::chrono;

using namespace nmea;

// ===========================================================
// ======================== GPS SATELLITE ====================
// ===========================================================

string GPSSatellite::toString()
{
  stringstream ss;

  ss << "[PRN: " << setw(3) << setfill(' ') << prn << " "
     << "  SNR: " << setw(3) << setfill(' ') << snr << " dB  "
     << "  Azimuth: " << setw(3) << setfill(' ') << azimuth << " deg "
     << "  Elevation: " << setw(3) << setfill(' ') << elevation << " deg  "
     << "]";

  return ss.str();
}
GPSSatellite::operator std::string() { return toString(); }

// =========================================================
// ======================== GPS ALMANAC ====================
// =========================================================

void GPSAlmanac::clear()
{
  lastPage       = 0;
  totalPages     = 0;
  processedPages = 0;
  visibleSize    = 0;
  satellites.clear();
}
void GPSAlmanac::updateSatellite(GPSSatellite sat)
{
  if (satellites.size() > visibleSize)
  {  // we missed the new almanac start page, start over.
    clear();
  }

  satellites.push_back(sat);
}
double GPSAlmanac::percentComplete()
{
  if (totalPages == 0)
  {
    return 0.0;
  }

  return double(processedPages) / double(totalPages) * 100.0;
}
double GPSAlmanac::averageSNR()
{
  double avg      = 0;
  double relevant = 0;
  for (size_t i = 0; i < satellites.size(); i++)
  {
    if (satellites[i].snr > 0)
    {
      relevant += 1.0;
    }
  }

  for (size_t i = 0; i < satellites.size(); i++)
  {
    if (satellites[i].snr > 0)
    {
      avg += satellites[i].snr / relevant;
    }
  }

  return avg;
}
double GPSAlmanac::minSNR()
{
  double min = 9999999;
  if (satellites.size() == 0)
  {
    return 0;
  }
  int32_t num_over_zero = 0;
  for (size_t i = 0; i < satellites.size(); i++)
  {
    if (satellites[i].snr > 0)
    {
      num_over_zero++;
      if (satellites[i].snr < min)
      {
        min = satellites[i].snr;
      }
    }
  }
  if (num_over_zero == 0)
  {
    return 0;
  }
  return min;
}

double GPSAlmanac::maxSNR()
{
  double max = 0;
  for (size_t i = 0; i < satellites.size(); i++)
  {
    if (satellites[i].snr > 0)
    {
      if (satellites[i].snr > max)
      {
        max = satellites[i].snr;
      }
    }
  }
  return max;
}

// ===========================================================
// ======================== GPS TIMESTAMP ====================
// ===========================================================

GPSTimestamp::GPSTimestamp()
{
  hour = 0;
  min  = 0;
  sec  = 0;

  month = 1;
  day   = 1;
  year  = 1970;

  rawTime = 0;
  rawDate = 0;
}

// indexed from 1!
std::string GPSTimestamp::monthName(uint32_t index)
{
  if (index < 1 || index > 12)
  {
    std::stringstream ss;
    ss << "[month:" << index << "]";
    return ss.str();
  }

  std::string names[] = {"January",   "February", "March",    "April",
                         "May",       "June",     "July",     "August",
                         "September", "October",  "November", "December"};
  return names[index - 1];
}

// Returns seconds since Jan 6, 1980. GPS Epoch time.
sys_time<milliseconds> GPSTimestamp::getTime()
{
  constexpr days gps_epoch_offset(
      days(3657));  // days from 1970-01-01 to 1980-01-06
  milliseconds gps_duration =
      sys_days{year_month_day(std::chrono::year(year),
                              std::chrono::month(month), std::chrono::day(day))}
          .time_since_epoch() -
      gps_epoch_offset + hours(hour) + minutes(min) +
      duration_cast<milliseconds>(duration<double>(sec));
  gps_time<milliseconds> gps_tp(gps_duration);
  return clock_cast<system_clock>(gps_tp);
}

void GPSTimestamp::setTime(double raw_ts)
{
  rawTime = raw_ts;

  hour = (int32_t)trunc(raw_ts / 10000.0);
  min  = (int32_t)trunc((raw_ts - hour * 10000) / 100.0);
  sec  = raw_ts - min * 100 - hour * 10000;
}

// ddmmyy
void GPSTimestamp::setDate(int32_t raw_date)
{
  rawDate = raw_date;
  // If uninitialized, use posix time.
  if (rawDate == 0)
  {
    month = 1;
    day   = 1;
    year  = 1970;
  }
  else
  {
    day   = (int32_t)trunc(raw_date / 10000.0);
    month = (int32_t)trunc((raw_date - 10000 * day) / 100.0);
    year  = raw_date - 10000 * day - 100 * month + 2000;
  }
}

std::string GPSTimestamp::toString()
{
  std::stringstream ss;
  ss << hour << "h " << min << "m " << sec << "s"
     << "  " << monthName(month) << " " << day << " " << year;
  return ss.str();
}

// =====================================================
// ======================== GPS FIX ====================
// =====================================================

GPSFix::GPSFix()
{
  quality = 0;    // Searching...
  status  = 'V';  // Void
  type    = 1;    // 1=none, 2=2d, 3=3d

  haslock = 0;

  dilution = 0;
  horizontalDilution =
      0;  // Horizontal - Best is 1, >20 is terrible, so 0 means uninitialized
  verticalDilution   = 0;
  latitude           = 0;
  longitude          = 0;
  speed              = 0;
  travelAngle        = 0;
  altitude           = 0;
  trackingSatellites = 0;
  visibleSatellites  = 0;
}

GPSFix::~GPSFix()
{
  // TODO Auto-generated destructor stub
}

// Returns the duration since the Host has received information
milliseconds GPSFix::timeSinceLastUpdate()
{
  auto time1 = std::chrono::system_clock::now();

  auto time2 = timestamp.getTime();

  return duration_cast<milliseconds>(time1 - time2);
}

bool GPSFix::hasEstimate() const
{
  return (latitude != 0 && longitude != 0) || (quality == 6);
}

bool GPSFix::setlock(bool locked)
{
  if (haslock != locked)
  {
    haslock = locked;
    return true;
  }
  return false;
}

bool GPSFix::locked() const { return haslock; }

// Returns meters
double GPSFix::horizontalAccuracy()
{
  // horizontal 2drms 95% = 4.0  -- from GPS CHIP datasheets
  return 4.0 * horizontalDilution;
}

// Returns meters
double GPSFix::verticalAccuracy()
{
  // Vertical 2drms 95% = 6.0  -- from GPS CHIP datasheets
  return 6.0 * verticalDilution;
}

// Takes a degree travel heading (0-360') and returns the name
std::string GPSFix::travelAngleToCompassDirection(double deg, bool abbrev)
{
  // normalize, just in case
  int32_t c = (int32_t)round(deg / 360.0 * 8.0);
  int32_t r = c % 8;
  if (r < 0)
  {
    r = 8 + r;
  }

  if (abbrev)
  {
    std::string dirs[] = {"N", "NE", "E", "SE", "S", "SW", "W", "NW", "N"};
    return dirs[r];
  }
  else
  {
    std::string dirs[] = {"North",      "North East", "East",
                          "South East", "South",      "South West",
                          "West",       "North West", "North"};
    return dirs[r];
  }
}

std::string fixStatusToString(char status)
{
  switch (status)
  {
    case 'A':
      return "Active";
    case 'V':
      return "Void";
    default:
      return "Unknown";
  }
}
std::string fixTypeToString(uint8_t type)
{
  switch (type)
  {
    case 1:
      return "None";
    case 2:
      return "2D";
    case 3:
      return "3D";
    default:
      return "Unknown";
  }
}
std::string fixQualityToString(uint8_t quality)
{
  switch (quality)
  {
    case 0:
      return "Invalid";
    case 1:
      return "Standard";
    case 2:
      return "DGPS";
    case 3:
      return "PPS fix";
    case 4:
      return "Real Time Kinetic";
    case 5:
      return "Real Time Kinetic (float)";
    case 6:
      return "Estimate";
    default:
      return "Unknown";
  }
}

std::string GPSFix::toString()
{
  stringstream ss;
  ios_base::fmtflags oldflags = ss.flags();

  ss << "========================== GPS FIX ================================"
     << endl
     << " Status: \t\t" << ((haslock) ? "LOCK!" : "SEARCHING...") << endl
     << " Satellites: \t\t" << trackingSatellites << " (tracking) of "
     << visibleSatellites << " (visible)" << endl
     << " < Fix Details >" << endl
     << "   Age:                " << timeSinceLastUpdate().count() << " s"
     << endl
     << "   Timestamp:          " << timestamp.toString()
     << "   UTC   \n\t\t\t(raw: " << timestamp.rawTime << " time, "
     << timestamp.rawDate << " date)" << endl
     << "   Raw Status:         " << status << "  ("
     << fixStatusToString(status) << ")" << endl
     << "   Type:               " << int(type) << "  (" << fixTypeToString(type)
     << ")" << endl
     << "   Quality:            " << int(quality) << "  ("
     << fixQualityToString(quality) << ")" << endl
     << "   Lat/Lon (N,E):      " << setprecision(6) << fixed << latitude
     << "' N, " << longitude << "' E" << endl;

  ss.flags(oldflags);  // reset

  ss << "   DOP (P,H,V):        " << dilution << ",   " << horizontalDilution
     << ",   " << verticalDilution << endl
     << "   Accuracy(H,V):      " << horizontalAccuracy() << " m,   "
     << verticalAccuracy() << " m" << endl;

  ss << "   Altitude:           " << altitude << " m" << endl
     << "   Speed:              " << speed << " km/h" << endl
     << "   Travel Dir:         " << travelAngle << " deg  ["
     << travelAngleToCompassDirection(travelAngle) << "]" << endl
     << "   SNR:                avg: " << almanac.averageSNR()
     << " dB   [min: " << almanac.minSNR() << " dB,  max:" << almanac.maxSNR()
     << " dB]" << endl;

  ss << " < Almanac (" << almanac.percentComplete() << "%) >" << endl;
  if (almanac.satellites.size() == 0)
  {
    ss << " > No satellite info in almanac." << endl;
  }
  for (size_t i = 0; i < almanac.satellites.size(); i++)
  {
    ss << "   [" << setw(2) << setfill(' ') << (i + 1) << "]   "
       << almanac.satellites[i].toString() << endl;
  }

  return ss.str();
}
GPSFix::operator std::string() { return toString(); }

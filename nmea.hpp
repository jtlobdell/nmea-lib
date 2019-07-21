#ifndef NMEA_HPP
#define NMEA_HPP

#include <vector>
#include <boost/optional.hpp>
#include <boost/variant.hpp>

namespace nmea {

enum class latitude_direction_t {
    north,
    south
};

enum class longitude_direction_t {
    east,
    west
};

enum class fix_quality_t {
    invalid,
    gps_fix,
    dgps_fix,
    pps_fix,
    real_time_kinematic,
    float_rtk,
    dead_reckoning,
    manual_input_mode,
    simulation_mode
};

enum class position_system_mode_indicator_t {
    autonomous,
    differential,
    estimated,
    manual,
    invalid
};

enum class gsa_mode_t {
    manual,
    automatic
};

enum class gsa_fix_type_t {
    unavailable,
    _2d,
    _3d
};

enum class data_status_t {
    active,
    invalid
};

struct utc_time_t {
    unsigned int hours;
    unsigned int minutes;
    float seconds;
};

struct ut_date_t {
    unsigned int dd;
    unsigned int mm;
    unsigned int yy;
};

struct latitude_t {
    unsigned int degrees;
    float minutes;
    latitude_direction_t dir;
};

struct longitude_t {
    unsigned int degrees;
    float minutes;
    longitude_direction_t dir;
};

// Global Positioning System Fix Data
struct gpgga {
    utc_time_t time;
    latitude_t latitude;
    longitude_t longitude;
    fix_quality_t fix_quality;
    unsigned int sats_tracked;
    float hdop; // horizontal degree of precision
    float msl_altitude; // mean sea level
    float geoid_separation; // height of geoid (MSL) above WGS84 ellipsoid
    boost::optional<float> time_since_dgps_update;
    boost::optional<unsigned int> dgps_station_id;
    unsigned int checksum;
};

// Geographic Position - Latitude / Longitude
struct gpgll {
    latitude_t latitude;
    longitude_t longitude;
    utc_time_t time;
    data_status_t data_status;
    position_system_mode_indicator_t position_system_mode_indicator;
    unsigned int checksum;
};

struct gpgsa {
    gsa_mode_t gsa_mode;
    gsa_fix_type_t gsa_fix_type;
    std::vector<unsigned int> satellites;
    float dilution_of_precision;
    float horizontal_dilution_of_precision;
    float vertical_dilution_of_precision;
    unsigned int checksum;
};

struct gpgsv_entry {
    unsigned int satellite_id_number;
    unsigned int elevation;
    unsigned int azimuth;
    unsigned int signal_noise_ratio;
};

// GNSS Satellites in View
struct gpgsv {
    unsigned int number_of_messages;
    unsigned int message_number;
    unsigned int satellites_in_view;
    std::vector<gpgsv_entry> gpgsv_entries;
    unsigned int checksum;
};

// Recommended Minimum Specific GNSS Data
struct gprmc {
    utc_time_t time;
    data_status_t data_status;
    latitude_t latitude;
    longitude_t longitude;
    float speed_over_ground;
    float course_over_ground;
    ut_date_t date;
    position_system_mode_indicator_t position_system_mode_indicator;
    unsigned int checksum;
};

// Course Over Ground and Ground Speed
struct gpvtg {
    float course_over_ground_true;
    float course_over_ground_magnetic;
    float ground_speed_knots;
    float ground_speed_kmph;
    position_system_mode_indicator_t position_system_mode_indicator;
    unsigned int checksum;
};

typedef boost::variant<
    nmea::gpgga,
    nmea::gpgll,
    nmea::gpgsa,
    nmea::gpgsv,
    nmea::gprmc,
    nmea::gpvtg
    >
nmea_sentence;


} // namespace nmea

#endif

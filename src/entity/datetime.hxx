#ifndef DATETIME_HXX
#define DATETIME_HXX

#include <iostream>
#include <odb/core.hxx>
#include <chrono> // For std::chrono
#include <iomanip> // For std::put_time
// --- New datetime class ---
#pragma db value type("TIMESTAMP") // Map to TIMESTAMP in PostgreSQL
class datetime {
    public:
        // Default constructor required by ODB
        datetime() : tp_(std::chrono::system_clock::time_point()) {}

        // Constructor from time components
        datetime(unsigned int year, unsigned int month, unsigned int day,
                 unsigned int hour, unsigned int minute, unsigned int second) {
            std::tm t{};
            t.tm_year = year - 1900;
            t.tm_mon = month - 1;
            t.tm_mday = day;
            t.tm_hour = hour;
            t.tm_min = minute;
            t.tm_sec = second;
            t.tm_isdst = -1; // Let mktime determine DST

            std::time_t tt = std::mktime(&t);
            tp_ = std::chrono::system_clock::from_time_t(tt);
        }
        static datetime now();
        std::time_t to_time_t() const;
        // Constructor from system_clock::time_point
        explicit datetime(const std::chrono::system_clock::time_point &tp)
            : tp_(tp)
        {}

        std::chrono::system_clock::time_point
        to_time_point() const {
            return tp_;
        }

    private:
        std::chrono::system_clock::time_point tp_;
};

inline std::ostream &
operator<< (std::ostream &os, const datetime &dt) {
    std::time_t tt = std::chrono::system_clock::to_time_t(dt.to_time_point());
    std::tm tm = *std::localtime(&tt); // Or std::gmtime(&tt) for UTC

    // Format the output
    return os << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
}

// Definition of static now()
inline datetime datetime::now() {
    using namespace std::chrono;
    auto tp = system_clock::now();
    auto s = duration_cast<seconds>(tp.time_since_epoch()).count();
    std::time_t t = static_cast<std::time_t>(s);
    std::tm *tm_ptr = std::localtime(&t);

    return datetime(
               tm_ptr->tm_year + 1900,
               tm_ptr->tm_mon + 1,
               tm_ptr->tm_mday,
               tm_ptr->tm_hour,
               tm_ptr->tm_min,
               tm_ptr->tm_sec
           );
}

inline std::time_t datetime::to_time_t() const {
    return std::chrono::system_clock::to_time_t(tp_);
}
// --- End of new datetime class ---
#endif // DATETIME_HXX

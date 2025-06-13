#ifndef DATETIME_HXX
#define DATETIME_HXX

#include <iostream>
#include <odb/core.hxx>

#pragma db value type("TIMESTAMP")
class datetime {
    public:
        datetime() = default;

        datetime(unsigned int year, unsigned int month, unsigned int day,
                 unsigned int hour = 0, unsigned int minute = 0, unsigned int second = 0)
            : year_(year), month_(month), day_(day),
              hour_(hour), minute_(minute), second_(second)
        {}

        datetime &operator=(const datetime &other) {
            if (this != &other) {
                year_ = other.year_;
                month_ = other.month_;
                day_ = other.day_;
                hour_ = other.hour_;
                minute_ = other.minute_;
                second_ = other.second_;
            }
            return *this;
        }



        unsigned int year() const {
            return year_;
        }
        unsigned int month() const {
            return month_;
        }
        unsigned int day() const {
            return day_;
        }
        unsigned int hour() const {
            return hour_;
        }
        unsigned int minute() const {
            return minute_;
        }
        unsigned int second() const {
            return second_;
        }

    private:
        unsigned int year_{0}, month_{0}, day_{0};
        unsigned int hour_{0}, minute_{0}, second_{0};
};

inline std::ostream &operator<<(std::ostream &os, const datetime &dt) {
    return os << dt.year() << '-'
           << dt.month() << '-'
           << dt.day() << ' '
           << dt.hour() << ':'
           << dt.minute() << ':'
           << dt.second();
}

inline bool operator==(const datetime &lhs, const datetime &rhs) {
    return lhs.year()   == rhs.year()   &&
           lhs.month()  == rhs.month()  &&
           lhs.day()    == rhs.day()    &&
           lhs.hour()   == rhs.hour()   &&
           lhs.minute() == rhs.minute() &&
           lhs.second() == rhs.second();
}

inline bool operator!=(const datetime &lhs, const datetime &rhs) {
    return !(lhs == rhs);
}

#endif // DATETIME_HXX

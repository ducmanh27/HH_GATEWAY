#ifndef TRAITS_PGSQL_HXX
#define TRAITS_PGSQL_HXX

#include <cstddef> // std::size_t
#include <cstring> // std::strncmp, std::memset, std::memcpy
#include <ctime>   // localtime, mktime, time_t, tm

#include <odb/pgsql/traits.hxx>
#include <odb/pgsql/details/endian-traits.hxx>
#include "datetime.hxx"
#include "bool.hxx"

namespace odb {
    namespace pgsql {

        // Bool mapping: true/false as VARCHAR(5)
        template <>
        class value_traits<bool, id_string> {
            public:
                using value_type = bool;
                using query_type = bool;
                using image_type = details::buffer;

                static void
                set_value(bool &v,
                          const details::buffer &b,
                          std::size_t n,
                          bool is_null) {
                    v = (!is_null && n == 4 && std::strncmp("true", b.data(), n) == 0);
                }

                static void
                set_image(details::buffer &b,
                          std::size_t &n,
                          bool &is_null,
                          bool v) {
                    is_null = false;
                    n = v ? 4 : 5;

                    if (n > b.capacity())
                        b.capacity(n);

                    std::memcpy(b.data(), (v ? "true" : "false"), n);
                }
        };

        // Mapping datetime <-> PostgreSQL TIMESTAMP (seconds since 2000-01-01)
        template <>
        class value_traits<datetime, id_timestamp> {
            public:
                using value_type = datetime;
                using query_type = datetime;
                using image_type = long long;

                // PostgreSQL epoch (2000-01-01 00:00:00 UTC)
                static const time_t epoch_diff = 946684800;

                static void
                set_value(datetime &v, const long long &i, bool is_null) {
                    if (is_null) {
                        v = datetime(0, 0, 0, 0, 0, 0);
                        return;
                    }

                    time_t ts = static_cast<time_t>(details::endian_traits::ntoh(i)) + epoch_diff;
                    tm t = *gmtime(&ts); // dùng gmtime thay cho localtime

                    v = datetime(t.tm_year + 1900, t.tm_mon + 1, t.tm_mday,
                                 t.tm_hour, t.tm_min, t.tm_sec);
                }

                static void
                set_image(long long &i, bool &is_null, const datetime &v) {
                    is_null = false;

                    tm t {};
                    t.tm_year = v.year() - 1900;
                    t.tm_mon  = v.month() - 1;
                    t.tm_mday = v.day();
                    t.tm_hour = v.hour();
                    t.tm_min  = v.minute();
                    t.tm_sec  = v.second();

                    time_t ts = timegm(&t); // DÙNG UTC thay vì local time

                    i = details::endian_traits::hton(static_cast<long long>(ts - epoch_diff));
                }
        };

    } // namespace pgsql
} // namespace odb

#endif // TRAITS_PGSQL_HXX

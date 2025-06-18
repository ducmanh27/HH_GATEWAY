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

        // --- Corrected datetime traits ---
        template <>
        class value_traits<datetime, id_timestamp> {
            public:
                using value_type = datetime;
                using query_type = datetime;
                using image_type = long long; // PostgreSQL TIMESTAMP uses 64-bit integer for microseconds

                // PostgreSQL epoch (2000-01-01 00:00:00 UTC) as time_point
                // Using std::chrono::system_clock::from_time_t to convert time_t to time_point
                static std::chrono::system_clock::time_point get_pgsql_epoch_tp() {
                    static const std::chrono::system_clock::time_point pgsql_epoch_tp =
                        std::chrono::system_clock::from_time_t(946684800); // Unix epoch seconds to 2000-01-01 00:00:00 UTC
                    return pgsql_epoch_tp;
                }
                static void
                set_value(datetime &v, const long long &i, bool is_null) {
                    if (is_null) {
                        v = datetime(std::chrono::system_clock::time_point());
                        return;
                    }

                    // 'i' is microseconds since PostgreSQL epoch (2000-01-01 00:00:00 UTC)
                    auto duration_from_pgsql_epoch = std::chrono::microseconds(details::endian_traits::ntoh(i));

                    // Add this duration to the PostgreSQL epoch time point
                    v = datetime(get_pgsql_epoch_tp() + duration_from_pgsql_epoch);
                }

                static void
                set_image(long long &i, bool &is_null, const datetime &v) {
                    is_null = false;

                    // Calculate duration from PostgreSQL epoch to the given datetime
                    auto duration_since_pgsql_epoch = v.to_time_point() - get_pgsql_epoch_tp();

                    // Convert to microseconds
                    i = details::endian_traits::hton(
                            std::chrono::duration_cast<std::chrono::microseconds>(duration_since_pgsql_epoch).count());
                }
        };

    } // namespace pgsql
} // namespace odb

#endif // TRAITS_PGSQL_HXX

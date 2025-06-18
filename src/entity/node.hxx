#ifndef NODE_HXX
#define NODE_HXX

#include <string>
#include <chrono>
#include <odb/core.hxx>
#include "datetime.hxx"
#include "bool.hxx"

#pragma db object
class Node {
    public:
        Node() = default;

        Node(const std::string &status,
             int room_id,
             double latitude,
             double longitude,
             bool is_keep_alive,
             const datetime &created_time,
             const datetime &updated_time,
             const std::string &mac_address)
            : status_(status),
              room_id_(room_id),
              latitude_(latitude),
              longitude_(longitude),
              is_keep_alive_(is_keep_alive),
              created_time_(created_time),
              updated_time_(updated_time),
              mac_address_(mac_address)
        {}

        // Getters
        unsigned long long id() const {
            return id_;
        }

        const std::string &status() const {
            return status_;
        }

        int room_id() const {
            return room_id_;
        }

        double latitude() const {
            return latitude_;
        }

        double longitude() const {
            return longitude_;
        }

        bool is_keep_alive() const {
            return is_keep_alive_;
        }

        const datetime &created_time() const {
            return created_time_;
        }

        const datetime &updated_time() const {
            return updated_time_;
        }

        const std::string &mac_address() const {
            return mac_address_;
        }

        // Setters
        void set_status(const std::string &status) {
            status_ = status;
        }

        void set_room_id(int room_id) {
            room_id_ = room_id;
        }

        void set_latitude(double latitude) {
            latitude_ = latitude;
        }

        void set_longitude(double longitude) {
            longitude_ = longitude;
        }

        void set_is_keep_alive(bool is_keep_alive) {
            is_keep_alive_ = is_keep_alive;
        }

        void set_created_time(const datetime &created_time) {
            created_time_ = created_time;
        }

        void set_updated_time(const datetime &updated_time) {
            updated_time_ = updated_time;
        }

        void set_id(unsigned long long id) {
            id_ = id;
        }

        void set_mac_address(const std::string &mac_address) {
            mac_address_ = mac_address;
        }

    private:
        friend class odb::access;

#pragma db id auto
        unsigned long long id_;

        std::string status_;
        int room_id_;
        double latitude_;
        double longitude_;
        bool is_keep_alive_;
        datetime created_time_;
        datetime updated_time_;
#pragma db unique
        std::string mac_address_;
};

#endif // NODE_HXX

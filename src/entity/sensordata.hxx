#ifndef SENSORDATA_HXX
#define SENSORDATA_HXX

#include <string>
#include <memory>
#include <odb/core.hxx>
#include "datetime.hxx"

// class Node; // forward declaration
#include "node.hxx"

#pragma db object
class SensorData {
    public:
        SensorData() = default;

        SensorData(
            std::shared_ptr<Node> node,
            double voltage,
            double current,
            double power,
            double energy,
            double pf,
            int dust,
            double frequency,
            double temperature,
            double humidity,
            const datetime &timestamp
        )
            : node_(std::move(node)),
              voltage_(voltage),
              current_(current),
              power_(power),
              energy_(energy),
              pf_(pf),
              dust_(dust),
              frequency_(frequency),
              temperature_(temperature),
              humidity_(humidity),
              timestamp_(timestamp)
        {}

        // Getters
        unsigned long long id() const {
            return id_;
        }
        std::shared_ptr<Node> node() const {
            return node_;
        }
        double voltage() const {
            return voltage_;
        }
        double current() const {
            return current_;
        }
        double power() const {
            return power_;
        }
        double energy() const {
            return energy_;
        }
        double pf() const {
            return pf_;
        }
        int dust() const {
            return dust_;
        }
        double frequency() const {
            return frequency_;
        }
        double temperature() const {
            return temperature_;
        }
        double humidity() const {
            return humidity_;
        }
        const datetime &timestamp() const {
            return timestamp_;
        }

        // Setters
        void set_node(std::shared_ptr<Node> n) {
            node_ = std::move(n);
        }
        void set_voltage(double v) {
            voltage_ = v;
        }
        void set_current(double v) {
            current_ = v;
        }
        void set_power(double v) {
            power_ = v;
        }
        void set_energy(double v) {
            energy_ = v;
        }
        void set_pf(double v) {
            pf_ = v;
        }
        void set_dust(int v) {
            dust_ = v;
        }
        void set_frequency(double v) {
            frequency_ = v;
        }
        void set_temperature(double v) {
            temperature_ = v;
        }
        void set_humidity(double v) {
            humidity_ = v;
        }
        void set_timestamp(const datetime &ts) {
            timestamp_ = ts;
        }

    private:
        friend class odb::access;

#pragma db id auto
        unsigned long long id_;

#pragma db not_null
        std::shared_ptr<Node> node_;  // many-to-one relationship

        double voltage_;
        double current_;
        double power_;
        double energy_;
        double pf_;
        int dust_;
        double frequency_;
        double temperature_;
        double humidity_;
        datetime timestamp_;
};

#endif // SENSORDATA_HXX

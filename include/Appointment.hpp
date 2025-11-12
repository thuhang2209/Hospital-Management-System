#ifndef APPOINTMENT_HPP
#define APPOINTMENT_HPP

#include "Patient.hpp"
#include "Doctor.hpp"
#include "AppointmentType.hpp"

#include <string>
#include <memory>
#include <vector>
#include <nlohmann/json.hpp>

class Appointment {
private:
    std::string id;
    Patient patient;
    Doctor doctor;
    std::shared_ptr<AppointmentType> type;
    std::string date;
    std::string time;
    std::string status;

public:
    Appointment() = default;
    Appointment(const std::string& id, const Patient& patient,
                const Doctor& doctor, std::shared_ptr<AppointmentType> type,
                const std::string& date, const std::string& time,
                const std::string& status = "Pending");

    // getters
    std::string getId() const;
    Patient getPatient() const;
    Doctor getDoctor() const;
    std::shared_ptr<AppointmentType> getType() const;
    std::string getDate() const;
    std::string getTime() const;
    std::string getStatus() const;

    // setters
    void setId(const std::string& id);
    void setPatient(const Patient& patient);
    void setDoctor(const Doctor& doctor);
    void setType(std::shared_ptr<AppointmentType> type);
    void setDate(const std::string& date);
    void setTime(const std::string& time);
    void setStatus(const std::string& status);

    // JSON
    nlohmann::json toJSON() const;

    // Create Appointment from JSON. We pass patient & doctor lists to resolve ids.
    static Appointment fromJSON(const nlohmann::json& j,
                                const std::vector<Patient>& patients,
                                const std::vector<Doctor>& doctors);
};

#endif // APPOINTMENT_HPP


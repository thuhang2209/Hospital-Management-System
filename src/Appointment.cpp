#include "Appointment.hpp"
#include <algorithm>
#include <stdexcept>

// Constructor
Appointment::Appointment(const std::string& id, const Patient& patient,
                         const Doctor& doctor, std::shared_ptr<AppointmentType> type,
                         const std::string& date, const std::string& time,
                         const std::string& status)
    : id(id), patient(patient), doctor(doctor), type(type),
      date(date), time(time), status(status) {}

// getters
std::string Appointment::getId() const { return id; }
Patient Appointment::getPatient() const { return patient; }
Doctor Appointment::getDoctor() const { return doctor; }
std::shared_ptr<AppointmentType> Appointment::getType() const { return type; }
std::string Appointment::getDate() const { return date; }
std::string Appointment::getTime() const { return time; }
std::string Appointment::getStatus() const { return status; }

// setters
void Appointment::setId(const std::string& v) { id = v; }
void Appointment::setPatient(const Patient& p) { patient = p; }
void Appointment::setDoctor(const Doctor& d) { doctor = d; }
void Appointment::setType(std::shared_ptr<AppointmentType> t) { type = t; }
void Appointment::setDate(const std::string& d) { date = d; }
void Appointment::setTime(const std::string& t) { time = t; }
void Appointment::setStatus(const std::string& s) { status = s; }

// toJSON
nlohmann::json Appointment::toJSON() const {
    return nlohmann::json{
        {"id", id},
        {"patientId", patient.getId()},
        {"doctorId", doctor.getId()},
        {"type", type ? type->toJSON() : nlohmann::json()},
        {"typeName", type ? type->getName() : "General"},
        {"date", date},
        {"time", time},
        {"status", status}
    };
}

// fromJSON: resolve patient and doctor by id and create appointment type via AppointmentType::fromJSON
Appointment Appointment::fromJSON(const nlohmann::json& j,
                                  const std::vector<Patient>& patients,
                                  const std::vector<Doctor>& doctors) 
{
    std::string pid = j.value("patientId", "");
    std::string did = j.value("doctorId", "");

    auto itP = std::find_if(patients.begin(), patients.end(),
                            [&](const Patient& p){ return p.getId() == pid; });
    auto itD = std::find_if(doctors.begin(), doctors.end(),
                            [&](const Doctor& d){ return d.getId() == did; });

    if (itP == patients.end() || itD == doctors.end()) {
        throw std::runtime_error("Missing patient or doctor in Appointment::fromJSON");
    }

    std::shared_ptr<AppointmentType> atype;
    if (j.contains("type") && j["type"].is_object())
        atype = AppointmentType::fromJSON(j["type"]);
    else
        atype = std::make_shared<AppointmentType>("", "General");

    return Appointment(
        j.value("id", ""),
        *itP,
        *itD,
        atype,
        j.value("date", ""),
        j.value("time", ""),
        j.value("status", "Pending")
    );
}


#include "Doctor.hpp"

Doctor::Doctor(const std::string& id, const std::string& name,
               const std::string& specialty, const std::string& phone, const std::string& email)
    : Staff(id, name, "Doctor", phone, email), specialty(specialty) {}

std::string Doctor::getSpecialty() const { return specialty; }
void Doctor::setSpecialty(const std::string& val) { specialty = val; }

nlohmann::json Doctor::toJSON() const {
    auto j = Staff::toJSON();
    j["specialty"] = specialty;
    return j;
}


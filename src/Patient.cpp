#include "Patient.hpp"

Patient::Patient(const std::string& id, const std::string& name,
                 const std::string& dob, const std::string& phone, const std::string& email)
    : Person(id, name, phone, email), dateOfBirth(dob) {}

std::string Patient::getDateOfBirth() const { return dateOfBirth; }
void Patient::setDateOfBirth(const std::string& dob) { dateOfBirth = dob; }

nlohmann::json Patient::toJSON() const {
    auto j = Person::toJSON();
    j["dateOfBirth"] = dateOfBirth;
    return j;
}




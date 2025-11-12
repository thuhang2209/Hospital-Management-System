#ifndef PATIENT_HPP
#define PATIENT_HPP

#include "Person.hpp"
#include <string>

class Patient : public Person {
private:
    std::string dateOfBirth;

public:
    Patient() = default;
    Patient(const std::string& id, const std::string& name,
            const std::string& dob, const std::string& phone, const std::string& email);

    std::string getDateOfBirth() const;
    void setDateOfBirth(const std::string& dob);

    nlohmann::json toJSON() const override;

    static Patient fromJSON(const nlohmann::json &j) {
    return Patient(
        j.value("id", ""),
        j.value("name", ""),
        j.value("dateOfBirth", ""),
        j.value("phone", ""),
        j.value("email", "")
    );
}

};


#endif // PATIENT_HPP
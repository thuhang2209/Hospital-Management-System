#ifndef DOCTOR_HPP
#define DOCTOR_HPP

#include "Staff.hpp"
#include <string>

class Doctor : public Staff {
private:
    std::string specialty;

public:
    Doctor() = default;
    Doctor(const std::string& id, const std::string& name,
           const std::string& specialty, const std::string& phone, const std::string& email);

    std::string getSpecialty() const;
    void setSpecialty(const std::string& specialty);

    nlohmann::json toJSON() const override;
    
    static Doctor fromJSON(const nlohmann::json &j) {
    return Doctor(
        j.value("id", ""),
        j.value("name", ""),
        j.value("specialty", ""),
        j.value("phone", ""),
        j.value("email", "")
    );
}

};


#endif // DOCTOR_HPP
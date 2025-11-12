#ifndef NURSE_HPP
#define NURSE_HPP

#include "Staff.hpp"
#include <string>

class Nurse : public Staff {
public:
    Nurse() = default;
    Nurse(const std::string& id, const std::string& name,
          const std::string& position, const std::string& phone, const std::string& email);

    std::string getPosition() const;
    void setPosition(const std::string& position);

    nlohmann::json toJSON() const override;

    static Nurse fromJSON(const nlohmann::json &j) {
    return Nurse(
        j.value("id", ""),
        j.value("name", ""),
        j.value("position", ""),
        j.value("phone", ""),
        j.value("email", "")
    );
}

};


#endif // NURSE_HPP
#include "Nurse.hpp"

Nurse::Nurse(const std::string& id, const std::string& name,
             const std::string& position, const std::string& phone, const std::string& email)
    : Staff(id, name, position, phone, email) {}

std::string Nurse::getPosition() const { return position; }
void Nurse::setPosition(const std::string& pos) { position = pos; }

nlohmann::json Nurse::toJSON() const {
    auto j = Staff::toJSON();
    j["position"] = position;
    return j;
}

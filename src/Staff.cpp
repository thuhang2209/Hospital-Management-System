#include "Staff.hpp"

Staff::Staff(const std::string& id, const std::string& name, const std::string& position,
             const std::string& phone, const std::string& email)
    : Person(id, name, phone, email), position(position) {}

std::string Staff::getPosition() const { return position; }
void Staff::setPosition(const std::string& val) { position = val; }

nlohmann::json Staff::toJSON() const {
    auto j = Person::toJSON();
    j["position"] = position;
    return j;
}

#include "AppointmentType.hpp"

// Constructors
AppointmentType::AppointmentType(const std::string& id, const std::string& name)
    : id(id), name(name) {}

AppointmentType::AppointmentType(const std::string& name)
    : id(""), name(name) {}

// Getters / setters
std::string AppointmentType::getId() const { return id; }
std::string AppointmentType::getName() const { return name; }
void AppointmentType::setId(const std::string& v) { id = v; }
void AppointmentType::setName(const std::string& v) { name = v; }

// Polymorphic
std::string AppointmentType::getClassName() const { return "AppointmentType"; }

nlohmann::json AppointmentType::toJSON() const {
    return nlohmann::json{
        {"id", id},
        {"name", name},
        {"class", getClassName()}
    };
}

// Derived class names
std::string CheckupType::getClassName() const { return "CheckupType"; }
std::string SurgeryType::getClassName() const { return "SurgeryType"; }
std::string EmergencyType::getClassName() const { return "EmergencyType"; }

// Factory from JSON
std::shared_ptr<AppointmentType> AppointmentType::fromJSON(const nlohmann::json& j) {
    std::string cls = j.value("class", "AppointmentType");
    std::string idv = j.value("id", "");
    std::string namev = j.value("name", "");

    if (cls == "CheckupType") return std::make_shared<CheckupType>(idv, namev);
    if (cls == "SurgeryType") return std::make_shared<SurgeryType>(idv, namev);
    if (cls == "EmergencyType") return std::make_shared<EmergencyType>(idv, namev);

    return std::make_shared<AppointmentType>(idv, namev);
}


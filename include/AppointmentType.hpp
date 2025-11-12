#ifndef APPOINTMENTTYPE_HPP
#define APPOINTMENTTYPE_HPP

#include <string>
#include <memory>
#include <nlohmann/json.hpp>

class AppointmentType {
protected:
    std::string id;
    std::string name;

public:
    AppointmentType() = default;
    AppointmentType(const std::string& id, const std::string& name);
    AppointmentType(const std::string& name);

    virtual ~AppointmentType() = default;

    // getters / setters
    std::string getId() const;
    std::string getName() const;
    void setId(const std::string& id);
    void setName(const std::string& name);

    // polymorphic behavior
    virtual std::string getClassName() const;
    virtual nlohmann::json toJSON() const;

    // create polymorphic AppointmentType from json
    static std::shared_ptr<AppointmentType> fromJSON(const nlohmann::json& j);
};

// Derived appointment types (names chosen to avoid collisions)
class CheckupType : public AppointmentType {
public:
    using AppointmentType::AppointmentType;
    std::string getClassName() const override;
};

class SurgeryType : public AppointmentType {
public:
    using AppointmentType::AppointmentType;
    std::string getClassName() const override;
};

class EmergencyType : public AppointmentType {
public:
    using AppointmentType::AppointmentType;
    std::string getClassName() const override;
};

#endif // APPOINTMENTTYPE_HPP


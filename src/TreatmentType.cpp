#include "TreatmentType.hpp"

// Constructors
TreatmentType::TreatmentType(const std::string& id, const std::string& name)
    : id(id), name(name) {}

TreatmentType::TreatmentType(const std::string& name)
    : id(""), name(name) {}

// Getters / setters
std::string TreatmentType::getId() const { return id; }
std::string TreatmentType::getName() const { return name; }
void TreatmentType::setId(const std::string& v) { id = v; }
void TreatmentType::setName(const std::string& v) { name = v; }

// Polymorphic
std::string TreatmentType::getClassName() const { return "TreatmentType"; }

nlohmann::json TreatmentType::toJSON() const {
    return nlohmann::json{
        {"id", id},
        {"name", name},
        {"class", getClassName()}
    };
}

// Derived class names
std::string Medication::getClassName() const { return "Medication"; }
std::string SurgeryTreatment::getClassName() const { return "SurgeryTreatment"; }
std::string Therapy::getClassName() const { return "Therapy"; }

// Factory from JSON
std::shared_ptr<TreatmentType> TreatmentType::fromJSON(const nlohmann::json& j) {
    std::string cls = j.value("class", "TreatmentType");
    std::string idv = j.value("id", "");
    std::string namev = j.value("name", "");

    if (cls == "Medication") return std::make_shared<Medication>(idv, namev);
    if (cls == "SurgeryTreatment") return std::make_shared<SurgeryTreatment>(idv, namev);
    if (cls == "Therapy") return std::make_shared<Therapy>(idv, namev);

    return std::make_shared<TreatmentType>(idv, namev);
}

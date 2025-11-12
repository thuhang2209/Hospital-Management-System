#include "Treatment.hpp"
#include <algorithm>
#include <stdexcept>

// Constructor
Treatment::Treatment(const std::string& id, const Patient& patient,
                     std::shared_ptr<TreatmentType> type,
                     const std::string& date, const std::string& notes,
                     const std::string& status)
    : id(id), patient(patient), type(type),
      date(date), notes(notes), status(status) {}

// Getters
std::string Treatment::getId() const { return id; }
Patient Treatment::getPatient() const { return patient; }
std::shared_ptr<TreatmentType> Treatment::getType() const { return type; }
std::string Treatment::getDate() const { return date; }
std::string Treatment::getNotes() const { return notes; }
std::string Treatment::getStatus() const { return status; }

// Setters
void Treatment::setId(const std::string& v) { id = v; }
void Treatment::setPatient(const Patient& p) { patient = p; }
void Treatment::setType(std::shared_ptr<TreatmentType> t) { type = t; }
void Treatment::setDate(const std::string& v) { date = v; }
void Treatment::setNotes(const std::string& v) { notes = v; }
void Treatment::setStatus(const std::string& v) { status = v; }

// Polymorphic
std::string Treatment::getClassName() const { return "Treatment"; }

nlohmann::json Treatment::toJSON() const {
    return {
        {"id", id},
        {"patientId", patient.getId()},
        {"type", type ? type->toJSON() : nlohmann::json()},
        {"typeName", type ? type->getName() : "General"},
        {"date", date},
        {"notes", notes},
        {"status", status},
        {"class", getClassName()}
    };
}

// Factory from JSON
std::shared_ptr<Treatment> Treatment::fromJSON(
    const nlohmann::json& j,
    const std::vector<Patient>& patients)
{
    std::string pid = j.value("patientId", "");
    auto itP = std::find_if(patients.begin(), patients.end(),
                            [&](const Patient& p){ return p.getId() == pid; });

    if (itP == patients.end())
        throw std::runtime_error("Missing patient in Treatment::fromJSON");

    // Use typeName if provided, otherwise use type object or default
    std::string typeName = j.value("typeName", "General");
    auto typePtr = j.contains("type")
        ? TreatmentType::fromJSON(j["type"])
        : std::make_shared<TreatmentType>("TT_" + typeName, typeName);

    std::string id = j.value("id", "");
    std::string date = j.value("date", "");
    std::string notes = j.value("notes", "");
    std::string status = j.value("status", "Ongoing");
    std::string cls = j.value("class", "Treatment");

    if (cls == "MedicationTreatment")
        return std::make_shared<MedicationTreatment>(id, *itP, typePtr, date, notes, status);
    if (cls == "SurgeryTreatmentRecord")
        return std::make_shared<SurgeryTreatmentRecord>(id, *itP, typePtr, date, notes, status);
    if (cls == "TherapyTreatment")
        return std::make_shared<TherapyTreatment>(id, *itP, typePtr, date, notes, status);

    return std::make_shared<Treatment>(id, *itP, typePtr, date, notes, status);
}

#ifndef TREATMENT_HPP
#define TREATMENT_HPP

#include <string>
#include <memory>
#include <vector>
#include <nlohmann/json.hpp>

#include "Patient.hpp"
#include "TreatmentType.hpp"

class Treatment {
protected:
    std::string id;
    Patient patient;
    std::shared_ptr<TreatmentType> type;
    std::string date;
    std::string notes;
    std::string status;

public:
    Treatment() = default;
    Treatment(const std::string& id, const Patient& patient,
              std::shared_ptr<TreatmentType> type,
              const std::string& date, const std::string& notes,
              const std::string& status = "Ongoing");

    virtual ~Treatment() = default;

    // Getters
    std::string getId() const;
    Patient getPatient() const;
    std::shared_ptr<TreatmentType> getType() const;
    std::string getDate() const;
    std::string getNotes() const;
    std::string getStatus() const;

    // Setters
    void setId(const std::string& id);
    void setPatient(const Patient& patient);
    void setType(std::shared_ptr<TreatmentType> type);
    void setDate(const std::string& date);
    void setNotes(const std::string& notes);
    void setStatus(const std::string& status);

    // Polymorphic
    virtual std::string getClassName() const;
    virtual nlohmann::json toJSON() const;

    static std::shared_ptr<Treatment> fromJSON(
        const nlohmann::json& j,
        const std::vector<Patient>& patients
    );
};

// -------- Derived Treatments (polymorphic) --------

class MedicationTreatment : public Treatment {
public:
    using Treatment::Treatment;
    std::string getClassName() const override { return "MedicationTreatment"; }
};

class SurgeryTreatmentRecord : public Treatment {   // renamed
public:
    using Treatment::Treatment;
    std::string getClassName() const override { return "SurgeryTreatmentRecord"; }
};

class TherapyTreatment : public Treatment {
public:
    using Treatment::Treatment;
    std::string getClassName() const override { return "TherapyTreatment"; }
};

#endif // TREATMENT_HPP

#ifndef TREATMENTTYPE_HPP
#define TREATMENTTYPE_HPP

#include <string>
#include <memory>
#include <nlohmann/json.hpp>

class TreatmentType {
protected:
    std::string id;
    std::string name;

public:
    TreatmentType() = default;
    TreatmentType(const std::string& id, const std::string& name);
    TreatmentType(const std::string& name); // constructor 1 tham số

    virtual ~TreatmentType() = default;

    std::string getId() const;
    std::string getName() const;
    void setId(const std::string& id);
    void setName(const std::string& name);

    virtual std::string getClassName() const;
    virtual nlohmann::json toJSON() const;

    static std::shared_ptr<TreatmentType> fromJSON(const nlohmann::json& j);
};

// ===============================
// Derived treatment type classes
// ===============================
class Medication : public TreatmentType {
public:
    using TreatmentType::TreatmentType;
    std::string getClassName() const override;
};

class SurgeryTreatment : public TreatmentType {
public:
    using TreatmentType::TreatmentType;
    std::string getClassName() const override;
};

class Therapy : public TreatmentType {
public:
    using TreatmentType::TreatmentType;
    std::string getClassName() const override;
};

#endif // TREATMENTTYPE_HPP


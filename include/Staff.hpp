#ifndef STAFF_HPP
#define STAFF_HPP

#include "Person.hpp"
#include <string>

class Staff : public Person {
protected:
    std::string position;

public:
    Staff() = default;
    Staff(const std::string& id, const std::string& name, const std::string& position,
          const std::string& phone, const std::string& email);

    std::string getPosition() const;
    void setPosition(const std::string& position);

    nlohmann::json toJSON() const override;
};


#endif // STAFF_HPP
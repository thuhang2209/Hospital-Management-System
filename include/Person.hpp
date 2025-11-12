#ifndef PERSON_HPP
#define PERSON_HPP

#include <string>
#include <nlohmann/json.hpp>

class Person {
protected:
    std::string id;
    std::string name;
    std::string phone;
    std::string email;

public:
    Person() = default;
    Person(const std::string& id, const std::string& name,
           const std::string& phone, const std::string& email);

    // Getters
    std::string getId() const;
    std::string getName() const;
    std::string getPhone() const;
    std::string getEmail() const;

    // Setters
    void setId(const std::string& id);
    void setName(const std::string& name);
    void setPhone(const std::string& phone);
    void setEmail(const std::string& email);

    // JSON
    virtual nlohmann::json toJSON() const;
};


#endif // PERSON_HPP
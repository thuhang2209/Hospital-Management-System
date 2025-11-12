#include "Person.hpp"

Person::Person(const std::string& id, const std::string& name,
               const std::string& phone, const std::string& email)
    : id(id), name(name), phone(phone), email(email) {}

std::string Person::getId() const { return id; }
std::string Person::getName() const { return name; }
std::string Person::getPhone() const { return phone; }
std::string Person::getEmail() const { return email; }

void Person::setId(const std::string& val) { id = val; }
void Person::setName(const std::string& val) { name = val; }
void Person::setPhone(const std::string& val) { phone = val; }
void Person::setEmail(const std::string& val) { email = val; }

nlohmann::json Person::toJSON() const {
    return {
        {"id", id},
        {"name", name},
        {"phone", phone},
        {"email", email}
    };
}



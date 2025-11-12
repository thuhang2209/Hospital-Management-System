#ifndef DATASTORE_HPP
#define DATASTORE_HPP

#include <vector>
#include <memory>
#include <nlohmann/json.hpp>

// Forward declarations (tránh vòng lặp include)
class Doctor;
class Nurse;
class Patient;
class Appointment;
class Treatment;

// Các vector toàn cục chứa dữ liệu chính của hệ thống
extern std::vector<Doctor> doctors;
extern std::vector<Nurse> nurses;
extern std::vector<Patient> patients;
extern std::vector<Appointment> appointments;
extern std::vector<std::shared_ptr<Treatment>> treatments;

// Hàm load / save dữ liệu từ file JSON
void loadData();
void saveData();

#endif // DATASTORE_HPP

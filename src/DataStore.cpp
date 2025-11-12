#include "DataStore.hpp"
#include "Doctor.hpp"
#include "Nurse.hpp"
#include "Patient.hpp"
#include "Appointment.hpp"
#include "AppointmentType.hpp"
#include "Treatment.hpp"
#include "TreatmentType.hpp"

#include <fstream>
#include <iostream>
#include <stdexcept>

using json = nlohmann::json;

// Khởi tạo các vector toàn cục
std::vector<Doctor> doctors;
std::vector<Nurse> nurses;
std::vector<Patient> patients;
std::vector<Appointment> appointments;
std::vector<std::shared_ptr<Treatment>> treatments;

void loadData() {
    std::ifstream file("data.json");
    if (!file.is_open()) {
        std::cerr << " Không tìm thấy file data.json, sẽ tạo mới.\n";
        return;
    }

    json data;
    file >> data;

    try {
        if (data.contains("doctors")) {
            for (auto& d : data["doctors"])
                doctors.push_back(Doctor::fromJSON(d));
        }

        if (data.contains("nurses")) {
            for (auto& n : data["nurses"])
                nurses.push_back(Nurse::fromJSON(n));
        }

        if (data.contains("patients")) {
            for (auto& p : data["patients"])
                patients.push_back(Patient::fromJSON(p));
        }

        if (data.contains("appointments")) {
            for (auto& a : data["appointments"]) {
                try {
                    appointments.push_back(
                        Appointment::fromJSON(a, patients, doctors)
                    );
                } catch (const std::exception& e) {
                    std::cerr << " Lỗi Appointment: " << e.what() << "\n";
                }
            }
        }

        if (data.contains("treatments")) {
            for (auto& t : data["treatments"]) {
                try {
                    treatments.push_back(
                        Treatment::fromJSON(t, patients)
                    );
                } catch (const std::exception& e) {
                    std::cerr << " Lỗi Treatment: " << e.what() << "\n";
                }
            }
        }

        std::cout << " Dữ liệu đã tải thành công.\n";
    } catch (const std::exception& e) {
        std::cerr << " Lỗi khi đọc dữ liệu: " << e.what() << "\n";
    }
}

void saveData() {
    json data;

    data["doctors"] = json::array();
    for (auto& d : doctors)
        data["doctors"].push_back(d.toJSON());

    data["nurses"] = json::array();
    for (auto& n : nurses)
        data["nurses"].push_back(n.toJSON());

    data["patients"] = json::array();
    for (auto& p : patients)
        data["patients"].push_back(p.toJSON());

    data["appointments"] = json::array();
    for (auto& a : appointments)
        data["appointments"].push_back(a.toJSON());

    data["treatments"] = json::array();
    for (auto& t : treatments)
        data["treatments"].push_back(t->toJSON());

    std::ofstream file("data.json");
    file << data.dump(4);

    std::cout << " Dữ liệu đã lưu thành công.\n";
}


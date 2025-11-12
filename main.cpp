#include <iostream>
#include <memory>
#include <vector>
#include <algorithm>
#include <csignal>
#include <atomic>
#include <thread>
#include <chrono>
#include "httplib.h"
#include "nlohmann/json.hpp"

#include "Doctor.hpp"
#include "Nurse.hpp"
#include "Patient.hpp"
#include "Appointment.hpp"
#include "Treatment.hpp"
#include "AppointmentType.hpp"
#include "TreatmentType.hpp"
#include "DataStore.hpp"

using json = nlohmann::json;

// ================== Global flag for server control ==================
std::atomic<bool> server_running(true);

// ================== Signal Handler ==================
void signal_handler(int signal) {
    std::cout << "\n Nhận tín hiệu tắt server (Signal: " << signal << ")...\n";
    server_running = false;
}

// ================== CORS helper ==================
void setCORS(httplib::Response &res) {
    res.set_header("Access-Control-Allow-Origin", "*");
    res.set_header("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
    res.set_header("Access-Control-Allow-Headers", "Content-Type");
}

// ================== Server main ==================
int main() {
    // ✅ Đăng ký xử lý tín hiệu
    std::signal(SIGINT, signal_handler);   // Ctrl+C
    std::signal(SIGTERM, signal_handler);  // kill command

    // ✅ Load dữ liệu
    loadData();

    // ✅ Chuẩn bị các type (đa hình)
    auto generalAppointment = std::make_shared<AppointmentType>("AT001", "Khám tổng quát");
    auto emergencyAppointment = std::make_shared<EmergencyType>("AT002", "Cấp cứu");
    auto surgeryAppointment = std::make_shared<SurgeryType>("AT003", "Phẫu thuật");
    
    auto physioTreatment = std::make_shared<Therapy>("TT001", "Vật lý trị liệu");
    auto medicationTreatment = std::make_shared<Medication>("TT002", "Điều trị thuốc");
    auto surgeryTreatment = std::make_shared<SurgeryTreatment>("TT003", "Phẫu thuật");

    // ✅ Server setup
    httplib::Server svr;

    // --- OPTIONS preflight for CORS ---
    svr.Options(R"(/.*)", [](const httplib::Request&, httplib::Response& res){
        setCORS(res);
        res.status = 200;
    });

    // --- Health ---
    svr.Get("/api/health", [](const httplib::Request&, httplib::Response& res){
        setCORS(res);
        res.set_content("OK", "text/plain");
    });

    // ================== DOCTORS ==================
    svr.Get("/api/doctors", [](const httplib::Request&, httplib::Response& res){
        setCORS(res);
        json j = json::array();
        for (auto& d : doctors) j.push_back(d.toJSON());
        res.set_content(j.dump(), "application/json");
    });

    svr.Get(R"(/api/doctors/(\w+))", [](const httplib::Request& req, httplib::Response& res){
        setCORS(res);
        auto it = std::find_if(doctors.begin(), doctors.end(),
                               [&](const Doctor& d){ return d.getId() == req.matches[1]; });
        if (it != doctors.end())
            res.set_content(it->toJSON().dump(), "application/json");
        else res.status = 404;
    });

    svr.Post("/api/doctors", [](const httplib::Request& req, httplib::Response& res){
        setCORS(res);
        try {
            auto j = json::parse(req.body);
            Doctor d(j["id"], j["name"], j["specialty"], j["phone"], j["email"]);
            doctors.push_back(d);
            saveData();
            res.set_content(d.toJSON().dump(), "application/json");
        } catch (...) {
            res.status = 400;
        }
    });

    svr.Put(R"(/api/doctors/(\w+))", [](const httplib::Request& req, httplib::Response& res){
        setCORS(res);
        auto it = std::find_if(doctors.begin(), doctors.end(),
                               [&](Doctor& d){ return d.getId() == req.matches[1]; });
        if (it != doctors.end()) {
            auto j = json::parse(req.body);
            if (j.contains("name")) it->setName(j["name"]);
            if (j.contains("specialty")) it->setSpecialty(j["specialty"]);
            if (j.contains("phone")) it->setPhone(j["phone"]);
            if (j.contains("email")) it->setEmail(j["email"]);
            saveData();
            res.set_content(it->toJSON().dump(), "application/json");
        } else res.status = 404;
    });

    svr.Delete(R"(/api/doctors/(\w+))", [](const httplib::Request& req, httplib::Response& res){
        setCORS(res);
        auto it = std::remove_if(doctors.begin(), doctors.end(),
                                 [&](Doctor& d){ return d.getId() == req.matches[1]; });
        if (it != doctors.end()) {
            doctors.erase(it, doctors.end());
            saveData();
            res.status = 204;
        } else res.status = 404;
    });

    // ================== NURSES ==================
    svr.Get("/api/nurses", [](const httplib::Request&, httplib::Response& res){
        setCORS(res);
        json j = json::array();
        for (auto& n : nurses) j.push_back(n.toJSON());
        res.set_content(j.dump(), "application/json");
    });

   svr.Get(R"(/api/nurses/(\w+))", [](const httplib::Request& req, httplib::Response& res){
        setCORS(res);
        auto it = std::find_if(nurses.begin(), nurses.end(),
                               [&](const Nurse& n){ return n.getId() == req.matches[1]; });
        if (it != nurses.end())
            res.set_content(it->toJSON().dump(), "application/json");
        else res.status = 404;
    });

    svr.Post("/api/nurses", [](const httplib::Request& req, httplib::Response& res){
        setCORS(res);
        try {
            auto j = json::parse(req.body);
            Nurse n(j["id"], j["name"], j["position"], j["phone"], j["email"]);
            nurses.push_back(n);
            saveData();
            res.set_content(n.toJSON().dump(), "application/json");
        } catch (...) {
            res.status = 400;
        }
    });

    svr.Put(R"(/api/nurses/(\w+))", [](const httplib::Request& req, httplib::Response& res){
        setCORS(res);
        auto it = std::find_if(nurses.begin(), nurses.end(),
                               [&](Nurse& n){ return n.getId() == req.matches[1]; });
        if (it != nurses.end()) {
            auto j = json::parse(req.body);
            if (j.contains("name")) it->setName(j["name"]);
            if (j.contains("position")) it->setPosition(j["position"]);
            if (j.contains("phone")) it->setPhone(j["phone"]);
            if (j.contains("email")) it->setEmail(j["email"]);
            saveData();
            res.set_content(it->toJSON().dump(), "application/json");
        } else res.status = 404;
    });

    svr.Delete(R"(/api/nurses/(\w+))", [](const httplib::Request& req, httplib::Response& res){
        setCORS(res);
        auto it = std::remove_if(nurses.begin(), nurses.end(),
                                 [&](Nurse& n){ return n.getId() == req.matches[1]; });
        if (it != nurses.end()) {
            nurses.erase(it, nurses.end());
            saveData();
            res.status = 204;
        } else res.status = 404;
    });

    // ================== PATIENTS ==================
    svr.Get("/api/patients", [](const httplib::Request&, httplib::Response& res){
        setCORS(res);
        json j = json::array();
        for (auto& p : patients) j.push_back(p.toJSON());
        res.set_content(j.dump(), "application/json");
    });

    svr.Get(R"(/api/patients/(\w+))", [](const httplib::Request& req, httplib::Response& res){
        setCORS(res);
        auto it = std::find_if(patients.begin(), patients.end(),
                               [&](const Patient& p){ return p.getId() == req.matches[1]; });
        if (it != patients.end())
            res.set_content(it->toJSON().dump(), "application/json");
        else res.status = 404;
    });

    svr.Post("/api/patients", [](const httplib::Request& req, httplib::Response& res){
        setCORS(res);
        try {
            auto j = json::parse(req.body);
            Patient p(j["id"], j["name"], j["dateOfBirth"], j["phone"], j["email"]);
            patients.push_back(p);
            saveData();
            res.set_content(p.toJSON().dump(), "application/json");
        } catch (...) {
            res.status = 400;
        }
    });

    svr.Put(R"(/api/patients/(\w+))", [](const httplib::Request& req, httplib::Response& res){
        setCORS(res);
        auto it = std::find_if(patients.begin(), patients.end(),
                               [&](Patient& p){ return p.getId() == req.matches[1]; });
        if (it != patients.end()) {
            auto j = json::parse(req.body);
            if (j.contains("name")) it->setName(j["name"]);
            if (j.contains("dateOfBirth")) it->setDateOfBirth(j["dateOfBirth"]);
            if (j.contains("phone")) it->setPhone(j["phone"]);
            if (j.contains("email")) it->setEmail(j["email"]);
            saveData();
            res.set_content(it->toJSON().dump(), "application/json");
        } else res.status = 404;
    });

    svr.Delete(R"(/api/patients/(\w+))", [](const httplib::Request& req, httplib::Response& res){
        setCORS(res);
        auto it = std::remove_if(patients.begin(), patients.end(),
                                 [&](Patient& p){ return p.getId() == req.matches[1]; });
        if (it != patients.end()) {
            patients.erase(it, patients.end());
            saveData();
            res.status = 204;
        } else res.status = 404;
    });

    // ================== APPOINTMENTS ==================
    svr.Get("/api/appointments", [](const httplib::Request&, httplib::Response& res){
        setCORS(res);
        json j = json::array();
        for (auto& a : appointments) j.push_back(a.toJSON());
        res.set_content(j.dump(), "application/json");
    });

    svr.Post("/api/appointments", [=](const httplib::Request& req, httplib::Response& res){
        setCORS(res);
        std::cout << "Receiving appointment creation request: " << req.body << std::endl;
        auto j = json::parse(req.body);

        // Validate required fields
        if (!j.contains("id") || !j.contains("patientId") || !j.contains("doctorId") || 
            !j.contains("date") || !j.contains("time")) {
            res.status = 400;
            res.set_content(R"({"error":"Missing required fields"})", "application/json");
            return;
        }

        auto itPatient = std::find_if(patients.begin(), patients.end(),
                                      [&](const Patient& p){ return p.getId() == j["patientId"]; });
        auto itDoctor = std::find_if(doctors.begin(), doctors.end(),
                                     [&](const Doctor& d){ return d.getId() == j["doctorId"]; });

        if (itPatient == patients.end() || itDoctor == doctors.end()) {
            res.status = 400;
            std::cout << "Error: Patient or Doctor not found" << std::endl;
            res.set_content(R"({"error":"Patient or Doctor not found"})", "application/json");
            return;
        }

        // Đa hình type
        std::shared_ptr<AppointmentType> type;
        std::string typeName = j.value("typeName", "Khám tổng quát");
        
        if (typeName == "Cấp cứu") {
            type = emergencyAppointment;
        } else if (typeName == "Phẫu thuật") {
            type = surgeryAppointment;
        } else {
            // Nếu là loại khác hoặc không có, dùng khám tổng quát
            type = generalAppointment;
        }

        // Add status if provided, default to "Pending"
        std::string status = j.value("status", "Pending");
        
        Appointment a(j["id"], *itPatient, *itDoctor, type, j["date"], j["time"], status);
        appointments.push_back(a);
        std::cout << "Created appointment: " << a.toJSON().dump() << std::endl;
        std::cout << "Total appointments: " << appointments.size() << std::endl;
        saveData();
        res.set_content(a.toJSON().dump(), "application/json");
    });

    // ================== TREATMENTS ==================
     svr.Get("/api/treatments", [](const httplib::Request&, httplib::Response& res){
        setCORS(res);
        json j = json::array();
        for (auto& t : treatments) j.push_back(t->toJSON()); // sửa: dùng t-> thay vì t.
        res.set_content(j.dump(), "application/json");
    });

    svr.Post("/api/treatments", [=](const httplib::Request& req, httplib::Response& res){
        setCORS(res);
        std::cout << "Receiving treatment creation request: " << req.body << std::endl;
        auto j = json::parse(req.body);

        // Validate required fields
        if (!j.contains("id") || !j.contains("patientId") || 
            !j.contains("date") || !j.contains("notes")) {
            res.status = 400;
            res.set_content(R"({"error":"Missing required fields"})", "application/json");
            return;
        }

        auto itPatient = std::find_if(patients.begin(), patients.end(),
                                      [&](const Patient& p){ return p.getId() == j["patientId"]; });
        if (itPatient == patients.end()) {
            res.status = 400;
            std::cout << "Error: Patient not found" << std::endl;
            res.set_content(R"({"error":"Patient not found"})", "application/json");
            return;
        }

        // Đa hình type
        std::shared_ptr<TreatmentType> type;
        std::string typeName = j.value("typeName", "Vật lý trị liệu");
        
        if (typeName == "Điều trị thuốc") {
            type = medicationTreatment;
        } else if (typeName == "Phẫu thuật") {
            type = surgeryTreatment;
        } else {
            // Nếu là loại khác hoặc không có, dùng vật lý trị liệu
            type = physioTreatment;
        }

        // Add status if provided, default to "Ongoing"
        std::string status = j.value("status", "Ongoing");

        // tạo shared_ptr thay vì đối tượng thường
        auto t = std::make_shared<Treatment>(
            j["id"], *itPatient, type, j["date"], j["notes"], status
        );

        treatments.push_back(t); // vector<shared_ptr<Treatment>>
        std::cout << "Created treatment: " << t->toJSON().dump() << std::endl;
        std::cout << "Total treatments: " << treatments.size() << std::endl;
        saveData();
        res.set_content(t->toJSON().dump(), "application/json"); // dùng t->toJSON()
    });

    // ================== SERVER RUN ==================
    std::cout << " Hospital Management Server running at http://localhost:8080\n";
    std::cout << " Press Ctrl+C to stop the server and save data.\n";

    // ✅ Khởi chạy server trong thread riêng
    std::thread server_thread([&]() {
        svr.listen("0.0.0.0", 8080);
    });

    // ✅ Chờ tín hiệu tắt
    while (server_running) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    // ✅ Dừng server và lưu dữ liệu
    std::cout << "\n Đang dừng server...\n";
    svr.stop();
    
    if (server_thread.joinable()) {
        server_thread.join();
    }

    std::cout << " Đang lưu dữ liệu...\n";
    saveData();
    std::cout << " Server đã dừng hoàn toàn.\n";

    return 0;
}

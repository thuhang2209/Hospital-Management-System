const express = require('express');
const cors = require('cors');
const fs = require('fs');
const path = require('path');
const app = express();

const PORT = 8080;
const DATA_FILE = path.join(__dirname, 'data.json');

app.use(cors({
    origin: ['http://127.0.0.1:5500', 'http://localhost:5500'],
    methods: ['GET', 'POST', 'PUT', 'DELETE', 'OPTIONS'],
    allowedHeaders: ['Content-Type']
}));
app.use(express.json());

// ----------------------------
// Hàm đọc/ghi file
// ----------------------------
function loadData() {
    try {
        if (fs.existsSync(DATA_FILE)) {
            const rawData = fs.readFileSync(DATA_FILE, 'utf8');
            const data = JSON.parse(rawData);
            console.log('✅ Đã tải dữ liệu từ file');
            return data;
        }
    } catch (error) {
        console.log('❌ Lỗi đọc file:', error.message);
    }
    return { doctors: [], nurses: [], patients: [], appointments: [], treatments: [] };
}

function saveData() {
    try {
        const data = { doctors, nurses, patients, appointments, treatments };
        fs.writeFileSync(DATA_FILE, JSON.stringify(data, null, 4));
        console.log('💾 Đã lưu dữ liệu');
    } catch (error) {
        console.log('❌ Lỗi lưu file:', error.message);
    }
}

// ----------------------------
// Tải dữ liệu
// ----------------------------
console.log('🔄 Đang tải dữ liệu...');
const initialData = loadData();
let doctors = initialData.doctors || [];
let nurses = initialData.nurses || [];
let patients = initialData.patients || [];
let appointments = initialData.appointments || [];
let treatments = initialData.treatments || [];

console.log('📊 Dữ liệu:', {
    doctors: doctors.length,
    nurses: nurses.length,
    patients: patients.length,
    appointments: appointments.length,
    treatments: treatments.length
});

// ----------------------------
// API endpoints - ĐẦY ĐỦ TẤT CẢ
// ----------------------------

// Health check
app.get('/api/health', (req, res) => {
    res.json({ 
        status: "OK", 
        data: {
            doctors: doctors.length,
            nurses: nurses.length,
            patients: patients.length,
            appointments: appointments.length,
            treatments: treatments.length
        }
    });
});

// ---------- DOCTORS ----------
app.get('/api/doctors', (req, res) => {
    console.log('GET /api/doctors -', doctors.length, 'bác sĩ');
    res.json(doctors);
});

app.get('/api/doctors/:id', (req, res) => {
    const doctor = doctors.find(d => d.id === req.params.id);
    if (!doctor) return res.status(404).json({ error: "Doctor not found" });
    res.json(doctor);
});

app.post('/api/doctors', (req, res) => {
    try {
        const newDoctor = {
            ...req.body,
            id: req.body.id || `D${(doctors.length + 1).toString().padStart(3, '0')}`
        };
        doctors.push(newDoctor);
        saveData();
        res.status(201).json(newDoctor);
    } catch (error) {
        res.status(400).json({ error: "Invalid data" });
    }
});

app.put('/api/doctors/:id', (req, res) => {
    const index = doctors.findIndex(d => d.id === req.params.id);
    if (index === -1) return res.status(404).json({ error: "Doctor not found" });
    doctors[index] = { ...doctors[index], ...req.body };
    saveData();
    res.json(doctors[index]);
});

app.delete('/api/doctors/:id', (req, res) => {
    const index = doctors.findIndex(d => d.id === req.params.id);
    if (index === -1) return res.status(404).json({ error: "Doctor not found" });
    const deleted = doctors.splice(index, 1)[0];
    saveData();
    res.json({ message: "Doctor deleted", deleted });
});

// ---------- NURSES ----------
app.get('/api/nurses', (req, res) => {
    console.log('GET /api/nurses -', nurses.length, 'y tá');
    res.json(nurses);
});

app.get('/api/nurses/:id', (req, res) => {
    const nurse = nurses.find(n => n.id === req.params.id);
    if (!nurse) return res.status(404).json({ error: "Nurse not found" });
    res.json(nurse);
});

app.post('/api/nurses', (req, res) => {
    try {
        const newNurse = {
            ...req.body,
            id: req.body.id || `N${(nurses.length + 1).toString().padStart(3, '0')}`
        };
        nurses.push(newNurse);
        saveData();
        res.status(201).json(newNurse);
    } catch (error) {
        res.status(400).json({ error: "Invalid data" });
    }
});

app.put('/api/nurses/:id', (req, res) => {
    const index = nurses.findIndex(n => n.id === req.params.id);
    if (index === -1) return res.status(404).json({ error: "Nurse not found" });
    nurses[index] = { ...nurses[index], ...req.body };
    saveData();
    res.json(nurses[index]);
});

app.delete('/api/nurses/:id', (req, res) => {
    const index = nurses.findIndex(n => n.id === req.params.id);
    if (index === -1) return res.status(404).json({ error: "Nurse not found" });
    const deleted = nurses.splice(index, 1)[0];
    saveData();
    res.json({ message: "Nurse deleted", deleted });
});

// ---------- PATIENTS ----------
app.get('/api/patients', (req, res) => {
    console.log('GET /api/patients -', patients.length, 'bệnh nhân');
    res.json(patients);
});

app.get('/api/patients/:id', (req, res) => {
    const patient = patients.find(p => p.id === req.params.id);
    if (!patient) return res.status(404).json({ error: "Patient not found" });
    res.json(patient);
});

app.post('/api/patients', (req, res) => {
    try {
        const newPatient = {
            ...req.body,
            id: req.body.id || `P${(patients.length + 1).toString().padStart(3, '0')}`
        };
        patients.push(newPatient);
        saveData();
        res.status(201).json(newPatient);
    } catch (error) {
        res.status(400).json({ error: "Invalid data" });
    }
});

app.put('/api/patients/:id', (req, res) => {
    const index = patients.findIndex(p => p.id === req.params.id);
    if (index === -1) return res.status(404).json({ error: "Patient not found" });
    patients[index] = { ...patients[index], ...req.body };
    saveData();
    res.json(patients[index]);
});

app.delete('/api/patients/:id', (req, res) => {
    const index = patients.findIndex(p => p.id === req.params.id);
    if (index === -1) return res.status(404).json({ error: "Patient not found" });
    const deleted = patients.splice(index, 1)[0];
    saveData();
    res.json({ message: "Patient deleted", deleted });
});

// ---------- APPOINTMENTS ----------
app.get('/api/appointments', (req, res) => {
    console.log('GET /api/appointments -', appointments.length, 'lịch hẹn');
    res.json(appointments);
});

app.get('/api/appointments/:id', (req, res) => {
    const appointment = appointments.find(a => a.id === req.params.id);
    if (!appointment) return res.status(404).json({ error: "Appointment not found" });
    res.json(appointment);
});

app.post('/api/appointments', (req, res) => {
    try {
        const newAppointment = {
            ...req.body,
            id: req.body.id || `A${(appointments.length + 1).toString().padStart(3, '0')}`,
            status: req.body.status || "Pending"
        };
        appointments.push(newAppointment);
        saveData();
        res.status(201).json(newAppointment);
    } catch (error) {
        res.status(400).json({ error: "Invalid data" });
    }
});

app.put('/api/appointments/:id', (req, res) => {
    const index = appointments.findIndex(a => a.id === req.params.id);
    if (index === -1) return res.status(404).json({ error: "Appointment not found" });
    appointments[index] = { ...appointments[index], ...req.body };
    saveData();
    res.json(appointments[index]);
});

app.delete('/api/appointments/:id', (req, res) => {
    const index = appointments.findIndex(a => a.id === req.params.id);
    if (index === -1) return res.status(404).json({ error: "Appointment not found" });
    const deleted = appointments.splice(index, 1)[0];
    saveData();
    res.json({ message: "Appointment deleted", deleted });
});

// ---------- TREATMENTS ----------
app.get('/api/treatments', (req, res) => {
    console.log('GET /api/treatments -', treatments.length, 'điều trị');
    res.json(treatments);
});

app.get('/api/treatments/:id', (req, res) => {
    const treatment = treatments.find(t => t.id === req.params.id);
    if (!treatment) return res.status(404).json({ error: "Treatment not found" });
    res.json(treatment);
});

app.post('/api/treatments', (req, res) => {
    try {
        const newTreatment = {
            ...req.body,
            id: req.body.id || `T${(treatments.length + 1).toString().padStart(3, '0')}`,
            status: req.body.status || "Ongoing"
        };
        treatments.push(newTreatment);
        saveData();
        res.status(201).json(newTreatment);
    } catch (error) {
        res.status(400).json({ error: "Invalid data" });
    }
});

app.put('/api/treatments/:id', (req, res) => {
    const index = treatments.findIndex(t => t.id === req.params.id);
    if (index === -1) return res.status(404).json({ error: "Treatment not found" });
    treatments[index] = { ...treatments[index], ...req.body };
    saveData();
    res.json(treatments[index]);
});

app.delete('/api/treatments/:id', (req, res) => {
    const index = treatments.findIndex(t => t.id === req.params.id);
    if (index === -1) return res.status(404).json({ error: "Treatment not found" });
    const deleted = treatments.splice(index, 1)[0];
    saveData();
    res.json({ message: "Treatment deleted", deleted });
});

// ----------------------------
// Start server
// ----------------------------
app.listen(PORT, () => {
    console.log('=================================');
    console.log('🏥 SERVER ĐANG CHẠY TRÊN PORT 8080');
    console.log('=================================');
    console.log(`📊 Dữ liệu: ${doctors.length} BS, ${nurses.length} YT, ${patients.length} BN`);
    console.log(`🌐 Truy cập: http://localhost:${PORT}`);
    console.log('=================================');
});
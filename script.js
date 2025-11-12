// ================== CẤU HÌNH MÁY CHỦ BACKEND ==================
const API_BASE = "http://localhost:8080/api";

// Cấu hình cho từng module
const MODULE_CONFIG = {
    'DOCTOR': { 
        plural: 'doctors', 
        name: 'BÁC SĨ', 
        fields: ['id','name','specialty','phone','email']
    },
    'NURSE': { 
        plural: 'nurses', 
        name: 'Y TÁ', 
        fields: ['id','name','position','phone','email'] 
    },
    'PATIENT': { 
        plural: 'patients', 
        name: 'BỆNH NHÂN', 
        fields: ['id','name','dateOfBirth','phone','email'] 
    },
    'APPOINTMENT': { 
        plural: 'appointments', 
        name: 'LỊCH HẸN', 
        fields: ['id','patientId','doctorId','typeName','date','time','status'],
        types: [
            { value: 'Khám tổng quát', label: 'Khám tổng quát' },
            { value: 'Cấp cứu', label: 'Cấp cứu' },
            { value: 'Phẫu thuật', label: 'Phẫu thuật' }
        ]
    },
    'TREATMENT': { 
        plural: 'treatments', 
        name: 'ĐIỀU TRỊ', 
        fields: ['id','patientId','typeName','date','notes','status'],
        types: [
            { value: 'Vật lý trị liệu', label: 'Vật lý trị liệu' },
            { value: 'Điều trị thuốc', label: 'Điều trị thuốc' },
            { value: 'Phẫu thuật', label: 'Phẫu thuật' }
        ]
    }
};

// Biến cờ để đảm bảo thông báo lỗi kết nối chỉ hiển thị 1 lần
let isConnectionErrorShown = false;
let CURRENT_MODULE = 'DOCTOR'; // Thêm biến toàn cục

/**
 * Hàm chèn thông báo log vào giao diện web.
 * @param {string} message - Nội dung thông báo.
 * @param {('info'|'error'|'success'|'warning')} type - Loại thông báo để chọn style CSS.
 */
function logToUI(message, type = 'info') {
    const logContainer = document.getElementById('GlobalLogDisplay');
    if (!logContainer) {
        if (type === 'error') console.error(`[LOG-ERROR] ${message}`);
        else if (type === 'warning') console.warn(`[LOG-WARN] ${message}`);
        else console.log(`[LOG-INFO] ${message}`);
        return;
    }
    
    const TIMESTAMP = new Date().toLocaleTimeString('vi-VN', {hour: '2-digit', minute:'2-digit', second:'2-digit'});
    
    const logEntry = document.createElement('div');
    logEntry.className = `log-entry log-${type}`;
    
    let icon = '';
    if (type === 'error') icon = '<i class="fas fa-exclamation-triangle"></i>';
    else if (type === 'success') icon = '<i class="fas fa-check-circle"></i>';
    else if (type === 'warning') icon = '<i class="fas fa-exclamation-circle"></i>';
    else icon = '<i class="fas fa-terminal"></i>';
    
    logEntry.innerHTML = `${icon} **[${TIMESTAMP}]** ${message}`;
    
    if (logContainer.firstChild) {
        logContainer.insertBefore(logEntry, logContainer.firstChild);
    } else {
        logContainer.appendChild(logEntry);
    }
    
    while (logContainer.children.length > 50) {
        logContainer.removeChild(logContainer.lastChild);
    }
}

function SHOW_FORM_MODE(module, mode) {
    window.FORM_MODE = mode;
    const formContainer = document.getElementById(`Form${module}AddUpdate`);
    const formTitle = document.getElementById('FormTitle');
    
    if (formContainer) {
        const isVisible = formContainer.style.display !== 'none';
        if (isVisible && window.FORM_MODE === mode) {
            formContainer.style.display = 'none';
            return;
        }

        formContainer.style.display = 'block';
        formTitle.textContent = mode === 'ADD' ? 
            `NHẬP THÔNG TIN ${MODULE_CONFIG[module].name} MỚI` : 
            `CẬP NHẬT ${MODULE_CONFIG[module].name} (NHẬP ID VÀO Ô TRÊN)`;

        // Reset form
        const form = document.getElementById(`${module.toLowerCase()}_form`);
        if (form) form.reset();

        // If update mode, prefill id from top input
        if (mode === 'UPDATE') {
            const topId = document.getElementById('action_id_input').value;
            if (topId) {
                const idInput = document.getElementById(`${module}_id_input`);
                if (idInput) idInput.value = topId;
            }
        }
    }
}

function CALL_API_WITH_ID(module, action) {
    const id = document.getElementById('action_id_input').value;
    if (!id || id.trim() === '') {
        logToUI('VUI LÒNG NHẬP ID VÀO Ô "ID/TỪ KHÓA" TRÊN CÙNG.', 'warning');
        return;
    }
    CALL_API(module, action, id);
}

// ĐÃ SỬA: Hàm hiển thị bảng với các cột phù hợp
function FORMAT_TO_TABLE(data, title) {
    const container = document.getElementById('DataDisplayContent');
    const titleElement = document.getElementById('DataDisplayTitle');

    if (titleElement) titleElement.textContent = title;

    if (!Array.isArray(data) || data.length === 0) {
        container.innerHTML = `<p class="text-gray-500 mt-4 p-4 bg-white rounded-lg border">Không tìm thấy dữ liệu nào.</p>`;
        return;
    }

    // Xác định module hiện tại để hiển thị các cột phù hợp
    const currentModule = window.CURRENT_MODULE || 'PATIENT';
    const config = MODULE_CONFIG[currentModule];
    
    let headers = [];
    let fields = [];

    // Định nghĩa cột hiển thị cho từng module
    if (currentModule === 'PATIENT') {
        headers = ['ID', 'NAME', 'DATE OF BIRTH', 'PHONE', 'EMAIL'];
        fields = ['id', 'name', 'dateOfBirth', 'phone', 'email'];
    } else if (currentModule === 'DOCTOR') {
        headers = ['ID', 'NAME', 'SPECIALTY', 'PHONE', 'EMAIL'];
        fields = ['id', 'name', 'specialty', 'phone', 'email'];
    } else if (currentModule === 'NURSE') {
        headers = ['ID', 'NAME', 'POSITION', 'PHONE', 'EMAIL'];
        fields = ['id', 'name', 'position', 'phone', 'email'];
    } else if (currentModule === 'APPOINTMENT') {
        headers = ['ID', 'PATIENT ID', 'DOCTOR ID', 'TYPE', 'DATE', 'TIME', 'STATUS'];
        fields = ['id', 'patientId', 'doctorId', 'typeName', 'date', 'time', 'status'];
    } else if (currentModule === 'TREATMENT') {
        headers = ['ID', 'PATIENT ID', 'TYPE', 'DATE', 'NOTES', 'STATUS'];
        fields = ['id', 'patientId', 'typeName', 'date', 'notes', 'status'];
    } else {
        // Fallback: hiển thị tất cả các trường
        fields = Object.keys(data[0]);
        headers = fields.map(field => field.toUpperCase());
    }

    const tableHtml = `
        <div class="overflow-x-auto mt-4">
            <table class="min-w-full bg-white border-collapse border border-gray-300">
                <thead>
                    <tr class="bg-gray-100">
                        ${headers.map(header => `
                            <th class="p-3 text-left border-b border-gray-300 font-semibold text-gray-700">${header}</th>
                        `).join('')}
                    </tr>
                </thead>
                <tbody>
                    ${data.map(item => `
                        <tr class="hover:bg-gray-50">
                            ${fields.map(field => `
                                <td class="p-3 border-b border-gray-300 text-sm">
                                    ${item[field] === undefined || item[field] === null ? 
                                        'N/A' : 
                                        typeof item[field] === 'object' ? 
                                        JSON.stringify(item[field]) : 
                                        item[field]
                                    }
                                </td>
                            `).join('')}
                        </tr>
                    `).join('')}
                </tbody>
            </table>
        </div>
    `;

    container.innerHTML = tableHtml;
}

function RENDER_MODULE_UI(module) {
    const config = MODULE_CONFIG[module];
    const area = document.getElementById('MAIN_FUNCTION_AREA');
    const titleElement = document.querySelector('.PAGE-TITLE-BM');
    
    area.innerHTML = '';
    
    if (titleElement) titleElement.textContent = `QUẢN LÝ ${config.name} (MODULE ${module})`;

    const html = `
        <div id="Form${module}AddUpdate" class="p-4 bg-white rounded-lg shadow-md mb-6 border-l-4 border-[var(--ACCENT-MINT)]" style="display: none;">
            <h2 class="font-semibold text-lg mb-4 text-[var(--MINT-DARK)]" id="FormTitle">NHẬP THÔNG TIN MỚI</h2>
            <form id="${module.toLowerCase()}_form" class="grid grid-cols-2 gap-x-6 gap-y-3" onsubmit="event.preventDefault(); CALL_API('${module}', window.FORM_MODE === 'ADD' ? 'ADD' : 'UPDATE');">
                ${config.fields.map(field => {
                    if (field === 'typeName' && config.types) {
                        return `
                            <div class="col-span-1 flex items-center">
                                <label for="${module}_${field}_input" class="w-28 text-sm font-medium text-gray-700 uppercase">${field}:</label>
                                <select id="${module}_${field}_input" name="${field}" class="flex-grow p-2 border border-gray-300 rounded-md text-sm">
                                    ${config.types.map(type => 
                                        `<option value="${type.value}">${type.label}</option>`
                                    ).join('')}
                                </select>
                            </div>
                        `;
                    } else if (field === 'date') {
                        return `
                            <div class="col-span-1 flex items-center">
                                <label for="${module}_${field}_input" class="w-28 text-sm font-medium text-gray-700 uppercase">${field}:</label>
                                <input type="date" id="${module}_${field}_input" name="${field}" class="flex-grow p-2 border border-gray-300 rounded-md text-sm">
                            </div>
                        `;
                    } else if (field === 'time') {
                        return `
                            <div class="col-span-1 flex items-center">
                                <label for="${module}_${field}_input" class="w-28 text-sm font-medium text-gray-700 uppercase">${field}:</label>
                                <input type="time" id="${module}_${field}_input" name="${field}" class="flex-grow p-2 border border-gray-300 rounded-md text-sm">
                            </div>
                        `;
                    } else if (field === 'status') {
                        return `
                            <div class="col-span-1 flex items-center">
                                <label for="${module}_${field}_input" class="w-28 text-sm font-medium text-gray-700 uppercase">${field}:</label>
                                <select id="${module}_${field}_input" name="${field}" class="flex-grow p-2 border border-gray-300 rounded-md text-sm">
                                    ${module === 'APPOINTMENT' ? `
                                        <option value="Pending">Đang chờ</option>
                                        <option value="Confirmed">Đã xác nhận</option>
                                        <option value="Completed">Hoàn thành</option>
                                        <option value="Cancelled">Đã hủy</option>
                                    ` : `
                                        <option value="Ongoing">Đang tiến hành</option>
                                        <option value="Completed">Hoàn thành</option>
                                        <option value="Cancelled">Đã hủy</option>
                                    `}
                                </select>
                            </div>
                        `;
                    } else if (field === 'notes') {
                        return `
                            <div class="col-span-2 flex items-start">
                                <label for="${module}_${field}_input" class="w-28 text-sm font-medium text-gray-700 uppercase">${field}:</label>
                                <textarea id="${module}_${field}_input" name="${field}" rows="3" class="flex-grow p-2 border border-gray-300 rounded-md text-sm"></textarea>
                            </div>
                        `;
                    } else if (field === 'dateOfBirth') {
                        return `
                            <div class="col-span-1 flex items-center">
                                <label for="${module}_${field}_input" class="w-28 text-sm font-medium text-gray-700 uppercase">${field}:</label>
                                <input type="date" id="${module}_${field}_input" name="${field}" class="flex-grow p-2 border border-gray-300 rounded-md text-sm">
                            </div>
                        `;
                    } else {
                        return `
                            <div class="col-span-1 flex items-center">
                                <label for="${module}_${field}_input" class="w-28 text-sm font-medium text-gray-700 uppercase">${field}:</label>
                                <input type="text" id="${module}_${field}_input" name="${field}" ${field === 'id' || field === 'name' ? 'required' : ''} class="flex-grow p-2 border border-gray-300 rounded-md text-sm">
                            </div>
                        `;
                    }
                }).join('')}
                <button type="submit" class="col-span-2 ACTION-BTN-BM BG-MINT-ACCENT mt-4"><i class="FAS FA-SAVE"></i> LƯU VÀO HỆ THỐNG</button>
            </form>
        </div>
        
        <div id="${module}ActionButtons" class="p-4 bg-white rounded-lg shadow-md">
            <h2 class="font-semibold text-lg mb-4 text-[var(--MINT-DARK)]">CHỨC NĂNG (MENU ${module})</h2>
            <div class="flex flex-wrap gap-3">
                <button class="ACTION-BTN-BM BG-MINT-ACCENT" onclick="CALL_API('${module}', 'LIST')"><i class="FAS FA-LIST-ALT"></i> 1. DANH SÁCH ${config.name}</button>
                <button class="ACTION-BTN-BM BG-MINT-ACCENT" onclick="SHOW_FORM_MODE('${module}', 'ADD')"><i class="FAS FA-USER-PLUS"></i> 2. THÊM ${config.name} MỚI</button>
                <button class="ACTION-BTN-BM BG-SKY-ACCENT" onclick="CALL_API_WITH_ID('${module}', 'SEARCH')"><i class="FAS FA-SEARCH"></i> 3. TÌM KIẾM BẰNG ID</button>
                <button class="ACTION-BTN-BM BG-RED-PASTEL" onclick="CALL_API_WITH_ID('${module}', 'REMOVE')"><i class="FAS FA-USER-TIMES"></i> 4. XÓA</button>
                <button class="ACTION-BTN-BM BG-SKY-ACCENT" onclick="SHOW_FORM_MODE('${module}', 'UPDATE')"><i class="FAS FA-USER-EDIT"></i> 5. CẬP NHẬT THÔNG TIN</button>
                ${module === 'DOCTOR' ? `<button class="ACTION-BTN-BM BG-SKY-ACCENT" onclick="CALL_API_WITH_ID('DOCTOR', 'SCHEDULE')"><i class="FAS FA-CALENDAR-ALT"></i> 6. XEM LỊCH LÀM VIỆC</button>` : ''}
            </div>
        </div>
    `;
    area.innerHTML = html;
    CALL_API(module, 'LIST', null, true);
}

function COLLECT_DATA_FROM_FORM(module) {
    const config = MODULE_CONFIG[module];
    const data = {};

    for (const field of config.fields) {
        const el = document.getElementById(`${module}_${field}_input`);
        if (el) {
            if (field === 'typeName' && config.types) {
                // Đảm bảo typeName luôn có giá trị hợp lệ từ select
                data[field] = el.value || config.types[0].value;
            } else if (field === 'status') {
                // Đảm bảo status luôn có giá trị mặc định
                data[field] = el.value || (module === 'APPOINTMENT' ? 'Pending' : 'Ongoing');
            } else {
                const value = el.value.trim();
                if (value || field === 'notes') { // notes có thể để trống
                    data[field] = value;
                }
            }
        }
    }

    // Validation dữ liệu
    if (!data.id || !data.id.trim()) {
        logToUI('Vui lòng nhập ID!', 'error');
        return null;
    }

    if (module === 'APPOINTMENT') {
        if (!data.patientId || !data.patientId.trim()) {
            logToUI('Vui lòng nhập ID bệnh nhân!', 'error');
            return null;
        }
        if (!data.doctorId || !data.doctorId.trim()) {
            logToUI('Vui lòng nhập ID bác sĩ!', 'error');
            return null;
        }
        if (!data.date) {
            logToUI('Vui lòng chọn ngày hẹn!', 'error');
            return null;
        }
        if (!data.time) {
            logToUI('Vui lòng chọn giờ hẹn!', 'error');
            return null;
        }
        
        // Validate giờ hẹn trong khoảng làm việc (8:00 - 17:00)
        const time = data.time.split(':');
        const hour = parseInt(time[0]);
        if (hour < 8 || hour >= 17) {
            logToUI('Giờ hẹn phải trong khoảng 8:00 - 17:00!', 'error');
            return null;
        }

        // Validate ngày hẹn không được là ngày trong quá khứ
        const appointmentDate = new Date(data.date);
        const today = new Date();
        today.setHours(0, 0, 0, 0);
        if (appointmentDate < today) {
            logToUI('Ngày hẹn không được là ngày trong quá khứ!', 'error');
            return null;
        }
    }

    if (module === 'TREATMENT') {
        if (!data.patientId || !data.patientId.trim()) {
            logToUI('Vui lòng nhập ID bệnh nhân!', 'error');
            return null;
        }
        if (!data.date) {
            logToUI('Vui lòng chọn ngày điều trị!', 'error');
            return null;
        }
        
        // Validate ngày điều trị không được là ngày trong tương lai
        const treatmentDate = new Date(data.date);
        const today = new Date();
        today.setHours(0, 0, 0, 0);
        if (treatmentDate > today) {
            logToUI('Ngày điều trị không được là ngày trong tương lai!', 'error');
            return null;
        }

        // Đảm bảo có ghi chú cho điều trị
        if (!data.notes || !data.notes.trim()) {
            logToUI('Vui lòng nhập ghi chú về điều trị!', 'error');
            return null;
        }

        // Validate loại điều trị
        if (!data.typeName || !data.typeName.trim()) {
            logToUI('Vui lòng chọn loại điều trị!', 'error');
            return null;
        }
    }

    return data;
}

async function CALL_API(module, action, targetId = null, isAutoLoad = false) {
    try {
        const config = MODULE_CONFIG[module];
        const MODULE_PLURAL = config.plural;
        const ACTION = action.toLowerCase();
        let dataToSend = null;
        let method = 'GET';
        let endpoint = `${API_BASE}/${MODULE_PLURAL}`;

        if (!isAutoLoad) {
            logToUI(`Bắt đầu gọi API: ${endpoint} (${ACTION})`, 'info');
        }

        if (ACTION === 'add') {
            method = 'POST';
            dataToSend = COLLECT_DATA_FROM_FORM(module);
            if (!dataToSend) return;
        } else if (ACTION === 'update') {
            method = 'PUT';
            dataToSend = COLLECT_DATA_FROM_FORM(module);
            if (!dataToSend || !dataToSend.id) return;
            endpoint += `/${dataToSend.id}`;
        } else if (targetId) {
            endpoint += `/${targetId}`;
            if (ACTION === 'remove') {
                method = 'DELETE';
            }
        }

        if (dataToSend) {
            logToUI(`Dữ liệu gửi đi: ${JSON.stringify(dataToSend)}`, 'info');
        }

        const response = await fetch(endpoint, {
            method: method,
            headers: { 'Content-Type': 'application/json' },
            body: dataToSend ? JSON.stringify(dataToSend) : null
        });

        if (!response.ok) {
            const errorText = await response.text();
            logToUI(`Lỗi API (${response.status}): ${errorText}`, 'error');
            return;
        }

        const data = await response.json();
        
        if (ACTION === 'list') {
            FORMAT_TO_TABLE(data, `DANH SÁCH ${config.name} (${data.length} MỤC)`);
            if (!isAutoLoad) {
                logToUI(`Đã tải ${data.length} mục ${config.name.toLowerCase()}.`, 'success');
            }
        } else if (ACTION === 'search') {
            FORMAT_TO_TABLE([data], `KẾT QUẢ TÌM KIẾM`);
            logToUI(`Đã tìm thấy kết quả cho ID: ${targetId}`, 'success');
        } else {
            logToUI(`Thao tác ${ACTION.toUpperCase()} thành công!`, 'success');
            // Refresh list after modification
            setTimeout(() => CALL_API(module, 'LIST', null, true), 500);
        }

        return data;
    } catch (error) {
        logToUI(`Lỗi: ${error.message}`, 'error');
        console.error(error);
    }
}

function showConnectionError() {
    if (!isConnectionErrorShown) {
        const message = "Không thể kết nối đến máy chủ C++! Đảm bảo hms_server.exe đang chạy trên cổng 8080.";
        logToUI(`LỖI KẾT NỐI: ${message}`, 'error');
        isConnectionErrorShown = true;
    }
}

async function fetchData(endpoint) {
    logToUI(`Đang gọi API: ${API_BASE}/${endpoint}`, 'info');

    try {
        const response = await fetch(`${API_BASE}/${endpoint}`);
        if (!response.ok) {
            logToUI(`Lỗi HTTP khi tải ${endpoint}: ${response.status} ${response.statusText}`, 'error');
            return [];
        }
        logToUI(`Tải dữ liệu ${endpoint} thành công (HTTP ${response.status})`, 'success');
        return await response.json();
    } catch (err) {
        logToUI(`LỖI MẠNG: Không thể kết nối đến máy chủ tại ${API_BASE}. Chi tiết: ${err.message}`, 'error');
        showConnectionError();
        return [];
    }
}

function LOADPAGE(module) {
    window.CURRENT_MODULE = module; // Đảm bảo lưu module hiện tại
    document.querySelectorAll('.NAV-ITEM-BM').forEach(li => li.classList.remove('active'));
    document.querySelector(`[onclick="LOADPAGE('${module}')"]`).closest('li').classList.add('active');
    RENDER_MODULE_UI(module);
}

// Khởi tạo khi trang load
window.addEventListener("DOMContentLoaded", async () => {
    logToUI("Đang khởi tạo hệ thống...", 'info');

    try {
        const res = await fetch(`${API_BASE}/health`);
        if(res.ok) {
            logToUI(`Máy chủ hoạt động: ${await res.text()}`, 'success');
        } else {
            logToUI(`Lỗi kiểm tra Health Check: ${res.statusText}`, 'warning');
        }
    } catch {
        logToUI("Không thể gọi /api/health. Server có thể chưa chạy.", 'warning');
    }

    LOADPAGE('DOCTOR');
    logToUI("Chương trình đã sẵn sàng.", 'success');
});

/**
 * @file LoadCellManager.cpp
 * @brief Implementation của LoadCellManager class
 */

#include "LoadCellManager.h"

/**
 * Constructor - Lưu trữ các thông số cấu hình
 * Không thực hiện khởi tạo phần cứng tại đây, chỉ lưu tham số
 */
LoadCellManager::LoadCellManager(int doutPin, int sckPin, float calibrationFactor)
    : doutPin(doutPin), sckPin(sckPin), calibrationFactor(calibrationFactor) {
}

/**
 * Khởi tạo kết nối với HX711 và cấu hình cảm biến
 * Bước 1: Thiết lập các chân giao tiếp
 * Bước 2: Áp dụng hệ số hiệu chuẩn
 * Bước 3: Tare về 0 để loại bỏ trọng lượng bát/khay chứa
 */
void LoadCellManager::init() {
    hx711.begin(doutPin, sckPin);
    hx711.set_scale(calibrationFactor);
    hx711.tare();  // Đặt điểm 0 ban đầu
    Serial.println("LoadCell Initialized");
}

/**
 * Đọc trọng lượng với độ chính xác cao
 * Lấy nhiều mẫu và tính trung bình để giảm nhiễu
 * @param samples Số lần đọc để tính trung bình (càng nhiều càng chính xác nhưng chậm hơn)
 * @return Trọng lượng tính bằng gram
 */
float LoadCellManager::getWeight(int samples) {
    return hx711.get_units(samples);
}

/**
 * Đặt lại điểm 0 của cảm biến
 * Sử dụng khi cần loại bỏ trọng lượng của vật chứa
 */
void LoadCellManager::tare() {
    hx711.tare();
}

/**
 * Cập nhật hệ số hiệu chuẩn
 * Hệ số này quyết định độ chính xác của phép đo
 * Cần hiệu chuẩn bằng cách sử dụng các quả cân chuẩn
 */
void LoadCellManager::setCalibrationFactor(float factor) {
    calibrationFactor = factor;
    hx711.set_scale(calibrationFactor);
}

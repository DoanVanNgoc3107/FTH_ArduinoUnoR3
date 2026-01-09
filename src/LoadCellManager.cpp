/**
 * @file LoadCellManager.cpp
 * @brief Implementation của LoadCellManager class
 */

#include "LoadCellManager.h"
#include <math.h>

/**
 * Constructor - Lưu trữ các thông số cấu hình
 * Không thực hiện khởi tạo phần cứng tại đây, chỉ lưu tham số
 */
LoadCellManager::LoadCellManager(int doutPin, int sckPin, float calibrationFactor)
    : doutPin(doutPin),
      sckPin(sckPin),
      calibrationFactor(calibrationFactor),
      filteredWeight(0.0f),
      hasFilteredWeight(false),
      noiseFloor(3.0f),
      spikeThreshold(50.0f),
      alpha(0.25f) {
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
 * Đọc trọng lượng nhanh với median filter để loại nhiễu
 * Median filter: lấy giá trị ở giữa sau khi sắp xếp - loại bỏ spike hiệu quả
 * @param samples Số mẫu đọc (mặc định 5 - tối ưu cho tốc độ và độ chính xác)
 * @return Trọng lượng tính bằng gram
 */
float LoadCellManager::getWeight(int samples) {
    if (!hx711.is_ready()) {
        return 0;
    }
    
    // Giới hạn samples để đảm bảo tốc độ (5 mẫu là tối ưu)
    if (samples > 7) samples = 7;
    if (samples < 3) samples = 3;
    
    // Đọc nhiều mẫu
    float readings[7];
    for (int i = 0; i < samples; i++) {
        readings[i] = hx711.get_units(1);
    }
    
    // Sắp xếp mảng (insertion sort - nhanh cho mảng nhỏ)
    for (int i = 1; i < samples; i++) {
        float key = readings[i];
        int j = i - 1;
        while (j >= 0 && readings[j] > key) {
            readings[j + 1] = readings[j];
            j--;
        }
        readings[j + 1] = key;
    }
    
    // Trả về giá trị median (ở giữa) - loại bỏ spike cao và thấp
    return readings[samples / 2];
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
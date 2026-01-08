/**
 * @file LoadCellManager.h
 * @brief Quản lý cảm biến cân điện tử HX711 để đo trọng lượng sản phẩm
 * @author FTH Arduino Uno Project
 * @date 2026
 * 
 * Class này đóng gói tất cả các chức năng liên quan đến việc đọc và xử lý
 * dữ liệu từ cảm biến cân HX711, bao gồm khởi tạo, hiệu chỉnh và đọc trọng lượng.
 */

#ifndef LOADCELL_MANAGER_H
#define LOADCELL_MANAGER_H

#include <Arduino.h>
#include "HX711.h"

class LoadCellManager {
private:
    HX711 hx711;              ///< Đối tượng HX711 để giao tiếp với cảm biến cân
    int doutPin;              ///< Chân DATA OUT của HX711
    int sckPin;               ///< Chân SERIAL CLOCK của HX711
    float calibrationFactor;  ///< Hệ số hiệu chuẩn để chuyển đổi giá trị đọc sang trọng lượng

public:
    /**
     * @brief Constructor - Khởi tạo LoadCellManager với các thông số cấu hình
     * @param doutPin Chân kết nối DATA OUT của HX711
     * @param sckPin Chân kết nối SERIAL CLOCK của HX711
     * @param calibrationFactor Hệ số hiệu chuẩn để chuyển đổi giá trị thô sang gram
     */
    LoadCellManager(int doutPin, int sckPin, float calibrationFactor);
    
    /**
     * @brief Khởi tạo và cấu hình cảm biến HX711
     * @details Thiết lập giao tiếp với HX711, áp dụng hệ số hiệu chuẩn và tare về 0
     */
    void init();
    
    /**
     * @brief Đọc trọng lượng từ cảm biến
     * @param samples Số lần đọc để lấy giá trị trung bình (mặc định: 10)
     * @return Trọng lượng đo được tính bằng gram
     */
    float getWeight(int samples = 10);
    
    /**
     * @brief Tare (cân bằng) cảm biến về 0
     * @details Đặt giá trị hiện tại làm mốc 0, loại bỏ trọng lượng khay chứa
     */
    void tare();
    
    /**
     * @brief Thay đổi hệ số hiệu chuẩn của cảm biến
     * @param factor Hệ số hiệu chuẩn mới
     */
    void setCalibrationFactor(float factor);
};

#endif

/**
 * @file SystemController.cpp
 * @brief Implementation của SystemController class
 */

#include "SystemController.h"

/**
 * Constructor - Khởi tạo và liên kết các module con
 * Thiết lập ngưỡng trọng lượng mặc định là 200g
 */
SystemController::SystemController(LoadCellManager* loadCell, ServoController* servoController, DisplayManager* display)
    : loadCell(loadCell), servoController(servoController), display(display), weightThreshold(200.0) {
}

/**
 * Khởi tạo tất cả các thành phần của hệ thống theo thứ tự:
 * 1. Serial Communication (9600 baud) - để debug và giám sát
 * 2. LoadCell - cảm biến đo trọng lượng
 * 3. Servo - cơ cấu phân loại
 * 4. Display - màn hình hiển thị
 */
void SystemController::init() {
    Serial.begin(9600);
    Serial.println("Serial Initialized");
    
    loadCell->init();
    loadCell->tare();  // Zero out the scale when empty
    
    servoController->init();
    display->init();
    
    Serial.println("Setup Complete");
}

/**
 * Thực thi chu trình chính của hệ thống
 * Quy trình:
 * 1. Đọc trọng lượng từ cảm biến (5 mẫu - median filter nhanh và chính xác)
 * 2. Gửi dữ liệu qua Serial để giám sát
 * 3. Hiển thị lên màn hình LCD
 * 4. Xử lý logic phân loại
 */
void SystemController::run() {
    float weight = loadCell->getWeight(5);  // 5 mẫu cho tốc độ tối ưu
    
    // Hiển thị qua Serial Monitor để debug
    Serial.print("Weight: ");
    Serial.print(weight);
    Serial.println(" g");
    
    // Hiển thị lên màn hình LCD
    display->displayWeight(weight);
    
    // Xử lý logic phân loại sản phẩm
    processWeight(weight);
}

/**
 * Thay đổi ngưỡng trọng lượng để phân loại
 * Có thể điều chỉnh trong quá trình vận hành
 */
void SystemController::setWeightThreshold(float threshold) {
    weightThreshold = threshold;
}

/**
 * Logic phân loại sản phẩm dựa trên trọng lượng
 * 
 * QUY TẮC PHÂN LOẠI:
 * - Nếu trọng lượng > ngưỡng (200g): Servo quay 90° - đẩy sản phẩm sang ngăn "Nặng"
 * - Nếu trọng lượng <= ngưỡng (200g): Servo về 0° - sản phẩm rơi vào ngăn "Nhẹ"
 * 
 * Có thể mở rộng thêm nhiều mức phân loại khác nhau
 */
void SystemController::processWeight(float weight) {
    if (weight > weightThreshold) {
        servoController->setServo1Angle(90);  // Phân loại: Nặng
    } else {
        servoController->setServo1Angle(0);   // Phân loại: Nhẹ
    }
}

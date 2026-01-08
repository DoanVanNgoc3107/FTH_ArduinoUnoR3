/**
 * @file ServoController.cpp
 * @brief Implementation của ServoController class
 */

#include "ServoController.h"

/**
 * Constructor - Lưu trữ thông tin chân điều khiển
 */
ServoController::ServoController(int servo1Pin, int servo2Pin)
    : servo1Pin(servo1Pin), servo2Pin(servo2Pin) {
}

/**
 * Khởi tạo và kiểm tra servo motor
 * Bước 1: Gắn servo vào các chân PWM
 * Bước 2: Thực hiện chuỗi kiểm tra (90° -> 0°) để đảm bảo hoạt động bình thường
 */
void ServoController::init() {
    servo1.attach(servo1Pin);
    servo2.attach(servo2Pin);
    
    // Kiểm tra hoạt động: chuyển động từ 90° về 0°
    servo1.write(90);
    servo2.write(90);
    delay(1000);  // Chờ servo đến vị trí
    servo1.write(0);
    servo2.write(0);
    
    Serial.println("Servos Initialized");
}

/**
 * Điều khiển servo 1 đến góc chỉ định
 * Sử dụng để đẩy sản phẩm vào ngăn tương ứng
 */
void ServoController::setServo1Angle(int angle) {
    servo1.write(angle);
}

/**
 * Điều khiển servo 2 đến góc chỉ định
 * Sử dụng để đẩy sản phẩm vào ngăn tương ứng
 */
void ServoController::setServo2Angle(int angle) {
    servo2.write(angle);
}

/**
 * Điều khiển cả 2 servo cùng lúc
 * Hữu ích khi cần phối hợp chuyển động đồng bộ
 */
void ServoController::setBothAngles(int angle1, int angle2) {
    servo1.write(angle1);
    servo2.write(angle2);
}

/**
 * Đưa tất cả servo về vị trí ban đầu (0 độ)
 * Sử dụng khi cần reset hệ thống hoặc kết thúc quy trình phân loại
 */
void ServoController::resetPosition() {
    servo1.write(0);
    servo2.write(0);
}

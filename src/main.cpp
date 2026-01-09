#include <Arduino.h>
#include "LoadCellManager.h"
#include "ServoController.h"
#include "DisplayManager.h"
#include "SystemController.h"

// ==================== CẤU HÌNH PHẦN CỨNG ====================

// Cấu hình chân kết nối HX711 LoadCell
constexpr int LOADCELL_DOUT_PIN = 2;  // Chân DATA OUT của HX711
constexpr int LOADCELL_SCK_PIN = 3;   // Chân SERIAL CLOCK của HX711

// Cấu hình chân điều khiển Servo Motor
constexpr int SERVO_1_PIN = 8;        // Servo phân loại chính
constexpr int SERVO_2_PIN = 9;        // Servo phân loại phụ

// Cấu hình màn hình LCD I2C
constexpr int LCD_ADDR = 0x27;        // Địa chỉ I2C của LCD (có thể là 0x3F)
constexpr int LCD_COLUMNS = 16;       // LCD 16 cột
constexpr int LCD_ROWS = 2;           // LCD 2 hàng

// Hệ số hiệu chuẩn cân (cần hiệu chỉnh theo cân thực tế)
constexpr float CALIBRATION_FACTOR = 340.0;  // Giá trị mẫu, cần hiệu chuẩn lại

// ==================== KHỞI TẠO CÁC ĐỐI TƯỢNG OOP ====================

// Tạo đối tượng quản lý cân điện tử
LoadCellManager loadCell(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN, CALIBRATION_FACTOR);

// Tạo đối tượng điều khiển servo motor
ServoController servoController(SERVO_1_PIN, SERVO_2_PIN);

// Tạo đối tượng quản lý màn hình LCD
DisplayManager display(LCD_ADDR, LCD_COLUMNS, LCD_ROWS);

// Tạo đối tượng điều khiển hệ thống tổng thể
SystemController systemController(&loadCell, &servoController, &display);

/**
 * @brief Hàm setup - Chạy 1 lần khi khởi động Arduino
 * @details Khởi tạo tất cả các module thông qua SystemController
 */
void setup() {
    systemController.init();  // Khởi tạo toàn bộ hệ thống
}

/**
 * @brief Hàm loop - Chạy liên tục sau khi setup hoàn tất
 * @details Thực thi chu trình chính: Đo -> Phân loại -> Hiển thị
 */
void loop() {
    systemController.run();   // Thực thi logic chính
    delay(500);               // Delay 500ms giữa các lần đo
}

/**
 * @file DisplayManager.cpp
 * @brief Implementation của DisplayManager class
 */

#include "DisplayManager.h"

/**
 * Constructor - Khởi tạo đối tượng LCD với các thông số cấu hình
 */
DisplayManager::DisplayManager(int address, int columns, int rows)
    : lcd(address, columns, rows), address(address), columns(columns), rows(rows) {
}

/**
 * Khởi tạo LCD và hiển thị thông báo khởi động
 * Bước 1: Khởi động LCD
 * Bước 2: Bật đèn nền (backlight)
 * Bước 3: Hiển thị thông báo hệ thống sẵn sàng
 */
void DisplayManager::init() {
    lcd.init();
    lcd.backlight();
    showStartupMessage();
}

/**
 * Xóa toàn bộ màn hình LCD
 * Tất cả ký tự sẽ bị xóa và con trỏ về vị trí (0,0)
 */
void DisplayManager::clear() {
    lcd.clear();
}

/**
 * Hiển thị text tại vị trí con trỏ hiện tại
 */
void DisplayManager::print(const String& text) {
    lcd.print(text);
}

/**
 * Hiển thị text tại vị trí chỉ định trên màn hình
 * Đặt con trỏ trước khi in để kiểm soát vị trí chính xác
 */
void DisplayManager::print(const String& text, int col, int row) {
    lcd.setCursor(col, row);
    lcd.print(text);
}

/**
 * Hiển thị trọng lượng dưới dạng:
 * Hàng 1: "Weight:"
 * Hàng 2: "<giá_trị> g"
 * Thêm khoảng trắng để xóa ký tự cũ còn sót lại
 */
void DisplayManager::displayWeight(float weight) {
    lcd.setCursor(0, 0);
    lcd.print("Weight:         ");  // Xóa dữ liệu cũ
    lcd.setCursor(0, 1);
    lcd.print(weight);
    lcd.print(" g      ");  // Xóa dữ liệu cũ
}

/**
 * Hiển thị thông báo hệ thống sẵn sàng
 * Thông báo sẽ hiển thị trong 2 giây rồi tự động xóa
 * Góp phần tạo trải nghiệm người dùng tốt hơn
 */
void DisplayManager::showStartupMessage() {
    lcd.print("SYSTEM READY...");
    delay(2000);
    lcd.clear();
}

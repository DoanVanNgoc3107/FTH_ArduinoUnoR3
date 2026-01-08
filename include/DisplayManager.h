/**
 * @file DisplayManager.h
 * @brief Quản lý màn hình LCD I2C để hiển thị thông tin
 * @author FTH Arduino Uno Project
 * @date 2026
 * 
 * Class này quản lý việc hiển thị thông tin lên màn hình LCD 16x2 qua giao thức I2C.
 * Hiển thị trọng lượng sản phẩm và trạng thái hệ thống cho người dùng.
 */

#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include <Arduino.h>
#include "LiquidCrystal_I2C.h"

class DisplayManager {
private:
    LiquidCrystal_I2C lcd;  ///< Đối tượng LCD I2C để giao tiếp với màn hình
    int address;            ///< Địa chỉ I2C của LCD (thường là 0x27 hoặc 0x3F)
    int columns;            ///< Số cột của LCD (16 cho LCD 16x2)
    int rows;               ///< Số hàng của LCD (2 cho LCD 16x2)

public:
    /**
     * @brief Constructor - Khởi tạo DisplayManager với thông số LCD
     * @param address Địa chỉ I2C của LCD (kiểm tra bằng I2C scanner)
     * @param columns Số cột của LCD
     * @param rows Số hàng của LCD
     */
    DisplayManager(int address, int columns, int rows);
    
    /**
     * @brief Khởi tạo màn hình LCD
     * @details Khởi động LCD, bật backlight và hiển thị thông báo khởi động
     */
    void init();
    
    /**
     * @brief Xóa toàn bộ nội dung trên màn hình
     */
    void clear();
    
    /**
     * @brief Hiển thị chuỗi text tại vị trí con trỏ hiện tại
     * @param text Nội dung cần hiển thị
     */
    void print(const String& text);
    
    /**
     * @brief Hiển thị chuỗi text tại vị trí chỉ định
     * @param text Nội dung cần hiển thị
     * @param col Cột bắt đầu (0-15)
     * @param row Hàng (0-1)
     */
    void print(const String& text, int col, int row);
    
    /**
     * @brief Hiển thị trọng lượng đo được lên màn hình
     * @param weight Trọng lượng tính bằng gram
     */
    void displayWeight(float weight);
    
    /**
     * @brief Hiển thị thông báo hệ thống sẵn sàng
     * @details Hiển thị "SYSTEM READY..." trong 2 giây rồi xóa màn hình
     */
    void showStartupMessage();
};

#endif

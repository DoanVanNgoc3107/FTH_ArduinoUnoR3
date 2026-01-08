/**
 * @file SystemController.h
 * @brief Bộ điều khiển tổng thể của hệ thống phân loại sản phẩm
 * @author FTH Arduino Uno Project
 * @date 2026
 * 
 * Đây là class chính điều phối tất cả các module trong hệ thống phân loại sản phẩm.
 * Nó tích hợp LoadCellManager, ServoController và DisplayManager để thực hiện
 * quy trình: Đo trọng lượng -> Phân loại -> Hiển thị kết quả
 */

#ifndef SYSTEM_CONTROLLER_H
#define SYSTEM_CONTROLLER_H

#include <Arduino.h>
#include "LoadCellManager.h"
#include "ServoController.h"
#include "DisplayManager.h"

class SystemController {
private:
    LoadCellManager* loadCell;          ///< Con trỏ đến module quản lý cân điện tử
    ServoController* servoController;   ///< Con trỏ đến module điều khiển servo
    DisplayManager* display;            ///< Con trỏ đến module quản lý màn hình
    float weightThreshold;              ///< Ngưỡng trọng lượng để phân loại sản phẩm (gram)

public:
    /**
     * @brief Constructor - Khởi tạo SystemController với các module con
     * @param loadCell Con trỏ đến đối tượng LoadCellManager
     * @param servoController Con trỏ đến đối tượng ServoController
     * @param display Con trỏ đến đối tượng DisplayManager
     */
    SystemController(LoadCellManager* loadCell, ServoController* servoController, DisplayManager* display);
    
    /**
     * @brief Khởi tạo tất cả các module của hệ thống
     * @details Khởi động Serial, LoadCell, Servo và Display theo thứ tự
     */
    void init();
    
    /**
     * @brief Thực thi chu trình chính của hệ thống
     * @details Đo trọng lượng -> Hiển thị -> Phân loại sản phẩm
     */
    void run();
    
    /**
     * @brief Thiết lập ngưỡng trọng lượng để phân loại
     * @param threshold Ngưỡng trọng lượng mới (gram)
     */
    void setWeightThreshold(float threshold);
    
private:
    /**
     * @brief Xử lý logic phân loại dựa trên trọng lượng
     * @param weight Trọng lượng cần xử lý (gram)
     * @details So sánh trọng lượng với ngưỡng và điều khiển servo tương ứng
     */
    void processWeight(float weight);
};

#endif

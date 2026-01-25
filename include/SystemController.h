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

// Trạng thái hệ thống
enum SystemState {
    STATE_IDLE,       // Chờ sản phẩm
    STATE_WEIGHING,   // Đang cân
    STATE_SORTING     // Đang phân loại
};

class SystemController {
private:
    LoadCellManager* loadCell;          ///< Con trỏ đến module quản lý cân điện tử
    ServoController* servoController;   ///< Con trỏ đến module điều khiển servo
    DisplayManager* display;            ///< Con trỏ đến module quản lý màn hình
    
    int irSensorPin;                    ///< Cảm biến IR phát hiện sản phẩm đến cân
    int irCountPin;                     ///< Cảm biến IR đếm sản phẩm đạt chuẩn
    
    float weightMin;                    ///< Ngưỡng trọng lượng tối thiểu (gram)
    float weightMax;                    ///< Ngưỡng trọng lượng tối đa (gram)
    
    int passCount;                      ///< Số sản phẩm đạt chuẩn
    int rejectCount;                    ///< Số sản phẩm bị loại
    
    SystemState currentState;           ///< Trạng thái hiện tại
    bool lastIRCountState;              ///< Trạng thái trước của cảm biến đếm (để phát hiện cạnh)

public:
    /**
     * @brief Constructor - Khởi tạo SystemController với các module con
     * @param loadCell Con trỏ đến đối tượng LoadCellManager
     * @param servoController Con trỏ đến đối tượng ServoController
     * @param display Con trỏ đến đối tượng DisplayManager
     * @param irSensorPin Chân cảm biến IR phát hiện sản phẩm
     * @param irCountPin Chân cảm biến IR đếm sản phẩm đạt chuẩn
     * @param weightMin Ngưỡng trọng lượng tối thiểu
     * @param weightMax Ngưỡng trọng lượng tối đa
     */
    SystemController(LoadCellManager* loadCell, ServoController* servoController, DisplayManager* display,
                     int irSensorPin, int irCountPin, float weightMin, float weightMax);
    
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
     * @brief Lấy số sản phẩm đạt chuẩn
     */
    int getPassCount();
    
    /**
     * @brief Lấy số sản phẩm bị loại
     */
    int getRejectCount();
    
private:
    /**
     * @brief Kiểm tra sản phẩm có đạt chuẩn không
     * @param weight Trọng lượng sản phẩm (gram)
     * @return true nếu đạt chuẩn [50-200g], false nếu loại
     */
    bool isProductValid(float weight);
    
    /**
     * @brief Xử lý logic phân loại dựa trên trọng lượng
     * @param weight Trọng lượng cần xử lý (gram)
     */
    void processWeight(float weight);
    
    /**
     * @brief Kiểm tra và đếm sản phẩm đạt chuẩn từ cảm biến IR cuối băng chuyền
     */
    void checkPassCounter();
};

#endif

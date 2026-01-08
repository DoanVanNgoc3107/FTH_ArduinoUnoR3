/**
 * @file ServoController.h
 * @brief Quản lý các servo motor để phân loại sản phẩm
 * @author FTH Arduino Uno Project
 * @date 2026
 * 
 * Class này điều khiển 2 servo motor để thực hiện việc phân loại sản phẩm
 * dựa trên trọng lượng. Servo có thể đẩy sản phẩm vào các ngăn khác nhau.
 */

#ifndef SERVO_CONTROLLER_H
#define SERVO_CONTROLLER_H

#include <Arduino.h>
#include "Servo.h"

class ServoController {
private:
    Servo servo1;   ///< Servo motor thứ nhất để phân loại
    Servo servo2;   ///< Servo motor thứ hai để phân loại
    int servo1Pin;  ///< Chân điều khiển PWM cho servo 1
    int servo2Pin;  ///< Chân điều khiển PWM cho servo 2

public:
    /**
     * @brief Constructor - Khởi tạo ServoController với các chân điều khiển
     * @param servo1Pin Chân PWM để điều khiển servo thứ nhất
     * @param servo2Pin Chân PWM để điều khiển servo thứ hai
     */
    ServoController(int servo1Pin, int servo2Pin);
    
    /**
     * @brief Khởi tạo và kiểm tra hoạt động của các servo
     * @details Attach servo vào chân PWM, thực hiện test chuyển động để kiểm tra
     */
    void init();
    
    /**
     * @brief Điều khiển góc xoay của servo 1
     * @param angle Góc xoay mong muốn (0-180 độ)
     */
    void setServo1Angle(int angle);
    
    /**
     * @brief Điều khiển góc xoay của servo 2
     * @param angle Góc xoay mong muốn (0-180 độ)
     */
    void setServo2Angle(int angle);
    
    /**
     * @brief Điều khiển cả 2 servo đồng thời
     * @param angle1 Góc xoay của servo 1 (0-180 độ)
     * @param angle2 Góc xoay của servo 2 (0-180 độ)
     */
    void setBothAngles(int angle1, int angle2);
    
    /**
     * @brief Đưa tất cả servo về vị trí ban đầu (0 độ)
     */
    void resetPosition();
};

#endif

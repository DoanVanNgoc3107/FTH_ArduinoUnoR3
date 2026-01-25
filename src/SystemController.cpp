/**
 * @file SystemController.cpp
 * @brief Implementation của SystemController class
 */

#include "SystemController.h"

/**
 * Constructor - Khởi tạo và liên kết các module con
 * Thiết lập ngưỡng trọng lượng và cảm biến IR
 */
SystemController::SystemController(LoadCellManager* loadCell, ServoController* servoController, DisplayManager* display,
                                   int irSensorPin, int irCountPin, float weightMin, float weightMax)
    : loadCell(loadCell), 
      servoController(servoController), 
      display(display),
      irSensorPin(irSensorPin),
      irCountPin(irCountPin),
      weightMin(weightMin),
      weightMax(weightMax),
      passCount(0),
      rejectCount(0),
      currentState(STATE_IDLE),
      lastIRCountState(HIGH) {
}

/**
 * Khởi tạo tất cả các thành phần của hệ thống theo thứ tự:
 * 1. Serial Communication (9600 baud) - để debug và giám sát
 * 2. LoadCell - cảm biến đo trọng lượng
 * 3. Servo - cơ cấu phân loại
 * 4. Display - màn hình hiển thị
 * 5. IR Sensors - cảm biến phát hiện và đếm sản phẩm
 */
void SystemController::init() {
    Serial.begin(9600);
    Serial.println("Serial Initialized");
    
    // Cấu hình cảm biến IR
    pinMode(irSensorPin, INPUT);
    pinMode(irCountPin, INPUT);
    Serial.println("IR Sensors Initialized");
    
    loadCell->init();
    loadCell->tare();  // Zero out the scale when empty
    
    servoController->init();
    servoController->resetPosition();  // Đưa servo về vị trí ban đầu
    
    display->init();
    
    Serial.println("=================================");
    Serial.println("HE THONG PHAN LOAI SAN PHAM");
    Serial.println("Nguong: 50g - 200g");
    Serial.println("=================================");
    Serial.println("Setup Complete - Ready!");
}

/**
 * Thực thi chu trình chính của hệ thống
 * Quy trình:
 * 1. Kiểm tra cảm biến IR để phát hiện sản phẩm đến
 * 2. Đọc trọng lượng từ cảm biến
 * 3. Phân loại: PASS hoặc REJECT
 * 4. Điều khiển servo gạt nếu sản phẩm lỗi
 * 5. Đếm sản phẩm đạt chuẩn từ cảm biến cuối băng chuyền
 */
void SystemController::run() {
    // Kiểm tra cảm biến đếm sản phẩm đạt chuẩn (chạy liên tục)
    checkPassCounter();
    
    // Đọc trọng lượng liên tục
    float weight = loadCell->getWeight(5);
    
    // Nếu trọng lượng < 10g thì coi như nhiễu, chưa có sản phẩm
    if (weight < 10.0) {
        return;  // Chưa có sản phẩm, tiếp tục vòng lặp
    }
    
    // ========== CÓ SẢN PHẨM TRÊN CÂN ==========
    Serial.println("\n>>> San pham tren can!");
    delay(200);  // Chờ ổn định
    
    // Đọc lại trọng lượng chính xác
    weight = loadCell->getWeight(5);
    
    Serial.print("Trong luong: ");
    Serial.print(weight, 1);
    Serial.print(" g -> ");
    
    // Kiểm tra đạt chuẩn
    bool isValid = isProductValid(weight);
    
    // Hiển thị lên LCD
    display->displayWeight(weight);
    
    // ========== SERVO 1: LUÔN GẠT 180° ĐỂ ĐẨY SẢN PHẨM LÊN BĂNG CHUYỀN ==========
    // Servo 1 đặt bên phải cân, gạt sang 180° để đẩy sản phẩm
    servoController->setServo1Angle(180);
    
    if (isValid) {
        // ========== SẢN PHẨM ĐẠT CHUẨN (50g - 200g) ==========
        Serial.println("DAT CHUAN!");
        passCount++;
        
        // Servo 2: KHÔNG LÀM GÌ - sản phẩm đi thẳng trên băng chuyền
        // Sản phẩm sẽ đi tới cuối băng chuyền bình thường
        
        delay(500);  // Chờ servo 1 hoàn thành gạt
        servoController->setServo1Angle(0);  // Đưa servo 1 về vị trí ban đầu
        
    } else {
        // ========== SẢN PHẨM KHÔNG ĐẠT CHUẨN (<50g hoặc >200g) ==========
        if (weight < weightMin) {
            Serial.println("LOAI - Qua nhe!");
        } else {
            Serial.println("LOAI - Qua nang!");
        }
        rejectCount++;
        
        delay(500);  // Chờ servo 1 hoàn thành gạt
        servoController->setServo1Angle(0);  // Đưa servo 1 về vị trí ban đầu
        
        // Delay chờ sản phẩm di chuyển từ cân tới vị trí servo 2 trên băng chuyền
        // (Thời gian này cần điều chỉnh theo tốc độ băng chuyền thực tế)
        delay(1500);  // <-- ĐIỀU CHỈNH THỜI GIAN NÀY THEO TỐC ĐỘ BĂNG CHUYỀN
        
        // Servo 2: Gạt 145° để đẩy sản phẩm lỗi ra ngoài băng chuyền
        servoController->setServo2Angle(145);
        delay(500);  // Chờ servo 2 gạt xong
        
        // Đưa servo 2 về vị trí ban đầu
        servoController->setServo2Angle(0);
    }
    
    // Hiển thị thống kê
    Serial.print("Thong ke: PASS=");
    Serial.print(passCount);
    Serial.print(" | REJECT=");
    Serial.println(rejectCount);
    
    // Chờ sản phẩm rời khỏi cân hoàn toàn
    delay(500);
}

/**
 * Kiểm tra sản phẩm có đạt chuẩn không
 * Đạt chuẩn: trọng lượng nằm trong khoảng [weightMin, weightMax]
 */
bool SystemController::isProductValid(float weight) {
    return (weight >= weightMin && weight <= weightMax);
}

/**
 * Kiểm tra cảm biến IR cuối băng chuyền để xác nhận sản phẩm đã đi qua
 * Phát hiện cạnh xuống (HIGH -> LOW) để phát hiện mỗi sản phẩm
 * Lưu ý: passCount đã được đếm tại cân, đây chỉ để xác nhận
 */
void SystemController::checkPassCounter() {
    bool currentIRState = digitalRead(irCountPin);
    
    // Phát hiện cạnh xuống (sản phẩm vừa đi qua cảm biến)
    if (lastIRCountState == HIGH && currentIRState == LOW) {
        // Chỉ log ra Serial để xác nhận, không đếm lại
        Serial.println(">>> San pham di qua cuoi bang chuyen!");
    }
    
    lastIRCountState = currentIRState;
}

/**
 * Lấy số sản phẩm đạt chuẩn
 */
int SystemController::getPassCount() {
    return passCount;
}

/**
 * Lấy số sản phẩm bị loại
 */
int SystemController::getRejectCount() {
    return rejectCount;
}

/**
 * Xử lý logic phân loại (legacy - giữ để tương thích)
 */
void SystemController::processWeight(float weight) {
    if (isProductValid(weight)) {
        servoController->setServo1Angle(0);   // Đạt chuẩn - cho đi qua
    } else {
        servoController->setServo1Angle(90);  // Loại - gạt bỏ
    }
}

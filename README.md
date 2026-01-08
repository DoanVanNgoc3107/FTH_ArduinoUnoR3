# ĐỒ ÁN 1: HỆ THỐNG PHÂN LOẠI SẢN PHẨM THEO TRỌNG LƯỢNG

## 📋 TỔNG QUAN DỰ ÁN

Hệ thống tự động phân loại sản phẩm dựa trên trọng lượng sử dụng Arduino Uno, cảm biến cân HX711, servo motor và màn hình LCD I2C. Đây là một ứng dụng thực tế trong tự động hóa sản xuất, giúp phân loại sản phẩm nhanh chóng và chính xác.

### 🎯 Mục Đích
- Tự động hóa quy trình phân loại sản phẩm theo trọng lượng
- Giảm thiểu sai sót do con người
- Tăng tốc độ phân loại
- Hiển thị thông tin trực quan cho người vận hành

### 🔧 Linh Kiện Sử Dụng
- **Arduino Uno**: Vi điều khiển chính
- **HX711 Load Cell**: Cảm biến đo trọng lượng (chính xác đến 1g)
- **2x Servo Motor**: Cơ cấu đẩy sản phẩm vào các ngăn phân loại
- **LCD I2C 16x2**: Màn hình hiển thị trọng lượng và trạng thái
- **Nguồn**: 5V/2A cho Arduino và các module

---

## ⚙️ CƠ CHẾ HOẠT ĐỘNG

### Quy Trình Phân Loại

```
┌─────────────────┐
│  Sản phẩm đặt  │
│   lên cân      │
└────────┬────────┘
         │
         ▼
┌─────────────────┐
│  HX711 đo       │
│  trọng lượng    │
└────────┬────────┘
         │
         ▼
┌─────────────────┐
│  So sánh với    │
│  ngưỡng (200g)  │
└────────┬────────┘
         │
    ┌────┴────┐
    │         │
    ▼         ▼
┌─────┐   ┌─────┐
│ > 200g│   │≤200g│
│Nặng │   │ Nhẹ │
└──┬──┘   └──┬──┘
   │         │
   ▼         ▼
┌─────────────────┐
│ Servo xoay 90°  │
│ hoặc về 0°      │
└────────┬────────┘
         │
         ▼
┌─────────────────┐
│ Sản phẩm rơi    │
│ vào ngăn tương  │
│ ứng             │
└─────────────────┘
```

### Nguyên Lý Hoạt Động Chi Tiết

1. **Đo Trọng Lượng**:
   - Sản phẩm được đặt lên load cell
   - HX711 đọc tín hiệu từ load cell và chuyển đổi thành giá trị số
   - Lấy 10 mẫu và tính trung bình để tăng độ chính xác

2. **Xử Lý Dữ Liệu**:
   - Arduino nhận giá trị từ HX711
   - Áp dụng hệ số hiệu chuẩn (calibration factor)
   - Chuyển đổi thành đơn vị gram

3. **Phân Loại**:
   - So sánh trọng lượng với ngưỡng (mặc định 200g)
   - Quyết định hướng đẩy sản phẩm

4. **Điều Khiển Servo**:
   - Nếu nặng (> 200g): Servo xoay 90° → Đẩy sang ngăn "Nặng"
   - Nếu nhẹ (≤ 200g): Servo về 0° → Sản phẩm rơi vào ngăn "Nhẹ"

5. **Hiển Thị**:
   - LCD hiển thị trọng lượng thời gian thực
   - Serial Monitor ghi lại dữ liệu để theo dõi và debug

---

## 🏗️ KIẾN TRÚC CODE (OOP)

Dự án được xây dựng theo mô hình **Lập Trình Hướng Đối Tượng (OOP)** với 4 class chính:

### 📦 Cấu Trúc Thư Mục

```
FTH_ArduinoUno/
├── include/                    # Header files (.h)
│   ├── LoadCellManager.h      # Quản lý cảm biến cân
│   ├── ServoController.h      # Điều khiển servo motor
│   ├── DisplayManager.h       # Quản lý màn hình LCD
│   └── SystemController.h     # Điều khiển tổng thể
├── src/                       # Implementation files (.cpp)
│   ├── main.cpp              # File chính
│   ├── LoadCellManager.cpp
│   ├── ServoController.cpp
│   ├── DisplayManager.cpp
│   └── SystemController.cpp
├── lib/                       # Thư viện bên ngoài
├── test/                      # Test cases
├── docs/                      # Tài liệu
│   └── GUIDE.md
├── platformio.ini            # Cấu hình PlatformIO
└── README.md                 # File này
```

### 🔷 1. LoadCellManager

**Chức năng**: Quản lý cảm biến cân HX711

```cpp
class LoadCellManager {
    - HX711 hx711              // Đối tượng HX711
    - int doutPin              // Chân DATA OUT
    - int sckPin               // Chân SERIAL CLOCK
    - float calibrationFactor  // Hệ số hiệu chuẩn
    
    + init()                   // Khởi tạo cảm biến
    + getWeight(samples)       // Đọc trọng lượng
    + tare()                   // Tare về 0
    + setCalibrationFactor()   // Thay đổi hệ số
}
```

**Nhiệm vụ**:
- Khởi tạo và cấu hình HX711
- Đọc giá trị trọng lượng với khả năng lọc nhiễu
- Tare (cân bằng) về 0
- Quản lý hệ số hiệu chuẩn

### 🔷 2. ServoController

**Chức năng**: Điều khiển các servo motor

```cpp
class ServoController {
    - Servo servo1             // Servo phân loại chính
    - Servo servo2             // Servo phân loại phụ
    - int servo1Pin            // Chân PWM servo 1
    - int servo2Pin            // Chân PWM servo 2
    
    + init()                   // Khởi tạo servo
    + setServo1Angle(angle)    // Điều khiển servo 1
    + setServo2Angle(angle)    // Điều khiển servo 2
    + setBothAngles()          // Điều khiển cả 2
    + resetPosition()          // Reset về 0°
}
```

**Nhiệm vụ**:
- Khởi tạo và kiểm tra servo
- Điều khiển góc xoay chính xác
- Quản lý vị trí của cơ cấu phân loại

### 🔷 3. DisplayManager

**Chức năng**: Quản lý màn hình LCD I2C

```cpp
class DisplayManager {
    - LiquidCrystal_I2C lcd    // Đối tượng LCD
    - int address              // Địa chỉ I2C
    - int columns              // Số cột (16)
    - int rows                 // Số hàng (2)
    
    + init()                   // Khởi tạo LCD
    + clear()                  // Xóa màn hình
    + print(text)              // In text
    + displayWeight(weight)    // Hiển thị trọng lượng
    + showStartupMessage()     // Thông báo khởi động
}
```

**Nhiệm vụ**:
- Khởi tạo và cấu hình LCD
- Hiển thị thông tin trực quan
- Quản lý nội dung hiển thị

### 🔷 4. SystemController

**Chức năng**: Điều khiển tổng thể hệ thống

```cpp
class SystemController {
    - LoadCellManager* loadCell
    - ServoController* servoController
    - DisplayManager* display
    - float weightThreshold    // Ngưỡng phân loại
    
    + init()                   // Khởi tạo hệ thống
    + run()                    // Chu trình chính
    + setWeightThreshold()     // Đặt ngưỡng
    - processWeight(weight)    // Logic phân loại
}
```

**Nhiệm vụ**:
- Khởi tạo tất cả các module
- Điều phối luồng hoạt động
- Xử lý logic phân loại
- Kết nối các module với nhau

### 🔄 Luồng Hoạt Động Trong Code

```
main.cpp
  │
  ├─> setup()
  │     └─> SystemController.init()
  │           ├─> Serial.begin()
  │           ├─> LoadCellManager.init()
  │           ├─> ServoController.init()
  │           └─> DisplayManager.init()
  │
  └─> loop()
        └─> SystemController.run()
              ├─> LoadCellManager.getWeight()
              ├─> Serial.print()
              ├─> DisplayManager.displayWeight()
              └─> processWeight()
                    └─> ServoController.setServo1Angle()
```

---

## 🔌 SƠ ĐỒ KẾT NỐI PHẦN CỨNG

### Arduino Uno Pinout

```
Arduino Uno
┌─────────────────┐
│                 │
│  D2  ──────────►│ HX711 DOUT
│  D3  ──────────►│ HX711 SCK
│                 │
│  D8  ──────────►│ Servo 1 (Signal)
│  D9  ──────────►│ Servo 2 (Signal)
│                 │
│  A4 (SDA) ─────►│ LCD I2C SDA
│  A5 (SCL) ─────►│ LCD I2C SCL
│                 │
│  5V  ──────────►│ VCC các module
│  GND ──────────►│ GND các module
└─────────────────┘
```

### Bảng Kết Nối Chi Tiết

| Arduino Pin | Kết Nối Đến | Mô Tả |
|-------------|-------------|-------|
| D2 | HX711 DOUT | Data Out của cảm biến cân |
| D3 | HX711 SCK | Serial Clock của cảm biến cân |
| D8 | Servo 1 Signal | PWM điều khiển Servo 1 |
| D9 | Servo 2 Signal | PWM điều khiển Servo 2 |
| A4 (SDA) | LCD SDA | I2C Data |
| A5 (SCL) | LCD SCL | I2C Clock |
| 5V | VCC modules | Nguồn 5V cho các module |
| GND | GND modules | Ground chung |

---

## 🚀 HƯỚNG DẪN SỬ DỤNG

### 1. Cài Đặt Môi Trường

#### Yêu Cầu
- **PlatformIO IDE** hoặc **Arduino IDE**
- **Board**: Arduino Uno
- **Framework**: Arduino

#### Thư Viện Cần Thiết
```ini
lib_deps = 
    bogde/HX711@^0.7.5
    marcoschwartz/LiquidCrystal_I2C@^1.1.4
```

### 2. Nạp Code

#### Sử dụng PlatformIO
```bash
# Clone project
git clone [repository-url]
cd FTH_ArduinoUno

# Build project
pio run

# Upload to Arduino
pio run --target upload

# Monitor Serial
pio device monitor
```

#### Sử dụng Arduino IDE
1. Mở file `main.cpp`
2. Cài đặt thư viện: HX711, LiquidCrystal_I2C
3. Chọn Board: Arduino Uno
4. Chọn Port tương ứng
5. Upload

### 3. Hiệu Chuẩn Cân

**⚠️ QUAN TRỌNG**: Cần hiệu chuẩn cân trước khi sử dụng!

```cpp
// Trong main.cpp, thay đổi CALIBRATION_FACTOR
constexpr float CALIBRATION_FACTOR = 0.05;  // Điều chỉnh giá trị này
```

**Cách hiệu chuẩn**:
1. Đặt cân lên không có gì (tare về 0)
2. Đặt quả cân chuẩn (ví dụ: 100g)
3. Đọc giá trị thô từ Serial Monitor
4. Tính: `calibration_factor = trọng_lượng_thật / giá_trị_thô`
5. Cập nhật lại `CALIBRATION_FACTOR`

### 4. Thay Đổi Ngưỡng Phân Loại

```cpp
// Trong SystemController constructor, thay đổi weightThreshold
// Mặc định: 200g
weightThreshold = 200.0;  // Điều chỉnh theo nhu cầu
```

Hoặc trong `setup()`:
```cpp
void setup() {
    systemController.init();
    systemController.setWeightThreshold(150.0);  // Đặt ngưỡng 150g
}
```

---

## 📊 GIÁM SÁT & DEBUG

### Serial Monitor

Mở Serial Monitor (9600 baud) để xem log:

```
Serial Initialized
LoadCell Initialized
Servos Initialized
SYSTEM READY...
Setup Complete

Weight: 0.00 g
Weight: 0.00 g
Weight: 150.50 g
Weight: 150.45 g
Weight: 250.30 g
Weight: 250.35 g
...
```

### LCD Display

```
┌────────────────┐
│Weight:         │  ← Dòng 1
│150.50 g        │  ← Dòng 2
└────────────────┘
```

---

## 🎓 ƯU ĐIỂM CỦA KIẾN TRÚC OOP

### 1. **Encapsulation (Đóng gói)**
- Mỗi class quản lý chức năng riêng
- Dữ liệu được bảo vệ bằng `private`
- Interface rõ ràng qua `public` methods

### 2. **Modularity (Module hóa)**
- Dễ bảo trì: Sửa 1 class không ảnh hưởng class khác
- Dễ test: Test riêng từng module
- Dễ mở rộng: Thêm tính năng mới không cần sửa code cũ

### 3. **Reusability (Tái sử dụng)**
- Các class có thể dùng lại cho dự án khác
- Không phụ thuộc vào logic cụ thể

### 4. **Maintainability (Dễ bảo trì)**
- Code rõ ràng, dễ đọc
- Comments chi tiết theo chuẩn Doxygen
- Cấu trúc file logic

### 5. **Scalability (Khả năng mở rộng)**
- Dễ thêm servo thứ 3, 4...
- Dễ thêm nhiều ngưỡng phân loại
- Dễ tích hợp với hệ thống lớn hơn

---

## 🔧 MỞ RỘNG & CẢI TIẾN

### Ý Tưởng Mở Rộng

1. **Phân loại nhiều mức**:
   ```cpp
   if (weight < 100) {
       // Rất nhẹ
   } else if (weight < 200) {
       // Nhẹ
   } else if (weight < 300) {
       // Trung bình
   } else {
       // Nặng
   }
   ```

2. **Lưu trữ dữ liệu**:
   - Thêm SD Card module
   - Ghi log vào file

3. **Kết nối không dây**:
   - ESP8266/ESP32 để gửi dữ liệu lên cloud
   - Giám sát từ xa qua app mobile

4. **Thêm Button**:
   - Button Start/Stop
   - Button Tare
   - Button thay đổi ngưỡng

5. **Cảnh báo âm thanh**:
   - Buzzer báo khi phát hiện sản phẩm
   - Âm thanh khác nhau cho mỗi loại

---

## 📚 TÀI LIỆU THAM KHẢO

### Datasheets
- [HX711 Datasheet](https://cdn.sparkfun.com/datasheets/Sensors/ForceFlex/hx711_english.pdf)
- [Arduino Uno Reference](https://www.arduino.cc/reference/en/)
- [Servo Library](https://www.arduino.cc/reference/en/libraries/servo/)

### Libraries
- [HX711 Library](https://github.com/bogde/HX711)
- [LiquidCrystal I2C](https://github.com/johnrickman/LiquidCrystal_I2C)

---

## 🐛 TROUBLESHOOTING

### Lỗi Thường Gặp

1. **LCD không hiển thị**:
   - Kiểm tra địa chỉ I2C (0x27 hoặc 0x3F)
   - Chạy I2C Scanner để tìm địa chỉ
   - Kiểm tra kết nối SDA, SCL

2. **Cân không chính xác**:
   - Hiệu chuẩn lại `CALIBRATION_FACTOR`
   - Kiểm tra kết nối HX711
   - Đảm bảo bề mặt cân ổn định

3. **Servo không hoạt động**:
   - Kiểm tra nguồn (cần ít nhất 5V/1A)
   - Kiểm tra kết nối signal
   - Test bằng servo example code

4. **Serial không có dữ liệu**:
   - Đảm bảo baud rate đúng (9600)
   - Kiểm tra driver CH340/CP210x
   - Chọn đúng COM port

---

## 👨‍💻 THÔNG TIN DỰ ÁN

- **Tên Dự Án**: Hệ Thống Phân Loại Sản Phẩm Theo Trọng Lượng
- **Mã Dự Án**: ĐỒ ÁN 1
- **Platform**: PlatformIO + Arduino Framework
- **Board**: Arduino Uno
- **Ngôn Ngữ**: C++ (OOP)
- **Năm**: 2026

---

## 📝 LICENSE

Dự án này được phát triển cho mục đích học tập và nghiên cứu.

---

## 🤝 ĐÓNG GÓP

Nếu bạn có ý tưởng cải tiến hoặc phát hiện lỗi, vui lòng:
1. Fork repository
2. Tạo branch mới
3. Commit changes
4. Tạo Pull Request

---

## 📞 LIÊN HỆ

Nếu có thắc mắc về dự án, vui lòng liên hệ qua:
- Email: [email của bạn]
- GitHub Issues: [link repository]

---

**Happy Coding! 🚀**
#   F T H _ A r d u i n o U n o R 3  
 
# **Automatic Plant Watering System 💧🌱**

## **Table of Contents**
1. [Giới thiệu](#giới-thiệu)
2. [Sơ đồ khối](#sơ-đồ-khối)
3. [Tính năng](#tính-năng)
4. [Cách cài đặt](#cách-cài-đặt)
5. [Hướng dẫn sử dụng](#hướng-dẫn-sử-dụng)
6. [Phân tích chi tiết](#phân-tích-chi-tiết)
7. [Đóng góp](#đóng-góp)
8. [Liên hệ](#liên-hệ)

---

## **Giới thiệu**
Hệ thống tưới cây tự động **Automatic Plant Watering System** giúp tối ưu hóa việc chăm sóc cây trồng thông qua cảm biến, trí tuệ nhân tạo (AI), và ESP32. Dự án này nhằm:

- Tự động tưới cây dựa trên điều kiện môi trường.
- Tiết kiệm nước và công sức.
- Duy trì môi trường phát triển tốt nhất cho cây.

---

## **Sơ đồ khối**
### **Block Diagram**

graph TD;
  A[Xác định các yếu tố ảnh hưởng đến cây trồng] --> B[Các cảm biến thu thập dữ liệu từ môi trường];
  B --> C[AI xử lí các thông số và đưa ra quyết định];
  C --> D{0: Không tưới | 1: Tưới cây};
  D --> E[Máy tính xác định lượng nước cần tưới];
  E --> F[Hoàn tất và Kiểm tra];

### **Chi tiết từng khối**
1. **Cảm biến**: 
   - Đo nhiệt độ, độ ẩm đất, độ ẩm không khí, và tình trạng thời tiết.
2. **AI xử lý**:
   - Phân tích dữ liệu và so sánh với vùng an toàn.
3. **Máy bơm**: 
   - Điều khiển servo và máy bơm để tưới cây hoặc che đậy khi cần thiết.

---

## **Tính năng**
- Tự động thu thập dữ liệu môi trường thông qua cảm biến.
- AI phân tích dữ liệu và đưa ra quyết định tưới cây.
- Điều khiển thủ công và giám sát qua ứng dụng điện thoại.
- Hệ thống che đậy khu vực trồng khi trời mưa.
- Lưu trữ dữ liệu trên đám mây để theo dõi lịch sử.

---

## **Cách cài đặt**
### **Yêu cầu phần cứng**
- ESP32
- Cảm biến: Độ ẩm đất, nhiệt độ, độ ẩm không khí, LCD.
- Máy bơm nước

### **Yêu cầu phần mềm**
- ESP-IDF
- Python (cho các công cụ AI và điều khiển cloud)

### **Hướng dẫn cài đặt**
1. Clone dự án:
   ```bash
   git clone https://github.com/ndhanh2711/AutomaticPlantWateringSystem.git
   cd project-name
   ```
2. Thiết lập ESP-IDF:
   ```bash
   idf.py set-target esp32
   idf.py build
   idf.py flash
   ```

---

## **Hướng dẫn sử dụng**
1. Khởi chạy ESP32🚀 và kết nối với hệ thống cảm biến🔄.
2. Mở ứng dụng để theo dõi dữ liệu và điều khiển thủ công.
3. Quan sát hệ thống⚙️ tự động điều chỉnh lượng nước tưới.
📊
---

## **Phân tích chi tiết**
### **1. Chọn loại cây trồng**
Mỗi loại cây có ngưỡng nhiệt độ, độ ẩm khác nhau. Hệ thống sẽ tùy chỉnh theo đặc điểm của từng loại cây.

### **2. Xác định yếu tố môi trường**
- **Nhiệt độ**: 20°C - 30°C 🌡️ - 🔥 - ❄️
- **Độ ẩm đất**: 30% - 60% 💧 - 🌬️ - 🌫️
- **Độ ẩm không khí**: 40% - 70%
- **Thời tiết**: Mưa hoặc nắng 🌦️ - 🌞

### **3. AI xử lý dữ liệu ⚡**
- **Đầu vào**: Dữ liệu từ cảm biến
- **Đầu ra**: Quyết định tưới cây hoặc không tưới
- **Cải tiến AI**:
  - Dự đoán xu hướng thời tiết dựa trên lịch sử dữ liệu.
  - Tối ưu hóa tần suất tưới cây.

---


## 📁 Cấu trúc dự án


graph TD;
.vscode/
components/
├── lcd/
│   ├── i2c-lcd.c
│   ├── i2c-lcd.h
│   ├── CMakeLists.txt
├── onechot_read/
│   ├── Temp_Hum/
|       |-- dht11.c
|       |-- dht11.h
|       |-- CMakeLists.txt
│   ├── Mois/
|       |-- mois.c
|       |-- mois.h
|       |-- CMakeLists.txt
│   ├── Light/
|       |-- ldr.c
|       |-- ldr.h
|       |-- CMakeLists.txt
data/
|   ├── Image....
main/
|   ├── main.c
|   ├── main.h
|   ├── CMakeLists.txt
CMakelists.txt/
README.md/
sdkconfig/


---

## **Những thách thức 💪 và giải pháp 💡**
### **Thách thức**
1. **Tín hiệu cảm biến không ổn định**:
   - Một số cảm biến có độ nhạy thấp hoặc bị ảnh hưởng bởi nhiễu từ môi trường.
2. **AI không dự đoán chính xác**:
   - Các thuật toán AI có thể thiếu dữ liệu lịch sử hoặc cần cải thiện để phù hợp hơn với điều kiện thực tế.
3. **Hao hụt năng lượng**:
   - Hệ thống ESP32 cần tối ưu hóa để tiết kiệm pin trong các tình huống không cần thiết.

### **Giải pháp**
1. **Cải thiện phần cứng**:
   - Chọn cảm biến có độ nhạy cao và kiểm tra tính ổn định trước khi triển khai.
2. **Cải thiện thuật toán AI**:
   - Thu thập thêm dữ liệu và điều chỉnh mô hình AI để phù hợp hơn với các tình huống cụ thể.
3. **Tối ưu hóa năng lượng**:
   - Sử dụng chế độ ngủ (deep sleep) trên ESP32 để giảm tiêu thụ năng lượng.

---

## **Đóng góp 🤝**
Chúng tôi chào đón mọi ý kiến và đóng góp từ cộng đồng. Vui lòng gửi pull request hoặc tạo issue trên GitHub.

---

## **Liên hệ**
- **Email **: 📧nguyenhanh2711abc@gmail.com
- **GitHub**: [🐙GitHub Profile](https://github.com/ndhanh2711)

# Automatic Plant Watering System
##                Block Diagram
### Xác định các yếu tố ảnh hưởng đến cây trồng
                            ||
                            ||
                           \||/
                            \/
### Các cảm biến thu nhập dữ liệu từ môi trường
                            ||
                            ||
                           \||/
                            \/
### AI xử lí các thông số và đưa ra quyết định
                            ||
                            ||
                           \||/
                            \/
                    | 0 : Không tưới  |
                    | 1 : Tưới cây    |
### Máy tính xác định lượng nước cần tưới cho cây
                            ||
                            ||
                           \||/
                            \/
                   Hoàn tất và Kiểm tra 
## Phân tích từng vấn đề
### 1. Chọn được loại cây mà thiết bị hướng đến
### 2. Xác định các yếu tố ảnh hưởng đến cây
- Nhiệt độ
- Độ ẩm đất
- Độ ẩm không khí
- Nhiệt độ ngoài trời
- Tình trạng thời tiết
Các cảm biến sẽ trả về các giá trị trong khoảng từ [a, b] a là min và b là max, mỗi thông số sẽ có a, b khác nhau và sẽ có những vùng an toàn [x, y] khác nhau cho cây. Việc vủa hệ thống là xác định các yếu tố nằm ngoài vùng an toàn và từ đó đưa ra quyết định về việc tưới cây.
### Ý tưởng cá nhân
- NDH 
-  Xây dựng phần mềm theo dõi tình trạng của những nơi được theo dõi, các thông số được phản hồi từ esp32 -> Cloud
- Hệ thống tưới cây: Tưới theo tự động và tưới thử công bằng điều khiển qua app, dùng servo để hướng vòi bơm đến vị trí cần bơm
- Cảm biến nắng mưa để thực hiện che đậy cho khu vực chứa cây

- Cải thiện về AI
    + Dự đoán xu hướng môi trường: AI có thể phân tích dữ liệu môi trường qua nhiều ngày hoặc tuần để dự đoán nhu cầu tưới của cây dựa trên các điều kiện thay đổi
    + 


# smart-garden-iot
Tên dự án: Khu vườn thông minh

Dự án được thực hiện bởi các thành viên nhóm 11 trong môn học Vi Xử Lý dưới sự hướng dẫn của thầy Hàn Huy Dũng

Các thành viên tham gia thực hiện:

Vũ Đức Thịnh 20200617, 

Trần Ngọc Minh 20203513,

Ngô Vũ Trường Sơn 20203560, 

Đặng Hữu Công Hiếu 20203417,

Sinh viên Trường Điện Điện tử, Đại học Bách Khoa Hà Nội

Mô tả về dự án: Dự án xây dựng một hệ thống tưới cây tự động cho vườn cây tiêu. Dựa vào những dữ liệu thu thập từ cảm biến bao gồm nhiệt độ không khí, độ ẩm không khí, độ ẩm đất từ đó tự động thực hiện bật/tắt máy bơm thông qua relay. Hệ thống còn cho phép cài đặt ngưỡng dành cho các dữ liệu để tự động bật tắt và có thể bật tắt thủ công tùy vào mục đích của người dùng.

------------------------------------------------------------------
Hướng dẫn build và chạy dự án (đối với hệ điều hành Windows)
1. Clone dự án và đặt chung folder với thư viện espressif (v5.0.1)
2. Mở ESP-IDF 5.0 PowerShell và dẫn tới thư mục chứa dự án
3. Nhập "idf.py build" để build dự án
4. Nhập "idf.py -p COM3 flash monitor" và nhấn giữ nút Boot trên bộ vi xử lí
5. Mở log và copy địa chỉ IP được cung cấp
6. Vào trình duyệt và nhập vào đường dẫn sau "[your_ip]:80/dashboard"
7. Enjoy!!!
------------------------------------------------------------------
Liên hệ: Vũ Đức Thịnh - vuducthinh1509@gmail.com

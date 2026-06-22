# Đồ án môn học: Xây dựng Thư viện sử dụng Templates

Tài liệu hướng dẫn biên dịch và chạy mã nguồn của đồ án môn học Cấu trúc dữ liệu và Giải thuật.

## 1. Yêu cầu môi trường
* Trình biên dịch C++ hỗ trợ tiêu chuẩn **C++17** trở lên (ví dụ: `g++`).
* Công cụ biên dịch `make` (không bắt buộc, có thể dùng các câu lệnh biên dịch thủ công tương đương).

> [!IMPORTANT]
> **Thư mục chạy lệnh:** Tất cả các lệnh biên dịch và chạy chương trình dưới đây đều phải được thực thi tại **thư mục gốc của dự án** (thư mục chứa tệp `Makefile` và tệp `README.md` này).

---

## 2. Biên dịch và Chạy chương trình kiểm thử thư viện (Library Tests)
Chương trình kiểm thử nằm tại thư mục `tests/` giúp xác thực hoạt động đúng đắn của các cấu trúc dữ liệu và giải thuật trong thư viện.

### Cách 1: Sử dụng Makefile (Khuyên dùng)
* **Biên dịch:**
  ```bash
  make
  ```
  Lệnh này sẽ tự động biên dịch và tạo ra tệp thực thi `test_algoracoon.exe` ở thư mục gốc.
* **Chạy chương trình:**
  ```bash
  make run
  ```
  hoặc chạy trực tiếp tệp thực thi:
  ```bash
  ./test_algoracoon.exe
  ```
* **Dọn dẹp các tệp sinh ra:**
  ```bash
  make clean
  ```

### Cách 2: Biên dịch thủ công
Nếu hệ thống của bạn không cài đặt `make`, chạy lệnh g++ sau:
```bash
g++ -std=c++17 -Wall -Wextra -pedantic -I. tests/main.cpp -o test_algoracoon.exe
```
Sau đó, chạy file thực thi:
* Trên Windows: `test_algoracoon.exe` hoặc `.\test_algoracoon.exe`
* Trên Linux/macOS: `./test_algoracoon.exe`

### Kết quả hiển thị thành công (OK)
Khi chạy chương trình thành công, màn hình sẽ hiển thị kết quả kiểm thử như dưới đây và tạm dừng để chờ người dùng nhấn phím:
```text
LinkedList forward: 1 2 3
LinkedList backward: 3 2 1
Stack top: 20
Queue front: first
PriorityQueue extract: 9
Sorted array: 1 2 5 5 6 9
Index of 5: 3
BST inorder: 3 8 10
AVL inorder: 10 20 30
AVL height: 2
HashTable alice: 95
HashTable contains bob: 1

Nhan phim Enter de thoat...
```
*Hãy nhấn phím `Enter` để thoát và đóng cửa sổ chương trình.*

---

## 3. Biên dịch và Chạy ứng dụng demo (Application)
Ứng dụng demo mô phỏng Cache (giả lập quản lý bộ nhớ) nằm tại thư mục `app/`, sử dụng các cấu trúc dữ liệu của thư viện để xử lý các yêu cầu thực tế.

### Biên dịch ứng dụng
Chạy lệnh biên dịch thủ công tại thư mục gốc:
```bash
g++ -std=c++17 -Wall -Wextra -pedantic -I. app/main.cpp -o app_main.exe
```

### Chạy ứng dụng
* Trên Windows: `app_main.exe` hoặc `.\app_main.exe`
* Trên Linux/macOS: `./app_main.exe`

### Kết quả hiển thị thành công (OK)
Khi ứng dụng khởi chạy thành công, giao diện tương tác dòng lệnh (CLI) sẽ hiển thị bảng trạng thái cache cùng với menu điều khiển như sau:
```text
======================================================================
TRẠNG THÁI CACHE:
...
 MENU ĐIỀU KHIỂN:
 1. Thêm yêu cầu Đọc (Read) địa chỉ ảo mới
 2. Thêm yêu cầu Ghi (Write) dữ liệu mới vào địa chỉ ảo
 3. Chạy từng bước yêu cầu kế tiếp (Step)
 4. Chạy toàn bộ yêu cầu trong hàng đợi (Process All)
 5. Xem báo cáo phân tích tần suất truy cập (Bubble Sort)
 6. Xem độ ưu tiên giải phóng bộ nhớ (Priority Queue - LFU)
 7. Thoát
 Lựa chọn của bạn:
```
*Nhập các số từ `1` đến `7` và làm theo hướng dẫn của menu để kiểm tra hoạt động của ứng dụng.*

# Thư viện algoracoon

Mã nguồn thư viện nằm trong thư mục `lib/` được cài đặt hoàn toàn bằng **C++ Templates** nhằm đảm bảo tính tổng quát (hoạt động với bất kỳ kiểu dữ liệu phần tử `T` nào). Toàn bộ khai báo và định nghĩa được tích hợp trong cùng một tệp `.hpp` tương ứng để tránh lỗi liên kết khi sử dụng template.

Dưới đây là mô tả chi tiết của 8 thành phần cấu trúc dữ liệu và giải thuật được cài đặt đúng theo yêu cầu của đồ án:

---

## 1. `LinkedList.hpp` (Danh sách liên kết đơn)
Cài đặt danh sách liên kết đơn `LinkedList<T>`.

**Các phương thức tối thiểu hỗ trợ:**
* `insertFront(value)`: Thêm một phần tử mới vào đầu danh sách.
* `insertBack(value)`: Thêm một phần tử mới vào cuối danh sách.
* `insertAt(index, value)`: Chèn phần tử tại vị trí `index` chỉ định.
* `remove(value)`: Tìm và xóa phần tử đầu tiên khớp với giá trị `value`.
* `removeAt(index)`: Xóa và trả về giá trị phần tử tại vị trí `index`.
* `find(value)`: Tìm kiếm phần tử trùng khớp với giá trị `value`, trả về vị trí (index) hoặc `-1` nếu không thấy.
* `size()`: Trả về số lượng phần tử hiện tại.
* `clear()`: Giải phóng toàn bộ danh sách.
* **Khả năng duyệt:** Hỗ trợ cơ chế duyệt qua toàn bộ danh sách từ đầu đến cuối và ngược lại từ cuối về đầu.

---

## 2. `Stack.hpp` (Ngăn xếp)
Cài đặt cấu trúc ngăn xếp `Stack<T>` theo cơ chế vào sau ra trước (LIFO).

**Các phương thức cốt lõi hỗ trợ:**
* `push(value)`: Đẩy một phần tử mới vào đỉnh ngăn xếp.
* `pop()`: Loại bỏ và trả về phần tử nằm ở đỉnh ngăn xếp.
* `top()`: Xem giá trị phần tử ở đỉnh ngăn xếp mà không loại bỏ.
* `empty()`: Kiểm tra ngăn xếp có đang rỗng hay không.
* `size()`: Trả về số lượng phần tử trong ngăn xếp.

---

## 3. `Queue.hpp` (Hàng đợi)
Cài đặt cấu trúc hàng đợi `Queue<T>` theo cơ chế vào trước ra trước (FIFO).

**Các phương thức cốt lõi hỗ trợ:**
* `enqueue(value)`: Thêm phần tử mới vào cuối hàng đợi.
* `dequeue()`: Loại bỏ và trả về phần tử ở đầu hàng đợi.
* `front()`: Xem giá trị phần tử ở đầu hàng đợi mà không loại bỏ.
* `empty()`: Kiểm tra hàng đợi có đang rỗng hay không.
* `size()`: Trả về số lượng phần tử trong hàng đợi.

---

## 4. `PriorityQueue.hpp` (Hàng đợi ưu tiên)
Cài đặt hàng đợi ưu tiên `PriorityQueue<T>` sử dụng cấu trúc Heap.

**Các phương thức cốt lõi hỗ trợ:**
* `insert(value)`: Thêm một phần tử mới vào hàng đợi ưu tiên.
* `extract()`: Loại bỏ và trả về phần tử có độ ưu tiên cao nhất.
* `peek()`: Xem phần tử có độ ưu tiên cao nhất mà không loại bỏ.
* `empty()`: Kiểm tra hàng đợi ưu tiên có đang rỗng hay không.
* `size()`: Trả về số lượng phần tử trong hàng đợi ưu tiên.

---

## 5. `Algorithms.hpp` (Các thuật toán sắp xếp và tìm kiếm)
Chứa các thuật toán sắp xếp và tìm kiếm thông dụng được thiết kế dưới dạng hàm mẫu (function templates).

**Các thuật toán sắp xếp:**
* `bubbleSort(arr, n)`: Sắp xếp nổi bọt.
* `selectionSort(arr, n)`: Sắp xếp chọn.
* `insertionSort(arr, n)`: Sắp xếp chèn.
* `heapSort(arr, n)`: Sắp xếp vun đống.
* `quickSort(arr, lo, hi)`: Sắp xếp nhanh.
* `mergeSort(arr, n)`: Sắp xếp trộn.
* *Lưu ý quan trọng:* Mỗi hàm sắp xếp đều hỗ trợ đối số bộ so sánh `Comp cmp = std::less<T>()` để cho phép sắp xếp theo bất kỳ tiêu chí so sánh nào (như tăng dần, giảm dần).

**Các thuật toán tìm kiếm:**
* `linearSearch(arr, n, key)`: Tìm kiếm tuyến tính trên mảng, trả về vị trí hoặc `-1` nếu không thấy.
* `binarySearch(arr, n, key)`: Tìm kiếm nhị phân trên mảng (yêu cầu mảng đầu vào phải được sắp xếp trước).

---

## 6. `BST.hpp` (Cây tìm kiếm nhị phân)
Cài đặt cây tìm kiếm nhị phân chuẩn `BST<T>`.

**Các phương thức cốt lõi hỗ trợ:**
* `insert(value)`: Chèn giá trị mới vào cây.
* `remove(value)`: Tìm và xóa giá trị khỏi cây.
* `search(value)`: Kiểm tra sự tồn tại của giá trị trên cây.
* **Duyệt cây:** Hỗ trợ duyệt cây theo 3 thứ tự: Tiền thứ tự (preorder), Trung thứ tự (inorder), Hậu thứ tự (postorder).

---

## 7. `AVL.hpp` (Cây BST tự cân bằng)
Cài đặt cây AVL `AVL<T>` thừa kế hoặc mở rộng từ cây BST.

**Đặc điểm cốt lõi:**
* Hỗ trợ đầy đủ các thao tác của BST (`insert`, `remove`, `search`).
* Có cơ chế tự động tái cân bằng thông qua các phép xoay cây (xoay đơn, xoay kép) sau mỗi thao tác chèn (`insert`) hoặc xóa (`remove`) để đảm bảo chiều cao cây luôn là tối ưu $O(\log n)$.

---

## 8. `HashTable.hpp` (Bảng băm với chuỗi AVL)
Cài đặt bảng băm `HashTable<K, V>` ánh xạ khóa `K` sang giá trị `V`.

**Đặc điểm cốt lõi:**
* Sử dụng cơ chế giải quyết đụng độ bằng **Separate Chaining** (chuỗi liên kết).
* Mỗi bucket là một cây tự cân bằng `AVL<pair<K, V>>` thay vì một danh sách liên kết đơn thông thường để tối ưu hóa thời gian tìm kiếm khi đụng độ lớn xảy ra.
* Không yêu cầu cơ chế tự động rehashing.

**Các phương thức cốt lõi hỗ trợ:**
* `insert(key, value)`: Thêm mới hoặc cập nhật giá trị tương ứng với khóa `key`.
* `remove(key)`: Xóa cặp khóa-giá trị ra khỏi bảng băm dựa trên khóa `key`.
* `find(key)`: Tìm kiếm khóa `key`, trả về con trỏ tới giá trị `V` tương ứng hoặc `nullptr` nếu không tìm thấy.
* `contains(key)`: Kiểm tra xem khóa `key` có tồn tại trong bảng băm hay không.
* `size()`: Trả về số lượng cặp khóa-giá trị hiện tại.
* `clear()`: Xóa toàn bộ dữ liệu trong bảng băm.

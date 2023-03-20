# Hotel_Management
project for my subject Database

Front-End: Glade/GTK

Back-End: C

DataBase: PostgreSQL
# Chạy DataBase 
Cài đặt PostgreSQL, sau đó dùng file query add dữ liệu vào.

Mọi thông tin về DataBase có sẵn ở trên, nếu muốn tự tạo dữ liệu mới thì có thể tham khảo. 

# Cách chạy chương trình
Bạn chỉ cần sử dụng 3 file Hotel.c , Hotel.glade, CMakeLists.txt 

Trước tiên, hãy install Cmake và Ninja

Tiếp theo tạo 1 thư mục đặt tên là "b"

Vào thư mục b và chạy lệnh: cmake \.. -G Ninja

Chạy lệnh: ninja

Tiếp đến, copy file hotel.glade vào trong b sau đó chạy lệnh trong b: ./hotel ./hotel.glade

# Lưu ý
Ở trên là hướng dẫn chạy trên ubuntu. Ở các hệ điều hành khác có thể không được.

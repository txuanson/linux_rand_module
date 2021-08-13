[![HCMUS](https://raw.githubusercontent.com/txuanson/linux_rand_module/main/logo.png)](https://www.hcmus.edu.vn/)
# :penguin: LINUX KERNEL MODULE RANDOM NUMBER
:sparkles: Module này sẽ tạo một `character device` để cho phép các tiến trình ở `userspace` có thể open và read các số ngẫu nhiên.
## :eyes: Tác giả:
- [Trần Xuân Sơn](https://github.com/txuanson)
## :wrench: Cài đặt:
- Đảm bảo linux đã cài đặt `make`, `gcc`
- Compile:
```sh
make
```
- Cài đặt module:
```sh
sudo insmod Random.ko
```
- Xem log:
```sh
dmesg
```
Lúc này ta thấy ở cuối dmesg xuất ra major và minor number của module.
- Chạy chương trình test:
```sh
make testRandom
./testRandom
```
Chương trình sẽ in ra một số ngẫu nhiên
- Gỡ module:
```sh
sudo rmmod Random
```
## :hammer: License
[GPL](https://choosealicense.com/licenses/gpl-3.0/)

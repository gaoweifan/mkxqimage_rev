# mkxqimage_rev
The resverse engineering result of `mkxqimage`, which is used to extrat XiaoMi Router Rom.(forked from NyaMisty/mkxqimage_rev)

## What's new (compared with NyaMisty/mkxqimage_rev)
1. add support for Redmi AX6000 (RB06), Xiaomi AX9000 (RA70) and Redmi AX6 (RA69)
2. complete function ```extract_ssh_rom_callback()``` which is used to unpack miwifi_ssh.bin
3. add public_ssh.pem which is used to verify miwifi_ssh.bin

## Compile
```
gcc mkxqimage.c -lssl -lcrypto -omkxq -w
```
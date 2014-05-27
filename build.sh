/usr/local/gcc-4.8.0-qt-4.8.4-for-mingw32/win32-gcc/bin/i586-mingw32-g++ -std=c++11 -I./include -ffreestanding -Wall -Wextra -fno-stack-protector -fleading-underscore -fshort-wchar -mno-sse -mno-sse2 -mno-mmx -c -o kernel.o helloworld.cpp
../toolchain/clang/bin/lld -flavor link /out:kernel.efi /entry:efi_main /subsystem:efi_application kernel.o
mkdir -p efiboot/efi/boot
cp kernel.efi efiboot/efi/boot/bootia32.efi

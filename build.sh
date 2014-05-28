CC_CMD="../toolchain/clang/bin/clang++ -target i686-pc--coff -I./include -I/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.9.sdk/usr/include -std=c++11 -ffreestanding -fno-stack-protector -fpic -fshort-wchar -fno-exceptions -Wall -Wextra -Wno-unused-private-field -Wno-ignored-attributes -mno-sse -mno-sse2 -mno-mmx -c"
$CC_CMD -o kernel.o helloworld.cpp || exit 1
$CC_CMD -o constants.o constants.cpp || exit 1
mkdir -p efiboot/efi/boot
../toolchain/clang/bin/lld -flavor link /out:efiboot/kernel.efi /entry:efi_main /subsystem:efi_application kernel.o constants.o
cp efiboot/kernel.efi efiboot/efi/boot/bootia32.efi

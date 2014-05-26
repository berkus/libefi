I have come up with a third way, using a gcc cross-compiler. Unfortunately there is no gcc target along the lines of x86_64-efi etc, so you need to choose something close. The most appropriate one I have found is i686-pc-mingw32-gcc for 32-bit and x86_64-w64-mingw32-gcc for 64-bit. These are available as cygwin packages and should also be available pre-built on most linux distributions. You should use the header files from gnu-efi and invoke as:
```
x86_64-w64-mingw32-gcc -Ignu-efi/inc -Ignu-efi/inc/x86_64 -Ignu-efi/inc/protocol -ffreestanding -Wall -Wextra <options to disable mmx/sse etc> -c -o kernel.o kernel.c
x86_64-w64-mingw32-gcc -nostdlib -Wl,-dll -shared -Wl,--subsystem,10 -e efi_main -o kernel.efi kernel.o
```
where 'gnu-efi' is the path the gnu-efi distribution. The main drawbacks of this method are that the 32-bit version appends leading underscores to names (you need to tell the compiler not to do this), and that it defines the preprocessor macro 'WINDOWS' which causes some 3rd party packages to expect windows.h to be available, when it clearly isn't in a freestanding boot environment. For an example of this method, see the code and Makefiles here[1].

jnc100

  [1]: http://www.tysos.org/redmine/projects/tysos/repository/show/branches/tysila3/tload/tloadefi


## With mingw toolchain

/usr/local/gcc-4.8.0-qt-4.8.4-for-mingw32/win32-gcc/bin/i586-mingw32-gcc -I../gnu-efi/inc -I../gnu-efi/inc/ia32 -I../gnu-efi/inc/protocol -ffreestanding -Wall -Wextra -fno-stack-protector -fno-leading-underscore -fshort-wchar -mno-sse -mno-sse2 -mno-mmx -c -o kernel.o kernel.c

/usr/local/gcc-4.8.0-qt-4.8.4-for-mingw32/win32-gcc/bin/i586-mingw32-gcc -nostdlib -Wl,-dll -shared -Wl,--subsystem,10 -e efi_main -o kernel.efi kernel.o

## With clang toolchain

../toolchain/clang/bin/clang -target i386-pc--coff -I../gnu-efi/inc -I../gnu-efi/inc/ia32 -Ignu-efi/inc/protocol -I/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.9.sdk/usr/include -ffreestanding -fno-stack-protector -fpic -fshort-wchar -Wall -Wextra -mno-sse -mno-sse2 -mno-mmx -c -o kernel.o kernel.c

../toolchain/clang/bin/lld -flavor link /out:kernel.efi /entry:efi_main /subsystem:efi_application kernel.o

## Working sample

Compile with mingw, link with lld:

/usr/local/gcc-4.8.0-qt-4.8.4-for-mingw32/win32-gcc/bin/i586-mingw32-gcc -I../gnu-efi/inc -I../gnu-efi/inc/ia32 -I../gnu-efi/inc/protocol -ffreestanding -Wall -Wextra -fno-stack-protector -fleading-underscore -fshort-wchar -mno-sse -mno-sse2 -mno-mmx -c -o kernel.o kernel.c

../toolchain/clang/bin/lld -flavor link /out:kernel.efi /entry:efi_main /subsystem:efi_application kernel.o

mv kernel.efi efiboot/

qemu-system-i386 -bios bios.bin -hdb fat:efiboot

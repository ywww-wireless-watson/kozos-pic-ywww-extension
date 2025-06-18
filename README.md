# KOZOS-PIC

---
## KOZOS-PIC ywww extension
Changes from the original KOZOS-PIC are summarized below:
- Major refactoring of the timer module and introduction of 32-bit timer functionality (`timer.c`/`timer.h`/`tmrdrv.c`/`tmrdrv.h`/`main.c`/`periodic.c`/`kozuser.h`, etc.)
- Addition of periodic task and latency measurement/visualization features (`periodic.c`/`kozuser.h`/`command.c`/`main.c`)
- Added `periodic`, `benchmark`, and `reset` commands to the command line (`command.c`)
- Added 1024-byte region to memory pool (`memory.c`)
- Redesign of interrupt vectors and handlers (`intr.h`/`vector.s`/`startup.S`/`Makefile`, etc.)
- High-precision time measurement and latency evaluation using 32-bit timer (`timer.c`/`tmrdrv.c`/`kozuser.h`/`periodic.c`)
- UART initialization and interrupt handling improvements (`serial.c`/`hardware.c`/`io.h`)
- Makefile Docker build support, auto dependency generation, and enhanced clean process (`Makefile`)
- Removed ELF loader, now supports only ihex (`bootload/elf.c`/`elf.h` removed, `main.c`/`Makefile` updated)
- Expanded command buffer size and buffer overflow protection (`consdrv.c`)
- Refactored and added memory dump and number conversion functions (`lib.c`/`lib.h`)
- Various bug fixes and stability improvements (enhanced error handling, robust interrupt/thread management, etc.)
- Adjustments to initialization, stack area, and linker script (`ld.scr`/`startup.S`/`main.c`)
- Added/updated license notice (`LICENSE`)

This project uses container image `ghcr.io/envzhu/kozos-pic-ongdb/build:latest` published by `envzhu` on GitHub Container Registry.

Original KOZOS-PIC README.md follows:
---

※日本語は下の方にあります。  

## Overview
This is a Embeded OS for PIC32MX270F256B based on Japanese "[KOZOS](http://kozos.jp/kozos/)" which is developped for learning and hobby.  
This repositry consists of that bootloader and OS.
This OS is developped with full scratch programing and bare metal programing.

### Bootloader
`KZLOAD`(kozos boot loader) loads firmware on RAM memory by `XMODEM` transfer and execute that.  
The loaded file must be hex file format.

### OS
`KOZOS` has functions like below.  
- Memory Management
- Thread Managemnt
	- Multi Task
	- Message Box
	
And has drivers like below.
- Serial Driver
- Timer Driver

## Demo
Displayed charactors on the led panel can be changed from PC by contacting with serial.  
I will upload this source code after several weeks.


## Requirement
### Software
- `mips-elf-gcc`  
It may be possible to use `XC32-compiler` and `pinguino-compiler`.I don't have checked yet.  
I installed with this command.  
```$ ../gcc-7.2.0/configure --prefix=(your install directry) --target=mips-elf --disable-nls --disable-threads --disable-shared --enable-languages=c --disable-bootstrap --disable-libssp```  
```$ make -j4```  
```$ make install```    

- `binutils`  
The original `mips-elf-objdump` has a bug which disable to convert elf files to other format files.  
Please use [this one](https://github.com/PinguinoIDE/pinguino-compilers) fixed the bug.  
You can find `p32-objdump` in pinguino-compilers/(Your OS version)/p32/bin/ directory.  
And rename to `mips-elf-gcc` or change Makefile.

- Terminal Emulator  
	For example, `Tera Term` and `minicom`.  
	- Setting

|||
|:-------|:-------|
| Baud Rate | 9600 bps |
| Data Length | 8bit |
| Stop bit Length | 1bit |
| parity | none |
| Hardware Flow Contorl | none |
| Software Flow Control | none |

### Hardware
- Target Board  
![ブレッドボード図](PIC32_kozos_breadbord.png)
KOZOS-PIC has been tested on PIC32MX270F256B.  
So you need such the above board or the compatible board.  
  
- Serial conversion cable  
 Of course, if your computer has a serial port, you don't need it.

## Build
To build this OS, please run the following command.  
```$ cd bootload ```  
```$ make ```  
```$ cd ../ ```   
```$ cd os ```   
```$ make ```  

## Usage
In your serial console,  please run the following command.  

1. ``` kzload> load```  

2. Send `os/kozos.hex`  by xmodem transfer.   
  
3. ``` kzload> run```  

## License
KOZOS-PIC is released under `MIT license`.

## Contact
If you are find something wrong with this OS or my English, please tell me because I am beginner at programing and English.Of course, I welcome your quetions,too.  
Twitter : @envzhu

## 開発中
　[ここ](https://github.com/envzhu/kozosbook-pic)で学習用組込みOS「[KOZOS](http://kozos.jp/kozos/)」をPIC(MIPS)に移植して、それを発展させたものです。
ターゲットは`PIC32MX270F256B`を想定しています。

## ブレッドボード図
![ブレッドボード図](PIC32_kozos_breadbord.png)
※気が向いたら見やすいように書き直します

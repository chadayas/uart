# STM32F446RE UART Bootloader

A bare metal UART bootloader for the STM32F446RE Nucleo board written in C++ with no HAL or RTOS dependencies. Receives a firmware binary from a Python host over USART2 and writes it to flash, then jumps to the application.

---

## Demo


---

## How It Works

### Boot Sequence

```
Power on / reset
      ↓
Reset_Handler (startup.s)
      ↓
main()
  ├── open_USART_config()   configure GPIOA AF, BRR, enable USART2
  ├── flash_init()          unlock flash CR register
  ├── start_transmission()  handshake with host, receive and write binary
  └── app_jump()            set SP, branch to application at 0x08008000
```

### Protocol

```
MCU sends 0x7F repeatedly until host responds with 0x79  (handshake)
Host sends 4-byte firmware length (little endian)
MCU erases sectors 2-7
MCU sends 0x79                                           (erase ACK)
Host sends binary in 32-byte chunks
MCU sends 0x79 after each chunk                          (chunk ACK)
MCU sends "MCU: write complete\n"
MCU jumps to 0x08008004
```

### Flash Layout

```
0x08000000  bootloader (sector 0-1, 32KB)
0x08008000  application (sectors 2-7, start of user flash)
```

---

## Register Abstraction

Peripheral registers are accessed through C++ structs with compile-time offset verification via `static_assert` + `offsetof`. No raw `#define` macros.

```cpp
namespace USART2 {
    constexpr uint32_t BASE { 0x40004400 };

    struct Register {
        volatile uint32_t STATUS_REG;    // 0x00
        volatile uint32_t DATA_REG;      // 0x04
        volatile uint32_t BAUDRATE_REG;  // 0x08
        volatile uint32_t CTRL_REG;      // 0x0C
    };
}

inline USART2::Register* usart2() {
    return reinterpret_cast<USART2::Register*>(USART2::BASE);
}
```

Peripherals covered: `RCC`, `GPIOA`, `USART2`, `FLASH`, `AIRCR`.

---

## Vector Table

The startup assembly uses `.org` to place only the handlers needed — no zero-padding for unused slots:

```asm
.section .isr_vector, "a", %progbits
.word _estack
.word Reset_Handler
.org 0x3C              // SysTick  (slot 15)
.word SysTick_handler
.org 0x98              // USART2   (slot 54, IRQ38)
.word Usart2IQR_handler
```

---

## MCU Reset

Software reset is triggered via the AIRCR register (Application Interrupt and Reset Control Register):

```cpp
aircr()->BASE = AIRCR::Enable::SYS_RESET;  // SYSRESETREQ + VECTKEY
AIRCR::Run::_DSB();                         // data sync barrier
```

The USART2 IRQ handler checks for a `0x80` reset byte from the host and triggers this reset, allowing reflashing while the application is running.

---

## Baud Rate

Running at **460800** baud using the STM32F446RE's 16MHz HSI clock.

BRR value derived from: `USARTDIV = f_ck / (8 x (2 - OVER8) x baud)`

```cpp
constexpr uint32_t BAUD_RATE = (2U << 4U) | 3U;  // mantissa=2, fraction=3
```

---

## Build

Requires `arm-none-eabi` toolchain.

```bash
make          # release build (-O1)
make debug    # debug build (-O0 -g)
make flash    # flash bootloader via st-flash
make clean
```

---

## Flash Firmware

```bash
python3 py/flash.py <port> <binary>

# example
python3 py/flash.py /dev/ttyACM0 blink.bin
```

---

## Hardware

- **Board**: STM32F446RE Nucleo-64
- **UART pins**: PA2 (TX), PA3 (RX) — AF7
- **Application start**: `0x08008000`
- **Toolchain**: `arm-none-eabi-g++`, `arm-none-eabi-ld`, `st-flash`
- **Debug**: OpenOCD + GDB (`arm-none-eabi-gdb`)

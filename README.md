# STM32-Digital-Clock-Keypad-LCD
A real-time digital clock project using STM32F030C8T6 and MCP7940M RTC module.  Displays hour, minute,  seconds,weekday, date, month and year on 16x2 LCD and supports time editing through 4x4 matrix keypad.
| Feature                           | Status    |
| --------------------------------- | --------- |
| Real-time display (HH:MM:SS)      | ✔ Working |
| Date display (DD/MM/YY & Weekday) | ✔ Working |
| Time editing through 4×4 keypad   | ✔ Working |
| BCD conversion & validation       | ✔ Working |
| Fully modular HAL-based firmware  | ✔ Working |

| Key     | Action                                            |
| ------- | ------------------------------------------------- |
| `#`     | Enter Edit Mode                                   |
| `D`     | Save & Exit Edit Mode                             |
| `0-9`   | Enter values                                      |
Users can update seconds → minutes → hours → weekday → date → month → year sequentially

| Component | Details                                               |
| --------- | ----------------------------------------------------- |
| MCU       | STM32F030C8T6                                         |
| RTC       | MCP7940M (I2C)                                        |
| Display   | 16×2 LCD (HD44780 controller, 8-bit mode)             |
| Input     | 4×4 Matrix Keypad                                     |
| Power     | 3.3V                                                  |
| PCB       | Custom PCB designed (instead of breadboard prototype) |

| Module     | Interface    | MCU Pins          |
| ---------- | ------------ | ----------------- |
| MCP7940M   | I2C          | SDA, SCL          |
| LCD 16×2   | 8-bit + Ctrl | D0–D7, RS, RW, EN |
| Keypad 4×4 | GPIO         | R1–R4, C1–C4      |
RTC uses BCD format, firmware handles all conversions internally

**Firmware Architecture**
│── Core/
│   ├── Inc/
│   │     ├── main.h
│   │     ├── LCD_Routine.h
│   │     ├── RTC_Routine.h
│   │     ├── keypad_routine.h
│   │     ├── bcd_routine.h
│   │     └── stm32f0xx_hal_conf.h
│   │
│   ├── Src/
│   │     ├── main.c
│   │     ├── LCD_Routine.c
│   │     ├── RTC_Routine.c
│   │     ├── keypad_routine.c
│   │     ├── bcd_routine.c
│   │     └── stm32f0xx_hal_msp.c
│
│── Drivers/
│── STM32F030C8TX.ioc
│── hardware/
└── README.md

**Tools Used**
STM32CubeIDE
STM32 HAL libraries
I2C protocol
HD44780 LCD driver
Keypad scanning algorithm
BCD to decimal conversion

# SAM-EXP MCU Source
This is code for SAM-EXP-MCU/ATSAMV71-based boards

## Summary
- Target: ATSAMV71Q21B (SAMV71)
- OS: FreeRTOS
- Support: UART CLI, USB Network CLI, some library included
- Contents: boot manager, drivers, middleware, utilities and tests (see `src/`)

## How to test
- Open UART console: 115200 8N1 and check boot logs
- Check USB enumeration and basic peripherals
- Run ncat to [192.168.6.8:2323] if board network is configured

## Upcoming
![alt text](docs/image.png)
- Add more driver
- XIP/Bootloader

## Changelog

### 07/01/2026
```
-> Sum: Release version for test
[Add] initial test release
```
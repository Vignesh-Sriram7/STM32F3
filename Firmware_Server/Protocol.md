STM32 Firmware Update Protocol (via UART)
Overview

This protocol defines the communication between an ESP32 and an STM32F303RE microcontroller for wireless firmware updates. The ESP32 downloads the firmware binary over Wi-Fi and sends it to the STM32 via UART using a simple custom protocol.

UART Configuration

| Parameter     | Value       |
|---------------|-------------|
| Baud Rate     | 115200      |
| Parity        | None        |
| Stop Bits     | 1           |
| Flow Control  | None        |
| Data Bits     | 8           |

---

Packet Format

Each firmware chunk is transmitted in a structured packet:


| Field  | Size (bytes) | Description                                  |
|--------|--------------|----------------------------------------------|
| START  | 1 (0x7E)      | Start of packet marker                      |
| SEQ    | 1            | Sequence number of the packet                |
| LEN    | 1            | Number of bytes in DATA                      |
| DATA   | N (≤ 256)    | Firmware binary chunk                        |
| CRC    | 1            | XOR or CRC8 of DATA                          |
| END    | 1 (0x7F)      | End of packet marker                        |



Acknowledgment System

- STM32 sends:
  - `ACK (0x06)` if CRC is valid.
  - `NACK (0x15)` if CRC fails — ESP32 resends that chunk.



Control Commands

| Command     | Byte | Purpose                        |
|-------------|------|--------------------------------|
| INIT        | 0x01 | Start transmission             |
| END         | 0x04 | End of transmission            |
| RESET       | 0x05 | Reboot STM32 into new firmware |

---

CRC / Checksum

- XOR all bytes in DATA to produce a single-byte checksum.
- Optional: Replace with CRC8/16 if higher robustness is needed.

---

Bootloader Requirements

- Must parse incoming packets.
- Handle flash writes for each chunk.
- Validate firmware checksum at the end.
- Jump to application address if all passes.

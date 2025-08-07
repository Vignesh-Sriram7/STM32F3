| Property            | Value                       |
| ------------------- | --------------------------- |
| **Total Flash**     | 512 KB                      |
| **Flash Page Size** | 2 KB (2048 bytes)           |
| **Pages**           | 256 pages (0 to 255)        |
| **Flash Range**     | `0x08000000` - `0x0807FFFF` |


| Region          | Pages | Start Address | Size     | Description                                      |
| --------------- | ----- | ------------- | -------- | ------------------------------------------------ |
| **Bootloader**  | 0–7   | `0x08000000`  | 16 KB    | Custom bootloader code                           |
| **Application** | 8–239 | `0x08004000`  | \~464 KB | Main application firmware (upgradable over UART) |
| **Reserved**    | 255   | `0x0807F800`  | 2 KB     | Update flags or persistent metadata              |

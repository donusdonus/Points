# 📦 TagStatic — Tag Management Library

TagStatic is a **C++11 embedded-friendly library** for managing data in a **Tag / Topic** structure.  
It is designed for basic type and supports selecting memory sources between **RAM** and **PSRAM**.  
The library is inspired by industrial tag systems such as **OPC**, **PLC Tag**, **BACnet**, and **MQTT**.

---

## 1. Usage Context

- Designed for **Embedded Systems, PLC, SCADA, BMS, and IoT** applications.
- Suitable for storing **sensor readings**, **actuator states**, **setpoints**, and **control variables**.
- Organizes data into **Topics** for logical grouping and easier retrieval.
- Allows dynamic **Add / Remove / Search** operations with proper memory cleanup.
- Runs on **MCUs (ESP32, ARM, RISC-V)** as well as **PC environments** for development and testing.

---

## 2. Supported Data Types

| Enum (isType) | Name     | Size (Bytes) | Example Value |
|---------------|----------|--------------|---------------|
| VAR_CHAR      | CHAR     | 1            | 'A'           |
| VAR_INT8      | INT8     | 1            | -10           |
| VAR_UINT8     | UINT8    | 1            | 255           |
| VAR_INT16     | INT16    | 2            | -32000        |
| VAR_UINT16    | UINT16   | 2            | 65535         |
| VAR_INT32     | INT32    | 4            | -2000000      |
| VAR_UINT32    | UINT32   | 4            | 4000000000    |
| VAR_FLOAT     | FLOAT    | 4            | 3.141         |
| VAR_DOUBLE    | DOUBLE   | 8            | 3.1415926535  |

---

## 3. Root Structure

```plaintext
TopicGroup
 ├── Topic
 │    ├── TagGroup
 │    │     ├── Tag
 │    │     │    ├── Name (RawMemory)
 │    │     │    ├── Data (RawMemory)
 │    │     │    ├── Type (isType)
 │    │     │    ├── MemoryType (isMemory)
 │    │     │    └── Linked-list to next Tag
 │    │     └── (Multiple Tags per TagGroup)
 │    └── Linked-list to next Topic
 └── (Multiple Topics per TopicGroup)

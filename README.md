# 📦 TagStatic — Tag Management Library

TagStatic is a **C++11 embedded-friendly library** for managing data in a **Tag / Topic** structure.  
It is designed for basic data types and supports selecting memory sources between **RAM** and **PSRAM**.  
The library is inspired by industrial tag systems such as **OPC**, **PLC Tag**, **BACnet**, and **MQTT**.

---

## 📖 Table of Contents

1. [Overview](#overview)  
2. [Features](#features)  
3. [Supported Data Types](#supported-data-types)  
4. [Architecture](#architecture)  
5. [Example Usage](#example-usage)  
6. [Example Output](#example-output)  
7. [License](#license)  
8. [Future Plans](#future-plans)  

---

## 1. Overview

- Designed for **Embedded Systems, PLC, SCADA, BMS, and IoT** applications.
- Suitable for storing **sensor readings**, **actuator states**, **setpoints**, and **control variables**.
- Organizes data into **Topics** for logical grouping and easier retrieval.
- Allows dynamic **Add / Remove / Search** operations with proper memory cleanup.
- Runs on **MCUs (ESP32, ARM, RISC-V)** as well as **PC environments** for development and testing.

---

## 2. Features

- **Dynamic Tag Management** — Create, edit, and delete tags at runtime.
- **Multiple Memory Sources** — Select between **RAM** and **PSRAM** for tag storage.
- **Type Safety** — Supports strongly-typed values with size introspection.
- **Linked List Structure** — Efficient memory usage and scalable hierarchy.
- **Embedded-Friendly** — Works without STL, low memory footprint.
- **Protocol-Ready** — Suitable for integration with **OPC**, **MQTT**, **BACnet**, and more.

---

## 3. Supported Data Types

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

## 4. Architecture

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
```

---

## 5. Example Usage

### 5.1 Example Structure

<details open>
<summary>📁 MyController</summary>

- 📁 ROOM_DATA_CENTER  
  - 📄 Temperature[0]  
  - 📄 Temperature[1]  
  - 📄 Humidity[0]  
  - 📄 Humidity[1]  
  - 📄 Temperature2[0]  
  - 📄 Humidity2[1]  

- 📁 ROOM_AHU1  
  - 📄 Temperature_Supply[1]  
  - 📄 Temperature_Return[1]  

- 📁 Chiller1  
  - 📄 Evap_Inlet_Temperature[1]  
  - 📄 Evap_Outet_Temperature[1]  
</details>

---

### 5.2 Example Code

```cpp
#include <cstdio>
#include <cstdint>
#include <cstring>
#include "TagStatic.h"

TopicGroup BMS;

int main()
{
    auto *ROOM_DATA_CENTER = BMS.Add("ROOM_DATA_CENTER");
    ROOM_DATA_CENTER->Items()->Add(isType::VAR_FLOAT, "Temperature", 2);
    ROOM_DATA_CENTER->Items()->Add(isType::VAR_FLOAT, "Humidity", 2);
    ROOM_DATA_CENTER->Items()->Add(isType::VAR_FLOAT, "Temperature2");
    ROOM_DATA_CENTER->Items()->Add(isType::VAR_FLOAT, "Humidity2");

    auto *ROOM_AHU1 = BMS.Add("ROOM_AHU1");
    ROOM_AHU1->Items()->Add(isType::VAR_FLOAT, "Temperature_Supply");
    ROOM_AHU1->Items()->Add(isType::VAR_FLOAT, "Temperature_Return");

    auto *Chiller = BMS.Add("Chiller1");
    Chiller->Items()->Add(isType::VAR_INT8, "Evap_Inlet_Temperature");
    Chiller->Items()->Add(isType::VAR_INT8, "Evap_Outet_Temperature");

    // Set values
    BMS.Find(1)->Items()->FindByIndex(0)->Set<float>(25.3, 0);
    BMS.Find(1)->Items()->FindByIndex(1)->Set<float>(55.4);

    // Print all topics and tags
    for (int n = 0; n < BMS.Count(); n++) {
        printf("%s", BMS.Find(n)->MonitorInfo());
        for (int o = 0; o < BMS.Find(n)->Items()->Count(); o++) {
            printf("%s", BMS.Find(n)->Items()->FindByIndex(o)->MonitorInfo());
            for (int p = 0; p < BMS.Find(n)->Items()->FindByIndex(o)->GetArraySize(); p++)
                printf("%s", BMS.Find(n)->Items()->FindByIndex(o)->MonitorValue(p));
        }
    }
    return 0;
}
```

---

## 6. Example Output

```plaintext
TOPIC ROOM_DATA_CENTER
  float Temperature[2]
    Temperature[0] = 0.000
    Temperature[1] = 0.000
  float Humidity[2]
    Humidity[0] = 0.000
    Humidity[1] = 0.000
  float Temperature2[1]
    Temperature2[0] = 0.000
  float Humidity2[1]
    Humidity2[0] = 0.000
TOPIC ROOM_AHU1
  float Temperature_Supply[1]
    Temperature_Supply[0] = 25.300
  float Temperature_Return[1]
    Temperature_Return[0] = 55.400
TOPIC Chiller1
  int8_t Evap_Inlet_Temperature[1]
    Evap_Inlet_Temperature[0] = 0
  int8_t Evap_Outet_Temperature[1]
    Evap_Outet_Temperature[0] = 0
```

---

## 7. License

MIT License — You are free to use, modify, and distribute this library with attribution.

---

## 8. Future Plans
- Working with **Data Memory** from source allocate.
- Support **protocol binding** (OPC UA, MQTT, BACnet).
- Implement **thread-safe operations FreeRTOS**.


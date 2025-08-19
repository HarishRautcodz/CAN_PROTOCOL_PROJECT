# 🛠️ Real-Time Vehicle Monitoring System using CAN

This project presents an **embedded vehicle communication system** built using the **CAN (Controller Area Network) protocol**.  
The architecture involves **three specialized microcontroller-based nodes**, each dedicated to handling **vehicle motion sensing, fuel monitoring, and turn signaling**. These nodes operate collaboratively over a **real-time CAN network**.

---

## 🔍 Project Overview

Utilizing the **LPC2129 ARM7 microcontroller**, the system demonstrates an efficient way to manage multiple peripherals distributed across different nodes.  
The CAN bus enables **robust and synchronized communication**, ensuring reliable data transfer between all modules in real-time.

---

## 🧩 Node Descriptions

### 🧠 Node A: Control & Display Unit
- **Peripherals Connected:**
  - 3-Axis Accelerometer (MMA7660 via I²C)
  - 20x4 Alphanumeric LCD
- **Functions:**
  - Detects directional tilt using accelerometer input  
  - Determines **left or right turn movement** (mimics steering)  
  - Sends directional commands to the Indicator Node via CAN  
  - Displays live **fuel percentage and turning status** on the LCD  
  - Acts as the **central decision and display unit**

---

### 🔁 Node B: Indicator Control Module
- **Peripherals Connected:**
  - 7 Active-Low LEDs configured in a linear array
- **Functions:**
  - Listens for turn instructions from the Control Node  
  - Activates animated LED sequences for turn indication:  
    - **Left Turn:** Lights shift from **left to right**  
    - **Right Turn:** Lights shift from **right to left**

---

### ⛽ Node C: Fuel Sensing Module
- **Peripherals Connected:**
  - Analog fuel level sensor (connected via ADC)
- **Functions:**
  - Continuously reads fuel level through ADC  
  - Converts analog values into fuel percentage  
  - Broadcasts fuel data to the Control Node over CAN bus  
  - Keeps the system updated with **live fuel status**

---

## 🔗 Communication & Interfaces

- **CAN Bus (Standard 2.0A):** Primary communication backbone among all nodes  
- **I²C Bus:** Used to fetch motion data from the accelerometer in Node A  
- **ADC Input:** For analog fuel level detection in Node C  

---

## ✅ Key Features

- 🔄 Real-time detection of motion using accelerometer  
- 📺 Live display of fuel and turn status on LCD  
- 🚨 Directional LED animation for turn indicators  
- 🧠 Decentralized control through distributed node architecture  
- ⚙️ Hardware-level implementation using real sensors on LPC2129 MCU

---

## 🧰 Hardware & Tools

| Component          | Description                                |
|--------------------|--------------------------------------------|
| **MCU**            | LPC2129 (ARM7-based architecture)          |
| **Accelerometer**  | MMA7660, I²C interface                     |
| **Display**        | 20x4 LCD (connected to Node A)             |
| **Indicators**     | 7 Active-Low LEDs (controlled in Node B)   |
| **Fuel Sensor**    | Analog input, read via ADC (Node C)        |
| **Protocols**      | CAN 2.0A, I²C, ADC                          |
| **Software Tools** | Keil µVision, Flash Magic, Proteus         |

---

## 🌍 Potential Applications

- Smart vehicle dashboards displaying dynamic info  
- CAN-based node communication learning modules  
- Automotive control unit simulations and prototypes  
- Embedded sensor-actuator communication projects

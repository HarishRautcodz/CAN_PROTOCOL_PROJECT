# CAN_PROTOCOL_PROJECT
# 🚗 Real-Time Vehicle Monitoring System using CAN

This project demonstrates a **practical implementation of a distributed embedded system** using the **CAN protocol**.  
The system consists of **three interconnected Nodes**, each responsible for a specific task such as **motion detection, turn indication, and fuel monitoring**.  
All nodes communicate through the **CAN bus** in real-time.

---

## 📖 System Overview
The project is built on the **LPC2129 (ARM7 MCU)** where multiple peripherals (sensors, display, LEDs) are distributed across three nodes.  
The CAN bus ensures reliable and synchronized communication among them.

---

## 🔑 Node Architecture

### 🖥️ Node-1: Main Controller Node
- **Connected Devices:**
  - MMA7660 3-axis Accelerometer (via I²C)
  - 16x2 LCD Display
- **Practical Role:**
  - Continuously reads tilt/motion data from the accelerometer  
  - Detects **left** or **right** motion (like simulating steering)  
  - Sends turning information to Indicator Node over CAN  
  - Receives real-time fuel level data from the Fuel Node  
  - Displays **Fuel Level + Turn Status** on the 16x2 LCD  
  - Works as the **central dashboard node**

---

### 💡 Node-2: Indicator Node
- **Connected Devices:**
  - 7 Active-Low LEDs (arranged in a line simulating vehicle indicators)
- **Practical Role:**
  - Receives turn commands (left or right) from Main Controller Node  
  - Displays dynamic LED **shifting patterns** depending on turn direction:  
    - ⬅️ **Left Turn:** LEDs light up sequentially from **left → right**  
    - ➡️ **Right Turn:** LEDs light up sequentially from **right → left**

---

### ⛽ Node-3: Fuel Monitoring Node
- **Connected Devices:**
  - Analog Fuel Gauge connected to ADC input
- **Practical Role:**
  - Continuously reads the fuel level via ADC  
  - Converts analog values into percentage format  
  - Transmits the current fuel level to the Main Node over CAN  
  - Ensures **real-time fuel data availability**

---

## 📡 Communication Protocols
- **CAN Bus (2.0A):** For real-time data exchange between all nodes  
- **I²C Protocol:** To read accelerometer values (Node-1)  
- **ADC Interface:** To acquire analog fuel gauge values (Node-3)  

---

## ⚙️ Practical Features
✔️ Real-time motion detection using accelerometer  
✔️ Centralized LCD display of **fuel % and turn status**  
✔️ Dynamic **indicator LED patterns** controlled over CAN  
✔️ Distributed multi-node communication architecture  
✔️ Hardware-tested on **LPC2129 with real sensors & peripherals**

---

## 🛠️ Hardware & Tools Used
- **Microcontroller:** LPC2129 (ARM7-based MCU)  
- **Sensor:** MMA7660 3-axis Accelerometer (I²C)  
- **Display:** 16x2 LCD (used as dashboard)  
- **Indicators:** 7 Active-Low LEDs (shift pattern controlled)  
- **Fuel Gauge:** ADC-based analog sensor  
- **Communication:** CAN Bus 2.0A  
- **Development Tools:** Keil µVision, Flash Magic, Proteus (simulation & debugging)

---

## 🚀 Real-World Applications
- Vehicle dashboards for **Fuel + Indicators**  
- CAN-based distributed embedded systems learning  
- Automotive ECU/Node prototyping and testing  
- Sensor-actuator communication projects  

# CAN_PROTOCOL_PROJECT

# 🚗 REAL TIME VEHICLE MONITORING SYSTEM USING CAN 

## 📖 Project Description

This project implements a **Controller Area Network (CAN) based communication system** with **three interconnected ECUs (Electronic Control Units / Nodes)**. Each node performs a specific task, and data is exchanged seamlessly over the CAN bus.

### 🔑 Node Architecture

1. **Main Node (ECU-1)**

   * **Connected Devices**:

     * MMA7660 3-axis Accelerometer (via **I²C**)
     * 20x4 LCD Display
   * **Functionality**:

     * Reads motion/tilt data from MMA7660.
     * Generates **interrupt signals** for **Left** and **Right turn indications**.
     * Receives fuel gauge data from Fuel Node and displays the value on the LCD.
     * Acts as the **central controller** for monitoring and display.

2. **Indicator Node (ECU-2)**

   * **Connected Devices**:

     * 7 Active-Low LEDs (for turn indicators).
   * **Functionality**:

     * Receives interrupt signals from Main Node over CAN.
     * Performs **LED shifting patterns**:

       * **Left turn** → LEDs shift from **left to right**.
       * **Right turn** → LEDs shift from **right to left**.

3. **Fuel Node (ECU-3)**

   * **Connected Devices**:

     * ADC-based Fuel Gauge.
   * **Functionality**:

     * Reads **fuel level data** using ADC.
     * Transmits the fuel gauge value to the Main Node over CAN.

---

### 📡 Communication Protocols Used

* **CAN Bus** → Data exchange between all nodes.
* **I²C Protocol** → Data acquisition from MMA7660 Accelerometer (Main Node).
* **ADC Interface** → Fuel Gauge sensor data acquisition (Fuel Node).

---

### ⚙️ Features

* ✅ Real-time accelerometer-based interrupt generation.
* ✅ CAN-based distributed control between three nodes.
* ✅ Dynamic LED shifting patterns for left/right indicators.
* ✅ Fuel level monitoring & display on LCD.
* ✅ Modular and scalable design for automotive/embedded applications.

---

### 🛠️ Hardware & Tools

* **Microcontroller**: LPC2129 (ARM7-based MCU)
* **Accelerometer**: MMA7660 (via I²C)
* **Display**: 20x4 LCD
* **Indicators**: 7 Active-Low LEDs
* **Fuel Gauge Sensor**: ADC-based input
* **Protocol**: CAN 2.0A

---

### 🚀 Applications

* Automotive embedded systems (dashboard, indicators, fuel monitoring).
* Distributed sensor-actuator networks.
* Real-time CAN communication learning project.

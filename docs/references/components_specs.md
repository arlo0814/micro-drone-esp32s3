# Hardware Subsystem Technical Specifications Reference

This document tracks the absolute maximum ratings, electrical operating thresholds, and specific pin footprints for the micro-quadcopter hardware ecosystem.

---

## 1. Microcontroller: ESP32-S3 Super Mini
*   **Form Factor:** Breakout Board (approx. 22.5mm x 18mm)
*   **Core Architecture:** Xtensa® 32-bit LX7 Dual-Core Processor
*   **Operating Clock Speed:** Up to 240 MHz
*   **Memory Allocations:** 512 KB SRAM / 8 MB Flash
*   **Logic Level Voltage:** 3.3V DC (Inputs/Outputs)
*   **Integrated Peripherals Used:** 
    *   Hardware I2C (`Wire` Peripheral mapped to GPIO 6 [SDA] and GPIO 5 [SCL])
    *   LEDC Peripheral (Hardware PWM generation across GPIO 1, 2, 3, 4)

---

## 2. Inertial Measurement Unit: MPU6050 Breakout Module
*   **Communication Protocol:** Hardware I2C (Fast Mode up to 400 kHz supported)
*   **ADC Resolution:** 16-bit analog-to-digital converters per axis channel
*   **Gyroscope Scale Window:** ±500 °/s (Configured range for micro-drone rotational dynamics)
*   **Accelerometer Scale Window:** ±4 G (Configured range to capture high-acceleration impacts)
*   **Internal Low-Pass Filtering:** Hardware register locked to a 44 Hz cutoff frequency step to eliminate structural frame resonance from coreless actuators.

---

## 3. Power Switches: SI2302 N-Channel MOSFETs
*   **Package Type:** SOT-23 Surface Mount Technology (SMT)
*   **Drain-Source Voltage ($V_{DS}$ Max):** 20 V
*   **Continuous Drain Current ($I_D$ Max):** 2.2 A to 2.8 A (Load-dependent at standard ambient temperatures)
*   **Static Drain-Source On-Resistance ($R_{DS(on)}$):** ~70 mΩ when driven at $V_{GS}$ = 4.5V
*   **Gate Threshold Voltage ($V_{GS(th)}$ Range):** 0.65 V Minimum to 1.2 V Maximum
*   **Engineering Note:** The ultra-low gate threshold voltage enables full channel saturation directly from the ESP32-S3's 3.3V GPIO pins without requiring external gate-driver ICs.

---

## 4. Actuators: 716 Brushed Coreless DC Motors
*   **Form Factor:** 7mm Diameter x 16mm Length Cylindrical Housing
*   **Operating Voltage:** 3.7V DC Nominal (Supplied via a single-cell 1S LiPo battery topology)
*   **No-Load Rotational Velocity:** ~45,000 to 50,000 RPM
*   **Current Draw Profiles:**
    *   *No-Load Current:* ~100 mA
    *   *Maximum Continuous Draw:* ~1.0 A to 1.2 A under full rotor load
    *   *Stall Current Limit:* ~1.5 A Maximum
*   **Driving Frequency:** 20 kHz ultrasonic PWM to stay clear of the audible range and smooth out current ripples through the motor inductive coils.

---

## 5. Manufacturer Original Datasheet Parameters

### A. SI2302 N-Channel MOSFET (Source: Vishay Siliconix / MCC)
These limits define the thermal limits of our low-side switching array during transient PWM events.

*   **Continuous Drain Current ($I_D$):** 
    *   $2.9\text{ A}$ maximum at an ambient temperature ($T_A$) of $25^\circ\text{C}$ (Steady State).
    *   $2.3\text{ A}$ maximum at an ambient temperature ($T_A$) of $70^\circ\text{C}$ (Steady State).
*   **Pulsed Drain Current ($I_{DM}$):** $10\text{ A}$ maximum (Pulse width limited by max junction temperature).
*   **Gate-to-Source Voltage ($V_{GS}$ Maximum Limit):** $\pm 8\text{ V}$ absolute maximum.
*   **Static Drain-Source On-Resistance ($R_{DS(on)}$ Max Thresholds):**
    *   Maximum $0.057\ \Omega$ ($57\text{ m}\Omega$) measured at $V_{GS} = 4.5\text{ V}, I_D = 3.6\text{ A}$.
    *   Maximum $0.075\ \Omega$ ($75\text{ m}\Omega$) measured at $V_{GS} = 2.5\text{ V}, I_D = 3.1\text{ A}$.
*   **Gate Threshold Voltage ($V_{GS(th)}$ Turn-on Bounds):** Minimum $0.40\text{ V}$, Maximum $0.85\text{ V}$ at $V_{DS} = V_{GS}, I_D = 250\ \mu\text{A}$.
*   **Dynamic Total Gate Charge ($Q_g$):** Typical $3.5\text{ nC}$, Maximum $5.5\text{ nC}$ (Crucial for computing our $100\ \Omega$ resistor charging time constants).

### B. MPU-6050 MotionTracking Device (Source: TDK InvenSense)
These register properties define the signal resolution limits of the data fed into our 1kHz complementary filter loop.

*   **Supply Input Voltage ($V_{DD}$):** $2.375\text{ V}$ to $3.46\text{ V}$ operating boundary.
*   **Full-Scale Range (FSR) Boundaries:**
    *   *Gyroscope:* Software selectable across $\pm 250, \pm 500, \pm 1000, \text{ and } \pm 2000^\circ/\text{s}$.
    *   *Accelerometer:* Software selectable across $\pm 2, \pm 4, \pm 8, \text{ and } \pm 16\text{ g}$.
*   **Sensitivity Scale Factors (16-bit ADC Outputs):**
    *   At configured $\pm 500^\circ/\text{s}$ Gyro range: $65.5\text{ LSB}/(^\circ/\text{s})$
    *   At configured $\pm 4\text{ g}$ Accelerometer range: $8192\text{ LSB}/\text{g}$
*   **Noise Density Characteristics:**
    *   *Gyroscope Rate Noise:* $0.005^\circ/\text{s}/\sqrt{\text{Hz}}$
    *   *Accelerometer Noise:* $400\ \mu\text{g}/\sqrt{\text{Hz}}$
*   **Clock Source Options:** Internal $8\text{MHz}$ oscillator, or can be slaved to the gyroscope axis PLL for high-precision timestamp matching across time steps.

## 6. Local Appendix Files

For exact component evaluation curves, absolute safe operating zones, and physical package dimension maps, open the following local PDF datasheets:

*   [View SI2302 N-Channel MOSFET Original Datasheet](./datasheets/SI2302_MOSFET.pdf)
*   [View MPU6050 MotionTracking IMU Original Datasheet](./datasheets/MPU6050_IMU.pdf)
*   [View ESP32-S3 Silicon Hardware Datasheet](./datasheets/ESP32S3_Chipset.pdf)
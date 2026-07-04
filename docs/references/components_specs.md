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
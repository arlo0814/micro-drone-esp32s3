# Design and Implementation of a Dual-Core FreeRTOS Attitude-Stabilized Flight Control System for an ESP32-S3 Micro-Quadcopter

## Abstract
*Draft your 150-250 word project summary here describing the development of the 100mm coreless micro-drone using discrete SI2302 N-MOSFETs, MPU6050 sensor fusion, and deterministic multitasking.*

---

## Chapter 1: Introduction
### 1.1 Background of the Project
### 1.2 Objectives
### 1.3 Scope and Delimitations

---

## Chapter 2: Theoretical Framework
### 2.1 Low-Pass Sensor Fusion Filtering
The discrete-time implementation formula for the Complementary Filter is established as:

$$\theta_{\text{actual}} = \alpha \cdot (\theta_{\text{prev}} + \omega_{\text{gyro}} \cdot \Delta t) + (1 - \alpha) \cdot \theta_{\text{accel}}$$

### 2.2 Parallel Proportional-Integral-Derivative (PID) Control Law
### 2.3 Plant Dynamics of Brushed Coreless Direct Current Motors

---

## Chapter 3: Methodology
### 3.1 Overall System Functional Block Diagram
### 3.2 Hardware Selection Rationale and Interface Maps
### 3.3 Symmetric Dual-Core Multi-Threading Task Priority Design

---

## Chapter 4: Implementation and Design Setup
### 4.1 Electrical Loop Layout and Star Grounding Schemes
### 4.2 Ultrasonic 20kHz PWM Hashing and Transient Switching Gates

---

## Chapter 5: References
* Track your academic literature sources here.

## Appendix A: Project Management Tracking
The chronological progression, resource scheduling constraints, and complete Work Breakdown Structure (WBS) tracking matrices are decoupled for modular reference. 

*   [Open Standalone Project Management Timeline Reference Worksheet](../references/project_timeline.md)
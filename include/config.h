#ifndef CONFIG_H
#define CONFIG_H

// --- HARDWARE INTERFACE PIN MAPPINGS ---
#define I2C_SDA_PIN 6  // Connected via 28 AWG/30 AWG wire
#define I2C_SCL_PIN 5  // Connected via 28 AWG/30 AWG wire

// Motor PWM Gate Outlines (Routed to 100R Gate Resistors -> SI2302 Gates)
#define MOTOR_FRONT_LEFT_PIN  1  // CW Orientation
#define MOTOR_FRONT_RIGHT_PIN 2  // CCW Orientation
#define MOTOR_REAR_LEFT_PIN   3  // CCW Orientation
#define MOTOR_REAR_RIGHT_PIN  4  // CW Orientation

// --- PWM CONTROL PARAMETERS ---
#define PWM_FREQ_HZ     20000 // 20kHz ultrasonic frequency to eliminate motor whine
#define PWM_RESOLUTION_BITS 8 // 0 to 255 scaling threshold

// --- CONTROL LAWS & FILTER GAINS ---
#define COMP_FILTER_ALPHA 0.98f // Trust gyroscope 98%, anchor with accelerometer 2%
#define MOTOR_MAX_DUTY    255.0f
#define MOTOR_MIN_DUTY    0.0f

#endif
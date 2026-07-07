#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// --- ESP-NOW WIRELESS COMMUNICATIONS ---
#define WIFI_CHANNEL        1        // Fixed channel to minimize frequency hopping latency
#define LINK_TIMEOUT_MS     200      // Safe connection drop window before forced auto-shutdown

// --- BATTERY & POWER SUBSYSTEM ---
#define BATTERY_ADC_PIN     1        // GPIO 1 connected to center of 10k/10k resistor divider
#define VOLTAGE_DIVIDER_RATIO 2.0f   // Inversion scalar to compute actual 1S voltage profile
#define VOLTAGE_CRITICAL    3.5f     // Forced cutoff minimum threshold step
#define VOLTAGE_MIN_ARM     3.7f     // Absolute arming minimum floor voltage

// --- FLIGHT CONTROLLER PERIPHERAL MAPPING ---
#define I2C_SDA_PIN         6        // Hardware I2C Data Lane pin on Super Mini
#define I2C_SCL_PIN         5        // Hardware I2C Clock Lane pin on Super Mini
#define MPU6050_I2C_ADDR    0x68     // Standard baseline device register address

// --- MOTOR ACTUATOR OUTPUT CHANNELS ---
#define MOTOR_FRONT_LEFT    2        // GPIO 2 -> Gate 1
#define MOTOR_FRONT_RIGHT   3        // GPIO 3 -> Gate 2
#define MOTOR_BACK_LEFT     4        // GPIO 4 -> Gate 3
#define MOTOR_BACK_RIGHT    7        // GPIO 7 -> Gate 4

// --- TIMING AND SIGNAL HARNESSING ---
#define PWM_FREQ            20000    // 20kHz Ultrasonic operating frequency
#define PWM_RES             8        // 8-bit dynamic resolution (Values scaled 0 - 255)

// --- Struct definition packed strictly for byte-array transmission over ESP-NOW ---
typedef struct __attribute__((__packed__)) t_drone_payload {
    int16_t throttle;   // Scaled 0 to 255 command input
    int16_t roll;       // Target absolute position orientation target (deg)
    int16_t pitch;      // Target absolute position orientation target (deg)
    int16_t yaw;        // Target rotational velocity offset tracking (deg/s)
    uint8_t arm_state;  // Hardware safety latch: 0 = Disarmed, 1 = Armed
} t_drone_payload;

#endif
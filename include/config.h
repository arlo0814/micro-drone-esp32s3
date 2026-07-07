#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// --- ESP-NOW WIRELESS COMMUNICATIONS ---
#define WIFI_CHANNEL        1        
#define LINK_TIMEOUT_MS     200      

// --- BATTERY & POWER SUBSYSTEM ---
#define BATTERY_ADC_PIN     1        // Center tap of the 10k/10k divider network
#define VOLTAGE_DIVIDER_RATIO 2.0f   // Math factor to scale voltage calculations
#define VOLTAGE_CRITICAL    3.5f     // Forced low-voltage cutoff threshold
#define VOLTAGE_MIN_ARM     3.7f     // Arming interlock protection threshold

// --- CONTROLLER PERIPHERAL MAPPING ---
#define I2C_SDA_PIN         6        
#define I2C_SCL_PIN         5        
#define MPU6050_I2C_ADDR    0x68     

// --- MOTOR ACTUATOR OUTPUT CHANNELS ---
#define MOTOR_FRONT_LEFT    2        
#define MOTOR_FRONT_RIGHT   3        
#define MOTOR_BACK_LEFT     4        
#define MOTOR_BACK_RIGHT    7        

#define PWM_FREQ            20000    
#define PWM_RES             8        

typedef struct __attribute__((__packed__)) t_drone_payload {
    int16_t throttle;   
    int16_t roll;       
    int16_t pitch;      
    int16_t yaw;        
    uint8_t arm_state;  
} t_drone_payload;

#endif
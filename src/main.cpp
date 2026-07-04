#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include "config.h"

// Hardware instances & variables
Adafruit_MPU6050 mpu;
TaskHandle_t FlightLoopHandle = NULL;
TaskHandle_t TelemetryHandle  = NULL;

// System States
float roll_angle = 0.0f, pitch_angle = 0.0f;
float target_roll = 0.0f, target_pitch = 0.0f; // Flat hover setpoints

// Core Task Prototypes
void FlightControlTask(void *pvParameters);
void TelemetryTask(void *pvParameters);

void setup() {
    Serial.begin(115200);
    
    // Initialize Hardware I2C 
    Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN, 400000); // 400kHz Fast Mode
    
    if (!mpu.begin()) {
        Serial.println("[-] IMU Initialization Critical Failure!");
        while (1) { delay(10); } // Stop system execution if MPU6050 drops out
    }
    
    // Configure MPU6050 ranges for micro-drone dynamics
    mpu.setAccelerometerRange(MPU6050_RANGE_4_G);
    mpu.setGyroRange(MPU6050_RANGE_500_DEG);
    mpu.setFilterBandwidth(MPU6050_BAND_44_HZ);

    // Pin 1kHz Flight Stability Loop to Core 1
    xTaskCreatePinnedToCore(
        FlightControlTask, "FlightLoop", 4096, NULL, 
        configMAX_PRIORITIES - 1, &FlightLoopHandle, 1
    );

    // Pin Telemetry & Housekeeping to Core 0
    xTaskCreatePinnedToCore(
        TelemetryTask, "TelemetryLoop", 4096, NULL, 
        1, &TelemetryHandle, 0
    );
}

void loop() {
    // Left empty: FreeRTOS scheduling engine manages threads automatically
    vTaskDelete(NULL);
}

// --- CORE 1: DETERMINISTIC ATTITUDE CONTROL LOOP ---
void FlightControlTask(void *pvParameters) {
    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xFrequency = pdMS_TO_TICKS(1); // Force exact 1ms cycle execution

    sensors_event_t a, g, temp;
    float dt = 0.001f; // 1ms standard time slice

    for (;;) {
        mpu.getEvent(&a, &g, &temp);

        // Calculate accelerometer tilt vectors
        float acc_pitch = atan2(-a.acceleration.x, sqrt(a.acceleration.y * a.acceleration.y + a.acceleration.z * a.acceleration.z)) * RAD_TO_DEG;
        float acc_roll  = atan2(a.acceleration.y, a.acceleration.z) * RAD_TO_DEG;

        // Complementary Filter Sensor Fusion Equation
        pitch_angle = COMP_FILTER_ALPHA * (pitch_angle + (g.gyro.y * RAD_TO_DEG) * dt) + (1.0f - COMP_FILTER_ALPHA) * acc_pitch;
        roll_angle  = COMP_FILTER_ALPHA * (roll_angle + (g.gyro.x * RAD_TO_DEG) * dt) + (1.0f - COMP_FILTER_ALPHA) * acc_roll;

        // Future PID & Mixer calculations will be appended directly below here...

        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
}

// --- CORE 0: NON-DETERMINISTIC TELEMETRY LOOP ---
void TelemetryTask(void *pvParameters) {
    for (;;) {
        // Output clean attitude data to your serial plotter
        Serial.printf("Roll:%.2f,Pitch:%.2f\n", roll_angle, pitch_angle);
        vTaskDelay(pdMS_TO_TICKS(20)); // Run at 50Hz refresh rate
    }
}
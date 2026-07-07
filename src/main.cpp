#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>
#include <Wire.h>
#include "config.h"

// --- Global Cross-Core Task Boundary Memory ---
volatile t_drone_payload pilot_data = {0, 0, 0, 0, 0};
volatile float global_battery_voltage = 4.2f;
unsigned long last_packet_time = 0;

// --- FreeRTOS Multitasking Handles ---
TaskHandle_t FlightLoopHandle = NULL;
TaskHandle_t TelemetryLoopHandle = NULL;

// --- Core 0: Asynchronous Wireless Data Packet Capture Callback ---
void onDataReceive(const uint8_t * mac_addr, const uint8_t *incomingData, int len) {
    if (len == sizeof(t_drone_payload)) {
        memcpy((void*)&pilot_data, incomingData, sizeof(t_drone_payload));
        last_packet_time = millis(); // Refresh link timestamp window
    }
}

// =========================================================================
// WBS 2.1 - CORE 0 TASK: COMMUNICATIONS, BATTERY MANAGEMENT, & FAILSAFES
// =========================================================================
void TelemetryLoopTask(void * pvParameters) {
    // 1. Initialize Wi-Fi and Radio Peripheral Structures
    WiFi.mode(WIFI_MODE_STA);
    if (esp_now_init() == ESP_OK) {
        esp_now_register_recv_cb(esp_now_recv_cb_t(onDataReceive));
    }

    // 2. Configure Local Hardware Inputs
    pinMode(BATTERY_ADC_PIN, INPUT);

    for(;;) {
        // --- A. Link Connection Failsafe Guard ---
        if (pilot_data.arm_state == 1 && (millis() - last_packet_time > LINK_TIMEOUT_MS)) {
            pilot_data.throttle = 0;
            pilot_data.arm_state = 0;
            Serial.println("CRITICAL FAULT: DISCONNECTED FROM TRANSMITTER. MOTORS CUT.");
        }

        // --- B. Battery Diagnostics and Calculations ---
        float rawADC = analogRead(BATTERY_ADC_PIN);
        float pinVoltage = (rawADC / 4095.0f) * 3.3f;
        global_battery_voltage = pinVoltage * VOLTAGE_DIVIDER_RATIO;

        // --- C. Under-Voltage Protection Execution ---
        if (pilot_data.arm_state == 1 && (global_battery_voltage < VOLTAGE_CRITICAL)) {
            pilot_data.throttle = 0;
            pilot_data.arm_state = 0;
            Serial.printf("CRITICAL BATTERY: %.2fV. SHUTTING DOWN SYSTEM LAUNCH.\n", global_battery_voltage);
        }

        // --- D. Telemetry Output Loop (Feeds external Python graphics tools) ---
        // Serial.printf("V:%.2f | T:%d | A:%d\n", global_battery_voltage, pilot_data.throttle, pilot_data.arm_state);

        vTaskDelay(pdMS_TO_TICKS(20)); // Keep fixed 50Hz scheduling footprint
    }
}

// =========================================================================
// WBS 2.1 - CORE 1 TASK: DETERMINISTIC 1kHz CONTROL STABILIZATION LOOP
// =========================================================================
void FlightLoopTask(void * pvParameters) {
    // 1. Initialize Physical Hardware I2C Buses
    Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN, 400000); // 400kHz Fast I2C Bus

    // MPU6050 Power Management Wake Command Register sequence
    Wire.beginTransmission(MPU6050_I2C_ADDR);
    Wire.write(0x6B); // PWR_MGMT_1 Register Address location
    Wire.write(0x00); // Write zero byte value to wake system up from sleep status
    Wire.endTransmission();

    // 2. Map LEDC PWM Hardware Engine Driver Pins
    ledcAttach(MOTOR_FRONT_LEFT,  PWM_FREQ, PWM_RES);
    ledcAttach(MOTOR_FRONT_RIGHT, PWM_FREQ, PWM_RES);
    ledcAttach(MOTOR_BACK_LEFT,   PWM_FREQ, PWM_RES);
    ledcAttach(MOTOR_BACK_RIGHT,  PWM_FREQ, PWM_RES);

    // 3. Setup Strict Periodic Timer Boundaries
    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xFrequency = pdMS_TO_TICKS(1); // Enforce exactly 1 millisecond execution loops

    for(;;) {
        // --- STEP A: Read raw physical MPU6050 registers over I2C ---
        // Placeholder for low-level register block reads when parts arrive

        // --- STEP B: Run math models (Complementary filter orientation tracking) ---
        // Placeholder for sensor fusion implementation equations

        // --- STEP C: Closed Loop Processing (Parallel axis PID structures) ---
        // Placeholder for error computation variables

        // --- STEP D: Plant Actuation (Motor Mixing Matrix Output Routing) ---
        if (pilot_data.arm_state == 1 && pilot_data.throttle > 10) {
            // Placeholder: Replace zeros with active localized PID loop adjustments
            int16_t m_fl = pilot_data.throttle + 0 - 0 + 0; 
            int16_t m_fr = pilot_data.throttle - 0 - 0 - 0;
            int16_t m_bl = pilot_data.throttle + 0 + 0 - 0;
            int16_t m_br = pilot_data.throttle - 0 + 0 + 0;

            // Enforce safe 8-bit array constraint boundaries (0 to 255 scaling)
            ledcWrite(MOTOR_FRONT_LEFT,  constrain(m_fl, 0, 255));
            ledcWrite(MOTOR_FRONT_RIGHT, constrain(m_fr, 0, 255));
            ledcWrite(MOTOR_BACK_LEFT,   constrain(m_bl, 0, 255));
            ledcWrite(MOTOR_BACK_RIGHT,  constrain(m_br, 0, 255));
        } else {
            // Safe Mode: Force absolute zero logic state output to gates
            ledcWrite(MOTOR_FRONT_LEFT,  0);
            ledcWrite(MOTOR_FRONT_RIGHT, 0);
            ledcWrite(MOTOR_BACK_LEFT,   0);
            ledcWrite(MOTOR_BACK_RIGHT,  0);
        }

        // Delay execution block loop until exactly 1ms timestamp window passes
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
}

// =========================================================================
// SYSTEM INITIALIZATION ENTRY THREAD
// =========================================================================
void setup() {
    Serial.begin(115200);
    analogReadResolution(12); // Stabilize analog parsing space explicitly to 12-bit (0-4095)

    // Spawn Multi-threaded task processing environments
    xTaskCreatePinnedToCore(TelemetryLoopTask, "TelemetryTask", 4096, NULL, 1, &TelemetryLoopHandle, 0); // Pinned to Core 0
    xTaskCreatePinnedToCore(FlightLoopTask, "FlightLoopTask", 4096, NULL, 3, &FlightLoopHandle, 1);       // Pinned to Core 1
}

void loop() {
    // Delete main setup thread execution stack to allocate full processor bandwidth to tasks
    vTaskDelete(NULL);
}
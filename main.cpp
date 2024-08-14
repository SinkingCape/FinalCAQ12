#include "mbed.h"
#include "DevI2C.h"         
#include "LSM6DSLSensor.h"  

// Initialise I2C communication (SDA: PB11, SCL: PB10)
static DevI2C devI2c(PB_11, PB_10);

static LSM6DSLSensor sensor(&devI2c,0xD4,D4,D5); 

// PWMOut objects for controlling the brightness of LEDs
PwmOut led1(LED1);
PwmOut led2(LED2);
PwmOut led3(LED3);

int main() {
    // Initialise the LSM6DSL sensor
    sensor.init(NULL); // Initialise the sensor.

    // Enable the accelerometer
    sensor.enable_x();

    // Set the PWM period for LEDs
    led1.period(0.001f); // Set PWM period to 1ms (1kHz frequency)
    led2.period(0.001f);
    led3.period(0.001f);

    int32_t axes[3];
    
    while (true) {
        // Read the accelerometer values
        sensor.get_x_axes(axes);  // Retrieve all axes data

        // Print raw accelerometer values for checking for bugs.
        printf("Accel X: %d, Y: %d, Z: %d\n", axes[0], axes[1], axes[2]);

        // Assuming the raw values represent milli-g (mg), we'll convert them to g
        float accelX = axes[0] / 1000.0f;  // Convert to g
        float accelY = axes[1] / 1000.0f;  // Convert to g
        float accelZ = axes[2] / 1000.0f;  // Convert to g

    
        float brightness1 = (accelX + 1.0f) / 4.0f; // LED1 (X-axis)
        float brightness2 = (accelY + 1.0f) / 4.0f; // LED2 (Y-axis)
        float brightness3 = (accelZ + 1.0f) / 4.0f; // LED3 (Z-axis)

        // Brightness
        brightness1 = brightness1 > 1.0f ? 1.0f : (brightness1 < 0.0f ? 0.0f : brightness1);
        brightness2 = brightness2 > 1.0f ? 1.0f : (brightness2 < 0.0f ? 0.0f : brightness2);
        brightness3 = brightness3 > 1.0f ? 1.0f : (brightness3 < 0.0f ? 0.0f : brightness3);

        // Set the LED brightness
        led1.write(brightness1);
        led2.write(brightness2);
        led3.write(brightness3);

        // Small delay to stabilise the readings
        thread_sleep_for(500);
    }
}
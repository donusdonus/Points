#define EMBEDDED_BUILD
#include <Points.h>

// 🌡️ Smart Home Temperature Controller
// Compatible with Arduino, ESP32, ESP8266

Points homeData;

void setup() {
    Serial.begin(9600);
    Serial.println("🏠 Smart Home Controller Starting...");
    
    // 🌡️ Initialize sensor data points
    homeData.Add(Point("RoomTemp", 22.5f));
    homeData.Add(Point("TargetTemp", 24.0f));
    homeData.Add(Point("Humidity", static_cast<uint8_t>(45)));
    homeData.Add(Point("HeaterOn", static_cast<uint8_t>(0)));
    
    // 📊 Historical temperature readings (last 5 minutes)
    float tempHistory[5] = {22.1f, 22.3f, 22.0f, 22.5f, 22.4f};
    homeData.Add(Point("TempHistory", tempHistory, 5));
    
    // 🕐 Schedule data (24 hours, each bit represents an hour)
    uint32_t schedule = 0x00FF0000; // Heat from 4PM to 12AM
    homeData.Add(Point("Schedule", schedule));
    
    Serial.println("✅ Initialization complete!");
    printSystemStatus();
}

void loop() {
    // 🌡️ Read temperature sensor (simulated)
    float currentTemp = readTemperatureSensor();
    
    // 📊 Update current temperature
    Point* temp = homeData.Find("RoomTemp");
    if (temp) {
        temp->setValue(currentTemp);
    }
    
    // 🎯 Get target temperature
    Point* target = homeData.Find("TargetTemp");
    float targetTemp = target ? target->getValue<float>() : 24.0f;
    
    // 🔥 Control heater based on temperature difference
    bool shouldHeat = (currentTemp < targetTemp - 0.5f);
    Point* heater = homeData.Find("HeaterOn");
    if (heater) {
        heater->setValue(static_cast<uint8_t>(shouldHeat ? 1 : 0));
        
        // 💡 Control actual heater pin
        digitalWrite(13, shouldHeat ? HIGH : LOW);
    }
    
    // 📊 Update temperature history
    updateTemperatureHistory(currentTemp);
    
    // 📝 Print status every 10 seconds
    static unsigned long lastPrint = 0;
    if (millis() - lastPrint > 10000) {
        printSystemStatus();
        lastPrint = millis();
    }
    
    delay(1000); // Update every second
}

float readTemperatureSensor() {
    // 🌡️ Simulate temperature reading
    // In real application, read from DHT22, DS18B20, etc.
    static float temp = 22.0f;
    temp += random(-5, 6) * 0.1f; // ±0.5°C variation
    return constrain(temp, 15.0f, 30.0f);
}

void updateTemperatureHistory(float newTemp) {
    // 📊 Shift historical data and add new reading
    Point* history = homeData.Find("TempHistory");
    if (history && history->getProperty().GetCapacity() >= 5) {
        // Shift old values
        for (int i = 0; i < 4; i++) {
            float oldValue = history->getValue<float>(i + 1);
            // Note: In real implementation, you'd need a method to set by index
            // This is a simplified example
        }
        // Add new value at the end
        // history->setValue(newTemp, 4); // Set at last position
    }
}

void printSystemStatus() {
    Serial.println("\n🏠 === Smart Home Status ===");
    
    // 🌡️ Temperature info
    Point* temp = homeData.Find("RoomTemp");
    Point* target = homeData.Find("TargetTemp");
    if (temp && target) {
        Serial.print("🌡️ Temperature: ");
        Serial.print(temp->getValue<float>(), 1);
        Serial.print("°C (Target: ");
        Serial.print(target->getValue<float>(), 1);
        Serial.println("°C)");
    }
    
    // 💨 Humidity
    Point* humidity = homeData.Find("Humidity");
    if (humidity) {
        Serial.print("💨 Humidity: ");
        Serial.print(humidity->getValue<uint8_t>());
        Serial.println("%");
    }
    
    // 🔥 Heater status
    Point* heater = homeData.Find("HeaterOn");
    if (heater) {
        Serial.print("🔥 Heater: ");
        Serial.println(heater->getValue<uint8_t>() ? "ON" : "OFF");
    }
    
    // 💾 Memory usage
    size_t totalMemory = 0;
    for (size_t i = 0; i < homeData.Count(); i++) {
        Point* point = homeData.At(i);
        if (point) {
            totalMemory += point->getMemoryUsage();
        }
    }
    Serial.print("💾 Memory used: ");
    Serial.print(totalMemory);
    Serial.println(" bytes");
    
    Serial.println("========================\n");
}

// 🎛️ Optional: Handle serial commands for changing target temperature
void serialEvent() {
    if (Serial.available()) {
        String command = Serial.readStringUntil('\n');
        command.trim();
        
        if (command.startsWith("set_temp ")) {
            float newTarget = command.substring(9).toFloat();
            if (newTarget >= 15.0f && newTarget <= 30.0f) {
                Point* target = homeData.Find("TargetTemp");
                if (target) {
                    target->setValue(newTarget);
                    Serial.print("🎯 Target temperature set to: ");
                    Serial.print(newTarget, 1);
                    Serial.println("°C");
                }
            } else {
                Serial.println("❌ Invalid temperature range (15-30°C)");
            }
        } else if (command == "status") {
            printSystemStatus();
        } else if (command == "help") {
            Serial.println("🔧 Available commands:");
            Serial.println("  set_temp <value> - Set target temperature");
            Serial.println("  status           - Show current status");
            Serial.println("  help             - Show this help");
        }
    }
}

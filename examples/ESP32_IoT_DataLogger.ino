#define EMBEDDED_BUILD
#include <Points.h>
#include <WiFi.h>
#include <ArduinoJson.h>

// 🌐 ESP32 IoT Data Logger with WiFi
// Collects sensor data and sends to cloud/server

const char* ssid = "YourWiFiSSID";
const char* password = "YourWiFiPassword";
const char* serverURL = "http://your-server.com/api/data";

Points sensorNetwork;
WiFiClient client;

void setup() {
    Serial.begin(115200);
    delay(1000);
    
    Serial.println("🚀 ESP32 IoT Data Logger Starting...");
    
    // 🌐 Initialize WiFi
    initializeWiFi();
    
    // 📊 Initialize sensor data points
    initializeSensorData();
    
    // 🔧 Configure sensor pins
    pinMode(34, INPUT);  // Analog sensor pin
    pinMode(2, OUTPUT);  // LED indicator
    
    Serial.println("✅ System ready!");
}

void loop() {
    // 📊 Read all sensors
    readSensors();
    
    // 💡 Update LED indicator
    updateStatusLED();
    
    // 📤 Send data to server every 30 seconds
    static unsigned long lastUpload = 0;
    if (millis() - lastUpload > 30000) {
        sendDataToServer();
        lastUpload = millis();
    }
    
    // 📝 Print local status every 10 seconds
    static unsigned long lastPrint = 0;
    if (millis() - lastPrint > 10000) {
        printLocalStatus();
        lastPrint = millis();
    }
    
    delay(2000); // Main loop delay
}

void initializeWiFi() {
    Serial.print("🌐 Connecting to WiFi");
    WiFi.begin(ssid, password);
    
    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 20) {
        delay(500);
        Serial.print(".");
        attempts++;
    }
    
    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\n✅ WiFi connected!");
        Serial.print("📡 IP address: ");
        Serial.println(WiFi.localIP());
        Serial.print("📶 Signal strength: ");
        Serial.print(WiFi.RSSI());
        Serial.println(" dBm");
    } else {
        Serial.println("\n❌ WiFi connection failed!");
    }
}

void initializeSensorData() {
    Serial.println("📊 Initializing sensor data points...");
    
    // 🌡️ Environmental sensors
    sensorNetwork.Add(Point("Temperature", 25.0f));
    sensorNetwork.Add(Point("Humidity", static_cast<uint8_t>(50)));
    sensorNetwork.Add(Point("Pressure", 1013.25f));
    sensorNetwork.Add(Point("LightLevel", static_cast<uint16_t>(512)));
    
    // 🔋 System status
    sensorNetwork.Add(Point("BatteryLevel", static_cast<uint8_t>(100)));
    sensorNetwork.Add(Point("WiFiSignal", static_cast<int8_t>(WiFi.RSSI())));
    sensorNetwork.Add(Point("FreeHeap", static_cast<uint32_t>(ESP.getFreeHeap())));
    sensorNetwork.Add(Point("Uptime", static_cast<uint32_t>(0)));
    
    // 📊 Data arrays for trend analysis
    uint16_t tempTrend[10] = {250, 251, 249, 252, 250, 248, 251, 250, 249, 250}; // x10 for precision
    sensorNetwork.Add(Point("TempTrend", tempTrend, 10));
    
    uint8_t humidityTrend[10] = {50, 51, 49, 52, 50, 48, 51, 50, 49, 50};
    sensorNetwork.Add(Point("HumidityTrend", humidityTrend, 10));
    
    // 🏷️ Device identification
    uint8_t deviceID[6];
    WiFi.macAddress(deviceID);
    sensorNetwork.Add(Point("DeviceMAC", deviceID, 6));
    
    Serial.println("✅ Sensor data points initialized!");
}

void readSensors() {
    // 🌡️ Read temperature (simulated - replace with real sensor)
    float temperature = 25.0f + random(-50, 51) * 0.1f; // 20-30°C range
    Point* tempPoint = sensorNetwork.Find("Temperature");
    if (tempPoint) {
        tempPoint->setValue(temperature);
    }
    
    // 💨 Read humidity (simulated)
    uint8_t humidity = 50 + random(-10, 11);
    Point* humidityPoint = sensorNetwork.Find("Humidity");
    if (humidityPoint) {
        humidityPoint->setValue(humidity);
    }
    
    // 🌫️ Read pressure (simulated)
    float pressure = 1013.25f + random(-50, 51) * 0.1f;
    Point* pressurePoint = sensorNetwork.Find("Pressure");
    if (pressurePoint) {
        pressurePoint->setValue(pressure);
    }
    
    // 💡 Read light level (actual analog reading)
    uint16_t lightLevel = analogRead(34);
    Point* lightPoint = sensorNetwork.Find("LightLevel");
    if (lightPoint) {
        lightPoint->setValue(lightLevel);
    }
    
    // 📶 Update WiFi signal strength
    Point* signalPoint = sensorNetwork.Find("WiFiSignal");
    if (signalPoint) {
        signalPoint->setValue(static_cast<int8_t>(WiFi.RSSI()));
    }
    
    // 💾 Update free heap memory
    Point* heapPoint = sensorNetwork.Find("FreeHeap");
    if (heapPoint) {
        heapPoint->setValue(static_cast<uint32_t>(ESP.getFreeHeap()));
    }
    
    // ⏱️ Update uptime
    Point* uptimePoint = sensorNetwork.Find("Uptime");
    if (uptimePoint) {
        uptimePoint->setValue(static_cast<uint32_t>(millis() / 1000));
    }
    
    // 📊 Update trends (simplified - in real app, use circular buffer)
    updateTrends(temperature, humidity);
}

void updateTrends(float temperature, uint8_t humidity) {
    // 📈 Update temperature trend (shift array and add new value)
    Point* tempTrend = sensorNetwork.Find("TempTrend");
    if (tempTrend) {
        // In a real implementation, you'd implement a proper circular buffer
        // This is a simplified example
        uint16_t tempInt = static_cast<uint16_t>(temperature * 10);
        // Shift and add new value logic would go here
    }
    
    // 📈 Update humidity trend
    Point* humidityTrend = sensorNetwork.Find("HumidityTrend");
    if (humidityTrend) {
        // Similar trend update logic for humidity
    }
}

void updateStatusLED() {
    // 💡 Blink LED based on system status
    static unsigned long lastBlink = 0;
    static bool ledState = false;
    
    // 🌐 Fast blink if WiFi disconnected, slow blink if connected
    unsigned long blinkInterval = (WiFi.status() == WL_CONNECTED) ? 2000 : 200;
    
    if (millis() - lastBlink > blinkInterval) {
        ledState = !ledState;
        digitalWrite(2, ledState ? HIGH : LOW);
        lastBlink = millis();
    }
}

void sendDataToServer() {
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("❌ WiFi not connected, skipping upload");
        return;
    }
    
    Serial.println("📤 Sending data to server...");
    
    // 📋 Create JSON payload
    DynamicJsonDocument doc(1024);
    
    // 🏷️ Device info
    uint8_t* mac = sensorNetwork.Find("DeviceMAC")->getRawValue();
    char macStr[18];
    snprintf(macStr, sizeof(macStr), "%02X:%02X:%02X:%02X:%02X:%02X",
             mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    doc["device_id"] = macStr;
    doc["timestamp"] = millis();
    
    // 📊 Sensor data
    doc["temperature"] = sensorNetwork.Find("Temperature")->getValue<float>();
    doc["humidity"] = sensorNetwork.Find("Humidity")->getValue<uint8_t>();
    doc["pressure"] = sensorNetwork.Find("Pressure")->getValue<float>();
    doc["light_level"] = sensorNetwork.Find("LightLevel")->getValue<uint16_t>();
    
    // 🔋 System status
    doc["wifi_signal"] = sensorNetwork.Find("WiFiSignal")->getValue<int8_t>();
    doc["free_heap"] = sensorNetwork.Find("FreeHeap")->getValue<uint32_t>();
    doc["uptime"] = sensorNetwork.Find("Uptime")->getValue<uint32_t>();
    
    // 🌐 Send HTTP POST request
    String jsonString;
    serializeJson(doc, jsonString);
    
    // Note: In a real implementation, you'd use HTTPClient library
    Serial.println("📊 JSON Payload:");
    Serial.println(jsonString);
    Serial.println("✅ Data upload simulated (implement HTTP client)");
}

void printLocalStatus() {
    Serial.println("\n📊 === ESP32 IoT Status ===");
    
    // 🌡️ Environmental data
    Serial.printf("🌡️ Temperature: %.1f°C\n", 
                  sensorNetwork.Find("Temperature")->getValue<float>());
    Serial.printf("💨 Humidity: %d%%\n", 
                  sensorNetwork.Find("Humidity")->getValue<uint8_t>());
    Serial.printf("🌫️ Pressure: %.2f hPa\n", 
                  sensorNetwork.Find("Pressure")->getValue<float>());
    Serial.printf("💡 Light Level: %d/4095\n", 
                  sensorNetwork.Find("LightLevel")->getValue<uint16_t>());
    
    // 🔋 System status
    Serial.printf("📶 WiFi Signal: %d dBm\n", 
                  sensorNetwork.Find("WiFiSignal")->getValue<int8_t>());
    Serial.printf("💾 Free Heap: %u bytes\n", 
                  sensorNetwork.Find("FreeHeap")->getValue<uint32_t>());
    Serial.printf("⏱️ Uptime: %u seconds\n", 
                  sensorNetwork.Find("Uptime")->getValue<uint32_t>());
    
    // 💾 Memory usage by Points library
    size_t totalMemory = 0;
    for (size_t i = 0; i < sensorNetwork.Count(); i++) {
        Point* point = sensorNetwork.At(i);
        if (point) {
            totalMemory += point->getMemoryUsage();
        }
    }
    Serial.printf("📊 Points Library Memory: %u bytes\n", totalMemory);
    
    Serial.println("========================\n");
}

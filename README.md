# IOT-Based-Smart-Fog-and-Mountain-Road-Safety-Alert-System-
Smart Fog Alert System for mountain road safety using multi-sensor fusion.

🔹 Overview:

This project presents an IoT‑based Smart Fog and Mountain Road Safety Alert System designed to enhance driver safety in low‑visibility and high‑risk terrains such as fog‑prone mountain roads. The system integrates multiple sensors and real‑time monitoring to detect obstacles, road edges, and vehicle tilt conditions, providing instant alerts to prevent accidents.

🔹 Working Principle:

The system continuously collects environmental and vehicle‑motion data using ultrasonic, IR, and MPU6050 sensors. The processed data is displayed locally on an LCD screen and remotely monitored through the ThingSpeak IoT cloud platform via ESP8266 Wi‑Fi connectivity.

If any dangerous condition such as cliff risk, steep tilt, or close obstacle detection occurs, the system activates buzzer alerts and visual indicators to warn the driver in real time.

🔹 Hardware Components Used:

1. ESP8266 / NodeMCU (Wi‑Fi microcontroller)
2. Ultrasonic Sensor (distance measurement)
3. IR Sensor (object detection)
4. MPU6050 (tilt & motion sensing)
5. LCD I2C Display
6. Buzzer (audio alert)
7. LED Indicators

🔹 Key Features:

1. Real‑time obstacle detection
2. Cliff / slope detection using tilt sensing
3. Multi‑sensor fusion safety monitoring
4.LCD live status display
5.Buzzer alert system
6.IoT cloud data logging (ThingSpeak)
7.Wi‑Fi connectivity via ESP8266

🔹 Alert Logic Implemented:

Cliff / steep tilt detected → Continuous buzzer alert
Obstacle within 10 cm → Intermittent buzzer beep
IR object detected → LED indication
Safe condition → Normal display status

🔹 IoT Integration:

Sensor data such as:
Distance
IR detection
Pitch angle
Acceleration (X, Y, Z)
is uploaded to ThingSpeak every 20 seconds for remote monitoring and analytics.

🔹 Applications:

Fog‑prone mountain highways
Blind curves & cliff roads
Accident‑prone hilly regions
Smart vehicle safety systems

🔹 Future Scope:

AI‑based accident prediction
Vehicle‑to‑Vehicle alert communication
Mobile app integration
Advanced radar & LiDAR sensing

📌 How to Use (Optional): 

1. Connect sensors as per circuit diagram.
2. Update Wi‑Fi SSID & password.
3. Add ThingSpeak Channel ID & API key.
4. Upload code to ESP8266.
5.Monitor alerts locally and on cloud.

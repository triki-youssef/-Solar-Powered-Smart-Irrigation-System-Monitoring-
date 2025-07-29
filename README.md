<h1>🌿 Solar-Powered Smart Irrigation System Monitoring 🌞</h1>

This project is a low-cost, solar-powered smart irrigation system built using an ESP32, various environmental sensors, and the Blynk IoT platform. It helps monitor real-time temperature, humidity, soil moisture, and water tank level, and automatically manages irrigation to reduce water waste — all while running off-grid using solar energy.

<h2>🧠 Features</h2>
- 🌡️ Real-time temperature & humidity monitoring (DHT11)

- 🌱 Soil moisture monitoring to detect dry soil

- 🚰 Water tank level monitoring using an analog sensor

- 💧 Automatic irrigation based on soil condition and temperature

- 🕹️ Manual pump control from Blynk mobile app

- 🧊 Cooldown system to prevent overwatering

- 🔋 Powered by solar panel + 12V lead-acid battery

- 📲 Real-time monitoring via Blynk mobile app

<h2>  🔧 Components Used</h2>

- ESP32 Dev Board : 	Wi-Fi-enabled microcontroller

  
- DHT11 Sensor : 	Temperature & humidity sensor


- Soil Moisture Sensor : 	Analog soil sensor


- Water Level Sensor :	Analog sensor for tank level


- Relay Module : Controls the 5V water pump



- Water Pump :	5V DC pump


- PWM Solar Charge Controller :	Regulates charging of battery


- 12V 1.5Ah Lead-Acid Battery	 : Energy storage


- Solar Panel	 : 12V panel

<h2>⚡ Solar Power Architecture</h2>
This system runs entirely on solar energy, making it ideal for agricultural or remote installations.

<h3>🔋 Power Setup:</h3>

- A solar panel charges a 12V 1.5Ah lead-acid battery via a PWM solar charge controller.

- The ESP32 and sensors are powered via the controller’s USB 5V OUTPUT

- The relay and pump also draw power from the battery.

<h2>📲 Blynk App Configuration : </h2>
<h3>Virtual Pin	Function :	</h3>
- V0	: Soil Moisture (%)	- Value
- V1	: Temperature (°C)	- Value 
- V2	: Humidity (%)	- Value 
- V3	: Water Tank Level (%) - 	value
- V4	: Manual Pump Control	Slider (1 = ON)



<h2>💻 System Behavior</h2>
- Reads all sensors every 5 seconds.

- If soil moisture < 30% and water tank is not LOW:

    -  Automatically irrigates for 6s if temp < 35°C, or 12s if temp > 35°C.

    -  After irrigation, enters a 1-minute cooldown period.

-Pump can be manually activated via Blynk (V4).

- All sensor data is sent to Blynk for remote monitoring.

<h2>🔄 Logic Flow (Simplified) : </h2>


  → Read Sensors

  
→ Check soil dryness & tank level


→ If dry & enough water:


     → Activate pump (6s–12s)

     
     → Start 1-minute cooldown

     
→ Else: 


     → Skip or wait

     
→ Manual mode overrides auto logic


→ Update Blynk dashboard  


<h2> 📹 Short video demo : </h2>







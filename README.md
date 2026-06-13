[🇬🇧 English](README.md) | [🇫🇷 Français](README.fr.md)

# Aero-system-automated — DIY Automated Aeroponic System with WiFi Remote Control

An open-source **automated aeroponic system** built around an **Arduino Mega 2560 + NodeMCU ESP8266**, remotely controlled over WiFi from any smartphone. This DIY smart farming project handles water temperature regulation, nutrient (TDS) monitoring, automated watering cycles, programmable grow lights, and real-time sensor logging — all from a self-hosted web interface served by the ESP8266.

Originally built from scratch during the March–April 2020 lockdown as a first electronics / Arduino project, it has been running reliably for weeks in real-world growing conditions. If you are looking for a hackable starting point for **hydroponics automation**, **WiFi-controlled grow boxes**, **Arduino Mega ESP8266 IoT projects**, or **DIY indoor farming**, this repo is a complete reference: full source code, wiring, schematics, photos and videos.

> Full build tutorial (assembly, schematics, photos, videos):
> - [Hackster.io — Automated Aeroponic System WiFi Remoted](https://www.hackster.io/alexch03/automated-aeroponic-system-wifi-remoted-b5eaea)
> - [Arduino Project Hub](https://create.arduino.cc/projecthub/alexch03/automated-aeroponic-system-wifi-remoted-b5eaea)

---

## Features

- Water temperature measurement and regulation (DS18B20) via a Peltier module
- Air temperature / humidity measurement (DHT22)
- Grow-box internal temperature measurement (LM35 on A2)
- Nutrient concentration measurement (TDS sensor on A8)
- Automated reservoir refilling (water level)
- Programmable lighting cycles (ON / OFF hours)
- Programmable watering cycles (duration, frequency)
- Manual override of pumps / lamps / peristaltic dosing pumps
- On-board data logging to a microSD card
- Local display on an I²C 16×2 LCD with custom animations
- Web interface (served by the ESP8266) reachable from any smartphone, protected with a password
- OTA firmware updates for the ESP8266 (no cable needed)

## Architecture

```
                     +--------------------+
   Sensors  ------>  |                    |  <---- Serial ---->  +--------------+
   Relays   <------- |   Arduino Mega 2560|                      | NodeMCU ESP  | <-- WiFi --> Smartphone
   Pumps    <------- |  (control loop,    |                      | (web server) |
   LCD      <------- |   sensor reading,  |                      +--------------+
   SD card  <------- |   automation logic)|
   RTC      <------- |                    |
                     +--------------------+
```

- The **Mega** handles all the logic: sensors, relays, pumps, RTC, SD, LCD.
- The **ESP8266** serves the web interface and exchanges values / setpoints with the Mega over a UART link using a simple text protocol `<field,value>`.

## Hardware

| Category | Component |
|---|---|
| Microcontrollers | Arduino Mega 2560, NodeMCU ESP8266 |
| Sensors | DS18B20 (water), DHT22 (air), LM35 (box), TDS sensor |
| Display | I²C 16×2 LCD |
| Storage | microSD card reader |
| Real-time clock | RTC DS1302 |
| Actuators | 3 × 12 V / 10 A relays, L298 H-bridge (Peltier) |
| Pumps | 2 × peristaltic 12 V (11 mL/min), 3 × brushless 12 V water pumps |
| Thermal regulation | Peltier module + heatsinks |
| Plumbing | PVC tubing + silicone |

## Pinout (Mega 2560)

Pins are defined in `MEGA-ESP_v08_propre.ino`:

| Function | Pin |
|---|---|
| Relay 1 (Peltier) | D24 |
| Pumps (relay) | D22 |
| Lamps (relay) | D26 |
| Motor A (L298) | D38 / D40 |
| Motor B (L298) | D36 / D34 |
| RTC DS1302 GND / VCC | D33 / D31 |
| DHT22 | D32 |
| OneWire (DS18B20) | D7 |
| LM35 (Tbox) | A2 |
| TDS | A8 |

## Software

### Required libraries (Mega side)

- `EEPROM`
- [`DHT sensor library`](https://github.com/adafruit/DHT-sensor-library)
- [`DallasTemperature`](https://github.com/milesburton/Arduino-Temperature-Control-Library) + `OneWire`
- `SPI`, `SD`
- [`LiquidCrystal_I2C`](https://github.com/johnrickman/LiquidCrystal_I2C)
- [`virtuabotixRTC`](https://github.com/chrisfryer78/ArduinoRTClibrary)
- [`ResponsiveAnalogRead`](https://github.com/dxinteractive/ResponsiveAnalogRead)

### Required libraries (ESP8266 side)

- `ESP8266WiFi`
- `ESP8266WebServer`
- `ArduinoOTA`

## Files in this repository

> Heads-up: the filenames are **misleading** — they are the opposite of what their names suggest.

| File | Target | Role |
|---|---|---|
| `MEGA-ESP_v08_propre.ino` | **Arduino Mega 2560** | Main loop, sensors, relays, pumps, LCD, SD, RTC, automation logic |
| `ESP_MEGA_proper_v08.ino` | **NodeMCU ESP8266** | WiFi, web server, OTA, serial bridge with the Mega |

## Configuration

Before flashing, **edit in `ESP_MEGA_proper_v08.ino`**:

```cpp
char* ssid     = "YOUR_SSID";
char* password = "YOUR_WIFI_PASSWORD";

const char* www_username = "admin";
const char* www_password = "YOUR_WEB_PASSWORD";
```

## Installation

1. Wire the components according to the schematic in the Hackster tutorial.
2. Open the Arduino IDE and install the libraries listed above.
3. Select the **Arduino Mega 2560** board → flash `MEGA-ESP_v08_propre.ino`.
4. Select the **NodeMCU 1.0 (ESP-12E)** board → set your SSID/password → flash `ESP_MEGA_proper_v08.ino`.
5. Connect the Mega (`Serial1`, pins 18/19) to the ESP8266 (TX/RX) — **be careful to use a 5 V → 3.3 V voltage divider** on the ESP RX line.
6. Read the ESP's IP address from the Serial Monitor, open it in your browser, and log in with the credentials above.

## Demo / videos

See the embedded videos on the Hackster.io project page.

## Note

> The web interface is currently in French. Labels can be translated by editing the strings in `ESP_MEGA_proper_v08.ino`. The code comments have been translated to English; only user-facing UI strings (LCD welcome message, web form labels) were intentionally kept in French to preserve the original UX.

## Disclaimer

This code ran for weeks in real conditions without trouble, but it was written self-taught during lockdown — it is not optimized and does not necessarily follow "clean" Arduino conventions. Use it as a tinkering base or as inspiration.

## Author

**Alexandros Pantelidis** — built in April 2020.

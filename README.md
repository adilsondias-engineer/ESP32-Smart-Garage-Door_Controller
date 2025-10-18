# 🚪 ESP32 Smart Garage Door Controller

> Professional IoT garage door automation system with web control, real-time monitoring, and OTA firmware updates

![Platform](https://img.shields.io/badge/Platform-ESP32-blue.svg)
![Framework](https://img.shields.io/badge/Framework-ESP--IDF%20v4.4%2B-green.svg)
![Language](https://img.shields.io/badge/Language-C%2FC%2B%2B-orange.svg)
![License](https://img.shields.io/badge/License-CC0--1.0-lightgrey.svg)
![Wokwi](https://img.shields.io/badge/Simulation-Wokwi-purple.svg)

---

## 📋 Table of Contents

- [Overview](#-overview)
- [Features](#-features)
- [System Architecture](#-system-architecture)
- [Hardware Setup](#-hardware-setup)
- [Quick Start](#-quick-start)
- [API Reference](#-api-reference)
- [Wokwi Simulation](#-wokwi-simulation)
- [Configuration](#-configuration)
- [Troubleshooting](#-troubleshooting)
- [Safety & Disclaimer](#️-safety--disclaimer)
- [Contributing](#-contributing)
- [License](#-license)

---

## 📋 Overview

Transform any standard garage door opener into a smart, remotely-accessible system. This ESP32-based controller provides professional-grade IoT capabilities including web interface, RESTful API, real-time state management, and over-the-air updates.

Built with **ESP-IDF** (Espressif IoT Development Framework) in C/C++, this project demonstrates production-quality embedded systems development suitable for home automation, industrial IoT, or as a portfolio piece showcasing embedded expertise.

### 🎯 Project Highlights

- **Production Architecture**: State machine-based control with proper error handling
- **Real Hardware Control**: Relay activation, magnetic sensor integration, GPIO management
- **Remote Access**: HTTP server with RESTful JSON API for home automation integration
- **Developer Friendly**: Includes Wokwi simulation for testing without physical hardware
- **Enterprise Features**: OTA updates, NTP time sync, structured logging
- **Portfolio Ready**: Demonstrates embedded C/C++, IoT protocols, real-time systems

### 💡 Use Cases

- **Home Automation**: Integrate with Home Assistant, Node-RED, or custom dashboards
- **Remote Access**: Control garage door from anywhere via web interface
- **Security**: Monitor door status and receive alerts
- **Smart Home Hub**: Part of larger IoT ecosystem with MQTT support
- **Learning Platform**: Study embedded systems, state machines, and IoT protocols

---

## ✨ Features

### Core Functionality

| Feature | Description |
|---------|-------------|
| 🚪 Remote Control | Open/close garage door via web interface or API |
| 📊 Real-time Monitoring | Continuous door state tracking with sensor validation |
| 🔄 State Machine | Robust state management (OPEN, OPENING, CLOSING, CLOSED) |
| 🔌 Hardware Sensors | Magnetic/limit switches detect actual door position |
| ⏱️ Safety Timeouts | Automatic error detection if door doesn't complete operation |

### Web & Connectivity

| Feature | Description |
|---------|-------------|
| 🌐 Web Interface | Clean, responsive HTML interface with status and controls |
| 📡 RESTful API | JSON endpoints for system integration |
| 📶 WiFi Stable | Automatic reconnection handling |
| 🔄 OTA Updates | Over-the-air firmware updates without physical access |
| ⏰ NTP Sync | Automatic time synchronization (Australian NTP servers) |

### Development & Testing

| Feature | Description |
|---------|-------------|
| 🧪 Wokwi Simulation | Full circuit simulation for development without hardware |
| 📊 Debug Logging | Comprehensive ESP-IDF logging with configurable levels |
| 🔧 Configurable | Easy setup via `idf.py menuconfig` |
| 📐 Modular Design | Clean component-based architecture |
| 📡 MQTT Ready | Infrastructure in place for MQTT integration |

---

## 🏗️ System Architecture

### Component Diagram

```
ESP32 Application Layer
├── HTTP Server (Web UI & JSON API)
├── Door State Machine (Control Logic)
└── OTA Update Manager

ESP-IDF Framework Layer
├── WiFi Stack
├── NTP Client
├── HTTP Server
└── FreeRTOS Tasks

Hardware Layer
├── Relay Module (GPIO 13)
├── Open Sensor (GPIO 34)
├── Close Sensor (GPIO 35)
└── Status LED (GPIO 18)
```

### State Machine Flow

```
INITIAL STATE
├── If Open Sensor Active → OPEN
└── If Close Sensor Active → CLOSED

OPEN STATE
└── Close Command → CLOSING → Close Sensor → CLOSED

CLOSED STATE
└── Open Command → OPENING → Open Sensor → OPEN

ERROR STATES
├── Timeout during OPENING → ERROR
└── Timeout during CLOSING → ERROR
```

---

## 🔧 Hardware Setup

### Required Components

| Qty | Component | Specification | Purpose | Approx. Cost |
|-----|-----------|--------------|---------|--------------|
| 1 | ESP32 DevKit | Any ESP32 dev board | Main controller | $8-15 |
| 1 | Relay Module | 5V single channel, optocoupler | Door opener control | $3-5 |
| 2 | Magnetic Switches | NO (Normally Open) | Position sensors | $2-4 each |
| 1 | LED | Red, 3mm or 5mm | Status indicator | $0.50 |
| 1 | Resistor | 220Ω | LED current limiting | $0.10 |
| 1 | Power Supply | 5V 2A USB or DC | Power ESP32 & relay | $5-10 |
| - | Wire | 22-24 AWG | Connections | $5 |
| - | Enclosure | Weatherproof (optional) | Protection | $10-20 |

**Total Cost**: ~$40-70 USD

### GPIO Pin Configuration

| Function | GPIO Pin | Type | Notes |
|----------|----------|------|-------|
| **Relay Control** | GPIO 13 | Output | Triggers garage door opener (active HIGH) |
| **Open Sensor** | GPIO 34 | Input | HIGH when door fully open (input-only pin) |
| **Close Sensor** | GPIO 35 | Input | HIGH when door fully closed (input-only pin) |
| **Status LED** | GPIO 18 | Output | Visual feedback (simulation only) |

⚠️ **Important**: GPIO 34 and 35 are **input-only** pins on ESP32. They have no internal pull-up/pull-down resistors. Use external 10kΩ pull-down resistors if sensors are active-high, or configure sensors as active-low.

### Wiring Diagram

**ESP32 to Relay Module:**
- ESP32 GPIO 13 → Relay IN
- ESP32 GND → Relay GND  
- ESP32 5V → Relay VCC
- Relay COM → Garage Door Opener Terminal 1
- Relay NO → Garage Door Opener Terminal 2

**Sensors to ESP32:**
- Open Sensor Terminal 1 → ESP32 GPIO 34
- Open Sensor Terminal 2 → ESP32 3.3V
- Close Sensor Terminal 1 → ESP32 GPIO 35
- Close Sensor Terminal 2 → ESP32 3.3V

**Status LED (Simulation):**
- ESP32 GPIO 18 → 220Ω Resistor → LED Anode → LED Cathode → GND

### Installation Steps

#### 1. Mount Position Sensors
- **Open Sensor**: Install where door is fully open (magnet aligns with switch)
- **Close Sensor**: Install where door is fully closed (magnet aligns with switch)
- Test sensor activation with multimeter (should show continuity when triggered)

#### 2. Connect Relay to Door Opener
- Identify garage door opener's wall button terminals
- Connect relay COM and NO terminals in parallel with button
- Relay simulates button press when activated
- **Test relay independently** before connecting to ESP32

#### 3. Wire ESP32 to Components
- Follow wiring diagram above
- Use color-coded wires (red=power, black=ground, other=signals)
- Ensure secure connections (solder or screw terminals)
- Double-check polarity before powering on

#### 4. Power Supply Considerations
- Use dedicated 5V power supply (not USB during deployment)
- Ensure common ground between ESP32 and relay
- Consider battery backup for critical installations
- Mount power supply in protected area

#### 5. Enclosure & Mounting
- Use weatherproof enclosure if mounted in garage
- Ensure adequate ventilation for ESP32 (generates heat)
- Mount securely away from door mechanism
- Label all connections for future maintenance

---

## 🚀 Quick Start

### Prerequisites

- **ESP-IDF v4.4+** ([Installation Guide](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/))
- **Python 3.7+** (included with ESP-IDF)
- **USB cable** for ESP32 programming
- **Serial terminal** (ESP-IDF monitor included)

### Step-by-Step Installation

#### 1. Clone Repository
```bash
git clone <repository-url>
cd GarageDoor
```

#### 2. Setup ESP-IDF Environment
```bash
# Linux/macOS
. $HOME/esp/esp-idf/export.sh

# Windows Command Prompt
%userprofile%\esp\esp-idf\export.bat

# Windows PowerShell
.$HOME/esp/esp-idf/export.ps1
```

#### 3. Configure WiFi Credentials
```bash
idf.py menuconfig
```

Navigate to:
- **Example Connection Configuration**
  - Set `WiFi SSID`: Your network name
  - Set `WiFi Password`: Your network password

**Optional Settings**:
- `Component config` → `ESP32-specific` → CPU frequency
- `Component config` → `FreeRTOS` → Task watchdog timeout
- `Partition Table` → Custom partition CSV (if OTA needed)

#### 4. Build Project
```bash
idf.py build
```

Expected output:
```
Project build complete. To flash, run:
idf.py -p (PORT) flash
```

**Build Time**: ~2-5 minutes (first build), ~30 seconds (incremental)

#### 5. Flash & Monitor
```bash
# Auto-detect port and flash
idf.py flash monitor

# Or specify port manually
idf.py -p /dev/ttyUSB0 flash monitor  # Linux
idf.py -p COM3 flash monitor          # Windows
```

**Flash Time**: ~30 seconds

**Monitor Controls**:
- `Ctrl+]` - Exit monitor
- `Ctrl+T` + `Ctrl+H` - Show help
- `Ctrl+T` + `Ctrl+R` - Restart ESP32

#### 6. Find IP Address
Watch serial output for:
```
I (5234) wifi_station: connected to WiFi
I (5237) wifi_station: got ip:192.168.1.42
I (5241) application: HTTP Server started on port 80
```

#### 7. Access Web Interface
Open browser: `http://192.168.1.42`

You should see:
- Current door status
- "Open Door" button
- "Close Door" button
- System uptime and time

---

## 📡 API Reference

### Web Interface Endpoints

#### Main Control Page
```http
GET /
GET /index
```

**Response**: HTML interface with door status and control buttons

**Features**:
- Real-time door status display
- Open/Close action buttons
- System time and uptime
- Clean, responsive design

---

### RESTful JSON API

### Get System Status

```http
GET /status
```

**Response:** application/json

```json
{
  "SystemTime": "2024-10-18 14:23:45",
  "UpTime": "2d 5h 12m 33s",
  "DoorStatus": "CLOSED"
}
```

**Door Status Values:**
- `OPEN` - Door fully open (sensor confirmed)
- `OPENING` - Door opening in progress
- `CLOSING` - Door closing in progress  
- `CLOSED` - Door fully closed (sensor confirmed)
- `UNKNOWN` - Initial state or sensor error

### Open Door Command

```http
POST /openDoor
```

**Behavior:**
1. Checks if door is CLOSED
2. Activates relay (pulse for 500ms)
3. Changes state to OPENING
4. Waits for open sensor confirmation
5. Changes to OPEN when sensor triggers

**Response:** 200 OK (command accepted)

**Error Cases:**
- Door already OPEN or OPENING
- Timeout waiting for sensor (30s)

### Close Door Command

```http
POST /closeDoor
```

**Behavior:**
1. Checks if door is OPEN
2. Activates relay (pulse for 500ms)
3. Changes state to CLOSING
4. Waits for close sensor confirmation
5. Changes to CLOSED when sensor triggers

**Response:** 200 OK (command accepted)

**Error Cases:**
- Door already CLOSED or CLOSING
- Timeout waiting for sensor (30s)

### Integration Examples

#### cURL
```bash
# Check door status
curl http://192.168.1.42/status

# Open door
curl -X POST http://192.168.1.42/openDoor

# Close door
curl -X POST http://192.168.1.42/closeDoor

# Pretty-print JSON
curl http://192.168.1.42/status | jq
```

#### Python
```python
import requests
import json

ESP32_IP = "192.168.1.42"
BASE_URL = f"http://{ESP32_IP}"

def get_status():
    """Get current door status"""
    response = requests.get(f"{BASE_URL}/status")
    return response.json()

def open_door():
    """Open the garage door"""
    response = requests.post(f"{BASE_URL}/openDoor")
    return response.status_code == 200

def close_door():
    """Close the garage door"""
    response = requests.post(f"{BASE_URL}/closeDoor")
    return response.status_code == 200

# Example usage
status = get_status()
print(f"Door is currently: {status['DoorStatus']}")

if status['DoorStatus'] == 'CLOSED':
    print("Opening door...")
    open_door()
```

#### Node-RED Flow
```json
[
  {
    "id": "get_status",
    "type": "http request",
    "method": "GET",
    "url": "http://192.168.1.42/status",
    "name": "Get Door Status"
  },
  {
    "id": "open_door",
    "type": "http request",
    "method": "POST",
    "url": "http://192.168.1.42/openDoor",
    "name": "Open Door"
  }
]
```

#### Home Assistant Integration
```yaml
# configuration.yaml

# REST Commands
rest_command:
  garage_open:
    url: 'http://192.168.1.42/openDoor'
    method: POST
  
  garage_close:
    url: 'http://192.168.1.42/closeDoor'
    method: POST

# Sensor for door status
sensor:
  - platform: rest
    name: Garage Door
    resource: 'http://192.168.1.42/status'
    json_attributes:
      - SystemTime
      - UpTime
      - DoorStatus
    value_template: '{{ value_json.DoorStatus }}'
    scan_interval: 5

# Cover entity (optional)
cover:
  - platform: template
    covers:
      garage_door:
        friendly_name: "Garage Door"
        value_template: >
          {% if is_state_attr('sensor.garage_door', 'DoorStatus', 'OPEN') %}
            open
          {% elif is_state_attr('sensor.garage_door', 'DoorStatus', 'CLOSED') %}
            closed
          {% else %}
            unknown
          {% endif %}
        open_cover:
          service: rest_command.garage_open
        close_cover:
          service: rest_command.garage_close
```

---

## 🧪 Wokwi Simulation

Test the complete system in a browser-based simulator without physical hardware.

### Online Simulation
Visit: **[Wokwi ESP32 Simulator](https://wokwi.com/)**

### Local Simulation

#### Prerequisites
```bash
# Install Wokwi CLI
npm install -g wokwi-cli

# Or use VS Code extension
# Search for "Wokwi" in Extensions marketplace
```

#### Running Simulation

**Method 1: Wokwi CLI**
```bash
# Build project first
idf.py build

# Run simulation (30 second timeout)
wokwi-cli --timeout 30000

# Or run indefinitely
wokwi-cli
```

**Method 2: VS Code Extension**
1. Install [Wokwi for VS Code](https://marketplace.visualstudio.com/items?itemName=wokwi.wokwi-vscode)
2. Open Command Palette (`F1` or `Ctrl+Shift+P`)
3. Run: `Wokwi: Start Simulator`
4. Monitor output in VS Code terminal

### Simulation Components

The `diagram.json` includes:
- ✅ ESP32 DevKit V1 (38-pin)
- ✅ 5V Single-channel relay module
- ✅ Red status LED with 220Ω resistor
- ✅ Serial monitor for debugging
- ✅ WiFi simulation (virtual network)

### Simulated Behavior

**What Works**:
- WiFi connection to simulated network
- HTTP server on localhost
- Relay activation visualization
- LED status indicator
- Serial logging output
- State machine transitions

**Limitations**:
- No physical sensors (states change automatically)
- Timing may differ from real hardware
- Some ESP32 peripherals simplified
- Port forwarding may be required for HTTP access

### Testing in Simulation

1. **Start Simulation**
```bash
wokwi-cli
```

2. **Wait for Boot**
Watch serial output for WiFi connection:
```
I (1234) wifi: WiFi initialized
I (2345) wifi: connected to SSID
I (2346) main: IP Address: 10.0.0.2
```

3. **Access Web Interface**
- Note IP from serial output
- Open browser to that IP
- Port forwarding: Check Wokwi docs for details

4. **Test Commands**
- Click "Open Door" button
- Watch serial output for state changes
- Observe LED and relay behavior
- Verify state transitions

5. **Monitor Serial Output**
```
I (5000) main: Door state: CLOSED
I (10000) main: Open door command received
I (10001) main: Relay activated
I (10501) main: Door state: OPENING
I (15000) main: Open sensor triggered
I (15001) main: Door state: OPEN
```

---

## ⚙️ Configuration

### WiFi Configuration
```bash
idf.py menuconfig
```
Navigate to:
- `Example Connection Configuration`
  - **WiFi SSID**: Your network name
  - **WiFi Password**: Your password
  - **Maximum retry**: 5 (default)

### Time Zone Setup

Edit `main/main.cpp` to set your timezone:
```cpp
// Australian Eastern Standard Time (AEST) - DEFAULT
setenv("TZ", "AEST-10AEDT,M10.1.0,M4.1.0/3", 1);

// Other common timezones:
// Pacific Standard Time
setenv("TZ", "PST8PDT,M3.2.0,M11.1.0", 1);

// Eastern Standard Time
setenv("TZ", "EST5EDT,M3.2.0,M11.1.0", 1);

// Central European Time
setenv("TZ", "CET-1CEST,M3.5.0,M10.5.0/3", 1);

// UTC (no DST)
setenv("TZ", "UTC0", 1);
```

**Format**: `STD offset DST,start,end`
- `STD`: Standard timezone abbreviation
- `offset`: Hours from UTC (negative = east of UTC)
- `DST`: Daylight saving time abbreviation
- `start`: DST start rule
- `end`: DST end rule

### NTP Server Configuration

Modify in `main/main.cpp`:
```cpp
// Australian NTP servers (default)
sntp_setservername(0, "0.au.pool.ntp.org");
sntp_setservername(1, "1.au.pool.ntp.org");

// Global NTP servers
sntp_setservername(0, "pool.ntp.org");
sntp_setservername(1, "time.google.com");
sntp_setservername(2, "time.cloudflare.com");

// Regional servers
// US: time.nist.gov
// EU: europe.pool.ntp.org
// Asia: asia.pool.ntp.org
```

### Door Operation Timeouts

Edit timing parameters in `main/main.cpp`:
```cpp
// Door operation timeout (milliseconds)
#define DOOR_TIMEOUT_MS  30000  // 30 seconds

// Relay pulse duration
#define RELAY_PULSE_MS   500    // 0.5 seconds

// Sensor debounce time
#define DEBOUNCE_MS      100    // 0.1 seconds

// State check interval
#define CHECK_INTERVAL_MS 100   // 0.1 seconds
```

### HTTP Server Port

Change server port in `main/http.c`:
```c
httpd_config_t config = HTTPD_DEFAULT_CONFIG();
config.server_port = 80;     // Default HTTP
// config.server_port = 8080; // Alternative port
config.max_uri_handlers = 10;
config.max_resp_headers = 8;
```

### TLS Certificate Setup

1. Place certificate bundle in `certs/dogdoorcert.pem`
2. Update path in `sdkconfig.defaults` if needed:
```
CONFIG_SERVER_CERT_PATH="certs/dogdoorcert.pem"
```

3. For HTTPS, modify HTTP server configuration:
```c
// In http.c
httpd_ssl_config_t conf = HTTPD_SSL_CONFIG_DEFAULT();
conf.port_secure = 443;
conf.servercert = server_cert_start;
conf.servercert_len = server_cert_end - server_cert_start;
```

### Logging Level

Configure in `idf.py menuconfig`:
- `Component config` → `Log output`
  - **Default log verbosity**: Info
  - Options: None, Error, Warning, Info, Debug, Verbose

Or set in code:
```cpp
esp_log_level_set("*", ESP_LOG_INFO);        // All components
esp_log_level_set("wifi", ESP_LOG_WARN);     // WiFi only warnings
esp_log_level_set("application", ESP_LOG_DEBUG); // Detailed app logs
```

---

## 🐛 Troubleshooting

### WiFi Connection Issues

**Problem**: ESP32 cannot connect to WiFi
```
E (5000) wifi: Failed to connect to SSID: YourNetwork
```

**Solutions**:
1. **Verify Credentials**
   ```bash
   idf.py menuconfig
   # Check SSID and password for typos
   ```

2. **Check WiFi Band**
   - ESP32 only supports 2.4GHz WiFi
   - Disable 5GHz or use separate 2.4GHz network

3. **Signal Strength**
   - Move ESP32 closer to router
   - Check for interference (microwaves, metal objects)
   - Use WiFi analyzer app to check signal

4. **Router Settings**
   - Ensure MAC filtering allows ESP32
   - Check maximum connected devices limit
   - Try different WiFi channel
   - Disable AP isolation mode

5. **Special Characters**
   - Avoid special characters in WiFi password
   - Use menuconfig (escapes automatically)

**Debug Commands**:
```bash
# View detailed WiFi logs
idf.py monitor | grep wifi

# Check WiFi configuration
idf.py menuconfig
```

---

### Door Not Responding

**Problem**: Clicking buttons has no effect on door

**Diagnostic Checklist**:
- [ ] Verify relay wiring (NO/COM to door opener terminals)
- [ ] Check relay power supply (5V, adequate current)
- [ ] Confirm GPIO 13 connected to relay IN pin
- [ ] Test relay manually (jumper GND to IN pin)
- [ ] Check door opener accepts momentary button press
- [ ] Verify door opener has power
- [ ] Review serial logs for relay activation messages

**Testing Relay**:
```cpp
// Add to main.cpp for relay test
void test_relay() {
    ESP_LOGI(TAG, "Testing relay...");
    gpio_set_level(RELAY_GPIO, 1);
    vTaskDelay(pdMS_TO_TICKS(1000));  // 1 second ON
    gpio_set_level(RELAY_GPIO, 0);
    ESP_LOGI(TAG, "Relay test complete");
}
```

**Expected Behavior**:
- Relay LED should illuminate
- Audible "click" sound
- Multimeter shows continuity between NO and COM

---

### Sensor Problems

**Problem**: Door state stuck in OPENING or CLOSING

**Common Causes**:
1. Sensors not triggering
2. Wiring issues
3. Missing pull resistors
4. Sensor misalignment
5. Magnet too far from switch

**Debugging Sensors**:
```cpp
// Add to main.cpp for continuous sensor monitoring
void monitor_sensors() {
    while(1) {
        int open = gpio_get_level(OPEN_SENSOR_GPIO);
        int close = gpio_get_level(CLOSE_SENSOR_GPIO);
        
        ESP_LOGI(TAG, "Sensors - Open: %d, Close: %d", open, close);
        vTaskDelay(pdMS_TO_TICKS(1000)); // Check every second
    }
}
```

**Solutions**:
1. **Check Connections**
   - GPIO 34 for open sensor
   - GPIO 35 for close sensor
   - Common ground connection

2. **Add Pull Resistors**
   - 10kΩ resistors from GPIO to GND
   - Required for GPIO 34/35 (input-only pins)

3. **Test with Multimeter**
   - Should read 3.3V when sensor triggered
   - 0V when not triggered

4. **Adjust Sensor Position**
   - Magnet must align within 5-10mm
   - Test by manually triggering sensor
   - Mark optimal position before mounting

5. **Verify Sensor Type**
   - Use Normally Open (NO) switches
   - Normally Closed (NC) requires different logic

---

### HTTP Server Issues

**Problem**: Cannot access web interface

**Diagnostic Steps**:
```bash
# 1. Ping ESP32
ping 192.168.1.42

# 2. Test with curl
curl -v http://192.168.1.42/status

# 3. Check port availability
netstat -an | grep 80  # Linux/Mac
netstat -an | findstr 80  # Windows
```

**Common Causes**:
1. **Wrong IP Address**
   - Check serial monitor for actual IP
   - IP may change if DHCP reassigns

2. **Firewall Blocking**
   - Windows: Allow port 80 inbound
   - Linux: `sudo ufw allow 80/tcp`
   - macOS: System Preferences → Security → Firewall

3. **Network Isolation**
   - ESP32 and computer on different subnets
   - Router guest network isolation enabled
   - VPN interfering with local access

4. **Browser Cache**
   - Hard refresh: `Ctrl+Shift+R` or `Cmd+Shift+R`
   - Clear browser cache
   - Try different browser

**Solutions**:
```bash
# Reserve IP in router DHCP settings
# Or set static IP in menuconfig

# Test from ESP32's network
# Use phone on same WiFi to test
```

---

### Build Errors

**Problem**: Compilation fails

**Common Error #1**: ESP-IDF not sourced
```bash
Error: idf.py command not found
```
**Solution**:
```bash
. $HOME/esp/esp-idf/export.sh  # Linux/Mac
%userprofile%\esp\esp-idf\export.bat  # Windows
```

**Common Error #2**: Wrong ESP-IDF version
```bash
Error: ESP-IDF version too old
```
**Solution**:
```bash
# Check version
idf.py --version

# Update ESP-IDF
cd $HOME/esp/esp-idf
git pull
git submodule update --init --recursive
./install.sh
```

**Common Error #3**: Missing dependencies
```bash
Error: Python packages missing
```
**Solution**:
```bash
cd $HOME/esp/esp-idf
./install.sh
. ./export.sh
```

**Common Error #4**: Corrupted build
```bash
Error: Ninja build failed
```
**Solution**:
```bash
# Clean and rebuild
idf.py fullclean
rm -rf build/
idf.py build
```

---

### Serial Monitor Issues

**Problem**: Cannot see serial output

**Solutions**:
1. **Check Port Permissions** (Linux/Mac)
   ```bash
   # Add user to dialout group
   sudo usermod -a -G dialout $USER
   # Log out and back in
   ```

2. **Verify Baud Rate**
   ```bash
   # Default is 115200
   idf.py monitor -b 115200
   ```

3. **Try Different USB Cable**
   - Some cables are power-only (no data lines)
   - Use cable that came with ESP32

4. **Check Drivers**
   - Windows: Install CP210x or CH340 drivers
   - macOS: May need to install drivers for CH340

---

### OTA Update Failures

**Problem**: Over-the-air update doesn't work

**Requirements**:
- Partition table includes OTA partitions
- Firmware binary accessible via HTTPS
- Valid TLS certificate
- Sufficient free flash space
- Stable network during update

**Partition Table Configuration**:
```csv
# partitions.csv
# Name,   Type, SubType, Offset,  Size, Flags
nvs,      data, nvs,     0x9000,  0x6000,
phy_init, data, phy,     0xf000,  0x1000,
factory,  app,  factory, 0x10000, 1M,
ota_0,    app,  ota_0,   ,        1M,
ota_1,    app,  ota_1,   ,        1M,
```

**Enable OTA**:
```bash
idf.py menuconfig
# Partition Table → Custom partition CSV file
# Set to: partitions.csv
```

**Testing OTA**:
```bash
# Build firmware
idf.py build

# Firmware binary location:
# build/GarageDoor.bin

# Host on HTTPS server
# Update URL in code to point to your server
```

---

### Memory Issues

**Problem**: ESP32 crashes or reboots randomly

**Check Free Memory**:
```cpp
// Add to main.cpp
void log_memory() {
    ESP_LOGI(TAG, "Free heap: %d bytes", esp_get_free_heap_size());
    ESP_LOGI(TAG, "Minimum free heap: %d bytes", 
             esp_get_minimum_free_heap_size());
}
```

**Common Causes**:
1. Memory leaks
2. Stack overflow
3. Heap fragmentation
4. Large buffers

**Solutions**:
```cpp
// Increase stack size for tasks
xTaskCreate(task_function, "task_name", 
            8192,  // Increase from 4096
            NULL, 5, NULL);

// Monitor stack usage
UBaseType_t stack_high_water = uxTaskGetStackHighWaterMark(NULL);
ESP_LOGI(TAG, "Stack remaining: %d bytes", stack_high_water * 4);
```

---

## 🛡️ Safety & Disclaimer

### ⚠️ CRITICAL WARNING

**This is an EDUCATIONAL PROJECT. DO NOT use in production without adding critical safety features.**

Garage doors are **heavy machinery** (150-400 lbs / 70-180 kg) capable of causing **serious injury or death**. Improper installation can result in:
- ❌ Property damage
- ❌ Personal injury
- ❌ Death
- ❌ Voided insurance coverage
- ❌ Legal liability

### Required Production Features

#### 1. 🔐 Authentication & Authorization
```cpp
// Current: NO security (anyone on network can control)
// Required: Implement proper authentication

// Example additions needed:
const char* AUTH_USER = "admin";
const char* AUTH_PASS = "secure_random_password";
bool require_authentication = true;

// Use JWT tokens, OAuth, or API keys
// Add HTTPS with TLS certificate
// Implement rate limiting
```

#### 2. 🚨 Manual Override
- Physical wall button must always work
- Bypass switch for electronic failure
- Manual chain/cord release mechanism
- Clear instructions for emergency operation

#### 3. 👁️ Obstruction Detection
```cpp
// MANDATORY for safety
bool check_obstruction() {
    // Check pressure sensors on door edge
    if (pressure_sensor_triggered()) return true;
    
    // Check infrared beam across opening
    if (!ir_beam_intact()) return true;
    
    // Monitor motor current draw
    if (motor_current_abnormal()) return true;
    
    return false;
}

// Auto-stop and reverse on obstruction
if (check_obstruction()) {
    stop_door();
    reverse_direction();
    trigger_alarm();
}
```

#### 4. 🛑 Emergency Stop
- Hardware emergency stop button (hard-wired)
- Software timeout protection
- Watchdog timer for system crashes
- Fail-safe power loss handling

#### 5. ✅ Fail-Safe Design
- Door defaults to manual mode on power loss
- Relay wiring prevents unexpected activation
- Redundant sensor checking
- Network loss doesn't affect manual operation
- Battery backup for critical functions

#### 6. ⚡ Electrical Safety
- Follow local electrical codes (NEC in US, AS/NZS 3000 in Australia)
- Use properly rated components (min. 10A relay)
- Ensure proper grounding and bonding
- Use optocoupler isolation between ESP32 and relay
- Professional licensed electrician installation **REQUIRED**
- GFCI protection if installed in damp location

### Legal Disclaimer

⚠️ **READ CAREFULLY BEFORE USE**

**THIS SOFTWARE IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND.**

The author(s) assume **ZERO liability** for:
- Property damage
- Personal injury or death
- Code violations
- Insurance claim denials
- Legal consequences
- Loss of any kind

**By using this software, you agree that:**
- You are solely responsible for ensuring safe installation
- You will comply with all local building codes and regulations
- You will install appropriate safety devices (obstruction sensors, emergency stop, etc.)
- You will have installation reviewed by licensed professionals
- You understand garage doors can cause serious injury or death
- You release the author(s) from all liability

**Recommendations:**
- ✅ Consult licensed electrician for installation
- ✅ Consult licensed garage door technician for integration
- ✅ Install UL 325 compliant safety devices
- ✅ Check your insurance policy (DIY automation may void coverage)
- ✅ Add clear warning signs near door
- ✅ Test safety features regularly
- ✅ Maintain detailed installation documentation

**Regulatory Compliance:**
- USA: Must comply with UL 325 standard
- Australia: Must comply with AS 4065
- Europe: Must comply with EN 13241-1
- Canada: Must comply with CAN/ULC-S319

### Insurance Considerations

⚠️ Many homeowner's insurance policies **exclude coverage** for:
- DIY electrical work
- Unlicensed modifications to garage doors
- Smart home devices not professionally installed

**Before installation:**
1. Contact your insurance provider
2. Ask about coverage implications
3. Get written confirmation if coverage continues
4. Document professional installation if required

---

## 📁 Project Structure

```
ESP32-Smart-Garage-Door_Controller/
│
├── main/                           # Main application code
│   ├── main.cpp                    # Entry point, state machine, GPIO control
│   ├── application.c               # Web UI and JSON handlers
│   ├── application.h               # Application header
│   ├── http.c                      # HTTP server implementation
│   ├── http.h                      # HTTP header
│   ├── mqtt.c                      # MQTT client (ready, disabled)
│   ├── mqtt.h                      # MQTT header
│   ├── utils.c                     # Utility functions (time, string)
│   ├── utils.h                     # Utils header
│   └── CMakeLists.txt              # Main component build config
│
├── components/                     # Reusable components
│   │
│   ├── protocol_common/            # Network connectivity helpers
│   │   ├── include/
│   │   │   └── protocol_common.h
│   │   ├── src/
│   │   │   └── protocol_common.c
│   │   └── CMakeLists.txt
│   │
│   └── ultrasonic/                 # Ultrasonic sensor driver (optional)
│       ├── include/
│       │   └── ultrasonic.h
│       ├── src/
│       │   └── ultrasonic.c
│       └── CMakeLists.txt
│
├── certs/                          # TLS certificates
│   └── dogdoorcert.pem             # Certificate bundle for HTTPS
│
├── build/                          # Build output (auto-generated)
│   ├── bootloader/                 # Bootloader binary
│   ├── partition_table/            # Partition table binary
│   ├── GarageDoor.bin              # Main firmware binary
│   └── ...                         # Other build artifacts
│
├── diagram.json                    # Wokwi circuit definition
├── wokwi.toml                      # Wokwi simulation config
├── sdkconfig                       # ESP-IDF configuration (generated)
├── sdkconfig.defaults              # Default configuration values
├── CMakeLists.txt                  # Root build configuration
├── partitions.csv                  # Flash partition layout (if custom)
├── README.md                       # This file
└── LICENSE                         # CC0-1.0 Public Domain
```

### Key Files Explained

#### **main/main.cpp**
- Application entry point (`app_main()`)
- FreeRTOS task creation
- Door state machine implementation
- GPIO initialization and control
- Sensor monitoring logic
- NTP time synchronization
- WiFi event handling

#### **main/application.c/h**
- HTTP request handlers
- HTML page generation
- JSON response formatting
- System status aggregation
- Door control commands

#### **main/http.c/h**
- ESP-IDF HTTP server wrapper
- URI handler registration
- Request parsing and routing
- Response header management

#### **main/mqtt.c/h**
- MQTT client infrastructure
- Currently disabled (ready for use)
- Publish/subscribe functions
- Connection management

#### **main/utils.c/h**
- String formatting functions
- Time calculation utilities
- Uptime formatting
- Helper functions

#### **components/protocol_common/**
- WiFi connection management
- Network event handling
- Automatic reconnection logic
- IP address retrieval

#### **components/ultrasonic/**
- HC-SR04 ultrasonic sensor driver
- Optional component for distance sensing
- Could be used for alternate door position detection

---

## 🎓 Skills Demonstrated

This project showcases professional embedded systems development capabilities:

### Embedded Systems
✅ **ESP-IDF Framework** - Professional IoT development framework  
✅ **FreeRTOS** - Real-time operating system task management  
✅ **GPIO Control** - Hardware abstraction layer implementation  
✅ **Interrupt Handling** - Event-driven programming patterns  
✅ **Memory Management** - Heap allocation, stack monitoring, leak prevention  

### IoT Development
✅ **WiFi Stack** - Complete network connectivity implementation  
✅ **HTTP Server** - RESTful API design and implementation  
✅ **JSON Parsing** - Data serialization for API responses  
✅ **OTA Updates** - Remote firmware update capability  
✅ **NTP Client** - Network time synchronization  

### State Machine Design
✅ **Finite State Machine** - Clean state management patterns  
✅ **Event-Driven Architecture** - Responsive system design  
✅ **Timeout Handling** - Robust error recovery  
✅ **State Transitions** - Logical flow control  

### Real-Time Systems
✅ **Sensor Polling** - Continuous hardware monitoring  
✅ **Debouncing** - Signal conditioning and noise filtering  
✅ **Task Synchronization** - Multi-threaded coordination  
✅ **Timing Constraints** - Meeting real-time deadlines  

### Hardware Integration
✅ **Relay Control** - Power electronics interfacing  
✅ **Sensor Reading** - Digital input processing  
✅ **Circuit Design** - Component selection and integration  
✅ **Power Management** - Supply voltage considerations  

### Software Architecture
✅ **Modular Design** - Component-based architecture  
✅ **Separation of Concerns** - Clean code organization  
✅ **API Design** - RESTful endpoint structure  
✅ **Documentation** - Professional technical writing  

### Applicable Job Roles

This project demonstrates skills for:
- **Embedded Software Engineer** - C/C++, ESP-IDF, hardware control
- **IoT Developer** - Network protocols, sensors, cloud connectivity
- **Firmware Engineer** - Low-level programming, state machines
- **Platform Engineer** - System integration, API design
- **Full-Stack IoT Engineer** - Hardware + firmware + web interface

---

## 🚀 Future Enhancements

### Phase 1: Core Safety Features (CRITICAL)
**Priority: HIGH - Required for any real-world use**

- [ ] **Authentication System**
  - [ ] Username/password web login
  - [ ] JWT token-based API authentication
  - [ ] Role-based access control (admin/user)
  - [ ] Session management and timeout
  - [ ] HTTPS with valid TLS certificate

- [ ] **Obstruction Detection**
  - [ ] Infrared beam sensor across doorway
  - [ ] Pressure sensors on door edge
  - [ ] Motor current sensing for abnormal resistance
  - [ ] Automatic stop and reverse on obstruction
  - [ ] Audible alarm on obstruction event

- [ ] **Emergency Systems**
  - [ ] Hardware emergency stop button
  - [ ] Manual override switch
  - [ ] Battery backup for critical functions
  - [ ] Fail-safe relay wiring
  - [ ] Watchdog timer for crash recovery

### Phase 2: Enhanced Features (SHORT-TERM)
**Priority: MEDIUM - Improves usability and reliability**

- [ ] **Mobile Applications**
  - [ ] iOS app (Swift/SwiftUI)
  - [ ] Android app (Kotlin/Jetpack Compose)
  - [ ] Push notifications for door events
  - [ ] Geofencing (auto-open when approaching)
  - [ ] Biometric authentication

- [ ] **Better State Management**
  - [ ] Persistent state storage (NVS flash)
  - [ ] State history logging with timestamps
  - [ ] Anomaly detection (stuck door, slow operation)
  - [ ] Predictive maintenance alerts
  - [ ] Usage statistics and analytics

- [ ] **Advanced Monitoring**
  - [ ] Temperature/humidity sensors
  - [ ] Door usage frequency tracking
  - [ ] Operation time measurements
  - [ ] Battery level monitoring (if using backup)
  - [ ] Network connectivity status

### Phase 3: Smart Home Integration (MEDIUM-TERM)
**Priority: MEDIUM - Integration with popular platforms**

- [ ] **MQTT Integration**
  - [ ] Enable MQTT client (currently disabled)
  - [ ] Publish door state changes
  - [ ] Subscribe to remote commands
  - [ ] Home Assistant MQTT discovery
  - [ ] QoS levels and retained messages

- [ ] **Voice Control**
  - [ ] Amazon Alexa skill
  - [ ] Google Assistant action
  - [ ] Apple HomeKit integration
  - [ ] Voice command processing
  - [ ] Multi-language support

- [ ] **Platform Integration**
  - [ ] Home Assistant custom component
  - [ ] SmartThings integration
  - [ ] OpenHAB binding
  - [ ] IFTTT webhooks
  - [ ] Zapier integration

### Phase 4: Advanced Features (LONG-TERM)
**Priority: LOW - Nice-to-have enhancements**

- [ ] **Computer Vision**
  - [ ] ESP32-CAM for visual door monitoring
  - [ ] License plate recognition
  - [ ] Person detection
  - [ ] Package delivery detection
  - [ ] Image capture on events

- [ ] **Machine Learning**
  - [ ] Usage pattern recognition
  - [ ] Predictive opening (learns your schedule)
  - [ ] Anomaly detection algorithms
  - [ ] Optimization recommendations
  - [ ] TensorFlow Lite integration

- [ ] **Multi-Door Support**
  - [ ] Control multiple garage doors
  - [ ] Centralized management dashboard
  - [ ] Per-door configuration
  - [ ] Group operations
  - [ ] Master control panel

- [ ] **Cloud Backend**
  - [ ] Remote access from anywhere (not just local network)
  - [ ] Cloud-based logging and analytics
  - [ ] Firmware version management
  - [ ] Device fleet management
  - [ ] Backup and restore configuration

---

## 🏆 Related Projects

This garage door controller is part of a comprehensive IoT portfolio demonstrating diverse embedded systems skills:

### 🐾 Smart Pet Door System
**Technology**: ESP32, C/C++, RFID/NFC, servo motor control  
**Features**: Selective pet access based on RFID tags, activity logging, MQTT integration, battery management  
**Skills**: Motor control, RFID reading, power optimization, pet detection algorithms  
**Status**: Operational  

### 🏠 Home Automation Hub
**Technology**: Linux server, MQTT Broker (Mosquitto), MySQL/MariaDB, Redis, Python web interfaces  
**Features**: Centralized device management, data analytics, web dashboard, Home Assistant integration  
**Skills**: Full-stack development, database design, system administration, message queuing  
**Status**: Production use  

### 🎮 Published Mobile Games
**Invasion Defender** - Strategic space tower defense  
- Platform: iOS ([App Store Link](https://apps.apple.com/us/app/invasion-defender/id1032941189))
- Technology: C/C++, Unity/Cocos2D, Adobe Photoshop
- Status: Published (2015)

**Clever Bunny** - Puzzle platformer game  
- Platform: iOS (App Store)
- Technology: C#, Unity Engine, custom level editor
- Status: Published

**Skills**: Game design, graphics (Photoshop), mobile development, Unity/Unreal Engine

### 🤖 Visual Automation Framework (RPA)
**Technology**: Python, OpenCV, Selenium, NumPy, PyAutoGUI  
**Features**: Computer vision-based automation, template matching, browser control, legacy system integration  
**Skills**: Computer vision, image processing, automation, pattern recognition  
**Repository**: [[Link to automation framework](https://github.com/adilsondias-engineer/visual_process_automation)]  
**Status**: Educational project demonstrating RPA techniques

---

## 📚 Learning Resources

### ESP32 & ESP-IDF
- [ESP-IDF Programming Guide](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/) - Official documentation
- [ESP32 Technical Reference Manual](https://www.espressif.com/sites/default/files/documentation/esp32_technical_reference_manual_en.pdf) - Hardware details
- [ESP32 Forum](https://esp32.com/) - Community support
- [Espressif GitHub](https://github.com/espressif/esp-idf) - Source code and examples

### FreeRTOS
- [FreeRTOS Documentation](https://www.freertos.org/Documentation/RTOS_book.html) - Comprehensive guide
- [FreeRTOS API Reference](https://www.freertos.org/a00106.html) - Function reference
- [Mastering the FreeRTOS Kernel](https://www.freertos.org/fr-content-src/uploads/2018/07/161204_Mastering_the_FreeRTOS_Real_Time_Kernel-A_Hands-On_Tutorial_Guide.pdf) - Free ebook

### IoT & Home Automation
- [Home Assistant Documentation](https://www.home-assistant.io/docs/) - Smart home platform
- [MQTT.org](https://mqtt.org/) - MQTT protocol specification
- [RESTful API Tutorial](https://restfulapi.net/) - API design best practices

### Hardware & Electronics
- [Random Nerd Tutorials - ESP32](https://randomnerdtutorials.com/projects-esp32/) - Excellent tutorials
- [ESP32 Pinout Reference](https://randomnerdtutorials.com/esp32-pinout-reference-gpios/) - GPIO guide
- [All About Circuits](https://www.allaboutcircuits.com/) - Electronics fundamentals

### YouTube Channels
- **Andreas Spiess** - Excellent ESP32 and IoT tutorials
- **GreatScott!** - Electronics projects and explanations
- **ElectroBOOM** - Electronics education (entertaining)
- **Ben Eater** - Computer architecture and embedded systems

### Books
- **"Programming Embedded Systems" by Michael Barr** - Embedded C fundamentals
- **"Making Embedded Systems" by Elecia White** - Design patterns
- **"The Car Hacker's Handbook" by Craig Smith** - Hardware security
- **"IoT Inc" by Bruce Sinclair** - IoT business and strategy

---

## 🤝 Contributing

Contributions welcome! Help improve this project's functionality, safety, and documentation.

### How to Contribute

1. **Fork the Repository**
   ```bash
   git clone https://github.com/adilsondias-engineer/ESP32-Smart-Garage-Door_Controller.git
   cd GarageDoor
   git checkout -b feature/my-enhancement
   ```

2. **Make Changes**
   - Follow existing code style
   - Add comments for complex logic
   - Update documentation
   - Test thoroughly

3. **Commit with Clear Messages**
   ```bash
   git commit -m "Add: Implement emergency stop button"
   git commit -m "Fix: Resolve sensor debouncing issue"
   git commit -m "Docs: Update API reference with examples"
   ```

4. **Submit Pull Request**
   - Describe changes clearly
   - Reference any related issues
   - Include test results
   - Update CHANGELOG if applicable

### Code Standards

- **Style**: Follow ESP-IDF coding conventions
- **Naming**: Use descriptive variable/function names
- **Comments**: JSDoc-style for functions
- **Functions**: Keep focused and small (<50 lines)
- **Error Handling**: Always check return values
- **Logging**: Use appropriate ESP_LOG levels

### Documentation Standards

- Update README for new features
- Include code examples where applicable
- Document configuration changes
- Add troubleshooting entries
- Keep diagrams current

### Testing Requirements

- Test on real ESP32 hardware
- Validate with Wokwi simulation
- Check for memory leaks
- Ensure proper error handling
- Verify sensor operation

### Priority Contribution Areas

**🔴 Critical (Safety)**:
- Authentication implementation
- Obstruction detection systems
- Emergency stop functionality
- Fail-safe mechanisms

**🟡 High Priority**:
- Mobile app development (iOS/Android)
- MQTT integration enablement
- Enhanced error recovery
- Unit test framework

**🟢 Medium Priority**:
- Web UI improvements
- Multi-language support
- Usage analytics
- Performance optimizations

**🔵 Nice to Have**:
- Tutorial videos
- Configuration wizard
- Alternative sensor support
- Additional platform integrations

---

## 📄 License

This project is dedicated to the **public domain** under the [CC0 1.0 Universal (CC0 1.0) Public Domain Dedication](https://creativecommons.org/publicdomain/zero/1.0/).

### What This Means

**You can freely:**
- ✅ Use commercially - Sell products based on this code
- ✅ Modify - Change anything without restrictions
- ✅ Distribute - Share with anyone
- ✅ Create derivatives - Build new projects
- ✅ No attribution required - Though appreciated!
- ✅ Private use - Use internally without sharing

### No Warranty

**This software is provided "AS IS", without warranty of any kind, express or implied, including but not limited to:**
- Warranties of merchantability
- Fitness for a particular purpose
- Non-infringement
- Error-free operation
- Security guarantees

**The author(s) are not liable for:**
- Property damage
- Personal injury or death
- Data loss
- Financial losses
- Legal consequences
- Any damages whatsoever

### Components with Different Licenses

- **ESP-IDF Framework**: Apache License 2.0
- **FreeRTOS**: MIT License
- **Ultrasonic Component**: BSD License (see component folder)

---

## 🙏 Acknowledgments

### Technology & Tools
- **Espressif Systems** - For the ESP32 SoC and excellent ESP-IDF framework
- **Wokwi** - For the amazing online simulator that makes hardware testing accessible
- **FreeRTOS** - For the robust real-time operating system
- **Open Source Community** - For countless libraries, examples, and support

### Learning & Inspiration
- **Home Assistant Community** - For IoT integration patterns and best practices
- **ESP32 Forums** - For troubleshooting help and creative solutions
- **Andreas Spiess** - For outstanding ESP32 tutorials on YouTube
- **Random Nerd Tutorials** - For comprehensive ESP32 guides and examples
- **GreatScott!** - For electronics education and project inspiration

### Special Thanks
- To everyone who contributes to open-source hardware and software
- To the maker community for sharing knowledge and encouraging learning
- To future contributors who will improve and extend this project
- To educators who make embedded systems accessible to everyone

---

## 📧 Contact & Support

### Author
**Adilson Dias**

- **GitHub**: [@adilsondias-engineer](https://github.com/adilsondias-engineer)
- **LinkedIn**: [Adilson Dias](https://linkedin.com/in/adilsondias)
- **Project**: Created as ESP-IDF learning project for IoT home automation

### Getting Help

**For Technical Issues**:
1. Check [Troubleshooting](#-troubleshooting) section
2. Search [GitHub Issues](https://github.com/adilsondias-engineer/ESP32-Smart-Garage-Door_Controller/issues)
3. Review [ESP32 Forum](https://esp32.com/) for similar questions
4. Open new issue with detailed information

**For General Questions**:
- [GitHub Discussions](https://github.com/adilsondias-engineer/ESP32-Smart-Garage-Door_Controller/discussions)
- [ESP32 Subreddit](https://www.reddit.com/r/esp32/)
- [Stack Overflow](https://stackoverflow.com/questions/tagged/esp32) (tag: `esp32`)

**For Collaboration**:
- Open to consulting opportunities
- Available for technical interviews
- Interested in IoT/embedded projects
- Portfolio reviews and feedback welcome

---

## 📊 Project Statistics

![Version](https://img.shields.io/badge/Version-1.0-blue)
![Lines of Code](https://img.shields.io/badge/Lines%20of%20Code-~2500-green)
![Components](https://img.shields.io/badge/Components-4-orange)
![Languages](https://img.shields.io/badge/Languages-C%2FC%2B%2B-red)
![Status](https://img.shields.io/badge/Status-Educational-yellow)

**Development Timeline**:
- Planning & Design: 1 week
- Core Implementation: 2 weeks  
- Hardware Testing: 1 week
- Documentation: 1 week
- **Total**: ~5 weeks part-time

**Code Metrics**:
- Main application: ~800 lines
- HTTP server: ~400 lines
- Utilities: ~300 lines
- MQTT (stub): ~200 lines
- Comments & documentation: ~800 lines
- **Total**: ~2,500 lines

---

## 🗺️ Version History

### Version 1.0 (Current) - October 2024
✅ Basic door control (open/close)  
✅ Position sensor integration  
✅ HTTP web interface  
✅ JSON REST API  
✅ NTP time synchronization  
✅ OTA update infrastructure  
✅ Wokwi simulation support  
✅ Comprehensive documentation  

### Planned Versions

**Version 1.1** - Authentication & Security
- Web login system
- API key authentication
- HTTPS support
- Security audit

**Version 2.0** - Safety Features
- Obstruction detection
- Emergency stop
- Enhanced error handling
- Safety certifications

**Version 3.0** - Smart Home Integration
- MQTT enabled
- Home Assistant integration
- Voice control support
- Mobile apps

---

**⭐ If you find this project helpful, please star it on GitHub!**

**🐛 Found a bug? [Open an issue](https://github.com/adilsondias-engineer/ESP32-Smart-Garage-Door_Controller/issues)**

**💡 Have an idea? [Start a discussion](https://github.com/adilsondias-engineer/ESP32-Smart-Garage-Door_Controller/discussions)**

---

*Last Updated: October 2024*  
*ESP-IDF Version: v4.4+*  
*Hardware: ESP32 DevKit*

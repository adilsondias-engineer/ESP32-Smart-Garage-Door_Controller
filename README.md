-> AI generated documentation with the following prompt:
Generate @README.md 

```markdown
# ESP32 Garage Door Controller

An IoT-enabled garage door controller built on ESP32 using ESP-IDF. This project provides remote monitoring and control of a garage door through a web interface or API(response JSON), with support for OTA updates and Wokwi simulation.

## Features

- 🚪 **Remote Door Control** - Open and close garage door via web interface
- 📊 **Real-time Status Monitoring** - Track door status (OPEN, OPENING, CLOSING, CLOSED)
- 🌐 **Web Interface** - Simple HTTP server with control buttons and status display
- 📡 **JSON API** - RESTful endpoints for system integration
- 🔄 **OTA Updates** - Over-the-air firmware update capability
- ⏰ **NTP Time Sync** - Automatic time synchronization with Australian NTP servers
- 🔌 **Position Sensors** - Hardware sensors detect door open/closed states
- 🧪 **Wokwi Simulation** - Test and debug using Wokwi simulator

## Hardware Requirements

### Components
- ESP32 DevKit board
- Single-channel relay module (5V)
- 2x magnetic/limit switches (door position sensors)
- Red LED (status indicator)
- 220Ω resistor
- Garage door opener with button terminals

### GPIO Pin Configuration

| Component | GPIO Pin | Description |
|-----------|----------|-------------|
| Relay Control | GPIO 13 | Controls garage door opener relay |
| Open Sensor | GPIO 34 | Detects when door is fully open |
| Close Sensor | GPIO 35 | Detects when door is fully closed |
| Status LED | GPIO 18 | Visual indicator (Wokwi simulation) |

## Software Architecture

### Core Components
- **HTTP Server** - Handles web interface and API requests
- **MQTT Client** - (Infrastructure ready, currently disabled)
- **Door State Machine** - Manages door status transitions
- **NTP Client** - Time synchronization
- **OTA Handler** - Firmware update management

### Door States
- `OPEN` - Door fully open (sensor triggered)
- `OPENING` - Door opening in progress
- `CLOSING` - Door closing in progress
- `CLOSED` - Door fully closed (sensor triggered)

## Building and Flashing

### Prerequisites
- ESP-IDF v4.4 or later
- Python 3.7+
- USB cable for ESP32 programming

### Build Instructions

1. Clone the repository:
```bash
git clone <repository-url>
cd GarageDoor
```

2. Set up ESP-IDF environment:
```bash
. $HOME/esp/esp-idf/export.sh
```

3. Configure WiFi credentials:
```bash
idf.py menuconfig
```
Navigate to: `Example Connection Configuration` → Set WiFi SSID and Password

4. Build the project:
```bash
idf.py build
```

5. Flash to ESP32:
```bash
idf.py -p <PORT> flash monitor
```

## Configuration

### WiFi Setup
Configure WiFi credentials via `idf.py menuconfig` under:
- `Example Connection Configuration`

### TLS Certificates
Place your certificate bundle in:
- `certs/dogdoorcert.pem`

Update path in `sdkconfig.defaults` if needed.

### Time Zone
Default timezone is set to AEST (Australian Eastern Standard Time). Modify in `main.cpp`:
```cpp
setenv("TZ", "AEST-10AEDT,M10.1.0,M4.1.0/3", 1);
```

## API Endpoints

### Web Interface
- `GET /` - Main control page with door status and control buttons
- `GET /index` - Alias for main page

### JSON API
- `GET /status` - Returns JSON with system info and door status
  ```json
  {
    "SystemTime": "2024-10-18 12:34:56",
    "UpTime": "1d 2h 34m 56s",
    "DoorStatus": "CLOSED"
  }
  ```

### Control Endpoints
- `POST /openDoor` - Trigger door to open
- `POST /closeDoor` - Trigger door to close

## Wokwi Simulation

The project includes a Wokwi configuration for testing without hardware:

1. Install Wokwi CLI or use Wokwi VS Code extension
2. Build the project with `idf.py build`
3. Run simulation: `wokwi-cli --timeout 30000`

The simulation includes:
- ESP32 DevKit
- Relay module
- Status LED with resistor
- Serial monitor for debugging

## Project Structure

```
GarageDoor/
├── main/
│   ├── main.cpp          # Application entry point and door logic
│   ├── application.c/h   # Web interface and status handlers
│   ├── http.c/h          # HTTP server implementation
│   ├── mqtt.c/h          # MQTT client (ready for use)
│   └── utils.c/h         # Utility functions
├── components/
│   ├── protocol_common/  # WiFi/Ethernet connection helpers
│   └── ultrasonic/       # Ultrasonic sensor driver (optional)
├── certs/                # TLS certificates
├── diagram.json          # Wokwi circuit diagram
├── wokwi.toml           # Wokwi configuration
└── CMakeLists.txt       # Build configuration
```

## Operation

1. **Power On**: ESP32 boots and connects to WiFi
2. **Initialization**: 
   - GPIO pins configured
   - NTP sync initiated
   - HTTP server started
   - Door status checked
3. **Access**: Navigate to ESP32's IP address in web browser
4. **Control**: Use "Open Door" or "Close Door" buttons
5. **Monitoring**: Door status updates automatically based on sensor inputs

## Safety Considerations

⚠️ **IMPORTANT**: This is a hobby project. For production use:
- Add manual override functionality
- Implement obstruction detection
- Include emergency stop mechanism
- Follow local electrical codes
- Consider fail-safe relay wiring
- Add user authentication to web interface

## Troubleshooting

### WiFi Connection Issues
- Check credentials in menuconfig
- Verify router is in range
- Check ESP32 serial output for connection status

### Door Not Responding
- Verify relay wiring and connections
- Check relay module power supply (5V)
- Confirm GPIO 13 is correctly connected
- Test relay manually with jumper wire

### Sensor Problems
- Check sensor connections (GPIO 34, 35)
- Verify sensors trigger HIGH when activated
- Use serial monitor to debug sensor readings

## License

This project is based on Espressif's ESP-IDF examples and includes components under various licenses:
- Main code: CC0-1.0 (Public Domain)
- Ultrasonic driver: BSD License

## Contributing

Contributions welcome! Please submit pull requests or open issues for bugs and feature requests.

## Author
Adilson Dias

Created as an ESP-IDF learning project for IoT home automation.
```
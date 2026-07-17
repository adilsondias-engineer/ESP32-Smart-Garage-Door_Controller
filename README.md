# ESP32 Smart Garage Door Controller

ESP32/ESP-IDF/FreeRTOS controller that automates a standard garage door opener via relay-triggered button simulation, with dual reed-switch position feedback, an HTTP status/control interface, and MQTT + OTA infrastructure.

## Overview

Rather than driving a motor directly, this controller taps into an existing garage door opener's physical wall-button circuit: a relay (GPIO 13) closes momentarily to simulate a button press, while two magnetic reed switches (GPIO 34/35) independently confirm actual door position (open/closed). This mirrors how the opener's own remote works, so no modification to the opener itself is required.

## Features

- **Relay-based door control** — 1-second GPIO pulse simulates the opener's physical button press
- **Dual reed-switch position sensing** — independent open/closed confirmation, not just a timer guess
- **Door state machine** — `OPEN / OPENING / CLOSING / CLOSED`
- **HTTP web interface** — status dashboard (system time, uptime, door status) + open/close controls
- **JSON status endpoint** — for external integration/automation
- **NTP time sync** — Australian pool servers
- **MQTT** — structured JSON status publishing (uptime, door state, IP/heartbeat), implemented in `mqtt.c`; decoupled via a dedicated task + queue so publishing never blocks the door-control task. *(Disabled in this published build — broker credentials in `mqtt.c` are placeholders; uncomment `mqtt_init()` in `main.cpp` and set your own broker details to enable.)*
- **OTA update scaffolding** — `esp_https_ota` + cert bundle included in the build; not yet wired into `app_main`.

## Architecture

```
app_main()
 ├── GPIO init (relay output, 2x reed-switch inputs)
 ├── NVS init
 ├── WiFi/network init (example_connect)
 ├── NTP time sync
 ├── HTTP server init + start
 ├── (MQTT init — currently disabled, see Features)
 └── Handle_Door_Task  (single FreeRTOS task)
       ├── on OPEN command: pulse relay, poll open-sensor, update state
       ├── on CLOSE command: pulse relay, poll closed-sensor, update state
       └── idle: continuously reconcile state against both reed switches
```

## Hardware

| Function | GPIO | Notes |
|---|---|---|
| Relay control (door trigger) | 13 | Output, active HIGH, 1s pulse |
| Open position sensor | 34 | Input-only pin, reed switch |
| Closed position sensor | 35 | Input-only pin, reed switch |

GPIO 34/35 are input-only ESP32 pins with no internal pull resistors — use external pull-down resistors or wire sensors for active-low if needed.

## Building

```bash
git clone https://github.com/adilsondias-engineer/ESP32-Smart-Garage-Door_Controller.git
cd ESP32-Smart-Garage-Door_Controller
idf.py menuconfig   # set WiFi SSID/password under Example Connection Configuration
idf.py build
idf.py -p <PORT> flash monitor
```

## Web Interface

Once connected to WiFi:
```
http://<device-ip>/
```
- `/` — status dashboard
- `/openDoor` — POST to trigger open
- `/closeDoor` — POST to trigger close

## Status

Functional, in personal use. MQTT and OTA are implemented and running in the author's deployed unit; both are left disabled/unwired in this published version pending credential cleanup (see Features above).

## Simulation

A Wokwi simulation config (`wokwi.toml`, `diagram.json`) is included for reference but is not actively maintained.

## License

CC0-1.0 (public domain) for original application code. Third-party components retain their own licenses:
- ESP-IDF: Apache License 2.0
- FreeRTOS: MIT License

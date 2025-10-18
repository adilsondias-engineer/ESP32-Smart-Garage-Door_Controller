/*
 * SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include <stdio.h>
#include <inttypes.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_chip_info.h"
#include "esp_system.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "nvs_handle.hpp"
#include "driver/gpio.h"
#include <esp_sntp.h>

#include "main.h"

#include "utils.h"
#include "application.h"
#include "mqtt.h"
#include "http.h"

// #include <sys/time.h>
#include "esp_event.h"
#include "esp_ota_ops.h"
#include "esp_http_client.h"
#include "esp_https_ota.h"
#include "protocol_examples_common.h"
#include "string.h"
// #ifdef CONFIG_EXAMPLE_USE_CERT_BUNDLE
#include "esp_crt_bundle.h"
// #endif

#include <sys/socket.h>
#if CONFIG_EXAMPLE_CONNECT_WIFI
#include "esp_wifi.h"
#endif

#define GARAGE_DOOR_RELAY_PIN GPIO_NUM_13
#define GARAGE_DOOR_OPEN_SENSOR_PIN GPIO_NUM_34
#define GARAGE_DOOR_CLOSED_SENSOR_PIN GPIO_NUM_35
#define HIGH 1
#define LOW 0
// #define DOG_APP_TAG "Dog App"
// #define TAG "Dog App"
// #define MQTT_STATUS_TOPIC MQTT_TOPIC "/status"

httpd_handle_t http_server;

static const char *GARAGE_DOOR_APP_TAG = "Garage Door App";
static const char *TAG = "Garage Door App";

enum doorStatus
{
    OPEN,
    OPENING,
    CLOSING,
    CLOSED
};

std::unique_ptr<nvs::NVSHandle> handle = NULL;
esp_err_t err = NULL;
int currentDoorStatus = CLOSED;
bool buttonPressed = false;

static void Handle_Door_Task(void *pvParameter);

static void _ntp_set_time_task(); // void *pvParameter
void _ntp_time_sync_notification_cb(struct timeval *tv);

void _ntp_set_time_task() // void *pvParameter
{
    // while (true)
    //{
    esp_sntp_setoperatingmode(SNTP_OPMODE_POLL);
    esp_sntp_setservername(0, "0.au.pool.ntp.org");
    esp_sntp_setservername(1, "1.au.pool.ntp.org");
    esp_sntp_setservername(2, "2.au.pool.ntp.org");
    esp_sntp_setservername(3, "3.au.pool.ntp.org");
    sntp_set_time_sync_notification_cb(_ntp_time_sync_notification_cb);
    esp_sntp_init();

    // printf("Date/Time updated at: %s\n", get_system_time_str());
    // vTaskDelay(pdMS_TO_TICKS(300000)); // 1s
    // }

    /* A task should NEVER return */
    // vTaskDelete(NULL);
}

void _ntp_time_sync_notification_cb(struct timeval *tv)
{
    printf("Date/Time updated at: %s\n", get_system_time_str());
}

//-----------------------------------------------------------------------------
extern "C" void openDoor_http_handle()
{
    currentDoorStatus = OPENING;
    buttonPressed = true;
}

extern "C" void closeDoor_http_handle()
{
    currentDoorStatus = CLOSING;
    buttonPressed = true;
}

extern "C" const char *getDoorStatus()
{
    const char *status = "NOT DEFINED";

    if (!buttonPressed){
        if (gpio_get_level(GARAGE_DOOR_OPEN_SENSOR_PIN) == HIGH)
        {
            currentDoorStatus = OPEN;
        //  gpio_set_level(GARAGE_DOOR_RELAY_PIN, LOW);
        }
        if (gpio_get_level(GARAGE_DOOR_CLOSED_SENSOR_PIN) == HIGH)
        {
            currentDoorStatus = CLOSED;
        //  gpio_set_level(GARAGE_DOOR_RELAY_PIN, LOW);
        }
    }

    switch (currentDoorStatus)
    {
    case OPEN:
        status = "OPEN";
        break;
    case CLOSED:
        status = "CLOSED";
        break;
    case OPENING:
        status = "OPENING";
        break;
    case CLOSING:
        status = "CLOSING";
        break;
    }
    return status;
}

extern "C" void app_main(void)
{

    // AEST-10AEDT,M10.1.0,M4.1.0/3
    setenv("TZ", "AEST-10AEDT,M10.1.0,M4.1.0/3", 1);
    tzset();

    ESP_LOGI(GARAGE_DOOR_APP_TAG, "Starting ... \n");
    esp_rom_gpio_pad_select_gpio(GARAGE_DOOR_RELAY_PIN);
    gpio_set_direction(GARAGE_DOOR_RELAY_PIN, GPIO_MODE_OUTPUT);

    esp_rom_gpio_pad_select_gpio(GARAGE_DOOR_OPEN_SENSOR_PIN);
    gpio_set_direction(GARAGE_DOOR_OPEN_SENSOR_PIN, GPIO_MODE_INPUT);
    // gpio_set_pull_mode(GARAGE_DOOR_OPEN_SENSOR_PIN, GPIO_PULLDOWN_ONLY);

    esp_rom_gpio_pad_select_gpio(GARAGE_DOOR_CLOSED_SENSOR_PIN);
    gpio_set_direction(GARAGE_DOOR_CLOSED_SENSOR_PIN, GPIO_MODE_INPUT);

    ESP_LOGI(GARAGE_DOOR_APP_TAG, "... Starting OTA\n");
    // Initialize NVS.
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        // 1.OTA app partition table has a smaller NVS partition size than the non-OTA
        // partition table. This size mismatch may cause NVS initialization to fail.
        // 2.NVS partition contains data in new format and cannot be recognized by this version of code.
        // If this happens, we erase NVS partition and initialize NVS again.
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    // get_sha256_of_partitions();

    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    /* This helper function configures Wi-Fi or Ethernet, as selected in menuconfig.
     * Read "Establishing Wi-Fi or Ethernet Connection" section in
     * examples/protocols/README.md for more information about this function.
     */
    ESP_ERROR_CHECK(example_connect());

#if CONFIG_EXAMPLE_CONNECT_WIFI
    /* Ensure to disable any WiFi power save mode, this allows best throughput
     * and hence timings for overall OTA operation.
     */
    esp_wifi_set_ps(WIFI_PS_NONE);
#endif // CONFIG_EXAMPLE_CONNECT_WIFI

    //  xTaskCreate(&simple_ota_example_task, "ota_example_task", 8192, NULL, 5, NULL);

    // xTaskCreate(_ntp_set_time_task, "_ntp_set_time_task", 2048, NULL, 1, NULL);
    _ntp_set_time_task();
    http_init();
    http_start_webserver(&http_server);
    // mqtt_init();
    application_init();

    // Read
    ESP_LOGI(GARAGE_DOOR_APP_TAG, "Checking current door state ...\n");
    // int32_t restart_counter = 0; // value will default to 0, if not set yet in NVS
    // err = handle->get_item("currentDoorStatus", currentDoorStatus);

    ESP_LOGI(GARAGE_DOOR_APP_TAG, "Current door state : %d\n", currentDoorStatus);

    ESP_LOGI(GARAGE_DOOR_APP_TAG, "Creating tasks .... \n");
    xTaskCreate(Handle_Door_Task, "Handle_Door_Task", 8192, NULL, 1, NULL);

    gpio_dump_io_configuration(stdout, (1ULL << GARAGE_DOOR_RELAY_PIN));

    ESP_LOGI(GARAGE_DOOR_APP_TAG, "Running ... \n");

    /// ESP_LOGI(TAG, "[APP] Startup..");
    ESP_LOGI(GARAGE_DOOR_APP_TAG, "[APP] Free memory: %" PRIu32 " bytes", esp_get_free_heap_size());
    ESP_LOGI(GARAGE_DOOR_APP_TAG, "[APP] IDF version: %s", esp_get_idf_version());
}

void Handle_Door_Task(void *params)
{

    while (true)
    {

        //     ESP_LOGI(GARAGE_DOOR_APP_TAG, "Handle_Door_Task  GARAGE_DOOR_OPEN_SENSOR_PIN %d\n",  gpio_get_level(GARAGE_DOOR_OPEN_SENSOR_PIN));
        //     ESP_LOGI(GARAGE_DOOR_APP_TAG, "Handle_Door_Task  GARAGE_DOOR_CLOSED_SENSOR_PIN %d\n",  gpio_get_level(GARAGE_DOOR_CLOSED_SENSOR_PIN));

        //   ESP_LOGI(DOG_APP_TAG, "Handle_Door_Task currentDoorStatus %d\n", currentDoorStatus);
        if (currentDoorStatus == OPENING && buttonPressed) // open the door
        {
            /*  if (gpio_get_level(GARAGE_DOOR_RELAY_PIN) == 1)
            {
                gpio_set_level(GARAGE_DOOR_RELAY_PIN, LOW);
            }*/
            gpio_set_level(GARAGE_DOOR_RELAY_PIN, HIGH);
            vTaskDelay(pdMS_TO_TICKS(1000)); // 1s
            gpio_set_level(GARAGE_DOOR_RELAY_PIN, LOW);
            //  ESP_LOGI(GARAGE_DOOR_APP_TAG, "Handle_Door_Task OPENING currentDoorStatus %d\n", currentDoorStatus);
            // step.setDir(true);
            // currentDoorStatus = OPEN;
            if (gpio_get_level(GARAGE_DOOR_OPEN_SENSOR_PIN) == HIGH)
            {
                currentDoorStatus = OPEN;
                //gpio_set_level(GARAGE_DOOR_RELAY_PIN, LOW);
            }
            buttonPressed = false;
        }
        else if (currentDoorStatus == CLOSING && buttonPressed)
        {

          /*  if (gpio_get_level(GARAGE_DOOR_RELAY_PIN) == 1)
            {
                gpio_set_level(GARAGE_DOOR_RELAY_PIN, LOW);
            }*/
            gpio_set_level(GARAGE_DOOR_RELAY_PIN, HIGH);
            vTaskDelay(pdMS_TO_TICKS(1000)); // 1s
            gpio_set_level(GARAGE_DOOR_RELAY_PIN, LOW);
            //  ESP_LOGI(GARAGE_DOOR_APP_TAG, "Handle_Door_Task  expected CLOSING=2 currentDoorStatus %d\n", currentDoorStatus);
            //  currentDoorStatus = CLOSED;

            if (gpio_get_level(GARAGE_DOOR_CLOSED_SENSOR_PIN) == HIGH)
            {
                currentDoorStatus = CLOSED;
              //  gpio_set_level(GARAGE_DOOR_RELAY_PIN, LOW);
            }
            buttonPressed = false;
        }
        else // if (currentDoorStatus == OPENING || currentDoorStatus == CLOSING)
        {

            if (gpio_get_level(GARAGE_DOOR_OPEN_SENSOR_PIN) == HIGH)
            {
                currentDoorStatus = OPEN;
               // gpio_set_level(GARAGE_DOOR_RELAY_PIN, LOW);
            }
            else if (gpio_get_level(GARAGE_DOOR_CLOSED_SENSOR_PIN) == HIGH)
            {
                currentDoorStatus = CLOSED;
                //gpio_set_level(GARAGE_DOOR_RELAY_PIN, LOW);
            }
            
            buttonPressed = false;
        }
        vTaskDelay(pdMS_TO_TICKS(1000)); // 1s
                                         // ESP_LOGI(GARAGE_DOOR_APP_TAG, "Handle_Door_Task  currentDoorStatus %d\n", currentDoorStatus);
    } // for

    /* A task should NEVER return */
    vTaskDelete(NULL);
}
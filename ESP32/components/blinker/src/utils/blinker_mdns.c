#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_log.h"
#include "mdns.h"
#include <sys/socket.h>
#include <netdb.h>
#include "blinker_utils.h"

// static const char *TAG = "blinker_mdns";

static bool m_init = false;

esp_err_t blinker_mdns_init(const char *host_name)
{
    // static bool m_init = false;
    if (m_init) {
        blinker_mdns_free();
    }
    
    m_init = true;

    ESP_ERROR_CHECK(mdns_init());
    ESP_ERROR_CHECK(mdns_hostname_set(host_name));
    ESP_ERROR_CHECK(mdns_instance_name_set("blinker"));

    mdns_txt_item_t serviceTxtData[1] = {
        {"deviceName", host_name}
    };

    ESP_ERROR_CHECK(mdns_service_add("blinker", "_blinker", "_tcp", 81, serviceTxtData, 1));

    return ESP_OK;
}

esp_err_t blinker_mdns_free(void)
{
    if (m_init) {
        m_init = false;

        mdns_free();
    }

    return ESP_OK;
}
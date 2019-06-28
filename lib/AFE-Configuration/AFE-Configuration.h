/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: https://www.smartnydom.pl/afe-firmware-pl/ */

#ifndef _AFE_Configuration_h
#define _AFE_Configuration_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#define SERVER_CMD_SAVE 1
#define SERVER_CMD_NONE 0

/* Device operating modes */
#define MODE_NORMAL 0
#define MODE_CONFIGURATION 1
#define MODE_ACCESS_POINT 2
#define MODE_NETWORK_NOT_SET 4
#define MODE_FIRST_TIME_LAUNCH 5

/* APIs */
#define API_HTTP 0
#define API_MQTT 1
#define API_DOMOTICZ 2

#define MAX_MQTT_TOPIC_LENGTH 70

/* URLs to AFE WebService */
#define AFE_URL_VALIDATE_KEY "http://api.smartnydom.pl/key/validate/"
#define AFE_URL_ADD_KEY "http://api.smartnydom.pl/key/add/"
#define AFE_KEY_FREQUENCY_VALIDATION 1440

/* Config sites IDs */
#define AFE_CONFIG_SITE_INDEX 0
#define AFE_CONFIG_SITE_DEVICE 1
#define AFE_CONFIG_SITE_NETWORK 2
#define AFE_CONFIG_SITE_MQTT 3
#define AFE_CONFIG_SITE_MQTT_TOPICS 4
#define AFE_CONFIG_SITE_DOMOTICZ 5
#define AFE_CONFIG_SITE_LED 6
#define AFE_CONFIG_SITE_RELAY 7
#define AFE_CONFIG_SITE_SWITCH 8
#define AFE_CONFIG_SITE_PASSWORD 9
#define AFE_CONFIG_SITE_PRO_VERSION 10
#define AFE_CONFIG_SITE_ANALOG_INPUT 11
#define AFE_CONFIG_SITE_EXIT 12
#define AFE_CONFIG_SITE_RESET 13
#define AFE_CONFIG_SITE_POST_RESET 14
#define AFE_CONFIG_SITE_FIRST_TIME 15
#define AFE_CONFIG_SITE_FIRST_TIME_CONNECTING 16
#define AFE_CONFIG_SITE_UPGRADE 17
#define AFE_CONFIG_SITE_POST_UPGRADE 18

/* Types of the devices */

#if defined(DEVICE_SONOFF_BASIC_V1)
#define AFE_DEVICE_TYPE_NAME "Sonoff Basic V1"
#define AFE_DEVICE_TYPE_ID 1
#elif defined(DEVICE_SONOFF_4CH)
#define AFE_DEVICE_TYPE_NAME "Sonoff 4CH"
#define AFE_DEVICE_TYPE_ID 2
#elif defined(DEVICE_SONOFF_DUAL_R2)
#define AFE_DEVICE_TYPE_NAME "Sonoff Dual R2"
#define AFE_DEVICE_TYPE_ID 3
#elif defined(DEVICE_SONOFF_TH10)
#define AFE_DEVICE_TYPE_NAME "Sonoff TH10"
#define AFE_DEVICE_TYPE_ID 4
#elif defined(DEVICE_SONOFF_TH16)
#define AFE_DEVICE_TYPE_NAME "Sonoff TH16"
#define AFE_DEVICE_TYPE_ID 5
#elif defined(DEVICE_SONOFF_TOUCH_1G)
#define AFE_DEVICE_TYPE_NAME "Sonoff Touch 1G"
#define AFE_DEVICE_TYPE_ID 6
#elif defined(DEVICE_SONOFF_TOUCH_2G)
#define AFE_DEVICE_TYPE_NAME "Sonoff Touch 2G"
#define AFE_DEVICE_TYPE_ID 7
#elif defined(DEVICE_SONOFF_TOUCH_3G)
#define AFE_DEVICE_TYPE_NAME "Sonoff Touch 3G"
#define AFE_DEVICE_TYPE_ID 8
#elif defined(DEVICE_SHELLY_1)
#define AFE_DEVICE_TYPE_NAME "Shelly-1"
#define AFE_DEVICE_TYPE_ID 20
#else
#define AFE_DEVICE_TYPE_NAME "ESP Generic"
#define AFE_DEVICE_TYPE_ID 0
#endif

/* Basic Switch */
#if defined(T0_CONFIG)
#define FIRMWARE_VERSION "2.0.2"
#define FIRMWARE_TYPE 0

// ADC Measurments available for all devices expect Shelly-1
#ifndef DEVICE_SHELLY_1
#define CONFIG_HARDWARE_ADC_VCC
#endif

#define CONFIG_FUNCTIONALITY_RELAY
#define CONFIG_FUNCTIONALITY_RELAY_AUTOONOFF

#define CONFIG_HARDWARE_MAX_NUMBER_OF_RELAYS 4
#define CONFIG_HARDWARE_MAX_NUMBER_OF_SWITCHES 5
#define CONFIG_HARDWARE_MAX_NUMBER_OF_LEDS 5

#if defined(DEVICE_SONOFF_BASIC_V1)
#define CONFIG_HARDWARE_NUMBER_OF_RELAYS 1
#define CONFIG_HARDWARE_NUMBER_OF_SWITCHES 4
#define CONFIG_HARDWARE_NUMBER_OF_LEDS 2
#elif defined(DEVICE_SONOFF_4CH)
#define CONFIG_HARDWARE_NUMBER_OF_RELAYS 4
#define CONFIG_HARDWARE_NUMBER_OF_SWITCHES 4
#define CONFIG_HARDWARE_NUMBER_OF_LEDS 1
#elif defined(DEVICE_SONOFF_TOUCH_1G)
#define CONFIG_HARDWARE_NUMBER_OF_RELAYS 1
#define CONFIG_HARDWARE_NUMBER_OF_SWITCHES 1
#define CONFIG_HARDWARE_NUMBER_OF_LEDS 1
#elif defined(DEVICE_SONOFF_TOUCH_2G)
#define CONFIG_HARDWARE_NUMBER_OF_RELAYS 2
#define CONFIG_HARDWARE_NUMBER_OF_SWITCHES 2
#define CONFIG_HARDWARE_NUMBER_OF_LEDS 1
#elif defined(DEVICE_SONOFF_TOUCH_3G)
#define CONFIG_HARDWARE_NUMBER_OF_RELAYS 3
#define CONFIG_HARDWARE_NUMBER_OF_SWITCHES 3
#define CONFIG_HARDWARE_NUMBER_OF_LEDS 1
#elif defined(DEVICE_SHELLY_1)
#define CONFIG_HARDWARE_NUMBER_OF_RELAYS 1
#define CONFIG_HARDWARE_NUMBER_OF_SWITCHES 1
#define CONFIG_HARDWARE_NUMBER_OF_LEDS 0
#define HARDWARE_SWITCH_GPIO_DIGIT_INPUT // Sets switch DigitialPin to INPUT
#else
#define CONFIG_HARDWARE_NUMBER_OF_RELAYS 4
#define CONFIG_HARDWARE_NUMBER_OF_SWITCHES 5
#define CONFIG_HARDWARE_NUMBER_OF_LEDS 5
#endif

/* Enables API On/Off via APIs */
//#define CONFIG_FUNCTIONALITY_API_CONTROL

/* Basic Switch + DS18B20 */
#elif defined(T1_CONFIG)
#define FIRMWARE_VERSION "1.4.0"
#define FIRMWARE_TYPE 1
#define CONFIG_HARDWARE_DS18B20
#define CONFIG_FUNCTIONALITY_RELAY
#define CONFIG_FUNCTIONALITY_RELAY_AUTOONOFF
#define CONFIG_FUNCTIONALITY_THERMOSTAT
#define CONFIG_FUNCTIONALITY_THERMAL_PROTECTION
/* Basic Switch + DTHxx */
#elif defined(T2_CONFIG)
#define FIRMWARE_VERSION "1.4.0"
#define FIRMWARE_TYPE 2
#define CONFIG_HARDWARE_DHXX
#define CONFIG_FUNCTIONALITY_RELAY
#define CONFIG_FUNCTIONALITY_RELAY_AUTOONOFF
#define CONFIG_FUNCTIONALITY_THERMOSTAT
#define CONFIG_FUNCTIONALITY_HUMIDISTAT
#define CONFIG_FUNCTIONALITY_THERMAL_PROTECTION
/* 4xRelay + PIR */
#elif defined(T3_CONFIG)
#define FIRMWARE_TYPE 3
#define FIRMWARE_VERSION "1.3.0"
#define CONFIG_HARDWARE_PIR
#define CONFIG_FUNCTIONALITY_RELAY
#define CONFIG_FUNCTIONALITY_RELAY_CONTROL_AUTOONOFF_TIME
/* 4xRelay */
#elif defined(T4_CONFIG)
#define FIRMWARE_VERSION "1.3.1"
#define FIRMWARE_TYPE 4
#define CONFIG_FUNCTIONALITY_RELAY
#define CONFIG_FUNCTIONALITY_RELAY_AUTOONOFF
/* Gate */
#elif defined(T5_CONFIG)
#define FIRMWARE_VERSION "2.0.0.B1"
#define FIRMWARE_TYPE 5
#define CONFIG_FUNCTIONALITY_RELAY_CONTROL_AUTOONOFF_TIME
#define CONFIG_FUNCTIONALITY_GATE
//#define CONFIG_HARDWARE_ADC_VCC

#define CONFIG_HARDWARE_MAX_NUMBER_OF_RELAYS 2
#define CONFIG_HARDWARE_MAX_NUMBER_OF_SWITCHES 5
#define CONFIG_HARDWARE_MAX_NUMBER_OF_LEDS 3

#define CONFIG_HARDWARE_NUMBER_OF_RELAYS 2
#define CONFIG_HARDWARE_NUMBER_OF_SWITCHES 5
#define CONFIG_HARDWARE_NUMBER_OF_LEDS 3

/* Wheater Station */
#elif defined(T6_CONFIG)
#define FIRMWARE_VERSION "1.0.0"
#define FIRMWARE_TYPE 6
#define CONFIG_HARDWARE_BMX80
#define CONFIG_HARDWARE_HPMA115S0
#define CONFIG_HARDWARE_BH1750
#define CONFIG_FUNCTIONALITY_RELAY

#endif

/* Configs related to functionalities */

#ifdef CONFIG_FUNCTIONALITY_THERMOSTAT

#ifndef CONFIG_FUNCTIONALITY_REGULATOR
#define CONFIG_FUNCTIONALITY_REGULATOR
#endif

#endif

#ifdef CONFIG_FUNCTIONALITY_HUMIDISTAT

#ifndef CONFIG_FUNCTIONALITY_REGULATOR
#define CONFIG_FUNCTIONALITY_REGULATOR
#endif

#endif

/* Configs related to specyfic hardware */

#ifdef CONFIG_HARDWARE_DS18B20

#ifndef CONFIG_TEMPERATURE
#define CONFIG_TEMPERATURE
#endif

#endif

#ifdef CONFIG_HARDWARE_DHXX

#ifndef CONFIG_TEMPERATURE
#define CONFIG_TEMPERATURE
#endif

#ifndef CONFIG_HUMIDITY
#define CONFIG_HUMIDITY
#endif

#endif

#ifdef CONFIG_HARDWARE_BMX80
#ifndef CONFIG_TEMPERATURE
#define CONFIG_TEMPERATURE
#endif

#ifndef CONFIG_HUMIDITY
#define CONFIG_HUMIDITY
#endif

#ifndef CONFIG_PRESSURE
#define CONFIG_PRESSURE
#endif

#ifndef CONFIG_HARDWARE_I2C
#define CONFIG_HARDWARE_I2C
#endif

#ifndef CONFIG_DOMOTICZ_CUSTOME_SENSOR
#define CONFIG_DOMOTICZ_CUSTOME_SENSOR
#endif

#endif

#ifdef CONFIG_HARDWARE_BH1750
#ifndef CONFIG_HARDWARE_I2C
#define CONFIG_HARDWARE_I2C
#endif
#endif

#ifdef CONFIG_HARDWARE_HPMA115S0

#ifndef CONFIG_HARDWARE_UART
#define CONFIG_HARDWARE_UART
#endif

#ifndef CONFIG_DOMOTICZ_CUSTOME_SENSOR
#define CONFIG_DOMOTICZ_CUSTOME_SENSOR
#endif

#endif

#ifdef CONFIG_HARDWARE_ADC_VCC
#ifndef CONFIG_DOMOTICZ_CUSTOME_SENSOR
#define CONFIG_DOMOTICZ_CUSTOME_SENSOR
#endif
#endif

/* Below three enables Relay.autoTurnOff */
#ifdef CONFIG_FUNCTIONALITY_RELAY_AUTOONOFF
#ifndef CONFIG_RELAY_AUTOONOFF_LISTENER
#define CONFIG_RELAY_AUTOONOFF_LISTENER
#endif
#endif

#ifdef CONFIG_HARDWARE_PIR
#ifndef CONFIG_RELAY_AUTOONOFF_LISTENER
#define CONFIG_RELAY_AUTOONOFF_LISTENER
#endif
#endif

#ifdef CONFIG_FUNCTIONALITY_GATE
#ifndef CONFIG_RELAY_AUTOONOFF_LISTENER
#define CONFIG_RELAY_AUTOONOFF_LISTENER
#endif
#endif

/* Enabling hardware for Gate functionality: Gate and Contactron */
#ifdef CONFIG_FUNCTIONALITY_GATE
#ifndef CONFIG_HARDWARE_GATE
#define CONFIG_HARDWARE_GATE
#endif
#ifndef CONFIG_HARDWARE_CONTACTRON
#define CONFIG_HARDWARE_CONTACTRON
#endif
#endif
/* End of: Enabling hardware for Gate functionality: Gate and Contactron */

#endif

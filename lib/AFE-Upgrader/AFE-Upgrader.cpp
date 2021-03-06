/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#include "AFE-Upgrader.h"

AFEUpgrader::AFEUpgrader(AFEDataAccess *_Data, AFEDevice *_Device) {
  Data = _Data;
  Device = _Device;
  FirmwareConfiguration = Data->getFirmwareConfiguration();
}

/* It returns true if firmware has been upgraded */
boolean AFEUpgrader::upgraded() {
#ifdef DEBUG
  Serial << endl
         << "INFO: Firmware version (stored) T" << FirmwareConfiguration.type
         << "-" << FirmwareConfiguration.version << "-"
         << (FirmwareConfiguration.api == AFE_API_STANDARD
                 ? "Standard"
                 : (FirmwareConfiguration.api == AFE_API_DOMOTICZ ? "Domoticz"
                                                                  : "Unknwon"));
  Serial << endl
         << "INFO: Firmware version (booted) T" << AFE_FIRMWARE_TYPE << "-"
         << AFE_FIRMWARE_VERSION << "-"
         << (AFE_FIRMARE_API == AFE_API_STANDARD ? "Standard" : "Domoticz");
#endif

  if (strcmp(FirmwareConfiguration.version, AFE_FIRMWARE_VERSION) == 0 &&
      FirmwareConfiguration.type == AFE_FIRMWARE_TYPE &&
      FirmwareConfiguration.api ==
#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
          AFE_API_DOMOTICZ
#else
          AFE_API_STANDARD
#endif
      ) {
    return false;
  } else {
    return true;
  }
}

/* It kicks-off firmware upgrade */
void AFEUpgrader::upgrade() {
  /* Upgraded version from one T to other T */
  if (FirmwareConfiguration.type != AFE_FIRMWARE_TYPE) {
#ifdef DEBUG
    Serial << endl << "INFO: Upgrading Firmware type";
#endif
    upgradeFirmwarType();
    Device->upgraded = AFE_UPGRADE_VERSION_TYPE;
    /* Upgrade from one version to other within the same T */
  } else if (strcmp(FirmwareConfiguration.version, AFE_FIRMWARE_VERSION) != 0) {
#ifdef DEBUG
    Serial << endl
           << "INFO: Upgrading Firmware T" << AFE_FIRMWARE_TYPE
           << " from version: " << FirmwareConfiguration.version << " to "
           << AFE_FIRMWARE_VERSION;
#endif
    updateFirmwareVersion();
    Device->upgraded = AFE_UPGRADE_VERSION;
  }

  /* Checking if in addition there has been API version change */
  if (FirmwareConfiguration.api != AFE_FIRMARE_API) {
#ifdef DEBUG
    Serial << endl
           << "INFO: Firmware API version upgraded"
           << " from version: " << FirmwareConfiguration.api << " to "
           << AFE_FIRMARE_API;
#endif
    updateFirmwareAPIVersion();
  }

#ifdef DEBUG
  Serial << endl
         << "INFO Upgrade to version " << AFE_FIRMWARE_VERSION << " completed";
#endif
}

void AFEUpgrader::upgradeFirmwarType() {
  Data->createFirmwareConfigurationFile();
  Data->createDeviceConfigurationFile();
  Data->createSwitchConfigurationFile();
#ifdef AFE_CONFIG_HARDWARE_RELAY
  Data->createRelayConfigurationFile();
#endif

/* Configuration files specyfic to T5 */
#ifdef AFE_CONFIG_HARDWARE_CONTACTRON
  Data->createContractonConfigurationFile();
#endif

#ifdef AFE_CONFIG_HARDWARE_GATE
  Data->createGateConfigurationFile();
#endif

/* Configuration files specyfic to T6 */
#ifdef AFE_CONFIG_HARDWARE_HPMA115S0
  Data->createHPMA115S0SensorConfigurationFile();
#endif

#ifdef AFE_CONFIG_HARDWARE_UART
  Data->createSerialConfigurationFile();
#endif

#ifdef AFE_CONFIG_HARDWARE_I2C
  Data->createI2CConfigurationFile();
#endif

#ifdef AFE_CONFIG_HARDWARE_BMEX80
  Data->createBMEX80SensorConfigurationFile();
#endif

#ifdef AFE_CONFIG_HARDWARE_BH1750
  Data->createBH1750SensorConfigurationFile();
#endif

#ifdef AFE_CONFIG_HARDWARE_AS3935
  Data->createAS3935SensorConfigurationFile();
#endif
}

void AFEUpgrader::updateFirmwareVersion() {

/* Upgrade to version T0-2.0.3 */
#ifdef T0_CONFIG
  if (strcmp(FirmwareConfiguration.version, "2.0.0") == 0 ||
      strcmp(FirmwareConfiguration.version, "2.0.1") == 0 ||
      strcmp(FirmwareConfiguration.version, "2.0.2") == 0) {
    upgradeToT0V210();
  }
#endif

  Data->saveFirmwareVersion(AFE_FIRMWARE_VERSION);
}

void AFEUpgrader::updateFirmwareAPIVersion() {

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
  if (AFE_FIRMARE_API == AFE_API_DOMOTICZ) {
    /* Checking if there is Domoticz server configuration file */
    if (!Data->fileExist(AFE_FILE_DOMOTICZ_CONFIGURATION)) {
      Data->createDomoticzConfigurationFile();
    }
    Data->saveFirmwareAPIVersion();
  }
#endif

  Data->saveFirmwareAPIVersion();
}

/* Specyfic upgrade to version T0 2.1 from version 2.0 */
void AFEUpgrader::upgradeToT0V210() {

  DEVICE newDevice;
  DEVICE_T0_200 oldDevice = Data->getDeviceT0v200Configuration();
  uint8_t counter = 0;

  // Copy data from old structure to new structure

  sprintf(newDevice.name, oldDevice.name);
  newDevice.api.http = oldDevice.api.http;
  newDevice.api.mqtt = oldDevice.api.mqtt;
#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
  newDevice.api.domoticz = oldDevice.api.domoticz;
#endif

#ifdef AFE_CONFIG_HARDWARE_ADC_VCC
  newDevice.isAnalogInput = oldDevice.isAnalogInput;
#endif

  for (uint8_t i = 0; i < sizeof(oldDevice.isSwitch); i++) {
    counter += oldDevice.isSwitch[i] ? 1 : 0;
  }

  newDevice.noOfSwitches = counter;

  counter = 0;
  for (uint8_t i = 0; i < sizeof(oldDevice.isRelay); i++) {
    counter += oldDevice.isRelay[i] ? 1 : 0;
  }

  newDevice.noOfRelays = counter;

#ifdef AFE_CONFIG_HARDWARE_LED
  counter = 0;
  for (uint8_t i = 0; i < sizeof(oldDevice.isLED); i++) {
    counter += oldDevice.isLED[i] ? 1 : 0;
  }
  newDevice.noOfLEDs = counter;
#endif

  // Save to new JSON structure configuration file
  Data->saveConfiguration(&newDevice);
}
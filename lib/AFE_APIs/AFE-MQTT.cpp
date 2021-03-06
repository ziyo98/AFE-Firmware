/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#include "AFE-MQTT.h"

AFEMQTT::AFEMQTT(){};

#ifdef AFE_CONFIG_HARDWARE_LED
void AFEMQTT::begin(AFEDataAccess *Data, char *DeviceName, AFELED *Led) {
  _Led = Led;
  begin(Data, DeviceName);
}
#endif

void AFEMQTT::begin(AFEDataAccess *Data, char *DeviceName) {
  _Data = Data;
  _DeviceName = DeviceName;
  configuration = _Data->getMQTTConfiguration();
  NetworkConfiguration = _Data->getNetworkConfiguration();

  esp.setTimeout(configuration.timeout);
  Broker.setClient(esp);

  if (strlen(configuration.ip) > 0) {
    IPAddress ip;
    if (ip.fromString(configuration.ip)) {
      Broker.setServer(configuration.ip, configuration.port);
    }
#ifdef DEBUG
    else {
      Serial << endl
             << "ERROR: Problem with MQTT IP address: " << configuration.ip;
    }
#endif
  } else if (strlen(configuration.host) > 0) {
    Broker.setServer(configuration.host, configuration.port);
  } else {
    isConfigured = false;
  }

  Data = {};

#ifdef DEBUG
  Serial << endl
         << "INFO: MQTT Configuration" << endl
         << "INFO: Host: " << configuration.host << endl
         << "INFO: IP: " << configuration.ip << endl
         << "INFO: Port: " << configuration.port << endl
         << "INFO: User: " << configuration.user << endl
         << "INFO: Password: " << configuration.password << endl
#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
         << "INFO: LWT IDX: " << configuration.lwt.idx;
#else
         << "INFO: LWT Topic: " << configuration.lwt.topic;
#endif // AFE_CONFIG_API_DOMOTICZ_ENABLED
#endif
}

#ifdef AFE_CONFIG_API_PROCESS_REQUESTS
void AFEMQTT::subscribe(const char *topic) {

  if (strlen(topic) > 0) {
#ifdef AFE_CONFIG_HARDWARE_LED
    _Led->on();
#endif
    Broker.subscribe(topic);
#ifdef DEBUG
    Serial << endl << " - " << topic;
#endif
#ifdef AFE_CONFIG_HARDWARE_LED
    _Led->off();
#endif
  }
}
#endif

void AFEMQTT::disconnect() {
  if (Broker.connected()) {
    Broker.disconnect();
  }
}

boolean AFEMQTT::listener() {
  boolean _ret = false;
  if (Broker.connected()) {
    _ret = Broker.loop();
#ifdef AFE_CONFIG_API_PROCESS_REQUESTS
    message.topic = Broker.topic;
    message.content = Broker.payload;
    message.length = Broker.length;
#endif
  } else {
    connect();
  }
  return _ret;
}

void AFEMQTT::connect() {

  if (isConfigured) {
    if (sleepMode) {
      if (millis() - sleepStartTime >=
          NetworkConfiguration.waitTimeSeries * 1000) {
        sleepMode = false;
      }
    } else {
#ifdef AFE_CONFIG_HARDWARE_LED
      if (ledStartTime == 0) {
        ledStartTime = millis();
      }
#endif
      if (delayStartTime == 0) {
        delayStartTime = millis();
        /* Connecing to MQTT Broker depending on LWT topics being set or no */

        boolean _connected;

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
        char lwtMessage[100];

        if (configuration.lwt.idx > 0) {
          sprintf(
              lwtMessage,
              "{\"command\":\"udevice\",\"idx\":%d,\"nvalue\":0,\"svalue\":\"%s\",\"Battery\":0,\"RSSI\":0}",
              configuration.lwt.idx,L_DISCONNECTED);

          _connected = Broker.connect(
              _DeviceName, configuration.user, configuration.password,
              AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, 2, false, lwtMessage);
        } else {
          _connected = Broker.connect(_DeviceName, configuration.user,
                                      configuration.password);
        }
#else
        _connected =
            strlen(configuration.lwt.topic) > 0
                ? Broker.connect(
                      _DeviceName, configuration.user, configuration.password,
                      configuration.lwt.topic, 2, false, "disconnected")
                : Broker.connect(_DeviceName, configuration.user,
                                 configuration.password);
#endif

        if (_connected) {
          eventConnectionEstablished = true;
          delayStartTime = 0;
#ifdef AFE_CONFIG_HARDWARE_LED
          ledStartTime = 0;
          _Led->off();
#endif
          connections = 0;

#ifdef DEBUG
          Serial << endl << "INFO: Connected to MQTT Broker";
#endif

          return;
        }
      }
#ifdef AFE_CONFIG_HARDWARE_LED
      if (millis() > ledStartTime + 500) {
        _Led->toggle();
        ledStartTime = 0;
      }
#endif
      if (millis() >
          delayStartTime + (NetworkConfiguration.waitTimeConnections * 1000)) {

        connections++;
        yield();
#ifdef DEBUG
        Serial << endl
               << "INFO: MQTT Connection attempt: " << connections + 1
               << " from " << NetworkConfiguration.noConnectionAttempts
               << ", connection status: " << Broker.state()
               << ", connection time: " << millis() - delayStartTime << "ms";

#endif
        delayStartTime = 0;
      }

      if (connections >= NetworkConfiguration.noConnectionAttempts) {
        sleepMode = true;
        sleepStartTime = millis();

        delayStartTime = 0;
#ifdef AFE_CONFIG_HARDWARE_LED
        ledStartTime = 0;
        _Led->off();
#endif
        connections = 0;

#ifdef DEBUG
        Serial << endl
               << "WARN: MQTT: Not able to connect to MQTT.Going to sleep mode "
                  "for "
               << NetworkConfiguration.waitTimeSeries << "sec.";
#endif
      }
    }
  }
}

void AFEMQTT::setReconnectionParams(
    uint8_t no_connection_attempts,
    uint8_t duration_between_connection_attempts,
    uint8_t duration_between_next_connection_attempts_series) {
  NetworkConfiguration.noConnectionAttempts = no_connection_attempts;
  NetworkConfiguration.waitTimeConnections =
      duration_between_connection_attempts;
  NetworkConfiguration.waitTimeSeries =
      duration_between_next_connection_attempts_series;
}

boolean AFEMQTT::eventConnected() {
  boolean returnValue = eventConnectionEstablished;
  eventConnectionEstablished = false;
  return returnValue;
}

boolean AFEMQTT::publish(const char *topic, const char *message) {

  boolean pubslishingStatus = false;

  if (Broker.state() == MQTT_CONNECTED) {
#ifdef AFE_CONFIG_HARDWARE_LED
    _Led->on();
#endif
#ifdef DEBUG
    Serial << endl << "----------- Publish MQTT -----------";
    Serial << endl << "Topic: " << topic;
    Serial << endl << "Message: " << message;
#endif
    if (strlen(topic) > 0) {
      pubslishingStatus = Broker.publish(topic, message);
    }
#ifdef DEBUG
    else {
      Serial << endl << "WARN: No MQTT topic.";
    }
#endif
#ifdef AFE_CONFIG_HARDWARE_LED
    _Led->off();
#endif
#ifdef DEBUG
    Serial << endl
           << "Status: "
           << (pubslishingStatus ? "published" : "NOT pubslished");
    Serial << endl << "------------------------------------";
#endif
  }

  return pubslishingStatus;
}
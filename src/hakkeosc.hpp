#ifndef HAKKEOSC
#define HAKKEOSC
#pragma once

#include "hakkesettings.hpp"
#include <NativeEthernet.h>
#include <NativeEthernetUdp.h>
#include <OSCBundle.h>
#include <OSCMessage.h>

namespace hakke {

template <int numLayers, int numPages, int numEncs> class HakkeOSC {
private:
  // Setup the Teensy's mac address
  static void teensyMAC(uint8_t *mac) {
    uint32_t m1 = HW_OCOTP_MAC1;
    uint32_t m2 = HW_OCOTP_MAC0;
    mac[0] = m1 >> 8;
    mac[1] = m1 >> 0;
    mac[2] = m2 >> 24;
    mac[3] = m2 >> 16;
    mac[4] = m2 >> 8;
    mac[5] = m2 >> 0;
  };

public:
  int devicePort, targetPort;
  IPAddress deviceIp, targetIp;
  EthernetUDP Udp;

  // buffers for receiving and sending data
  char packetBuffer[UDP_TX_PACKET_MAX_SIZE]; // buffer to hold incoming packet,
  // char ReplyBuffer[];                        // a string to send back

  HakkeState<numLayers, numPages, numEncs> &settings;
  // Teensy mac address
  uint8_t mac[6];

  // FIXME this is a ridiculous hack to avoid encoders trying to send before
  // this is setup. Should rather be setup properly in other objects before
  // using this
  bool initialized{false}, connected{false};

  HakkeOSC(HakkeState<numLayers, numPages, numEncs> &globalSettings)
      : settings(globalSettings){};

  bool checkHardware() {
    // Check for Ethernet hardware present
    if (Ethernet.hardwareStatus() == EthernetNoHardware) {
	  // Hardware not found
      return false;
    }
    {
  //     if (Ethernet.linkStatus() == LinkOFF) {
		// // Cable not connected
  //       return false;
  //     }
	  return true;
    }

    return false;
  }

  void setup() {
    targetPort = settings.targetPort;
    targetIp = settings.targetIp;

    devicePort = settings.devicePort;
    deviceIp = settings.deviceIp;

    // Setup mac address
    teensyMAC(mac);

    Ethernet.begin(mac, deviceIp /*, gateway, gateway, subnet*/);

    // Send startup message and start UDP
    if (checkHardware() && Ethernet.linkStatus() == LinkON) {
      connect();
	  connected = true;
    }
    {
	  Serial.println("Could not initialize OSC setup");
	}

	initialized = true;
  };

  // Start ethernet and udp and send a startup message
  void connect() {
	Serial.println("OSC connected");
    Udp.begin(devicePort);
    sendCharsMsg("/hello", "hi");
  }

  void disconnect(){
	Serial.println("OSC disconnected");
	// FIXME: How to stop Ethernet?
	// Ethernet.stop();
	Udp.stop();
  }

  void loop(){

	// Make sure to .begin or .stop the networking classes
	// Otherwise, it crashes/hangs (because of the UDP class) until the ethernet cable is connected
	bool hardwareStatus = checkHardware();
	bool cableStatus = Ethernet.linkStatus() == LinkON;

	if(connected != cableStatus){
	  Serial.println("OSC connection status changed");
	  if (initialized && hardwareStatus && cableStatus) {
		connect();
	  } {
		disconnect();
	  }
	  connected = cableStatus;
	}
  }

  // FIXME: This seems inefficient
  void sendFloatMsg(const char *msgAddr, float val) {
    if (initialized && connected) {
      OSCMessage msg(msgAddr);
      msg.add(val);
      Udp.beginPacket(targetIp, targetPort);
      msg.send(Udp);
      Udp.endPacket();
      msg.empty();
    }
  }

  void sendCharsMsg(const char *msgAddr, const char *val) {
    if (initialized && connected) {
      OSCMessage msg(msgAddr);

      msg.add(val);
      Udp.beginPacket(targetIp, targetPort);
      msg.send(Udp);
      Udp.endPacket();
      msg.empty();
    }
  }
};

} // namespace hakke
#endif

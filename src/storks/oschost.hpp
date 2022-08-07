#pragma once
#ifndef OSCSTORK
#define OSCSTORK

#include "QNDNSClient.h"
#include <QNEthernet.h>

#include <OSCBundle.h>
#include <OSCMessage.h>

namespace storkspace {

using namespace qindesign::network;
static constexpr int UdpMaxSize = 1000;
/**
 * @brief OSCHost
 * @author Mads Kjeldgaard & Thom Johansen
 * @date 08-10-2022
 * @details Interface for OSC, allowing to send and receive osc and handling all
 * the networking intricacies
 */

class OSCHost {
public:
  EthernetUDP Udp;

  // OSCHost();
  // ~OSCHost();

  void setup(const unsigned int targetPort, IPAddress my_ip,
             IPAddress target_ip,
             IPAddress my_netmask = IPAddress(255, 255, 0, 0)) {
    outPort = targetPort;

    uint8_t mac[6];
    Ethernet.macAddress(mac);
    Serial.printf("MAC = %02x:%02x:%02x:%02x:%02x:%02x\n", mac[0], mac[1],
                  mac[2], mac[3], mac[4], mac[5]);
    Ethernet.onLinkState([](bool state) {
      Serial.printf("[Ethernet] Link %s\n", state ? "ON" : "OFF");
      // When setting a static IP, the address will be set but a link
      // might not yet exist
      // const bool hasIP = (Ethernet.localIP() != INADDR_NONE);
      // systemReady(hasIP, state);
    });

    osc_server_ip = target_ip;

    IPAddress my_gw(192, 168, 1, 1);

    Ethernet.begin(my_ip, my_netmask, my_gw);
    Serial.println("begun");
    Udp.begin(8888);
  };

  void systemReady(bool hasIP, bool hasLink) {
    Serial.printf("System is%s ready\n", (hasIP && hasLink) ? "" : " not");
    networkUsable = hasIP && hasLink;
    ipValid = false;
    // *** Notification or start/stop/restart code goes here
  };

  // Send float
  void sendEncoder(float val, const unsigned int channelNum = 1,
                   const unsigned int layerNum = 1,
                   const unsigned int encoderNum = 1) {

    char msg_addr[24];
    snprintf(msg_addr, sizeof(msg_addr), "/storks/encoder/%i/%i/%i", channelNum,
             layerNum, encoderNum);

    oscb.add(msg_addr).add(val);
  }

  // Send int
  void sendEncoder(int val, const unsigned int channelNum = 1,
                   const unsigned int layerNum = 1,
                   const unsigned int encoderNum = 1) {

    char msg_addr[24];
    snprintf(msg_addr, sizeof(msg_addr), "/storks/encoder/%i/%i/%i", channelNum,
             layerNum, encoderNum);

    oscb.add(msg_addr).add(val);
  }

  void sendButton(int val, const unsigned int channelNum = 1,
                  const unsigned int layerNum = 1,
                  const unsigned int buttonNum = 1){

    char msg_addr[24];

    snprintf(msg_addr, sizeof(msg_addr), "/storks/button/%i/%i/%i", channelNum,
             layerNum, buttonNum);

    oscb.add(msg_addr).add(val);
  }

  void readPacketsIncoming() {
    // if there's data available, read a packet
    int packetSize = Udp.parsePacket();
    if (packetSize) {
      Serial.print("Received packet of size ");
      Serial.println(packetSize);
      Serial.print("From ");
      IPAddress remote = Udp.remoteIP();
      for (int i = 0; i < 4; i++) {
        Serial.print(remote[i], DEC);
        if (i < 3) {
          Serial.print(".");
        }
      }
      Serial.print(", port ");
      Serial.println(Udp.remotePort());

      // read the packet into packetBufffer
      Udp.read(packetBuffer, UdpMaxSize);
      Serial.println("Contents:");
      Serial.println(packetBuffer);

      // send a reply to the IP address and port that sent us the packet we
      // received Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
      // Udp.write(ReplyBuffer);
      // Udp.endPacket();
    }
  }
  void loop() {
    if (sendTimer > 10) {
      // if (networkUsable)

      if (oscb.size() > 0) {
        Udp.beginPacket(osc_server_ip, outPort);
        oscb.send(Udp);
        Udp.endPacket();
        oscb.empty();
      }

      // reset state arrays
      sendTimer = 0;
    }
  }

private:
  elapsedMillis sendTimer;
  bool networkUsable;
  unsigned int outPort;

  char packetBuffer[UdpMaxSize]; // buffer to hold incoming packet,
  // char ReplyBuffer[];            // a string to send back
  IPAddress osc_server_ip;
  bool ipValid = false;

  OSCBundle oscb;
};

} // namespace storkspace
#endif

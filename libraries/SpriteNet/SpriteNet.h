/*
*	SpriteNet.h - Library for exchanging packets between Sprites
*
*	Author: Tane Tatum
*
*	Last Updated: 9-12-18
*/

#ifndef _SPRITENET_H
#define _SPRITENET_H

#include "sprite.h"
#include "cc430radio.h"


struct Packet{
	Packet();
	unsigned char packetLength;		//Total length of transmitted packet (bytes)
	unsigned char destination;		//Destination of packet
	unsigned char origin;			//Original sender of packet
	unsigned char sender;			//Most recent sender of packet
	unsigned char packetID;			//Unique packet ID
	unsigned char dataLength;		//Length of data in Packet
	unsigned char *data;			//data in Packet
};



class SpriteNet {
private:
	unsigned char _address;
	CC1101Settings _settings;
	unsigned char _power;
public:
	SpriteNet(unsigned char address, CC1101Settings settings);
	SpriteNet(unsigned char address);
	void sendPacket(Packet *packet);	// Trasnmits packet
	void repeatPacket(Packet *packet);	// Rebroadcasts packet
	void listen(Packet *packet);		// Sets radio to recieve and listens for packets
	void printPacket(Packet *packet);	// Prints packet contents to Serial port
	void txInit();						// Initializes SpriteNet radio settings - call before transmitting
};

#endif
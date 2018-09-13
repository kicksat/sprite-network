/*
*	SpriteNet.cpp - Library for exchanging packets between Sprites
*
*	Author: Tane Tatum
*
*	Last Updated: 9-12-18
*/

#include "SpriteNet.h"
#include "sprite.h"
#include "cc430radio.h"
#include "hardwareSerial.h"

//Initialize Packet Structure
Packet::Packet() {
	//Generate random byte as packetID
	long randLong = random(255);
	unsigned char randByte;
	randByte = (int)((randLong & 0xFF));
	packetID = randByte;
};


//Initailze Sprite Net
SpriteNet::SpriteNet(unsigned char address, CC1101Settings settings) {
	_address = address;
	_settings = settings;
	_power = 0xC0;			// Transmit power setting (0xC0 for max power, 0x50 for mid power, 0x12 for min power)
}

//Initailze Sprite Net
SpriteNet::SpriteNet(unsigned char address) {

	/* Intial Radio Settings Registers
		Values determined from TI SmartRF Studio 7 software:
		Base Frequency: 437.239655MHz,  Channel: 0, Channel Spacing: 199.951172kHz, Carrier Frequency: 437.239655MHz
		Xtal Frequency: 26.0Mhz, Data Rate: 0.808954kBaud (min),  RX Filter BW: 58.035714kHz
		Modulation: 2-FSK,  Deviation: 5.157471kHz,  TX Power: 10dBm (max)
		*/
	CC1101Settings SpriteNetSettings;	   // Object to store SpriteNet radio settings
	SpriteNetSettings.fsctrl1 = 0x06;      // Frequency synthesizer control.
	SpriteNetSettings.fsctrl0 = 0x00;      // Frequency synthesizer control.
	SpriteNetSettings.freq2 = 0x10;        // Frequency control word, high byte.
	SpriteNetSettings.freq1 = 0xD1;        // Frequency control word, middle byte (Base Frequency)
	SpriteNetSettings.freq0 = 0x21;        // Frequency control word, low byte (Base Frequency)
	SpriteNetSettings.mdmcfg4 = 0xF5;      // Modem configuration. ***  (C8)
	SpriteNetSettings.mdmcfg3 = 0x05;      // Modem configuration. ***  (93)
	SpriteNetSettings.mdmcfg2 = 0x03;      // Modem configuration. (Modulation, Manchester encoding)
	SpriteNetSettings.mdmcfg1 = 0x22;      // Modem configuration.  (Preamble Length, Channel Spacing)
	SpriteNetSettings.mdmcfg0 = 0xF8;      // Modem configuration.  (Channel Spacing)
	SpriteNetSettings.channr = 0x00;       // Channel number.
	SpriteNetSettings.deviatn = 0x15;      // Modem deviation setting (when FSK modulation is enabled).
	SpriteNetSettings.frend1 = 0x56;       // Front end RX configuration.
	SpriteNetSettings.frend0 = 0x10;       // Front end RX configuration.
	SpriteNetSettings.mcsm0 = 0x18;        // Main Radio Control State Machine configuration.
	SpriteNetSettings.foccfg = 0x16;       // Frequency Offset Compensation Configuration.
	SpriteNetSettings.bscfg = 0x6C;        // Bit synchronization Configuration.
	SpriteNetSettings.agcctrl2 = 0x03;     // AGC control.
	SpriteNetSettings.agcctrl1 = 0x40;     // AGC control.
	SpriteNetSettings.agcctrl0 = 0x91;     // AGC control.
	SpriteNetSettings.fscal3 = 0xE9;       // Frequency synthesizer calibration.
	SpriteNetSettings.fscal2 = 0x2A;       // Frequency synthesizer calibration.
	SpriteNetSettings.fscal1 = 0x00;       // Frequency synthesizer calibration.
	SpriteNetSettings.fscal0 = 0x1F;       // Frequency synthesizer calibration.
	SpriteNetSettings.fstest = 0x59;       // Frequency synthesizer calibration.
	SpriteNetSettings.test2 = 0x81;        // Various test settings.p
	SpriteNetSettings.test1 = 0x35;        // Various test settings.
	SpriteNetSettings.test0 = 0x09;        // Various test settings.
	SpriteNetSettings.fifothr = 0x47;      // RXFIFO and TXFIFO thresholds.
	SpriteNetSettings.iocfg2 = 0x29;       // GDO2 output pin configuration
	SpriteNetSettings.iocfg0 = 0x06;       // GDO0 output pin configuration
	SpriteNetSettings.pktctrl1 = B00001100;// Packet automation control. (Appends packets and enables CRC autoflush)
	SpriteNetSettings.pktctrl0 = 0x05;     // Packet automation control.
	SpriteNetSettings.addr = 0x00;         // Device address.
	SpriteNetSettings.pktlen = 0xFF;       // RX max packet length.

	_address = address;
	_settings = SpriteNetSettings;
	_power = 0xC0;							// Transmit power setting (0xC0 for max power, 0x50 for mid power, 0x12 for min power)
}

void SpriteNet::sendPacket(Packet *packet) {
	//Compile Packet into single array
	unsigned char headerLength = 5;								// Number of bytes in header
	packet->packetLength = headerLength + packet->dataLength;	// Total Packet Length
	unsigned char txPacket[packet->packetLength];
	txPacket[0] = packet->destination;
	txPacket[1] = packet->origin;
	txPacket[2] = packet->sender;
	txPacket[3] = packet->packetID;
	txPacket[4] = packet->dataLength;
	for (int i = 0; i < packet->dataLength; i++) {
		txPacket[i + 5] = packet->data[i];
	}

	unsigned char *p_txPacketLength = &(packet->packetLength);	// Pointer to length variable
	Sprite.radio.writeTXBuffer(p_txPacketLength, 1);            // Write PacketLength as first byte in TX FIFO
	Sprite.radio.writeTXBuffer(txPacket, packet->packetLength); // Write packet data to TX FIFO
	Sprite.radio.strobe(RF_STX);                                // Set radio to transmit state

	//Wait for transmission to finish
	char status = Sprite.radio.strobe(RF_SNOP);
	while (status != 0x0F)										// Status = idle and txFIFO empty
	{
		status = Sprite.radio.strobe(RF_SNOP);
	}
}

void SpriteNet::repeatPacket(Packet *packet) {
	Packet repeatPacket = *packet;				// Copy packet
	repeatPacket.sender = _address;				// Change sender address to current Sprite's address
	Packet *p_repeatPacket = &repeatPacket;
	SpriteNet::sendPacket(p_repeatPacket);		// Transmit edited packet
}

void SpriteNet::listen(Packet *packet) {
	Sprite.radio.strobe(RF_SRX);								// Switch Radio to recieve mode
	unsigned char rxLength;
	for (int i = 0; i < 200; i++) {								// Repeat a number of cyles before returning to main thread 
		delay(10);												// Delay for radio startup and listen for messages
		rxLength = Sprite.radio.readRegister(RXBYTES);			// Check length of data in RX FIFO
		if (rxLength != 0) {									// If message recieved (length is not zero)

			// Wait for packet reception to finish
			int counter = 0;
			bool timeout = 0;									// Recieve timeout flag
			char status = Sprite.radio.strobe(RF_SNOP);
			while ((status & 0xF0) == 0x10)						// Wait for packet to be recieved (While status = RX)
			{
				status = Sprite.radio.strobe(RF_SNOP);
				counter++;
				delay(1);
				// If recieve times out, set Radio to idle
				if (counter > 1000) {
					Sprite.radio.strobe(RF_SIDLE);				// If recieve times out, set Radio to idle
					timeout = 1;								// Set recieve timeout flag
				}
			}

			// If Recieve did not time out, process incoming packet
			if (!timeout) {
				rxLength = Sprite.radio.readRegister(RXBYTES);		// Read how many bytes are in rxFIFO
				unsigned char payload[rxLength];
				Sprite.radio.readRXBuffer(payload, rxLength);		// Read RX FIFO
				Sprite.radio.strobe(RF_SFRX);                       // Flush RX FIFO (not necessarily required)
				packet->packetLength = payload[0];
				packet->destination = payload[1];
				packet->origin = payload[2];
				packet->sender = payload[3];
				packet->packetID = payload[4];
				packet->dataLength = payload[5];

				//Check if packet and data length make sense
				if ((packet->dataLength) == (packet->packetLength - 5)) {
					for (int i = 0; i < packet->dataLength; i++) {
						packet->data[i] = payload[i + 6];
						delay(1);
					}
				}
				// If packet is corrupt and should be ignored
				else {
					packet->packetLength = 0;
					packet->dataLength = 0;
				}
			}

			// If recieve did time out, ignore packet
			else {
				packet->packetLength = 0;
				packet->dataLength = 0;
			}
			break;
		}

		// If no packet is recieved
		else {
			packet->dataLength = 0;
			packet->packetLength = 0;
		}
	}
}

// Prints raw packet data to serial
void SpriteNet::printPacket(Packet *packet) {
	Serial.print("Packet:  ");
	Serial.print(packet->packetLength);			//Print total packet length
	Serial.print(" ");
	Serial.print(packet->destination);			//Print packet destination address
	Serial.print(" ");
	Serial.print(packet->origin);				//Print packet origin address
	Serial.print(" ");
	Serial.print(packet->sender);				//Print packet sender address
	Serial.print(" ");
	Serial.print(packet->packetID);				//Print packet ID
	Serial.print(" ");
	Serial.print(packet->dataLength);			//Print data length
	Serial.print(" ");

	//Print packet data
	for (int i = 0; i < packet->dataLength; i++) {
		Serial.print(packet->data[i]);
		Serial.print(" ");
	}
	Serial.println(" ");
}

// Switches radio to SpriteNet settings
void SpriteNet::txInit() {
	Sprite.radio.reset();							// Reset radio core
	Sprite.radio.writeConfiguration(&_settings);    // Write settings to configuration registers
	Sprite.radio.writePATable(_power);				// Set radio power

	//Put radio into idle state
	char status;
	status = Sprite.radio.strobe(RF_SIDLE);
	while (status & 0xF0)
	{
		status = Sprite.radio.strobe(RF_SNOP);
	}
}
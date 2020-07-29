/*
 Name:	      echoBot.ino
 Created:     20/06/2020
 Author:      Tolentino Cotesta <cotestatnt@yahoo.com>
 Description: a simple example that check for incoming messages
              and reply the sender with the received message.
	      The message will be forwarded also in a public channel
	      and to a specific userid.
*/
#include <Arduino.h>
#include "AsyncTelegram.h"
AsyncTelegram myBot;

const char* ssid = "XXXXXXXX";     		// REPLACE mySSID WITH YOUR WIFI SSID
const char* pass = "XXXXXXXX";     		// REPLACE myPassword YOUR WIFI PASSWORD, IF ANY
const char* token = "XXXXXXXXXXXXXXXXXXXX";   	// REPLACE myToken WITH YOUR TELEGRAM BOT TOKEN

// Name of public channel (your bot must be in admin group)
const char* channel = "@tolentino_cotesta";	

const uint8_t LED = 4;

void setup() {
     pinMode(LED_BUILTIN, OUTPUT);
	// initialize the Serial
	Serial.begin(115200);
	Serial.println("Starting TelegramBot...");

	WiFi.setAutoConnect(true);   
	WiFi.mode(WIFI_STA);
 	
	WiFi.begin(ssid, pass);
	delay(500);
	while (WiFi.status() != WL_CONNECTED) {
		Serial.print('.');
		delay(500);
	}

	// Set the Telegram bot properies
	myBot.setUpdateTime(2000);
	myBot.setTelegramToken(token);
	
	// Check if all things are ok
	Serial.print("\nTest Telegram connection... ");
	myBot.begin() ? Serial.println("OK") : Serial.println("NOK");
	
	Serial.print("Bot name: @");	
	Serial.println(myBot.userName);
}

void loop() {

	static uint32_t ledTime = millis();
	if (millis() - ledTime > 150) {
		ledTime = millis();
		digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
	}

	// a variable to store telegram message data
	TBMessage msg;

	// if there is an incoming message...
	if (myBot.getNewMessage(msg)){		

		// Send a message to your public channel
		String message ;		
		message += "Message from @";
		message += myBot.userName;
		message += ":\n";
		message += msg.text;
		Serial.println(message);		
		myBot.sendToChannel(channel, message, true);		

		// Send a message to specific user who has started your bot
		// Target user can find it's own userid with the bot @JsonDumpBot 
		// https://t.me/JsonDumpBot 				
		int32_t userid = 1234567890;	
		myBot.sendToUser(userid, msg.text);	

		// echo the received message
		myBot.sendMessage(msg, msg.text);				
    }
}


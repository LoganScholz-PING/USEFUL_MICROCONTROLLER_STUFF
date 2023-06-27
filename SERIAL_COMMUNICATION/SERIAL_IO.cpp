#define _Serial_IO_h
#include "SERIAL_IO.h"

void SetupSerialPC() {
	
	int i = 0;
	while (i < PARM_COUNT) {
		inputParmBuff[i] = (char*)malloc(64);
		++i;
	}
}

#pragma region SerialActions

void serialCmd_Hello() 
{
	Serial.println("Hello");
}

void PrintHelp() 
{
	Serial.println("!");
	Serial.println("! === HELP === ");
	Serial.println("! \"?\": This help");
	Serial.println("! \"Hello\" ....... Hello");
	Serial.println("!");
}

#pragma endregion SerialActions


#pragma region SerialEventHandler

void serialEvent() {
	// Reads serial stream until encountering a carriage return
	// (throws an error if encountering a line feed character)
	while (Serial.available() && stringComplete == false) {
		char inChar = Serial.read();

		if ((inChar == '\r') || (inChar == 0x0D)) { // 0x0D is CR is \r
			inData[string_index] = 0;
			string_index = 0;
			stringComplete = true;
		}
		else if (inChar == 0x0A) { // 0x0A is LF is \n
			Serial.println("SERIAL_INCORRECT_FORMAT_USE_CR_NOT_LF");
			inData[string_index] = 0;
			string_index = 0;
			stringComplete = false;
		}
		else {
			inData[string_index] = toupper(inChar);
			string_index++;
			inString[inStringPtr++] = inChar;
		}
	}
}

void HandleSerialIO() {
	
	if (Serial.available() > 0) {
		serialEvent();
	}

	if (stringComplete)	{
		ParseInputSerialData();
		//Serial.println(inString);
		*inString = 0;
		inStringPtr = 0;

		// Reset the system for further input of data
		stringComplete = false;
	}
}

#pragma endregion SerialEventHandler


#pragma region ParseSerialCommand

void ParseInputSerialData() {

	bool ok = true;
	char* p = inData;

	//Serial.print(">> SERIAL RECEIVED: ["); Serial.print(p); Serial.println("]");

	if (strcmp(p, "?") == 0) {
		PrintHelp();
	}
	else if (strcmp(p, "HELLO") == 0) {
		serialCmd_Hello();
	}
	else {
		// reset inputParmBuff
		int count = 0;
		while (count < PARM_COUNT) {
			inputParmBuff[count++][0] = 0;
		}
		
		// if delimiter ('=') found, try to load up ParmBuff
		count = 0;
		char* data = inData;
		char* strtokIndx;
		while ((strtokIndx = strtok_r(data, "=", &data)) != NULL) {
			strcpy(inputParmBuff[count], strtokIndx);
			++count;
		}

		if (count > 0) {
			// "Parsable" commands live here (any command containing a "=")
			char* cmd = inputParmBuff[0];
			char* val = inputParmBuff[1];
			
			/**** Usage examples for delimited data:
			
			if(strcmp(cmd, "DUTY") == 0) {
				float duty = atof(val);
				serialCmd_CHANGE_MTR_DUTY(duty);
			}
			else if (strcmp(cmd, "FREQ") == 0) {
				uint32_t freq = atoi(val);
				serialCmd_CHANGE_MTR_FREQ(freq);
			}
			else if (strcmp(cmd, "SET_SHORT") == 0) {
				serialCmd_SET_SHORT_LENGTH(val);
			}
			else {
				Serial.print("UnknownSerialCmd="); Serial.println(p);
			}
			
			****/
		}
	}
}

#pragma endregion ParseSerialCommand


/*
 * Gamepad Library
 * Developer: FURiOUS <furious.pro>
 * Version: 1.0
 * License: http://creativecommons.org/licenses/by-sa/3.0/legalcode
 *
 */

#ifndef GAMEPAD_h
#define GAMEPAD_h

#include "HID.h"

#if !defined(_USING_HID)

#warning "Using legacy HID core (non pluggable)"

#else

class Gamepad_
{
private:
	bool     autoSendState;
	int8_t	 xAxis;
	int8_t	 yAxis;
	uint16_t buttons;

public:
	Gamepad_();

	void begin(bool initAutoSendState = true);
	void end();

	void setYAxis(int8_t value);
	int8_t getYAxis();
	void setXAxis(int8_t value);
	int8_t getXAxis();

	void setButton(uint8_t button, bool state);
	bool getButton(uint8_t button);
	void pressButton(uint8_t button);
	void releaseButton(uint8_t button);

	void sendState();
};
extern Gamepad_ Gamepad;

#endif
#endif

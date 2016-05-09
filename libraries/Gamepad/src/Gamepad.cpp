/*
 * Gamepad Library
 * Developer: FURiOUS <furious.pro>
 * Version: 1.0
 * License: http://creativecommons.org/licenses/by-sa/3.0/legalcode
 *
 */
 
#include "Gamepad.h"

#if defined(_USING_HID)

#define GAMEPAD_REPORT_ID  0x03
#define GAMEPAD_STATE_SIZE 3

static const uint8_t _hidReportDescriptor[] PROGMEM = {
  
    // Gamepad
    0x05, 0x01,               // USAGE_PAGE (Generic Desktop)
    0x09, 0x04,               // USAGE (Joystick)
    0xa1, 0x01,               // COLLECTION (Application)
    0x85, GAMEPAD_REPORT_ID, // REPORT_ID (3)

    // 8 Buttons
    0x05, 0x09,               //   USAGE_PAGE (Button)
    0x19, 0x01,               //   USAGE_MINIMUM (Button 1)
    0x29, 0x08,               //   USAGE_MAXIMUM (Button 8)
    0x15, 0x00,               //   LOGICAL_MINIMUM (0)
    0x25, 0x01,               //   LOGICAL_MAXIMUM (1)
    0x75, 0x01,               //   REPORT_SIZE (1)
    0x95, 0x08,               //   REPORT_COUNT (8)
    0x55, 0x00,               //   UNIT_EXPONENT (0)
    0x65, 0x00,               //   UNIT (None)
    0x81, 0x02,               //   INPUT (Data,Var,Abs)

    // X and Y Axis
    0x05, 0x01,               //   USAGE_PAGE (Generic Desktop)
    0x09, 0x01,               //   USAGE (Pointer)
    0x15, 0x81,               //   LOGICAL_MINIMUM (-127)
    0x25, 0x7f,               //   LOGICAL_MAXIMUM (127)
    0xA1, 0x00,               //   COLLECTION (Physical)
    0x09, 0x30,               //     USAGE (x)
    0x09, 0x31,               //     USAGE (y)
    0x75, 0x08,               //     REPORT_SIZE (8)
    0x95, 0x02,               //     REPORT_COUNT (2)
    0x81, 0x02,               //     INPUT (Data,Var,Abs)
    0xc0,                     //   END_COLLECTION
    0xc0                      // END_COLLECTION
};

Gamepad_::Gamepad_()
{
    // Setup HID report structure
	static HIDSubDescriptor node(_hidReportDescriptor, sizeof(_hidReportDescriptor));
	HID().AppendDescriptor(&node);
    
    // Initalize State
    xAxis = 0;
    yAxis = 0;
    buttons = 0;
}

void Gamepad_::begin(bool initAutoSendState)
{
    autoSendState = initAutoSendState;
    sendState();
}

void Gamepad_::end()
{
}

/* BUTTONS */
void Gamepad_::setButton(uint8_t button, bool pressed)
{
    if (pressed)
        pressButton(button);
    else
        releaseButton(button);
}
bool Gamepad_::getButton(uint8_t button)
{
    return bitRead(buttons, button) == 1;
}
void Gamepad_::pressButton(uint8_t button)
{
    bitSet(buttons, button);
    if (autoSendState) sendState();
}
void Gamepad_::releaseButton(uint8_t button)
{
    bitClear(buttons, button);
    if (autoSendState) sendState();
}

/* X AXIS */
void Gamepad_::setXAxis(int8_t value)
{
    xAxis = value;
    if (autoSendState) sendState();
}
int8_t Gamepad_::getXAxis()
{
    return xAxis;
}

/* Y AXIS */
void Gamepad_::setYAxis(int8_t value)
{
    yAxis = value;
    if (autoSendState) sendState();
}
int8_t Gamepad_::getYAxis()
{
    return yAxis;
}

void Gamepad_::sendState()
{
    int8_t data[GAMEPAD_STATE_SIZE];
    data[0] = buttons;
    data[1] = xAxis;
    data[2] = yAxis;

    // HID().SendReport(Report number, array of values in same order as HID descriptor, length)
    HID().SendReport(GAMEPAD_REPORT_ID, data, GAMEPAD_STATE_SIZE);
}

Gamepad_ Gamepad;

#endif
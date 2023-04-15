/*++

Module Name:

    ftsSoftware.h

Abstract:

    Contains all the definitions and information related to the IC from a fw/driver point of view

--*/
/*
* FW Event IDs and Types
* Event IDs and Types pushed by the FW into the FIFO
*/
#define EVT_ID_NOEVENT						0x00	/*No Events*/
#define EVT_ID_CONTROLLER_READY				0x03	/*Controller ready, issued after a system reset.*/
#define EVT_ID_ENTER_POINT					0x13	/*Touch enter in the sensing area*/
#define EVT_ID_MOTION_POINT					0x23	/*Touch motion (a specific touch changed position)*/
#define EVT_ID_LEAVE_POINT					0x33	/*Touch leave the sensing area*/
#define EVT_ID_STATUS_UPDATE				0x43	/*FW report a system condition change*/
#define EVT_ID_USER_REPORT					0x53	/*User related events triggered (keys, gestures, proximity etc)*/
#define EVT_ID_ERROR						0xF3	/*Error Event*/

#define TOUCH_ID_MAX                        10

#define TOUCH_TYPE_INVALID					0x00	/*Invalid touch type*/
#define TOUCH_TYPE_FINGER					0x01	/*Finger touch*/
#define TOUCH_TYPE_GLOVE					0x02	/*Glove touch*/
#define TOUCH_TYPE_STYLUS					0x03	/*Stylus touch*/
#define TOUCH_TYPE_PALM						0x04	/*Palm touch*/
#define TOUCH_TYPE_HOVER					0x05	/*Hovering touch*/

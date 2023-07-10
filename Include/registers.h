
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
#define EVT_ID_DEBUG						0xE3	/*Debug Info*/
#define EVT_ID_ERROR						0xF3	/*Error Event*/

#define NUM_EVT_ID							(((EVT_ID_ERROR & 0xF0) >> 4) + 1)	/*Max number of unique event IDs supported*/
#define TOUCH_ID_MAX                        10

/*
* Types of EVT_ID_STATUS_UPDATE events
*/
#define EVT_TYPE_STATUS_ECHO				0x01	/*Echo event, contain the first 5 bytes of the FW command sent*/
#define EVT_TYPE_STATUS_FRAME_DROP			0x03	/*Some frame was skipped during the elaboration*/
#define EVT_TYPE_STATUS_FORCE_CAL			0x05	/*Force Calibration has triggered*/
#define EVT_TYPE_STATUS_WATER				0x06	/*Water Mode*/
#define EVT_TYPE_STATUS_SS_RAW_SAT			0x07	/*Self Sense data saturated*/
#define EVT_TYPE_STATUS_POCKET				0x09

#define TOUCH_TYPE_INVALID					0x00	/*Invalid touch type*/
#define TOUCH_TYPE_FINGER					0x01	/*Finger touch*/
#define TOUCH_TYPE_GLOVE					0x02	/*Glove touch*/
#define TOUCH_TYPE_STYLUS					0x03	/*Stylus touch*/
#define TOUCH_TYPE_PALM						0x04	/*Palm touch*/
#define TOUCH_TYPE_HOVER					0x05	/*Hovering touch*/
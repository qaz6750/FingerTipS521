/*++
	Copyright (c) Microsoft Corporation. All Rights Reserved.
	Sample code. Dealpoint ID #843729.

	Module Name:

		ftinternal.h

	Abstract:

		Contains common types and defintions used internally
		by the multi touch screen driver.

	Environment:

		Kernel mode

	Revision History:

--*/

#pragma once

#include <wdm.h>
#include <wdf.h>
#include <controller.h>
#include <resolutions.h>
#include <Cross Platform Shim/bitops.h>
#include <Cross Platform Shim/hweight.h>
#include <report.h>

// Ignore warning C4152: nonstandard extension, function/data pointer conversion in expression
#pragma warning (disable : 4152)

// Ignore warning C4201: nonstandard extension used : nameless struct/union
#pragma warning (disable : 4201)

// Ignore warning C4201: nonstandard extension used : bit field types other than in
#pragma warning (disable : 4214)

// Ignore warning C4324: 'xxx' : structure was padded due to __declspec(align())
#pragma warning (disable : 4324)

#define EVT_ID_NOEVENT						0x00	/*No Events*/
#define EVT_ID_CONTROLLER_READY				0x03	/*Controller ready, issued after a system reset.*/
#define EVT_ID_ENTER_POINT					0x13	/*Touch enter in the sensing area*/
#define EVT_ID_MOTION_POINT					0x23	/*Touch motion (a specific touch changed position)*/
#define EVT_ID_LEAVE_POINT					0x33	/*Touch leave the sensing area*/
#define EVT_ID_STATUS_UPDATE				0x43	/*FW report a system condition change*/
#define EVT_ID_USER_REPORT					0x53	/*User related events triggered (keys, gestures, proximity etc)*/
#define EVT_ID_DEBUG						0xE3	/*Debug Info*/
#define EVT_ID_ERROR						0xF3	/*Error Event*/

typedef struct _FOCAL_TECH_TOUCH_DATA
{
	BYTE PositionX_High : 4;
	BYTE Reserved0 : 2;
	BYTE EventFlag : 2;

	BYTE PositionX_Low;

	BYTE PositionY_High : 4;
	BYTE TouchId : 4;

	BYTE PositionY_Low;

	BYTE TouchWeight;

	BYTE Reserved1 : 4;
	BYTE TouchArea : 4;
} FOCAL_TECH_TOUCH_DATA, * PFOCAL_TECH_TOUCH_DATA;

typedef struct _FOCAL_TECH_EVENT_DATA
{
	BYTE Reserved0 : 4;
	BYTE DeviceMode : 3;
	BYTE Reserved1 : 1;

	BYTE GestureId;

	BYTE NumberOfTouchPoints : 4;
	BYTE Reserved2 : 4;

	FOCAL_TECH_TOUCH_DATA TouchData[6];
} FOCAL_TECH_EVENT_DATA, * PFOCAL_TECH_EVENT_DATA;

#define TOUCH_POOL_TAG_F12              (ULONG)'21oT'

//
// Logical structure for getting registry config settings
//
typedef struct _RM4_F01_CTRL_REGISTERS_LOGICAL
{
	UINT32 SleepMode;
	UINT32 NoSleep;
	UINT32 ReportRate;
	UINT32 Configured;
	UINT32 InterruptEnable;
	UINT32 DozeInterval;
	UINT32 DozeThreshold;
	UINT32 DozeHoldoff;
} FT5X_F01_CTRL_REGISTERS_LOGICAL;

#define FT5X_MILLISECONDS_TO_TENTH_MILLISECONDS(n) n/10
#define FT5X_SECONDS_TO_HALF_SECONDS(n) 2*n

//
// Logical structure for getting registry config settings
//
typedef struct _FT5X_F11_CTRL_REGISTERS_LOGICAL
{
	UINT32 ReportingMode;
	UINT32 AbsPosFilt;
	UINT32 RelPosFilt;
	UINT32 RelBallistics;
	UINT32 Dribble;
	UINT32 PalmDetectThreshold;
	UINT32 MotionSensitivity;
	UINT32 ManTrackEn;
	UINT32 ManTrackedFinger;
	UINT32 DeltaXPosThreshold;
	UINT32 DeltaYPosThreshold;
	UINT32 Velocity;
	UINT32 Acceleration;
	UINT32 SensorMaxXPos;
	UINT32 SensorMaxYPos;
	UINT32 ZTouchThreshold;
	UINT32 ZHysteresis;
	UINT32 SmallZThreshold;
	UINT32 SmallZScaleFactor;
	UINT32 LargeZScaleFactor;
	UINT32 AlgorithmSelection;
	UINT32 WxScaleFactor;
	UINT32 WxOffset;
	UINT32 WyScaleFactor;
	UINT32 WyOffset;
	UINT32 XPitch;
	UINT32 YPitch;
	UINT32 FingerWidthX;
	UINT32 FingerWidthY;
	UINT32 ReportMeasuredSize;
	UINT32 SegmentationSensitivity;
	UINT32 XClipLo;
	UINT32 XClipHi;
	UINT32 YClipLo;
	UINT32 YClipHi;
	UINT32 MinFingerSeparation;
	UINT32 MaxFingerMovement;
} FT5X_F11_CTRL_REGISTERS_LOGICAL;

//
// Driver structures
//

typedef struct _FT5X_CONFIGURATION
{
	FT5X_F01_CTRL_REGISTERS_LOGICAL DeviceSettings;
	FT5X_F11_CTRL_REGISTERS_LOGICAL TouchSettings;
	UINT32 PepRemovesVoltageInD3;
} FT5X_CONFIGURATION;


typedef struct _FTS521_CONTROLLER_CONTEXT
{
	WDFDEVICE FxDevice;
	WDFWAITLOCK ControllerLock;

	//
	// Power state
	//
	DEVICE_POWER_STATE DevicePowerState;

	//
	// Register configuration programmed to chip
	//
	TOUCH_SCREEN_SETTINGS TouchSettings;
	FT5X_CONFIGURATION Config;

	UCHAR Data1Offset;

	BYTE MaxFingers;

    int HidQueueCount;
} FTS521_CONTROLLER_CONTEXT;

NTSTATUS
Fts521BuildFunctionsTable(
	IN FTS521_CONTROLLER_CONTEXT* ControllerContext,
	IN SPB_CONTEXT* SpbContext
);

NTSTATUS
Fts521ChangePage(
	IN FTS521_CONTROLLER_CONTEXT* ControllerContext,
	IN SPB_CONTEXT* SpbContext,
	IN int DesiredPage
);

NTSTATUS
Fts521ConfigureFunctions(
	IN FTS521_CONTROLLER_CONTEXT* ControllerContext,
	IN SPB_CONTEXT* SpbContext
);

NTSTATUS
Fts521ServiceInterrupts(
	IN FTS521_CONTROLLER_CONTEXT* ControllerContext,
	IN SPB_CONTEXT* SpbContext,
	IN PREPORT_CONTEXT ReportContext
);

#define FT5X_F01_DEVICE_CONTROL_SLEEP_MODE_OPERATING  0
#define FT5X_F01_DEVICE_CONTROL_SLEEP_MODE_SLEEPING   1

#pragma pack(push)
#pragma pack(1)
typedef enum _FT5X_F12_REPORTING_FLAGS
{
	FT5X_F12_REPORTING_CONTINUOUS_MODE = 0,
	FT5X_F12_REPORTING_REDUCED_MODE = 1,
	FT5X_F12_REPORTING_WAKEUP_GESTURE_MODE = 2,
} FT5X_F12_REPORTING_FLAGS;
#pragma pack(pop)

NTSTATUS
Fts521SetReportingFlags(
    IN FTS521_CONTROLLER_CONTEXT* ControllerContext,
    IN SPB_CONTEXT* SpbContext,
    IN UCHAR NewMode,
    OUT UCHAR* OldMode
);

NTSTATUS
Fts521ChangeChargerConnectedState(
    IN FTS521_CONTROLLER_CONTEXT* ControllerContext,
    IN SPB_CONTEXT* SpbContext,
    IN UCHAR ChargerConnectedState
);

NTSTATUS
Fts521ChangeSleepState(
    IN FTS521_CONTROLLER_CONTEXT* ControllerContext,
    IN SPB_CONTEXT* SpbContext,
    IN UCHAR SleepState
);

NTSTATUS
Ft5xGetFirmwareVersion(
    IN FTS521_CONTROLLER_CONTEXT* ControllerContext,
    IN SPB_CONTEXT* SpbContext
);

NTSTATUS
Fts521CheckInterrupts(
    IN FTS521_CONTROLLER_CONTEXT* ControllerContext,
    IN SPB_CONTEXT* SpbContext,
    IN ULONG* InterruptStatus
);

NTSTATUS
Fts521ConfigureInterruptEnable(
    IN FTS521_CONTROLLER_CONTEXT* ControllerContext,
    IN SPB_CONTEXT* SpbContext
);


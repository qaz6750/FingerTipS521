/*++
      Copyright (c) Microsoft Corporation. All Rights Reserved.
      Sample code. Dealpoint ID #843729.

      Module Name:

            rmiinternal.c

      Abstract:

            Contains Synaptics initialization code

      Environment:

            Kernel mode

      Revision History:

--*/

#include <Cross Platform Shim\compat.h>
#include <report.h>
#include <fts521\ftsinternal.h>
#include <ftsinternal.tmh>

BYTE FTS521_LOCKDOWN[3] = { 0xA4, 0x06, 0x70 };
BYTE FTS521_READ_ONE_EVENTS[1] = { 0x85 };
BYTE FTS521_READ_EVENTS[1] = { 0x86 };
BYTE FTS521_SCAN_MODE[3] = { 0xA0, 0x00, 0x00 };
BYTE FTS521_GESTURE[6] = { 0xA2, 0x03, 0x20, 0x00, 0x00, 0x01 };
BYTE FTS521_ONLY_SINGLE[4] = { 0xC0, 0x02, 0x01, 0x1E };
BYTE FTS521_SINGLE_DOUBLE[4] = { 0xC0, 0x02, 0x01, 0x1E };

BYTE eventbuf[256];

NTSTATUS
Fts521BuildFunctionsTable(
      IN FTS521_CONTROLLER_CONTEXT* ControllerContext,
      IN SPB_CONTEXT* SpbContext
)
{
      UNREFERENCED_PARAMETER(SpbContext);
      UNREFERENCED_PARAMETER(ControllerContext);

      return STATUS_SUCCESS;
}

NTSTATUS
Fts521ChangePage(
      IN FTS521_CONTROLLER_CONTEXT* ControllerContext,
      IN SPB_CONTEXT* SpbContext,
      IN int DesiredPage
)
{
      UNREFERENCED_PARAMETER(SpbContext);
      UNREFERENCED_PARAMETER(ControllerContext);
      UNREFERENCED_PARAMETER(DesiredPage);

      return STATUS_SUCCESS;
}

NTSTATUS
Fts521ConfigureFunctions(
      IN FTS521_CONTROLLER_CONTEXT* ControllerContext,
      IN SPB_CONTEXT* SpbContext
)
{
    NTSTATUS status;
    FTS521_CONTROLLER_CONTEXT* controller;
    controller = (FTS521_CONTROLLER_CONTEXT*)ControllerContext;

    status = FtsWrite(SpbContext, FTS521_LOCKDOWN, 3);
    if (NT_SUCCESS(status))
    {
        Trace(
            TRACE_LEVEL_ERROR,
            TRACE_INTERRUPT,
            "Writing Lockdown code into the IC done ");
    }

    //active scan on
    FTS521_SCAN_MODE[1] = 0x00;
    FTS521_SCAN_MODE[2] = 0x01;
    FtsWrite(SpbContext, FTS521_SCAN_MODE, 3);

    //FtsWrite(SpbContext, FTS521_GESTURE, 3);

    //low power scan off
    FTS521_SCAN_MODE[1] = 0x00;
    FTS521_SCAN_MODE[2] = 0x00;
    FtsWrite(SpbContext, FTS521_SCAN_MODE, 3);

    //active scan on
    FTS521_SCAN_MODE[1] = 0x00;
    FTS521_SCAN_MODE[2] = 0x01;
    FtsWrite(SpbContext, FTS521_SCAN_MODE, 3);

    return STATUS_SUCCESS;
}

#define TOUCH_MAX_FINGER_NUM 10

NTSTATUS
Fts521GetObjectStatusFromControllerF12(
    IN VOID* ControllerContext,
    IN SPB_CONTEXT* SpbContext,
    IN DETECTED_OBJECTS* Data
)
/*++

Routine Description:

      This routine reads raw touch messages from hardware. If there is
      no touch data available (if a non-touch interrupt fired), the
      function will not return success and no touch data was transferred.

Arguments:

      ControllerContext - Touch controller context
      SpbContext - A pointer to the current i2c context
      Data - A pointer to any returned F11 touch data

Return Value:

      NTSTATUS, where only success indicates data was returned

--*/
{
    NTSTATUS status;
    BYTE                  touchType;
    BYTE                  touchId;

    FTS521_CONTROLLER_CONTEXT* controller;

    PFOCAL_TECH_EVENT_DATA controllerData = NULL;
    controller = (FTS521_CONTROLLER_CONTEXT*)ControllerContext;

    int remain = 0;
    int x = 0;
    int y = 0;
    int base = 0;
    int i = 0;

    status = FtsWriteReadU8UX(SpbContext, FTS521_READ_EVENTS, &eventbuf[0], 3, 8);

    if (!NT_SUCCESS(status))
    {
        Trace(
            TRACE_LEVEL_ERROR,
            TRACE_INTERRUPT,
            "Error reading finger status data - 0x%08lX",
            status);

        goto exit;
    }

    if (remain > 0)
    {
        FtsWriteReadU8UX(SpbContext, FTS521_READ_EVENTS, &eventbuf[8], 3, 2);
    }

    for (i = 0; i < TOUCH_MAX_FINGER_NUM; i++) {

        base = i * 8;
        touchType = eventbuf[base + 1] & 0x0F;
        touchId = (eventbuf[base + 1] & 0xF0) >> 4;

        x = ((eventbuf[base + 3] & 0x0F) << 8) | (eventbuf[base + 2]);
        y = (eventbuf[base + 4] << 4) | ((eventbuf[base + 3] & 0xF0) >> 4);

            switch (eventbuf[i * 8 + 0])
            {
            case EVT_ID_ENTER_POINT:
            case EVT_ID_MOTION_POINT:
                Data->States[i] = OBJECT_STATE_FINGER_PRESENT_WITH_ACCURATE_POS;
                break;
            case EVT_ID_LEAVE_POINT:
                Data->States[i] = OBJECT_STATE_NOT_PRESENT;
                break;
            }
        

        Data->Positions[i].X = x;
        Data->Positions[i].Y = y;

          Trace(
              TRACE_LEVEL_ERROR,
              TRACE_INTERRUPT,
              "TOUCH event: 0x%02x - ID[%d], x: %d, y:%d",
              eventbuf[i * 8 + 0], touchId, x, y);
      }
      
  exit:
      return status;

}

NTSTATUS
TchServiceObjectInterrupts(
      IN FTS521_CONTROLLER_CONTEXT* ControllerContext,
      IN SPB_CONTEXT* SpbContext,
      IN PREPORT_CONTEXT ReportContext
)
{
      NTSTATUS status = STATUS_SUCCESS;
      DETECTED_OBJECTS data;

      RtlZeroMemory(&data, sizeof(data));

      //
      // See if new touch data is available
      //

      status = Fts521GetObjectStatusFromControllerF12(
            ControllerContext,
            SpbContext,
            &data
      );

      if (!NT_SUCCESS(status))
      {
            Trace(
                  TRACE_LEVEL_VERBOSE,
                  TRACE_SAMPLES,
                  "No object data to report - 0x%08lX",
                  status);

            goto exit;
      }

      status = ReportObjects(
            ReportContext,
            data);

      if (!NT_SUCCESS(status))
      {
            Trace(
                  TRACE_LEVEL_VERBOSE,
                  TRACE_SAMPLES,
                  "Error while reporting objects - 0x%08lX",
                  status);

            goto exit;
      }

exit:
      return status;
}


NTSTATUS
Fts521ServiceInterrupts(
      IN FTS521_CONTROLLER_CONTEXT* ControllerContext,
      IN SPB_CONTEXT* SpbContext,
      IN PREPORT_CONTEXT ReportContext
)
{
      NTSTATUS status = STATUS_SUCCESS;

      TchServiceObjectInterrupts(ControllerContext, SpbContext, ReportContext);

      return status;
}

NTSTATUS
Fts521SetReportingFlags(
    IN FTS521_CONTROLLER_CONTEXT* ControllerContext,
    IN SPB_CONTEXT* SpbContext,
    IN UCHAR NewMode,
    OUT UCHAR* OldMode
)
{
      UNREFERENCED_PARAMETER(SpbContext);
      UNREFERENCED_PARAMETER(ControllerContext);
      UNREFERENCED_PARAMETER(NewMode);
      UNREFERENCED_PARAMETER(OldMode);

      return STATUS_SUCCESS;
}

NTSTATUS
Fts521ChangeChargerConnectedState(
    IN FTS521_CONTROLLER_CONTEXT* ControllerContext,
    IN SPB_CONTEXT* SpbContext,
    IN UCHAR ChargerConnectedState
)
{
      UNREFERENCED_PARAMETER(SpbContext);
      UNREFERENCED_PARAMETER(ControllerContext);
      UNREFERENCED_PARAMETER(ChargerConnectedState);

      return STATUS_SUCCESS;
}

NTSTATUS
Fts521ChangeSleepState(
    IN FTS521_CONTROLLER_CONTEXT* ControllerContext,
    IN SPB_CONTEXT* SpbContext,
    IN UCHAR SleepState
)
{
      UNREFERENCED_PARAMETER(SpbContext);
      UNREFERENCED_PARAMETER(ControllerContext);
      UNREFERENCED_PARAMETER(SleepState);

      return STATUS_SUCCESS;
}

NTSTATUS
Fts521CheckInterrupts(
    IN FTS521_CONTROLLER_CONTEXT* ControllerContext,
    IN SPB_CONTEXT* SpbContext,
    IN ULONG* InterruptStatus
)
{
      UNREFERENCED_PARAMETER(SpbContext);
      UNREFERENCED_PARAMETER(ControllerContext);
      UNREFERENCED_PARAMETER(InterruptStatus);

      return STATUS_SUCCESS;
}

NTSTATUS
Fts521ConfigureInterruptEnable(
    IN FTS521_CONTROLLER_CONTEXT* ControllerContext,
    IN SPB_CONTEXT* SpbContext
)
{
      UNREFERENCED_PARAMETER(SpbContext);
      UNREFERENCED_PARAMETER(ControllerContext);

      return STATUS_SUCCESS;
}
/*++
    Module Name:

        spb.h

    Abstract:

        This module contains the touch driver I2C helper definitions.

    Environment:

        Kernel Mode

    Revision History:

--*/

VOID
SpbDeviceClose(
    _In_  PDEVICE_CONTEXT  pDevice
);
VOID
FtsWriteData(
    _In_ PDEVICE_CONTEXT pDevice,
    _In_ PVOID pInputBuffer,
    _In_ size_t inputBufferLength
);
VOID
FtsWriteReadData(
    _In_ PDEVICE_CONTEXT pDevice,
    _In_ PVOID pInputBuffer,
    _In_ PVOID pOutputBuffer,
    _In_ size_t inputBufferLength,
    _In_ size_t outputBufferLength
);
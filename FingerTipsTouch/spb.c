/*++
    Module Name:

        spb.c

    Abstract:

        Contains all I2C-specific functionality

    Environment:

        Kernel mode

    Revision History:

--*/

#include "Include/FingerTipsTouch.h"
#include "Include/spb.h"

VOID
SpbDeviceClose(
    _In_  PDEVICE_CONTEXT  pDevice
)
{
    WdfInterruptDisable(pDevice->Interrupt);

    WdfIoTargetClose(pDevice->SpbController);
}
VOID 
FtsWriteData(
    _In_ PDEVICE_CONTEXT pDevice,
    _In_ PVOID pInputBuffer,
    _In_ size_t inputBufferLength
)
{
    WDF_MEMORY_DESCRIPTOR  inMemoryDescriptor;
    ULONG_PTR  bytesWritten = (ULONG_PTR)NULL;
    NTSTATUS status;


    WDF_MEMORY_DESCRIPTOR_INIT_BUFFER(&inMemoryDescriptor,
        pInputBuffer,
        (ULONG)inputBufferLength);

    status = WdfIoTargetSendWriteSynchronously(
        pDevice->SpbController,
        NULL,
        &inMemoryDescriptor,
        NULL,
        NULL,
        &bytesWritten
    );

    if (!NT_SUCCESS(status))
    {
    }
}

VOID
FtsWriteReadData(
    _In_ PDEVICE_CONTEXT pDevice,
    _In_ PVOID pInputBuffer,
    _In_ PVOID pOutputBuffer,
    _In_ size_t inputBufferLength,
    _In_ size_t outputBufferLength
)
{
    WDF_MEMORY_DESCRIPTOR  inMemoryDescriptor;
    WDF_MEMORY_DESCRIPTOR  outMemoryDescriptor;
    ULONG_PTR  bytesWritten = (ULONG_PTR)NULL;
    ULONG_PTR  bytesRead = (ULONG_PTR)NULL;
    NTSTATUS status;


    WDF_MEMORY_DESCRIPTOR_INIT_BUFFER(&inMemoryDescriptor,
        pInputBuffer,
        (ULONG)inputBufferLength);

    status = WdfIoTargetSendWriteSynchronously(
        pDevice->SpbController,
        NULL,
        &inMemoryDescriptor,
        NULL,
        NULL,
        &bytesWritten
    );

    if (!NT_SUCCESS(status))
    {
        return;
    }

    WDF_MEMORY_DESCRIPTOR_INIT_BUFFER(&outMemoryDescriptor,
        pOutputBuffer,
        (ULONG)outputBufferLength);

    status = WdfIoTargetSendReadSynchronously(
        pDevice->SpbController,
        NULL,
        &outMemoryDescriptor,
        NULL,
        NULL,
        &bytesRead
    );

    if (!NT_SUCCESS(status))
    {
    }
}

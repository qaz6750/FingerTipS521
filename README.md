# FingerTipS Touch (FTS521) Driver for Windows

## Main
* This driver is mainly applicable to Xiaomi 9(Cepheus) devices,
* This FingerTipS Capacitive Touch driver (KMDF) is modified from the vhidmini in Windows Driver Samples.
* This repository has been forked from https://github.com/edk2-porting/FingerTipS-Touch for the base and modified to work with Focal Tech digitizers.

## Disclaimer

* This driver has not been completed yet, and there are still right-click issues on Xiaomi 9 (Cepheus)
* It contains debugging code and may also lack annotations.
* Due to various reasons, tracking has been replaced by DbgPrint.

## Acknowledgements

* written by 傲世惊魂 (QQ:1413930626)

## Related topics

* [Creating WDF HID Minidrivers](https://docs.microsoft.com/windows-hardware/drivers/wdf/creating-umdf-hid-minidrivers)

* [Human Input Devices Design Guide](https://docs.microsoft.com/windows-hardware/drivers/hid/)

* [Human Input Devices Reference](https://docs.microsoft.com/windows-hardware/drivers/ddi/content/_hid/)

* [UMDF HID Minidriver IOCTLs](https://docs.microsoft.com/windows-hardware/drivers/ddi/content/hidport/)

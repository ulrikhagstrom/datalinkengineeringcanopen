
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the ZCAN4USBFX_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// ZCAN4USBFX_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef ZCAN4USBFX_EXPORTS
#define ZCAN4USBFX_API __declspec(dllexport)
#else
#define ZCAN4USBFX_API __declspec(dllimport)
#endif


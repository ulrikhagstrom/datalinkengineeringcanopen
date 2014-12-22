mkdir zip
mkdir zip\libs
copy canopenlib32.lib zip\libs
copy canopenlib64.lib zip\libs
mkdir zip\dlls
copy canopenlib32.dll zip\dlls
copy canopenlib32_NET.dll zip\dlls
copy canopenlib64.dll zip\dlls
copy canopenlib64_NET.dll zip\dlls
mkdir zip\dlls\ems
mkdir zip\dlls\ixxat
mkdir zip\dlls\kvaser
mkdir zip\dlls\can232
mkdir zip\dlls\canusb
mkdir zip\dlls\castor
mkdir zip\dlls\peak
mkdir zip\dlls\tinycan
mkdir zip\dlls\zantic
mkdir zip\inc
copy ..\..\canopenlib32_dll\inc\*.* zip\inc


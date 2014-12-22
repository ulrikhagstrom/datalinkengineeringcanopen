mkdir zip
mkdir zip\libs
copy canopenlib32.lib zip\libs
copy canopenlib64.lib zip\libs
mkdir zip\dlls
copy canopenlib32.dll zip\dlls
copy canopenlib64.dll zip\dlls
mkdir zip\inc
copy ..\..\canopenlib32_dll\inc\*.* zip\inc


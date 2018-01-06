for /f "tokens=*" %%i in ('bin\date.exe +"%%y%%m%%d%%H%%M%%S"') do  set timestamp=%%i
echo %timestamp%


bin\hex2bin.exe -e raw input/data.hex

bin\sox.exe.lnk -r 8000 -e float -b 32 -c 1 %~dp0\input\data.raw %~dp0\output\data.wav

ren output\data.wav %timestamp%.wav
copy input\data.hex output\data.hex
ren output\data.hex %timestamp%.hex

del input\data.hex
del input\data.raw
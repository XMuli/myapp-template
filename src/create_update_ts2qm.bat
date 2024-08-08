@echo off

set "QT_DIR=C:/Qt/5.15.2/msvc2019_64/bin"
set "SOURCE_DIR=./"
set "OUTPUT_TS_DIR=./translations"
set "OUTPUT_QM_DIR=../bin/translations"

rem ������Ҫ�������б� https://www.afforange.com/556.html
rem set "LANGUAGES=en_us zh_cn zh_tw pt_br de zh_sg es ar fr ru en_gb pt ja it ko pl nl tr id uk th sv fi cs hi el he no da hu vi ro sr bg"
set "LANGUAGES=en_us zh_cn zh_tw"

if not exist "%OUTPUT_QM_DIR%" (
    mkdir "%OUTPUT_QM_DIR%"
)

rem ��ʼ��������
set "TOTAL_UPDATED=0"
set "SUCCESS_COUNT=0"
set "FAILURE_COUNT=0"

rem ���������б�
for %%a in (%LANGUAGES%) do (
    rem ִ�� lupdate ����
    echo ִ�� lupdate ����: %QT_DIR%/lupdate.exe -locations absolute -no-obsolete %SOURCE_DIR% -ts "%OUTPUT_TS_DIR%/%%a.ts"
    %QT_DIR%\lupdate.exe -locations absolute -no-obsolete %SOURCE_DIR% -ts "%OUTPUT_TS_DIR%/%%a.ts"
    
    rem ִ�� lrelease ����
    echo ִ�� lrelease ����: %QT_DIR%/lrelease.exe "%OUTPUT_TS_DIR%/%%a.ts" -qm "%OUTPUT_QM_DIR%/%%a.qm"
    %QT_DIR%\lrelease.exe "%OUTPUT_TS_DIR%/%%a.ts" -qm "%OUTPUT_QM_DIR%/%%a.qm"

    rem ���¼�����
    set /A "TOTAL_UPDATED+=1"
    if %ERRORLEVEL% EQU 0 (
        set /A "SUCCESS_COUNT+=1"
    ) else (
        set /A "FAILURE_COUNT+=1"
    )
)

rem ���ͳ�ƽ��
echo �ű�ִ�����
echo ������ %TOTAL_UPDATED% ���ļ���
echo �ɹ������� %SUCCESS_COUNT% ���ļ���
echo ����ʧ���� %FAILURE_COUNT% ���ļ���

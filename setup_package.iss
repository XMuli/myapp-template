; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!
#pragma parseroption -p
#define MyAppName "myapp-template"
#define MyAppVersion "1.1.0"
#define MyAppPublisher "Zhang Wengeng"
#define MyAppCompany "XMuli"
#define MyAppURL "https://myapp-template.xmuli.tech"
#define MyAppExeName "myapp-template.exe"                     
#define MyArchitecture "x64"   ; x64    x86
#define MyCOMPILER_ID "msvc"   ; mingw  msvc
#define MySrc "."
#define MyBinDir "bin"         ; 相对于 .iss 文件的路径
#define MyOutputDir "./"
#define MyGUID "{DC18A426-847A-494B-A112-1A864A8E45C0}"

[Setup]
; NOTE: The value of AppId uniquely identifies this application. Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{#MyGUID}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={autopf}\{#MyAppName}
DefaultGroupName={#MyAppName}
;DisableProgramGroupPage=yes
LicenseFile={#MyBinDir}\resources\licenses\License.md
InfoBeforeFile={#MyBinDir}\resources\licenses\PrivacyAndDataProtection.md
;InfoAfterFile={#MyBinDir}\resources\licenses\other.md
;控制面板删除的图标
UninstallDisplayIcon={app}\{#MyAppExeName}
UninstallDisplayName={#MyAppName}

; Uncomment the following line to run in non-administrative install mode (install for current user only.)
; PrivilegesRequiredOverridesAllowed 的两个参数，dialog 可选安装路径； commandline 不可选路径
PrivilegesRequired=lowest
PrivilegesRequiredOverridesAllowed=commandline   
OutputDir={#MyOutputDir}
OutputBaseFilename={#MyAppName}_setup_{#MyAppVersion}_{#MyArchitecture}_{#MyCOMPILER_ID}_qt
SetupIconFile={#MyBinDir}\resources\logo\logo.ico
Password=
Compression=lzma
SolidCompression=yes
WizardStyle=modern
;开启日志功能
;SetupLogging=yes


[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"
Name: "armenian"; MessagesFile: "compiler:Languages\Armenian.isl"
Name: "brazilianportuguese"; MessagesFile: "compiler:Languages\BrazilianPortuguese.isl"
Name: "bulgarian"; MessagesFile: "compiler:Languages\Bulgarian.isl"
Name: "catalan"; MessagesFile: "compiler:Languages\Catalan.isl"
Name: "chinesesimplified"; MessagesFile: "compiler:Languages\ChineseSimplified.isl"
Name: "chinesetraditional"; MessagesFile: "compiler:Languages\ChineseTraditional.isl"
Name: "corsican"; MessagesFile: "compiler:Languages\Corsican.isl"
Name: "czech"; MessagesFile: "compiler:Languages\Czech.isl"
Name: "danish"; MessagesFile: "compiler:Languages\Danish.isl"
Name: "dutch"; MessagesFile: "compiler:Languages\Dutch.isl"
Name: "finnish"; MessagesFile: "compiler:Languages\Finnish.isl"
Name: "french"; MessagesFile: "compiler:Languages\French.isl"
Name: "german"; MessagesFile: "compiler:Languages\German.isl"
Name: "hebrew"; MessagesFile: "compiler:Languages\Hebrew.isl"
Name: "hungarian"; MessagesFile: "compiler:Languages\Hungarian.isl"
Name: "icelandic"; MessagesFile: "compiler:Languages\Icelandic.isl"
Name: "italian"; MessagesFile: "compiler:Languages\Italian.isl"
Name: "japanese"; MessagesFile: "compiler:Languages\Japanese.isl"
Name: "norwegian"; MessagesFile: "compiler:Languages\Norwegian.isl"
Name: "polish"; MessagesFile: "compiler:Languages\Polish.isl"
Name: "portuguese"; MessagesFile: "compiler:Languages\Portuguese.isl"
Name: "russian"; MessagesFile: "compiler:Languages\Russian.isl"
Name: "slovak"; MessagesFile: "compiler:Languages\Slovak.isl"
Name: "slovenian"; MessagesFile: "compiler:Languages\Slovenian.isl"
Name: "spanish"; MessagesFile: "compiler:Languages\Spanish.isl"
Name: "turkish"; MessagesFile: "compiler:Languages\Turkish.isl"
Name: "ukrainian"; MessagesFile: "compiler:Languages\Ukrainian.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"

[Files]
Source: "{#MyBinDir}\{#MyAppExeName}"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#MyBinDir}\*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs
; NOTE: Don't use "Flags: ignoreversion" on any shared system files


;[Registry]
;Root: HKCU; Subkey: "Software\Microsoft\Windows\CurrentVersion\Uninstall\{{#MyGUID}_is1"; ValueType: string; ValueName: "DisplayIcon"; ValueData: "{app}\{#MyAppExeName},0"
;Root: HKCU; Subkey: "Software\Microsoft\Windows\CurrentVersion\Uninstall\{{#MyGUID}_is1"; ValueType: string; ValueName: "DisplayName"; ValueData: "{#MyAppName} {#MyAppVersion}"
;Root: HKCU; Subkey: "Software\Microsoft\Windows\CurrentVersion\Uninstall\{{#MyGUID}_is1"; ValueType: string; ValueName: "UninstallString"; ValueData: "{uninstallexe}"
;Root: HKCU; Subkey: "Software\Microsoft\Windows\CurrentVersion\Uninstall\{{#MyGUID}_is1"; ValueType: string; ValueName: "Publisher"; ValueData: "{#MyAppPublisher}"
;Root: HKCU; Subkey: "Software\Microsoft\Windows\CurrentVersion\Uninstall\{{#MyGUID}_is1"; ValueType: string; ValueName: "DisplayVersion"; ValueData: "{#MyAppVersion}"
;Root: HKCU; Subkey: "Software\Microsoft\Windows\CurrentVersion\Uninstall\{{#MyGUID}_is1"; ValueType: string; ValueName: "URLInfoAbout"; ValueData: "{#MyAppURL}"
;Root: HKCU; Subkey: "Software\Microsoft\Windows\CurrentVersion\Uninstall\{{#MyGUID}_is1"; ValueType: dword; ValueName: "NoModify"; ValueData: 1
;Root: HKCU; Subkey: "Software\Microsoft\Windows\CurrentVersion\Uninstall\{{#MyGUID}_is1"; ValueType: dword; ValueName: "NoRepair"; ValueData: 1

[Icons]
Name: "{group}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{group}\{cm:ProgramOnTheWeb,{#MyAppName}}"; Filename: "{#MyAppURL}"
Name: "{autodesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon

[Registry]
; 将安装路径写入注册表
Root: HKCU; Subkey: "Software\{#MyAppCompany}\{#MyAppName}"; ValueType: string; ValueName: "InstallPath"; ValueData: "{app}"

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent

[Code]
procedure InstallVCRuntime( );
var
  szAppName, szParam, szExecutable, szArchitecture, szVCRuntimeInstalled, szFileExists: String;
  nRetCode: Integer;
  bFileExists, bVCRuntimeInstalled: Boolean; // 定义一个布尔变量
  
begin
  szArchitecture := '{#MyArchitecture}';
  if szArchitecture  = 'x64' then
  begin
    szAppName := 'vc_redist.x64.exe';
  end
  else
  begin
    szAppName := 'vc_redist.x86.exe';
  end;

  szExecutable := ExpandConstant('{app}\') + szAppName;
  bVCRuntimeInstalled := RegKeyExists(HKEY_LOCAL_MACHINE, 'SOFTWARE\WOW6432Node\Microsoft\Windows\CurrentVersion\Uninstall\{8bdfe669-9705-4184-9368-db9ce581e0e7}');   // x64或x86 均为此 guid 数值
  bFileExists := FileExists(szExecutable);
  
  if bVCRuntimeInstalled then
    szVCRuntimeInstalled := 'True'
  else
    szVCRuntimeInstalled := 'False';
    
  if bFileExists then
    szFileExists := 'True'
  else
    szFileExists := 'False';

	Log('-->szAppName:' + szAppName + ' szExecutable:' + szExecutable + ' szArchitecture:' + szArchitecture + ' szVCRuntimeInstalled:' + szVCRuntimeInstalled + ' szFileExists:' + szFileExists + ' The value of {app} is: ' + ExpandConstant('{app}'));

  if bVCRuntimeInstalled then      // 系统若已经安装了 VC_redist 则跳过
    begin
    Log('VC_redist is already installed. Skipping installation.');
    end
  else
    if bFileExists then            // 此文件存在才安装
      begin
	  Log('VC_redist is install');
      szParam := '/install /quiet /norestart';
      Exec(szExecutable, szParam, '', SW_HIDE, ewWaitUntilTerminated, nRetCode);
      Sleep(100);
      end
    else
      begin
      Log('Executable file not found: ' + szExecutable);
      end
end;

procedure CurStepChanged(CurStep: TSetupStep);
var
  logfilepathname, logfilename, newfilepathname: string;
begin
  if CurStep = ssPostInstall then   // 通常是在安装程序成功安装应用程序之后执行的步骤
  begin
    InstallVCRuntime();
    Log('RESULTCODE=0');
  end;

  logfilepathname := ExpandConstant('{log}');
  logfilename := ExtractFileName(logfilepathname);
  newfilepathname := ExpandConstant('{app}\') + logfilename;

  if CurStep = ssDone then          // 当前的安装步骤是否已经完成（Done）
  begin
    FileCopy(logfilepathname, newfilepathname, false);
  end;
end;

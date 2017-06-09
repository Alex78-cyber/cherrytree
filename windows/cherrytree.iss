; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

#define MyAppName "CherryTree"
#define MyAppVersion "0.38.1"
#define MyAppPublisher "Giuseppe Penone"
#define MyAppURL "http://www.giuspen.com/cherrytree/"
#define MyAppExeName "cherrytree.exe"

[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{DBA7384C-E1C6-44B5-A3B4-C94F2F0B8C0C}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
;AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={pf}\{#MyAppName}
DefaultGroupName={#MyAppName}
DisableProgramGroupPage=yes
OutputDir=C:\Documents and Settings\giu\Desktop
LicenseFile=C:\Documents and Settings\giu\Desktop\cherrytree_{#MyAppVersion}_win32_portable\license.txt
OutputBaseFilename=cherrytree_{#MyAppVersion}_setup
SetupIconFile=C:\Documents and Settings\giu\Desktop\cherrytree_{#MyAppVersion}_win32_portable\bin\glade\cherrytree.ico
Compression=lzma
SolidCompression=yes
ChangesAssociations=yes

[Registry]
Root: HKCR; Subkey: ".ctb"; ValueType: string; ValueName: ""; ValueData: "CherryTreeB"; Flags: uninsdeletevalue
Root: HKCR; Subkey: "CherryTreeB"; ValueType: string; ValueName: ""; ValueData: "CherryTree SQLite Document"; Flags: uninsdeletekey
Root: HKCR; Subkey: "CherryTreeB\DefaultIcon"; ValueType: string; ValueName: ""; ValueData: "{app}\bin\{#MyAppExeName},0"
Root: HKCR; Subkey: "CherryTreeB\shell\open\command"; ValueType: string; ValueName: ""; ValueData: """{app}\bin\{#MyAppExeName}"" ""%1"""
Root: HKCR; Subkey: ".ctx"; ValueType: string; ValueName: ""; ValueData: "CherryTreeX"; Flags: uninsdeletevalue
Root: HKCR; Subkey: "CherryTreeX"; ValueType: string; ValueName: ""; ValueData: "CherryTree SQLite Zipped"; Flags: uninsdeletekey
Root: HKCR; Subkey: "CherryTreeX\DefaultIcon"; ValueType: string; ValueName: ""; ValueData: "{app}\bin\{#MyAppExeName},0"
Root: HKCR; Subkey: "CherryTreeX\shell\open\command"; ValueType: string; ValueName: ""; ValueData: """{app}\bin\{#MyAppExeName}"" ""%1"""
Root: HKCR; Subkey: ".ctd"; ValueType: string; ValueName: ""; ValueData: "CherryTreeD"; Flags: uninsdeletevalue
Root: HKCR; Subkey: "CherryTreeD"; ValueType: string; ValueName: ""; ValueData: "CherryTree XML Document"; Flags: uninsdeletekey
Root: HKCR; Subkey: "CherryTreeD\DefaultIcon"; ValueType: string; ValueName: ""; ValueData: "{app}\bin\{#MyAppExeName},0"
Root: HKCR; Subkey: "CherryTreeD\shell\open\command"; ValueType: string; ValueName: ""; ValueData: """{app}\bin\{#MyAppExeName}"" ""%1"""
Root: HKCR; Subkey: ".ctz"; ValueType: string; ValueName: ""; ValueData: "CherryTreeZ"; Flags: uninsdeletevalue
Root: HKCR; Subkey: "CherryTreeZ"; ValueType: string; ValueName: ""; ValueData: "CherryTree XML Zipped"; Flags: uninsdeletekey
Root: HKCR; Subkey: "CherryTreeZ\DefaultIcon"; ValueType: string; ValueName: ""; ValueData: "{app}\bin\{#MyAppExeName},0"
Root: HKCR; Subkey: "CherryTreeZ\shell\open\command"; ValueType: string; ValueName: ""; ValueData: """{app}\bin\{#MyAppExeName}"" ""%1"""

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"
Name: "brazilianportuguese"; MessagesFile: "compiler:Languages\BrazilianPortuguese.isl"
Name: "catalan"; MessagesFile: "compiler:Languages\Catalan.isl"
Name: "czech"; MessagesFile: "compiler:Languages\Czech.isl"
Name: "danish"; MessagesFile: "compiler:Languages\Danish.isl"
Name: "dutch"; MessagesFile: "compiler:Languages\Dutch.isl"
Name: "finnish"; MessagesFile: "compiler:Languages\Finnish.isl"
Name: "french"; MessagesFile: "compiler:Languages\French.isl"
Name: "german"; MessagesFile: "compiler:Languages\German.isl"
Name: "greek"; MessagesFile: "compiler:Languages\Greek.isl"
Name: "hebrew"; MessagesFile: "compiler:Languages\Hebrew.isl"
Name: "hungarian"; MessagesFile: "compiler:Languages\Hungarian.isl"
Name: "italian"; MessagesFile: "compiler:Languages\Italian.isl"
Name: "japanese"; MessagesFile: "compiler:Languages\Japanese.isl"
Name: "norwegian"; MessagesFile: "compiler:Languages\Norwegian.isl"
Name: "polish"; MessagesFile: "compiler:Languages\Polish.isl"
Name: "portuguese"; MessagesFile: "compiler:Languages\Portuguese.isl"
Name: "russian"; MessagesFile: "compiler:Languages\Russian.isl"
Name: "serbiancyrillic"; MessagesFile: "compiler:Languages\SerbianCyrillic.isl"
Name: "serbianlatin"; MessagesFile: "compiler:Languages\SerbianLatin.isl"
Name: "slovenian"; MessagesFile: "compiler:Languages\Slovenian.isl"
Name: "spanish"; MessagesFile: "compiler:Languages\Spanish.isl"
Name: "ukrainian"; MessagesFile: "compiler:Languages\Ukrainian.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked
Name: "quicklaunchicon"; Description: "{cm:CreateQuickLaunchIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked; OnlyBelowVersion: 0,6.1

[Files]
Source: "C:\Documents and Settings\giu\Desktop\cherrytree_{#MyAppVersion}_win32_portable\*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Icons]
Name: "{group}\{#MyAppName}"; Filename: "{app}\bin\{#MyAppExeName}"
Name: "{group}\{cm:ProgramOnTheWeb,{#MyAppName}}"; Filename: "{#MyAppURL}"
Name: "{group}\{cm:UninstallProgram,{#MyAppName}}"; Filename: "{uninstallexe}"
Name: "{commondesktop}\{#MyAppName}"; Filename: "{app}\bin\{#MyAppExeName}"; Tasks: desktopicon
Name: "{userappdata}\Microsoft\Internet Explorer\Quick Launch\{#MyAppName}"; Filename: "{app}\bin\{#MyAppExeName}"; Tasks: quicklaunchicon

[Run]
Filename: "{app}\bin\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent


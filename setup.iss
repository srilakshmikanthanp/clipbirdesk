; This Script used to build installer for clipbird application using inno setup

#define MyAppPublisher "srilakshmikanthanp"
#define MyAppName "clipbird"
#define MyAppVersion "1.2.0"
#define MyAppURL "https://github.com/srilakshmikanthanp/clipbirdesk"
#define MyAppExeName "clipbird.exe"

[Setup]
AppPublisher={#MyAppPublisher}
AppVerName={#MyAppName} {#MyAppVersion}
AppId={{3A8D9FCA-9F95-4947-8AB0-3E364ED765E1}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={autopf}\{#MyAppName}
DisableProgramGroupPage=yes
LicenseFile=LICENSE
OutputBaseFilename=Setup
SetupIconFile=assets\images\logo.ico
Compression=lzma
SolidCompression=yes
WizardStyle=modern
OutputDir=.\dist
UninstallDisplayIcon={app}\{#MyAppExeName}

[Registry]
Root: HKLM; Subkey: "Software\Microsoft\Windows\CurrentVersion\Run"; ValueType: string; ValueName: "{#MyAppName}"; ValueData: """{app}\{#MyAppExeName}"""; Flags: uninsdeletevalue

[Files]
Source: "D:\source\github\srilakshmikanthanp\clipbirdesk\.setup\*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Icons]
Name: "{autodesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon
Name: "{autoprograms}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"

[Run]
Filename: {app}\vc_redist.x64.exe; Parameters: "/q /norestart"; StatusMsg: "Installing VC++ Redistributables..."
Filename: msiexec.exe; Parameters: "/i ""{app}\Bonjour64.msi"" /qn /norestart"; StatusMsg: "Installing Bonjour..."
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent

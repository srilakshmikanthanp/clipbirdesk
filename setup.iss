; This Script used to build installer for clipbird application using inno setup

; constants used in the script
#define ClipbirdPublisher "srilakshmikanthanp"
#define ClipbirdName "clipbird"
#define ClipbirdVersion GetValue("conf/VERSION")
#define ClipbirdUUId GetValue("conf/UUID")
#define ClipbirdURL "https://github.com/srilakshmikanthanp/clipbirdesk"
#define ClipbirdExeName "clipbird.exe"

[Setup]
AppPublisher={#ClipbirdPublisher}
AppVerName={#ClipbirdName} {#ClipbirdVersion}
AppId={#ClipbirdUUId}
AppVersion={#ClipbirdVersion}
AppName={#ClipbirdName}
AppPublisherURL={#ClipbirdURL}
AppSupportURL={#ClipbirdURL}
AppUpdatesURL={#ClipbirdURL}
DefaultDirName={autopf}\{#ClipbirdName}
DisableProgramGroupPage=yes
LicenseFile=LICENSE
OutputBaseFilename=Setup
SetupIconFile=assets\images\logo.ico
OutputDir=.\dist
Compression=lzma
SolidCompression=yes
WizardStyle=modern
UninstallDisplayIcon={app}\{#ClipbirdExeName}

[Registry]
; To start app on windows startup
Root      : HKLM;                                             \
Subkey    : "Software\Microsoft\Windows\CurrentVersion\Run";  \
ValueType : string;                                           \
ValueName : "{#ClipbirdName}";                                \
ValueData : """{app}\{#ClipbirdExeName}""";                   \
Flags     : uninsdeletevalue

[Files]
; List of files to be included in the installer
Source  : ".\.setup\*";                                 \
DestDir : "{app}";                                      \
Flags   : ignoreversion recursesubdirs createallsubdirs

[Languages]
; Include English language
Name          : "english";              \
MessagesFile  : "compiler:Default.isl"

[Tasks]
; Create desktop icon Optionality
Name              : "desktopicon";             \
Description       : "{cm:CreateDesktopIcon}";  \
GroupDescription  : "{cm:AdditionalIcons}";    \
Flags             : unchecked

[Icons]
; Desktop Icon
Name      : "{autodesktop}\{#ClipbirdName}"; \
Filename  : "{app}\{#ClipbirdExeName}";      \
Tasks     : desktopicon

; Start Menu Icon
Name      : "{autoprograms}\{#ClipbirdName}"; \
Filename  : "{app}\{#ClipbirdExeName}"

[Run]
; Install Bonjour service for Windows
Filename    : msiexec.exe;                                    \
Parameters  : "/i ""{app}\Bonjour64.msi"" /qn /norestart";    \
StatusMsg   : "Installing Bonjour..."

; Install VC++ Redistributables
Filename    : {app}\vc_redist.x64.exe;              \
Parameters  : "/q /norestart";                      \
StatusMsg   : "Installing VC++ Redistributables..."

; Start the application after installation
Filename    : "{app}\{#ClipbirdExeName}";                                    \
Description : "{cm:LaunchProgram,{#StringChange(ClipbirdName, '&', '&&')}}"; \
Flags       : nowait postinstall skipifsilent

; function used read UUID from file
[Code]
function GetValue(FileName: String): String;
var
  F: TextFile;
  S: String;
begin
  Result := '';
  if not FileExists(FileName) then
    Exit;
  AssignFile(F, FileName);
  Reset(F);
  Readln(F, S);
  CloseFile(F);
  Result := S;
end;

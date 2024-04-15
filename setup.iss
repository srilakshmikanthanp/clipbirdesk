; This Script used to build installer for clipbird application using inno setup

; constants used in the script
#define ClipbirdPublisher "srilakshmikanthanp"
#define ClipbirdName "clipbird"
#define ClipbirdVersion "1.3.2"
#define ClipbirdUUID "3A8D9FCA-9F95-4947-8AB0-3E364ED765E1"
#define ClipbirdURL "https://github.com/srilakshmikanthanp/clipbirdesk"
#define ClipbirdExeName "clipbird.exe"

[Setup]
AppPublisher={#ClipbirdPublisher}
AppVerName={#ClipbirdName} {#ClipbirdVersion}
AppId={#ClipbirdUUID}
AppVersion={#ClipbirdVersion}
AppName={#ClipbirdName}
AppPublisherURL={#ClipbirdURL}
AppSupportURL={#ClipbirdURL}
AppUpdatesURL={#ClipbirdURL}
DefaultDirName={autopf}\{#ClipbirdName}
DisableProgramGroupPage=yes
LicenseFile=NOTICE
OutputBaseFilename=clipbird-x64
SetupIconFile=assets\images\logo.ico
OutputDir=.\dist
Compression=lzma
SolidCompression=yes
WizardStyle=modern
UninstallDisplayIcon={app}\{#ClipbirdExeName}
UsePreviousLanguage=no

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

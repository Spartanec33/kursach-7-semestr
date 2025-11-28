[Setup]
AppName=Factory Graph Editor
AppVersion=1.0.0
AppPublisher=Лазарев Артём Андреевич
DefaultDirName={autopf}\Factory GE
DefaultGroupName=Factory GE
OutputDir=output
OutputBaseFilename=Factory Graph Editor Setup
Compression=lzma2
SolidCompression=yes
ShowLanguageDialog=yes
LicenseFile=components\license_ru.txt
SetupIconFile=components\appIcon.ico
WizardStyle=modern
DisableWelcomePage=no

[Languages]
Name: "russian"; MessagesFile: "compiler:Languages\Russian.isl"

[CustomMessages]
russian.MyAppDescription=Программа для построения графов и их поставок
russian.LicenseAccepted=Я &принимаю условия соглашения

[Tasks]
Name: "desktopicon"; Description: "Создать &ярлык на рабочем столе"; GroupDescription: "Дополнительные ярлыки:"
Name: "quicklaunchicon"; Description: "Создать ярлык в &панели быстрого запуска"; GroupDescription: "Дополнительные ярлыки:"; Flags: unchecked

[Files]
Source: "..\release\kursach.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\release\*.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\release\platforms\*.dll"; DestDir: "{app}\platforms"; Flags: ignoreversion
Source: "..\config.ini"; DestDir: "{app}"; Flags: ignoreversion onlyifdoesntexist

[Icons]
Name: "{group}\Factory GE"; Filename: "{app}\kursach.exe"; IconFilename: "{app}\kursach.exe"
Name: "{autodesktop}\Factory GE"; Filename: "{app}\kursach.exe"; Tasks: desktopicon; IconFilename: "{app}\kursach.exe"
Name: "{userappdata}\Microsoft\Internet Explorer\Quick Launch\Factory GE"; Filename: "{app}\kursach.exe"; Tasks: quicklaunchicon; IconFilename: "{app}\kursach.exe"

[Run]
Filename: "{app}\kursach.exe"; Description: "Запустить Factory GE"; Flags: nowait postinstall skipifsilent

[Messages]
WelcomeLabel1=Добро пожаловать в программу установки [name]
WelcomeLabel2=Эта программа установит [name] на ваш компьютер.%n%nПеред установкой необходимо принять лицензионное соглашение.
ClickNext=Нажмите Далее для продолжения.
WizardLicense=Лицензионное соглашение
LicenseLabel=Пожалуйста, прочтите следующее лицензионное соглашение. Для продолжения установки вы должны принять условия соглашения.
LicenseLabel3=
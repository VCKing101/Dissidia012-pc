; The Windows installer: a wizard that asks for the install folder and the player's ISO, installs the program
; (built without the game's code, with the compiler beside it) and, when the player ticks it, the Battle BGM Maker,
; and sets the game up from the ISO:
;
;   D012Native --install <iso> [--in-place]   decrypts the executable into data\ (PortableKit's installer)
;   D012Native --compile-game                 makes the game's code into game-code\ (host/d012_compile_game.cpp)
;
; Built by tools/port/package_installer.py, which stages the folder and passes AppVersion, Stage and OutputDir:
;   ISCC /DAppVersion=0.3.0 /DStage=<staged folder> /DOutputDir=<dir> dissidia012.iss
; Silent install for tests: setup.exe /VERYSILENT /CURRENTUSER /ISO="<path>" [/DIR="<folder>"] [/COPYISO]
;
; MIT licence; see README.md at the repository root.

#ifndef AppVersion
  #error Pass /DAppVersion=<version>
#endif
#ifndef Stage
  #error Pass /DStage=<the staged folder>
#endif
#ifndef OutputDir
  #define OutputDir "."
#endif

#define AppName "Dissidia 012 (PC port)"
#define AppExe "D012Native.exe"

[Setup]
AppId={{E3C5076B-EFF5-41BF-97A4-BC9738C4C436}
AppName={#AppName}
AppVersion={#AppVersion}
AppVerName={#AppName} {#AppVersion}
AppPublisher=VCKing101
AppPublisherURL=https://github.com/VCKing101/Dissidia012-pc
AppSupportURL=https://github.com/VCKing101/Dissidia012-pc/issues
DefaultDirName={localappdata}\Programs\Dissidia012
DefaultGroupName={#AppName}
DisableProgramGroupPage=yes
; Everything goes in the player's own folders: no administrator prompt.
PrivilegesRequired=lowest
ArchitecturesAllowed=x64compatible
ArchitecturesInstallIn64BitMode=x64compatible
MinVersion=10.0
OutputDir={#OutputDir}
OutputBaseFilename=Dissidia012-PC-Setup-{#AppVersion}
Compression=lzma2/ultra64
SolidCompression=yes
WizardStyle=modern
InfoBeforeFile={#Stage}\README.txt
UninstallDisplayIcon={app}\{#AppExe}
UninstallDisplayName={#AppName}
; The game's code (~200 MB after compiling) and the disc image when copied are made after the files.
ExtraDiskSpaceRequired=1500000000

[Tasks]
Name: "desktopicon"; Description: "Create a desktop shortcut"; GroupDescription: "Shortcuts:"
Name: "bgmmaker"; Description: "Battle BGM Maker: a tool for making your own battle music mods, with loop points (about 4 MB). Playing mods made by others does not need it."; GroupDescription: "Tools:"; Flags: unchecked

[Files]
; The Battle BGM Maker (port/bgm_maker) and the encoder its "Make a PSP / PPSSPP ISO" runs, with the encoder's
; licences and source, only when the player chose it.
#define BgmMakerFiles "\BgmMaker.exe,\atracdenc.exe,\licenses\atracdenc-*,\licenses\libgha-LICENSE.txt,\licenses\KISS-FFT-LICENSE.txt"
Source: "{#Stage}\*"; Excludes: "{#BgmMakerFiles}"; DestDir: "{app}"; Flags: recursesubdirs createallsubdirs ignoreversion
Source: "{#Stage}\BgmMaker.exe"; DestDir: "{app}"; Tasks: bgmmaker; Flags: ignoreversion
Source: "{#Stage}\atracdenc.exe"; DestDir: "{app}"; Tasks: bgmmaker; Flags: ignoreversion
Source: "{#Stage}\licenses\atracdenc-*"; DestDir: "{app}\licenses"; Tasks: bgmmaker; Flags: ignoreversion
Source: "{#Stage}\licenses\libgha-LICENSE.txt"; DestDir: "{app}\licenses"; Tasks: bgmmaker; Flags: ignoreversion
Source: "{#Stage}\licenses\KISS-FFT-LICENSE.txt"; DestDir: "{app}\licenses"; Tasks: bgmmaker; Flags: ignoreversion

[InstallDelete]
; The Python Battle BGM Maker of 0.3.0 and the FFmpeg programs it ran: BgmMaker.exe replaces them.
Type: filesandordirs; Name: "{app}\bgm-maker"
Type: files; Name: "{app}\ffmpeg.exe"
Type: files; Name: "{app}\ffprobe.exe"
Type: files; Name: "{app}\avdevice-61.dll"
Type: files; Name: "{app}\avfilter-10.dll"
Type: files; Name: "{app}\swscale-8.dll"
Type: files; Name: "{app}\licenses\Python-LICENSE.txt"
; The Battle BGM Maker of an earlier install, when the player unticked it this time.
Type: files; Name: "{app}\BgmMaker.exe"; Tasks: not bgmmaker
Type: files; Name: "{app}\atracdenc.exe"; Tasks: not bgmmaker
Type: files; Name: "{app}\licenses\atracdenc-*"; Tasks: not bgmmaker
Type: files; Name: "{app}\licenses\libgha-LICENSE.txt"; Tasks: not bgmmaker
Type: files; Name: "{app}\licenses\KISS-FFT-LICENSE.txt"; Tasks: not bgmmaker
Type: files; Name: "{group}\Battle BGM Maker.lnk"; Tasks: not bgmmaker

[Dirs]
Name: "{app}\data\mods"; Flags: uninsneveruninstall

[Icons]
Name: "{group}\{#AppName}"; Filename: "{app}\{#AppExe}"; WorkingDir: "{app}"
Name: "{group}\Battle BGM Maker"; Filename: "{app}\BgmMaker.exe"; WorkingDir: "{app}"; Comment: "Make your own battle music mods, with their loop points"; Tasks: bgmmaker
Name: "{group}\Mods folder"; Filename: "{app}\data\mods"
Name: "{group}\Uninstall {#AppName}"; Filename: "{uninstallexe}"
Name: "{autodesktop}\{#AppName}"; Filename: "{app}\{#AppExe}"; WorkingDir: "{app}"; Tasks: desktopicon

[Run]
Filename: "{app}\{#AppExe}"; WorkingDir: "{app}"; Description: "Play now"; Flags: postinstall nowait skipifsilent

[UninstallDelete]
; What setup made from the player's disc. data\ (saves, settings, mods) is removed only if the player says so.
Type: filesandordirs; Name: "{app}\game-code"
Type: filesandordirs; Name: "{app}\game-code.new"
Type: filesandordirs; Name: "{app}\game-code.old"
Type: files; Name: "{app}\game-code-status.txt"
Type: files; Name: "{app}\game-code-build.log"
Type: files; Name: "{app}\setup-install.log"
Type: files; Name: "{app}\data\EBOOT.ELF"
Type: files; Name: "{app}\data\disc.iso"
Type: dirifempty; Name: "{app}"

[Code]
const
  IsoPageText = 'Choose the .iso of your own Dissidia 012 Duodecim Final Fantasy disc (USA, ULUS10566). '
    + 'Setup reads the game from it, decrypts its program and compiles the game''s code for your PC. '
    + 'Nothing is downloaded, and nothing made from your disc leaves your PC.';
  MinimumIsoBytes = 100000000;

var
  IsoPage: TInputFileWizardPage;
  CopyIsoBox: TNewCheckBox;
  ProgressPage: TOutputProgressWizardPage;
  LastLine: String;

function IsoPath(): String;
begin
  Result := ExpandConstant('{param:ISO|}');
  if (Result = '') and (IsoPage <> nil) then Result := IsoPage.Values[0];
end;

function CopyIso(): Boolean;
begin
  Result := (Pos('/COPYISO', UpperCase(GetCmdTail)) > 0) or ((CopyIsoBox <> nil) and CopyIsoBox.Checked);
end;

procedure InitializeWizard;
begin
  IsoPage := CreateInputFilePage(wpSelectDir, 'Your copy of the game', 'Where is your disc image?', IsoPageText);
  IsoPage.Add('Disc image (.iso):', 'Disc images (*.iso)|*.iso|All files|*.*', '.iso');
  IsoPage.Values[0] := GetPreviousData('IsoPath', ExpandConstant('{param:ISO|}'));

  CopyIsoBox := TNewCheckBox.Create(IsoPage);
  CopyIsoBox.Parent := IsoPage.Surface;
  CopyIsoBox.Top := IsoPage.Edits[0].Top + IsoPage.Edits[0].Height + ScaleY(16);
  CopyIsoBox.Width := IsoPage.SurfaceWidth;
  CopyIsoBox.Height := ScaleY(34);
  CopyIsoBox.Caption := 'Copy the disc image into the install folder (1.7 GB more), so the original can be moved '
    + 'or deleted. Otherwise the game reads it where it is.';
  CopyIsoBox.Checked := GetPreviousData('CopyIso', '0') = '1';

  ProgressPage := CreateOutputProgressPage('Setting up the game',
    'Setup is making the game from your disc. This takes a few minutes; the PC may be busy meanwhile.');
end;

procedure RegisterPreviousData(PreviousDataKey: Integer);
begin
  SetPreviousData(PreviousDataKey, 'IsoPath', IsoPath());
  if CopyIso() then
    SetPreviousData(PreviousDataKey, 'CopyIso', '1')
  else
    SetPreviousData(PreviousDataKey, 'CopyIso', '0');
end;

function NextButtonClick(CurPageID: Integer): Boolean;
var
  Size: Int64;
begin
  Result := True;
  if (IsoPage <> nil) and (CurPageID = IsoPage.ID) then begin
    if not FileExists(IsoPage.Values[0]) then begin
      MsgBox('Choose your disc image: the file was not found.', mbError, MB_OK);
      Result := False;
    end else if CompareText(ExtractFileExt(IsoPage.Values[0]), '.iso') <> 0 then begin
      MsgBox('The disc image must be an .iso file (CSO and other compressed formats are not read).', mbError, MB_OK);
      Result := False;
    end else if FileSize64(IsoPage.Values[0], Size) and (Size < MinimumIsoBytes) then begin
      MsgBox('That file is too small to be the Dissidia 012 disc image.', mbError, MB_OK);
      Result := False;
    end;
  end;
end;

{ Each line the program prints: "[compile-game] <percent>% <message>" moves the progress bar. }
procedure OnOutput(const S: String; const Error, FirstLine: Boolean);
var
  Line, Number: String;
  Space, Percent: Integer;
begin
  Log(S);
  Line := Trim(S);
  if Line = '' then exit;
  LastLine := Line;
  if Pos('[compile-game] ', Line) = 1 then begin
    Line := Copy(Line, Length('[compile-game] ') + 1, MaxInt);
    Space := Pos('% ', Line);
    if Space > 0 then begin
      Number := Copy(Line, 1, Space - 1);
      Percent := StrToIntDef(Number, -1);
      if Percent >= 0 then begin
        ProgressPage.SetText(Copy(Line, Space + 2, MaxInt), '');
        ProgressPage.SetProgress(Percent, 100);
      end;
    end;
  end;
end;

function Run(const Params, What: String): Boolean;
var
  Code: Integer;
begin
  LastLine := '';
  Log('Running ' + ExpandConstant('{app}\{#AppExe}') + ' ' + Params);
  Result := ExecAndLogOutput(ExpandConstant('{app}\{#AppExe}'), Params, ExpandConstant('{app}'), SW_HIDE,
    ewWaitUntilTerminated, Code, @OnOutput) and (Code = 0);
  if not Result then
    MsgBox(What + ' did not succeed.' + #13#10#13#10 + LastLine + #13#10#13#10
      + 'The log is in ' + ExpandConstant('{app}') + ' (game-code-build.log) and in Setup''s log. '
      + 'Run Setup again to retry; your saves are kept.', mbError, MB_OK);
end;

function AlreadyInstalledFrom(const Iso: String): Boolean;
begin
  { The decrypted executable is there and was made from this disc image, kept where it was. }
  Result := FileExists(ExpandConstant('{app}\data\EBOOT.ELF'))
    and (CompareText(GetPreviousData('IsoPath', ''), Iso) = 0)
    and (GetPreviousData('CopyIso', '0') = IntToStr(Ord(CopyIso())));
end;

procedure CurStepChanged(CurStep: TSetupStep);
var
  Iso, Params: String;
begin
  if CurStep <> ssPostInstall then exit;
  Iso := IsoPath();
  ProgressPage.Show;
  try
    if not AlreadyInstalledFrom(Iso) then begin
      ProgressPage.SetText('Reading your disc and decrypting the game''s program', '');
      ProgressPage.SetProgress(0, 100);
      Params := '--install ' + AddQuotes(Iso);
      if not CopyIso() then Params := Params + ' --in-place';
      if not Run(Params, 'Setting the game up from your disc image') then exit;
    end;
    ProgressPage.SetText('Making the game''s code for your PC', '');
    Run('--compile-game', 'Compiling the game''s code');
  finally
    ProgressPage.Hide;
  end;
end;

{ Uninstalling: the saves, settings and mods in data\ stay unless the player chooses otherwise. }
var
  DeleteData: Boolean;

function InitializeUninstall(): Boolean;
begin
  DeleteData := (not UninstallSilent) and (MsgBox('Also delete your saves, settings and mods?' + #13#10#13#10
    + 'Choose No to keep them in ' + ExpandConstant('{app}\data') + ' for a later install.',
    mbConfirmation, MB_YESNO or MB_DEFBUTTON2) = IDYES);
  Result := True;
end;

procedure CurUninstallStepChanged(CurUninstallStep: TUninstallStep);
begin
  if (CurUninstallStep = usPostUninstall) and DeleteData then
    DelTree(ExpandConstant('{app}\data'), True, True, True);
end;

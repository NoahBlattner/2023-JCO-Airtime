:: Ce script génère l'executable de gameframework en mode release pour le d�ploiement.
:: Auteur : Jérome Conus
:: Date   : 06.1209.2022

@ECHO OFF

::Avant de changer de dossier, on mémorise le dossier actuel
::set OLDDIR=%CD%

set PATH=%PATH%;C:\Qt\Tools\mingw1120_64\bin

C:\Qt\6.4.0\mingw_64\bin\qmake.exe ..\src\2023-JCO-Airtime.pro

C:\Qt\Tools\mingw1120_64\bin\mingw32-make.exe -f Makefile qmake_all

:: Compilation et link
C:\Qt\Tools\mingw1120_64\bin\mingw32-make.exe -j2

IF ERRORLEVEL 1 GOTO errorHandling
:: errorhandling, errorlevel >= 1

:no_error
:: Nettoyage
C:\Qt\Tools\mingw1120_64\bin\mingw32-make.exe clean -j2

:: Création du dossier release qui contiendra les fichiers de d�ploiement
mkdir ..\release
copy .\release\2023-JCO-Airtime.exe ..\release\ /Y
xcopy ..\res ..\release\res\ /E /Y

c:\Qt\6.4.0\mingw_64\bin\windeployqt.exe ..\release\2023-JCO-Airtime.exe

:: Suppression du dossier inutiles créés par windeployqt
rmdir ..\release\translations /S /Q
rmdir ..\release\tls /S /Q
:: Rétablissement du dossier d'origine
::cd %OLDDIR%
exit

:errorHandling
echo Erreur de compilation !
pause
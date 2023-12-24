"C:\Program Files\MRE SDK V3.0.00\tools\DllPackage.exe" "D:\MyGitHub\Config_parser\Config_parser.vcproj"
if %errorlevel% == 0 (
 echo postbuild OK.
  copy Config_parser.vpp ..\..\..\MoDIS_VC9\WIN32FS\DRIVE_E\Config_parser.vpp /y
exit 0
)else (
echo postbuild error
  echo error code: %errorlevel%
  exit 1
)


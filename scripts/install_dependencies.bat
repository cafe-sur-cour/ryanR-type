if not exist "vcpkg\" goto clone
if not exist "vcpkg\bootstrap-vcpkg.bat" goto remove_and_clone
goto bootstrap

:remove_and_clone
rmdir /s /q vcpkg

:clone
git clone https://github.com/Microsoft/vcpkg.git

:bootstrap
cd vcpkg

call .\bootstrap-vcpkg.bat

set VCPKG_ROOT=%cd%
set PATH=%PATH%;%VCPKG_ROOT%

echo VCPKG_ROOT set to %VCPKG_ROOT%

setx VCPKG_ROOT "%VCPKG_ROOT%"

echo.
echo =====================================================
echo VCPKG_ROOT has been set permanently to: %VCPKG_ROOT%
echo Please restart your terminal for changes to take effect.
echo =====================================================

cd ..

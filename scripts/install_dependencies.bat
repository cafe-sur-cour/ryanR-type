if not exist "vcpkg\" goto clone
if not exist "vcpkg\bootstrap-vcpkg.bat" goto remove_and_clone
goto bootstrap

:remove_and_clone
rmdir /s /q vcpkg

:clone
git clone https://github.com/Microsoft/vcpkg.git

:bootstrap
cd vcpkg

.\bootstrap-vcpkg.bat

set VCPKG_ROOT=%cd%
set PATH=%PATH%;%VCPKG_ROOT%

echo VCPKG_ROOT set to %VCPKG_ROOT%

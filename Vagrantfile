# -*- mode: ruby -*-
# vi: set ft=ruby :

$script = <<-SCRIPT
$ErrorActionPreference = "Stop"
echo "Installing Visual Studio..".
choco install -y visualstudio2019community --package-parameters "--add Microsoft.VisualStudio.Component.WinXP --add Microsoft.VisualStudio.ComponentGroup.NativeDesktop.WinXP --add Microsoft.VisualStudio.Component.VC.ATLMFC --add Microsoft.VisualStudio.Component.VC.v141.x86.x64".
choco install -y visualstudio2019-workload-nativedesktop
echo "Installing cmake.."
choco install -y cmake --version 3.20.2
echo "Installing premake.."
curl.exe --location --output premake.zip --url "https://github.com/premake/premake-core/releases/download/v5.0.0-alpha16/premake-5.0.0-alpha16-windows.zip"
Expand-Archive -Force premake.zip C:\\Windows\\system32
echo "Installing conan.."
choco install -y conan --version 1.36.0
echo "Configuring conan.."
refreshenv
# Even after refreshenv the command conan is not found.
# Therefore the full path is specified here.
# If the user uses `vagrant powershell` later, conan is in the PATH.
& "C:\\Program Files\\Conan\\conan\\conan" profile new default --detect
& "C:\\Program Files\\Conan\\conan\\conan" profile update settings.compiler.cppstd=17 default
& "C:\\Program Files\\Conan\\conan\\conan" profile update settings.arch=x86 default
& "C:\\Program Files\\Conan\\conan\\conan" profile update settings.compiler.toolset=v141_xp default
& "C:\\Program Files\\Conan\\conan\\conan" profile update settings.compiler.runtime=MT default
& "C:\\Program Files\\Conan\\conan\\conan" remote add bincrafters "https://api.bintray.com/conan/bincrafters/public-conan"
SCRIPT

Vagrant.configure("2") do |config|
  config.vm.box = "gusztavvargadr/windows-10"
  config.vm.box_version = "2009.0.2012"
  config.vm.provision "shell", inline: $script
end

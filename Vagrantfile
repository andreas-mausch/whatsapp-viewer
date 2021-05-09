# -*- mode: ruby -*-
# vi: set ft=ruby :

$script = <<-SCRIPT
echo "Installing Visual Studio".
choco install -y visualstudio2019community --package-parameters "--add Microsoft.VisualStudio.Component.WinXP --add Microsoft.VisualStudio.Component.VC.ATLMFC".
choco install -y visualstudio2019-workload-nativedesktop
SCRIPT

Vagrant.configure("2") do |config|
  config.vm.box = "gusztavvargadr/windows-10"
  config.vm.provision "shell", inline: $script
end

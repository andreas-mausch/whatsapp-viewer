# -*- mode: ruby -*-
# vi: set ft=ruby :

$script = <<-SCRIPT
$ErrorActionPreference = "Stop"
echo "Installing Chocolatey package manager"
Set-ExecutionPolicy Bypass -Scope Process -Force; [System.Net.ServicePointManager]::SecurityProtocol = [System.Net.ServicePointManager]::SecurityProtocol -bor 3072; iex ((New-Object System.Net.WebClient).DownloadString('https://community.chocolatey.org/install.ps1'))
echo "Installing Visual Studio"
choco install -y visualstudio2019community --package-parameters "--add Microsoft.VisualStudio.Component.WinXP --add Microsoft.VisualStudio.Component.VC.ATLMFC" --version=16.11.21.0
choco install -y visualstudio2019-workload-nativedesktop --version=1.0.1
choco install -y rsync --version=6.2.7
SCRIPT

Vagrant.configure("2") do |config|
  config.vm.box = "peru/windows-10-enterprise-x64-eval"
  config.vm.box_version = "20221106.01"
  config.vm.hostname = "whatsapp-viewer-win10"

  config.vm.provider "libvirt" do |vb|
    vb.memory = 4096
    vb.cpus = 4
  end

  config.vm.provision "shell", inline: $script
  # Enable the synced folder after the provisioning,
  # it requires rsync to be installed.
  config.vm.synced_folder ".", "C:\\vagrant", disabled: true
end

**NOTE:** We never did anything with this since it was deemed out of scope for the thesis.

On this page we explain what has to be done to install and enable MiniKube to work with Intel SGX.

# MiniKube

Installed MiniKube with instructions here: <https://minikube.sigs.k8s.io/docs/start/linux/>

## Set kvm2 to default

`minikube config set driver kvm2`

## kubectl

Installed kubectl since MiniKube recommended it. <https://kubernetes.io/docs/tasks/tools/install-kubectl/#install-using-native-package-management>

## KVM

Installed KVM with instructions here: <https://help.ubuntu.com/community/KVM/Installation>

(Ubuntu 18.10 instructions are alright)

### Add user to kvm group
`usermod -a -G kvm $USER`

### Enable intel\_iommu

Since virt-host-validate complained a little I performed the following changes

Modify _/etc/default/grub_ from

`GRUB_CMDLINE_LINUX_DEFAULT=""`
To

`GRUB_CMDLINE_LINUX_DEFAULT="intel_iommu=on"`

Run
```
sudo update-grub
sudo reboot
```

### Installing Virtualization support (KVM and QEMU) for Intel SGX
Instructions: <https://software.intel.com/en-us/articles/virtualizing-intel-software-guard-extensions-with-kvm-and-qemu>

NOTE: Use a newer tag than the one suggested in the article if it exists...? but this might install a newer kernel than needed, should be investigated.

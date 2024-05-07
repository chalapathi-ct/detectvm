# VMware and VBox Detector 👋
![Swagger Validator](https://img.shields.io/swagger/valid/3.0?specUrl=https%3A%2F%2Fraw.githubusercontent.com%2FOAI%2FOpenAPI-Specification%2Fmaster%2Fexamples%2Fv2.0%2Fjson%2Fpetstore-expanded.json) ![GitHub last commit](https://img.shields.io/github/last-commit/mayhapsDev/detectvm) ![rating](https://img.shields.io/badge/Rating-5%2F5-brightgreen)

## ✨ Overview ✨
- Can detect VMware (If VMware tools are installed)
- Can detect Oracle VirtualBox
- Can self delete if a vm is detected
- Header file is included with all the functions in a namespace DetectVM

Checks for the psuedo device ``\\.\VBoxMiniRdrDN`` which exists in virtualbox vms.  
Checks for the vmware tools registry ``SOFTWARE\VMware, Inc.\VMware Tool`` .  
Self Delete function spawns a hidden cmd shell which gets the file path and deletes it.  

## Prerequisites

Uses standard libraries  
Can be built using g++ (Sometimes cannot run on VMs using gcc unsure why)
```
g++ antivm.cpp -o antivm.exe -static-libgcc -static-libstdc++  -static -lpthread
```

## Demo 🎥
### VMware Demonstration
![VMware Demonstration](demos/demo.gif)
### VirtualBox Demonstration
![VirtualBox Demonstration](demos/demo2.gif)

## Author

- char*#1337 on discord
- mayhapsDev on GitHub

## 🤝 Contributing

Contributions, issues and feature requests are welcome!

Feel free to check 

## Show your support

Give a ⭐️ if this project helped you!


## To-Do

- [ ] Add KVM detection
- [x] Add Hyper-V detection
- [ ] Add QEMU detection
- [ ] Improve VMware detection

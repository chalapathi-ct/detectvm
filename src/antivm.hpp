#include <windows.h>
#include <tchar.h>
#include <stdbool.h>

#include "hyperv.hpp"
#include <cstdint>

bool cpuid_check() {
    unsigned int eax, ebx, ecx, edx;
    char hyper_vendor_id[13];

    __asm__ __volatile__("cpuid"
                         : "=a" (eax), "=b" (ebx), "=c" (ecx), "=d" (edx)
                         : "a" (0x1));

    if (ecx & (1 << 31)) {
        __asm__ __volatile__("cpuid"
                             : "=a" (eax), "=b" (ebx), "=c" (ecx), "=d" (edx)
                             : "a" (0x40000000));

        memcpy(hyper_vendor_id + 0, &ebx, 4);
        memcpy(hyper_vendor_id + 4, &ecx, 4);
        memcpy(hyper_vendor_id + 8, &edx, 4);
        hyper_vendor_id[12] = '\0';

        if (!strcmp(hyper_vendor_id, "VMwareVMware")) {
            return true; 
        }
    }
    
    return false;
}

bool bios_version_check()
{
    HKEY hKey2;
    LONG openResult = RegOpenKeyExA(
        HKEY_LOCAL_MACHINE,
        "HARDWARE\\DESCRIPTION\\System\\BIOS",
        0,
        KEY_READ,
        &hKey2);
    if (openResult != ERROR_SUCCESS)
    {
        return false;
    }

    DWORD dataSize = 0;
    LONG queryResult = RegQueryValueExA(
        hKey2,
        "BIOSVersion",
        NULL,
        NULL,
        NULL,
        &dataSize);

    if (queryResult != ERROR_SUCCESS)
    {
        RegCloseKey(hKey2);
        return false;
    }

    LPBYTE dataBuffer = new BYTE[dataSize];

    LONG getValueResult = RegGetValueA(
        hKey2,
        NULL,
        "BIOSVersion",
        RRF_RT_REG_SZ,
        NULL,
        dataBuffer,
        &dataSize);

    RegCloseKey(hKey2);
    if (getValueResult != ERROR_SUCCESS)
    {
        delete[] dataBuffer;
        return false;
    }

    if (strncmp(reinterpret_cast<const char *>(dataBuffer), "VMW", 3) == 0 || strncmp(reinterpret_cast<const char *>(dataBuffer), "VMware", 6) == 0)
    {

        delete[] dataBuffer;
        return true;
    }
    delete[] dataBuffer;
    return false;
}

namespace DetectVM
{
    bool IsVboxVM()
    {
        HANDLE handle = CreateFile(_T("\\\\.\\VBoxMiniRdrDN"), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if (handle != INVALID_HANDLE_VALUE)
        {
            CloseHandle(handle);
            return true;
        }
        return false;
    }

    bool IsVMwareVM()
    {
        HKEY hKey = 0;
        long res = RegOpenKeyEx(HKEY_LOCAL_MACHINE, TEXT("SOFTWARE\\VMware, Inc.\\VMware Tools"), 0, KEY_QUERY_VALUE, &hKey);
        if (res == ERROR_SUCCESS)
        {
            RegCloseKey(hKey);
            return true;
        } 
        return cpuid_check() && bios_version_check();
    }

    bool IsMsHyperV()
    {
        return HyperV::DetectBySystemManufacturer() || HyperV::DetectByBiosVendor() || HyperV::DetectBySystemFamily() || HyperV::DetectByProductName();
    }
}
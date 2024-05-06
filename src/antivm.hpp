#pragma once

#include <windows.h>
#include <tchar.h>
#include <stdbool.h>

#include "hyperv.hpp"

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
        RegCloseKey(hKey);
        if (res == -1)
        {
            return true;
        }
        else
        {
            HKEY hKey;
            LONG openResult = RegOpenKeyExA(
                HKEY_LOCAL_MACHINE,
                "HARDWARE\\DESCRIPTION\\System\\BIOS",
                0,
                KEY_READ,
                &hKey);
            if (openResult != ERROR_SUCCESS)
            {
                return false;
            }


            DWORD dataSize = 0;
            LONG queryResult = RegQueryValueExA(
                hKey,
                "BIOSVersion",
                NULL,
                NULL,
                NULL,
                &dataSize);
            
            
            if (queryResult != ERROR_SUCCESS)
            {
                RegCloseKey(hKey);
                return false;
            }

            LPBYTE dataBuffer = new BYTE[dataSize];

            LONG getValueResult = RegGetValueA(
                hKey,
                NULL,
                "BIOSVersion",
                RRF_RT_REG_SZ,
                NULL,
                dataBuffer,
                &dataSize);

            if (getValueResult != ERROR_SUCCESS)
            {
                delete[] dataBuffer;
                RegCloseKey(hKey);
                return false;
            }
            hKey = 0;
            res = RegOpenKeyEx(HKEY_LOCAL_MACHINE, TEXT("SOFTWARE\\VMware, Inc.\\VMware Drivers"), 0, KEY_QUERY_VALUE, &hKey);
            RegCloseKey(hKey);

            if ( res == ERROR_SUCCESS && (strncmp(reinterpret_cast<const char *>(dataBuffer), "VMW", 3) == 0 || strncmp(reinterpret_cast<const char *>(dataBuffer), "VMware", 6) == 0))
            { 
                return true;
            }
            else
            {
                return false; 
            }
            delete[] dataBuffer;
            RegCloseKey(hKey);
        }
        return false;
    }

    bool IsMsHyperV()
    {
        return HyperV::DetectBySystemManufacturer() || HyperV::DetectByBiosVendor() || HyperV::DetectBySystemFamily() || HyperV::DetectByProductName();
    }
}
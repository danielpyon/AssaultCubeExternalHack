// AssaultCubeExternalHack.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <iostream>
#include <Windows.h>
#include <vector>
#include "proc.h"


int main()
{
    // Get ProcId of target process
    DWORD procId = GetProcId(L"ac_client.exe");

    // Get module base address
    uintptr_t moduleBase = GetModuleBaseAddress(procId, L"ac_client.exe");

    // Get handle to process
    HANDLE hProcess = NULL;
    hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, procId);

    // Resolve base address of pointer chain
    uintptr_t dynamicPtrBaseAddr = moduleBase + 0x10f4f4; // offset to entity
    std::cout << "DynamicPtrBaseAddr = " << "0x" << std::hex << dynamicPtrBaseAddr << std::endl;

    // Resolve ammo pointer chain
    std::vector<unsigned int> ammoOffsets = { 0x374, 0x14, 0x00 };
    uintptr_t ammoAddr = FindDMAAddy(hProcess, dynamicPtrBaseAddr, ammoOffsets);

    // Read ammo value
    int ammoValue = 0;
    ReadProcessMemory(hProcess, (BYTE*)ammoAddr, &ammoValue, sizeof(ammoValue), nullptr);
    std::cout << "Current ammo = " << std::dec << ammoValue << std::endl;

    // Write ammo value
    int newAmmo = 1337;
    WriteProcessMemory(hProcess, (BYTE*)ammoAddr, &newAmmo, sizeof(newAmmo), nullptr);

    // Read out again
    ReadProcessMemory(hProcess, (BYTE*)ammoAddr, &ammoValue, sizeof(ammoValue), nullptr);
    std::cout << "New ammo = " << std::dec << ammoValue << std::endl;

    // Increase health
    std::vector<unsigned int> healthOffsets = { 0xf8 };
    uintptr_t healthAddr = FindDMAAddy(hProcess, dynamicPtrBaseAddr, healthOffsets);
    int healthValue = 0;

    ReadProcessMemory(hProcess, (BYTE*)healthAddr, &healthValue, sizeof(healthAddr), nullptr);
    int newHealth = 999;
    WriteProcessMemory(hProcess, (BYTE*)healthAddr, &newHealth, sizeof(newHealth), nullptr);


    getchar();

}

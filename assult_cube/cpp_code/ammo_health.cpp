#include<iostream>
#include<windows.h>
#include<string.h>
#include<tlhelp32.h>

using namespace std;

//global variable
HANDLE hProc = NULL;
DWORD pId;

bool attachprocess(char* processname){
    PROCESSENTRY32 procEntry32;

    procEntry32.dwSize = sizeof(PROCESSENTRY32);

    //getting snapshot of all process
    auto hProcSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);

    if(hProcSnap == INVALID_HANDLE_VALUE){
        cout << "[*]Process Snapshot Failed";
        return false;
    }

    //loop to the process list
    while(Process32Next(hProcSnap,&procEntry32)){
        cout << procEntry32.szExeFile << endl;

        //check fpor the target process
        if (!strcmp(processname,(char *)procEntry32.szExeFile)){
            //if found
            cout << "[*]Found Process " << procEntry32.szExeFile << " with Process ID: " << procEntry32.th32ProcessID << endl;
            pId = procEntry32.th32ProcessID;
            hProc = OpenProcess(PROCESS_ALL_ACCESS,false,pId);

            if(hProc == NULL){
                cout << "Opening Failed" << endl;
            }
            CloseHandle(hProcSnap);
            return true;
        }
    }

    return false;

}
//get module address
DWORD GetModuleBaseAddress(char* lpszModuleName, DWORD pID) {
    DWORD dwModuleBaseAddress = 0;
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pID); // make snapshot of all modules within process
    MODULEENTRY32 ModuleEntry32 = { 0 };
    ModuleEntry32.dwSize = sizeof(MODULEENTRY32);
 
    if (Module32First(hSnapshot, &ModuleEntry32)) //store first Module in ModuleEntry32
    {
        do {
            if (!strcmp((char *)ModuleEntry32.szModule, lpszModuleName)) // if Found Module matches Module we look for -> done!
            {
                dwModuleBaseAddress = (DWORD)ModuleEntry32.modBaseAddr;
                break;
            }
        } while (Module32Next(hSnapshot, &ModuleEntry32)); // go through Module entries in Snapshot and store in ModuleEntry32
 
 
    }
    CloseHandle(hSnapshot);
    return dwModuleBaseAddress;
}

DWORD GetPointerAddress(DWORD gameBaseAddr, DWORD address,int n, DWORD *offsets){
    //hProc = OpenProcess(PROCESS_ALL_ACCESS,false,pId);

    DWORD offset_null;
    ReadProcessMemory(hProc, (LPVOID*)(gameBaseAddr + address), &offset_null, sizeof(offset_null), 0);
    DWORD pointeraddress = offset_null; // the address we need
    for(int i=0;i<n-1;i++)
        ReadProcessMemory(hProc, (LPVOID*)(pointeraddress + offsets[i]), &pointeraddress, sizeof(pointeraddress), 0);
    
    //add the last offset and return
    return pointeraddress += offsets[n-1];


}

//template to write process memory
template <class dataType>
void wpm(dataType valtoWrite,DWORD addresToWrite){
    WriteProcessMemory(hProc,(PVOID)addresToWrite,&valtoWrite,sizeof(dataType),0);
}

int main(){
    //offsets
    DWORD ammo_offset[] = {0x8,0x65C,0x34,0x6C,0x1D0};
    DWORD health_offset[] = {0x8,0xA0,0x34,0x34,0x3E0};

    char processname[] = "ac_client.exe";
    if(attachprocess(processname)){
        char modulename1[]= "ac_client.exe";
        
        //get the game's base adress
        DWORD gamebaseaddress1 = GetModuleBaseAddress(modulename1,pId);

        //ammo and health adress
        DWORD base_address = 0x0010A280;

        //get ammo's pointer adddress
        DWORD health_addresstowrite = GetPointerAddress(gamebaseaddress1,base_address,5,health_offset);
        DWORD ammo_addresstowrite = GetPointerAddress(gamebaseaddress1,base_address,5,ammo_offset);
        while(true){
            wpm<int>(1000,ammo_addresstowrite);
            wpm<int>(800,health_addresstowrite);
        }
    }
    else{
        cout << "[-]Process not found in the prosess snapshots" << endl;
    }
 
    

    return 0;

}
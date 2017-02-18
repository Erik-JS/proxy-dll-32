// DINPUT8 - MAIN.CPP
#include "main.h"
#include "../sharedcode/Logging.h"
#include "../sharedcode/PluginLoader.h"

FARPROC p;
HMODULE originalDINPUT8 = NULL;

void loadOriginalDINPUT8()
{
    char dllpath[MAX_PATH];
    GetSystemDirectory(dllpath, MAX_PATH);
    strcat(dllpath, "\\dinput8.dll");
    originalDINPUT8 = LoadLibrary(dllpath);
    if(!originalDINPUT8)
        return;
    p = GetProcAddress(originalDINPUT8, "DirectInput8Create");
}

DWORD WINAPI Start(LPVOID lpParam)
{
    logStart("dinput8_proxy.log");
    loadOriginalDINPUT8();
    if(!originalDINPUT8)
    {
        logprintf(">>Error loading original DINPUT8.DLL!\n");
        logEnd();
        return 0;
    }
    loadPlugins(".");
    loadPlugins("ASI");
    logEnd();
    return 0;
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    switch (fdwReason)
    {
        case DLL_PROCESS_ATTACH:
            // attach to process
            // return FALSE to fail DLL load
            CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Start, 0, 0, 0);
            break;

        case DLL_PROCESS_DETACH:
            // detach from process
            if(originalDINPUT8)
                FreeLibrary(originalDINPUT8);
            break;

        case DLL_THREAD_ATTACH:
            // attach to thread
            break;

        case DLL_THREAD_DETACH:
            // detach from thread
            break;
    }
    return TRUE; // successful
}

extern "C" __declspec(dllexport) void DirectInput8Create()
{
    asm("jmp _p\n\t");
}

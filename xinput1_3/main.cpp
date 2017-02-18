// XINPUT1_3 - MAIN.CPP
#include "main.h"
#include "../sharedcode/Logging.h"
#include "../sharedcode/PluginLoader.h"

FARPROC p[7];
HMODULE originalXINPUT = NULL;

void loadOriginalINPUT()
{
    char dllpath[MAX_PATH];
    GetSystemDirectory(dllpath, MAX_PATH);
    strcat(dllpath, "\\xinput1_3.dll");
    originalXINPUT = LoadLibrary(dllpath);
    if(!originalXINPUT)
        return;
    p[0] = GetProcAddress(originalXINPUT, "XInputEnable");
    p[1] = GetProcAddress(originalXINPUT, "XInputGetBatteryInformation");
    p[2] = GetProcAddress(originalXINPUT, "XInputGetCapabilities");
    p[3] = GetProcAddress(originalXINPUT, "XInputGetDSoundAudioDeviceGuids");
    p[4] = GetProcAddress(originalXINPUT, "XInputGetKeystroke");
    p[5] = GetProcAddress(originalXINPUT, "XInputGetState");
    p[6] = GetProcAddress(originalXINPUT, "XInputSetState");
}

DWORD WINAPI Start(LPVOID lpParam)
{
    logStart("xinput1_3_proxy.log");
    loadOriginalINPUT();
    if(!originalXINPUT)
    {
        logprintf(">>Error loading original XINPUT1_3.DLL!\n");
        logEnd();
        return 0;
    }
    loadPlugins(".");
    loadPlugins("ASI");
    logEnd();
    return 0;
}

BOOL APIENTRY DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
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
            if(originalXINPUT)
                FreeLibrary(originalXINPUT);
            break;

        case DLL_THREAD_ATTACH:
            // attach to thread
            break;

        case DLL_THREAD_DETACH:
            // detach from thread
            break;
    }
    return TRUE; // successful (do Code::Blocks devs ever realized it spells as 'succesful' here?)
}

extern "C" __declspec(dllexport) void XInputEnable()
{
    asm("jmp *_p + 0 * 4\n\t");
}

extern "C" __declspec(dllexport) void XInputGetBatteryInformation()
{
    asm("jmp *_p + 1 * 4\n\t");
}

extern "C" __declspec(dllexport) void XInputGetCapabilities()
{
    asm("jmp *_p + 2 * 4\n\t");
}

extern "C" __declspec(dllexport) void XInputGetDSoundAudioDeviceGuids()
{
    asm("jmp *_p + 3 * 4\n\t");
}

extern "C" __declspec(dllexport) void XInputGetKeystroke()
{
    asm("jmp *_p + 4 * 4\n\t");
}

extern "C" __declspec(dllexport) void XInputGetState()
{
    asm("jmp *_p + 5 * 4\n\t");
}

extern "C" __declspec(dllexport) void XInputSetState()
{
    asm("jmp *_p + 6 * 4\n\t");
}

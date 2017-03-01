// D3D9 - MAIN.CPP
#include "main.h"
#include "../sharedcode/Logging.h"
#include "../sharedcode/PluginLoader.h"

FARPROC p[15];
HMODULE originalD3D9 = NULL;

void loadOriginalD3D9()
{
    char d3d9path[MAX_PATH];
    GetSystemDirectory(d3d9path, MAX_PATH);
    strcat(d3d9path, "\\d3d9.dll");
    originalD3D9 = LoadLibrary(d3d9path);
    if(!originalD3D9)
        return;
    p[0] = GetProcAddress(originalD3D9, "D3DPERF_BeginEvent");
    p[1] = GetProcAddress(originalD3D9, "D3DPERF_EndEvent");
    p[2] = GetProcAddress(originalD3D9, "D3DPERF_GetStatus");
    p[3] = GetProcAddress(originalD3D9, "D3DPERF_QueryRepeatFrame");
    p[4] = GetProcAddress(originalD3D9, "D3DPERF_SetMarker");
    p[5] = GetProcAddress(originalD3D9, "D3DPERF_SetOptions");
    p[6] = GetProcAddress(originalD3D9, "D3DPERF_SetRegion");
    p[7] = GetProcAddress(originalD3D9, "DebugSetLevel");
    p[8] = GetProcAddress(originalD3D9, "DebugSetMute");
    p[9] = GetProcAddress(originalD3D9, "Direct3D9EnableMaximizedWindowedModeShim");
    p[10] = GetProcAddress(originalD3D9, "Direct3DCreate9");
    p[11] = GetProcAddress(originalD3D9, "Direct3DCreate9Ex");
    p[12] = GetProcAddress(originalD3D9, "Direct3DShaderValidatorCreate9");
    p[13] = GetProcAddress(originalD3D9, "PSGPError");
    p[14] = GetProcAddress(originalD3D9, "PSGPSampleTexture");
}

DWORD WINAPI Start(LPVOID lpParam)
{
    logStart("d3d9_proxy32.log");
    loadOriginalD3D9();
    if(!originalD3D9)
    {
        logprintf(">>Error loading original D3D9.DLL!\n");
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
            if(originalD3D9)
                FreeLibrary(originalD3D9);
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

extern "C" __declspec(dllexport) void D3DPERF_BeginEvent()
{
    asm("jmp *_p + 0 * 4\n\t");
}

extern "C" __declspec(dllexport) void D3DPERF_EndEvent()
{
    asm("jmp *_p + 1 * 4\n\t");
}

extern "C" __declspec(dllexport) void D3DPERF_GetStatus()
{
    asm("jmp *_p + 2 * 4\n\t");
}

extern "C" __declspec(dllexport) void D3DPERF_QueryRepeatFrame()
{
    asm("jmp *_p + 3 * 4\n\t");
}

extern "C" __declspec(dllexport) void D3DPERF_SetMarker()
{
    asm("jmp *_p + 4 * 4\n\t");
}

extern "C" __declspec(dllexport) void D3DPERF_SetOptions()
{
    asm("jmp *_p + 5 * 4\n\t");
}

extern "C" __declspec(dllexport) void D3DPERF_SetRegion()
{
    asm("jmp *_p + 6 * 4\n\t");
}

extern "C" __declspec(dllexport) void DebugSetLevel()
{
    asm("jmp *_p + 7 * 4\n\t");
}

extern "C" __declspec(dllexport) void DebugSetMute()
{
    asm("jmp *_p + 8 * 4\n\t");
}

extern "C" __declspec(dllexport) void Direct3D9EnableMaximizedWindowedModeShim()
{
    asm("jmp *_p + 9 * 4\n\t");
}

extern "C" __declspec(dllexport) void Direct3DCreate9()
{
    asm("jmp *_p + 10 * 4\n\t");
}

extern "C" __declspec(dllexport) void Direct3DCreate9Ex()
{
    asm("jmp *_p + 11 * 4\n\t");
}

extern "C" __declspec(dllexport) void Direct3DShaderValidatorCreate9()
{
    asm("jmp *_p + 12 * 4\n\t");
}

extern "C" __declspec(dllexport) void PSGPError()
{
    asm("jmp *_p + 13 * 4\n\t");
}

extern "C" __declspec(dllexport) void PSGPSampleTexture()
{
    asm("jmp *_p + 14 * 4\n\t");
}

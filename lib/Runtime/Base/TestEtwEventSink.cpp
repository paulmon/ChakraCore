//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#include "RuntimeBasePch.h"

#ifdef TEST_ETW_EVENTS
#include "Base/EtwTrace.h"

char const * const TestEtwEventSink::CreateEventSinkProcName = STRINGIZE(CREATE_EVENTSINK_PROC_NAME);
TestEtwEventSink* TestEtwEventSink::Instance = NULL;

//#ifdef _CHAKRACOREUWP
//WINBASEAPI
//_Ret_maybenull_
//HMODULE
//WINAPI
//LoadLibraryW(
//    _In_ LPCWSTR lpLibFileName
//);
//#endif

bool TestEtwEventSink::Load()
{
#ifdef _CHAKRACOREUWP
    return false;
#else
    char16 const * dllname = Js::Configuration::Global.flags.TestEtwDll;
    if(!dllname)
    {
        return false;
    }
    HMODULE hModule = ::LoadLibraryW(dllname);
    if (hModule == nullptr)
    {
        Output::Print(_u("ERROR: Unable to load ETW event sink %s\n"), dllname);
        Js::Throw::FatalInternalError();
    }

    CreateEventSink procAddress = (CreateEventSink)::GetProcAddress(hModule, CreateEventSinkProcName);

    if (procAddress == nullptr)
    {
        Output::Print(_u("ERROR: Unable to get function %S from dll %s\n"), CreateEventSinkProcName, dllname);
        Js::Throw::FatalInternalError();
    }

    // CONSIDER: pass null and skip rundown testing (if a command line switch is present).
    Instance = procAddress(&EtwTrace::PerformRundown, PHASE_TRACE1(Js::EtwPhase));
    if (Instance == nullptr)
    {
        Output::Print(_u("ERROR: Failed to create ETW event sink from dll %s\n"), dllname);
        Js::Throw::FatalInternalError();
    }
    return true;
#endif // _CHAKRACOREUWP
}

bool TestEtwEventSink::IsLoaded()
{
    return Instance != NULL;
}

void TestEtwEventSink::Unload()
{
    if(Instance != NULL)
    {
        Instance->UnloadInstance();
        Instance = NULL;
    }
}
#endif

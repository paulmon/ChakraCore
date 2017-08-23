//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------

#include "RuntimePlatformAgnosticPch.h"
#include "Common.h"
#include "ChakraPlatform.h"

namespace PlatformAgnostic
{
    SystemInfo::PlatformData SystemInfo::data;

    SystemInfo::PlatformData::PlatformData()
    {
#ifdef _CHAKRACOREUWP
        totalRam = static_cast<size_t>(2 * 1024 * 1024); // UWP - hardcode to 2 MB for now
#else
        ULONGLONG ram;
        if (GetPhysicallyInstalledSystemMemory(&ram) == TRUE)
        {
            totalRam = static_cast<size_t>(ram) * 1024;
        }
#endif
    }

    bool SystemInfo::GetMaxVirtualMemory(size_t *totalAS)
    {
        SYSTEM_INFO info;
        GetSystemInfo(&info);
        *totalAS = (size_t) info.lpMaximumApplicationAddress;
        return true;
    }

}

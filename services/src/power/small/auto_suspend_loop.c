/*
 * Copyright (c) 2021 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <unistd.h>
#include "hilog_wrapper.h"
#include "power/suspend_ops.h"

static const int32_t SUSPEND_TIME = 10;

static BOOL AutoSuspendLoopFunc(AutoSuspendWait waitFunc)
{
    POWER_HILOGD("Prepare suspend"); // here to prepare to enter suspend

    // wait callback to check if suspend block counter is ready
    waitFunc();

    POWER_HILOGD("Enter suspend");
    sleep(SUSPEND_TIME); // using sleep to simulate suspend

    POWER_HILOGD("Exit suspend");
    return TRUE;
}

AutoSuspendLoop AutoSuspendLoopInit()
{
    return AutoSuspendLoopFunc;
}

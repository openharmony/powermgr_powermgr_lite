/*
 * Copyright (c) 2026 Huawei Device Co., Ltd.
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

#include "power_pm_kal.h"

#include <los_pm.h>

#include "hilog_wrapper.h"

int32_t KalPmLockRequest(const char *name)
{
    if (name == NULL) {
        POWER_HILOGE("Invalid lock name");
        return -1;
    }

    uint32_t ret = LOS_PmLockRequest(name);
    if (ret != LOS_OK) {
        POWER_HILOGE("Failed to request lock: %{public}s, ret: %{public}u", name, ret);
        return -1;
    }
    return KAL_PM_SUCCESS;
}

int32_t KalPmLockRelease(const char *name)
{
    if (name == NULL) {
        POWER_HILOGE("Invalid lock name");
        return -1;
    }

    uint32_t ret = LOS_PmLockRelease(name);
    if (ret != LOS_OK) {
        POWER_HILOGE("Failed to release lock: %{public}s, ret: %{public}u", name, ret);
        return -1;
    }
    return KAL_PM_SUCCESS;
}

uint32_t KalPmReadLock(void)
{
    return LOS_PmReadLock();
}

int32_t KalPmSuspend(uint32_t wakeCount)
{
    uint32_t ret = LOS_PmSuspend(wakeCount);
    if (ret != LOS_OK) {
        POWER_HILOGE("Failed to suspend, ret: %{public}u", ret);
        return -1;
    }
    return KAL_PM_SUCCESS;
}

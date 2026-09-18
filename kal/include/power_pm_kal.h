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

#ifndef POWERMGR_POWER_PM_KAL_H
#define POWERMGR_POWER_PM_KAL_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#define KAL_PM_SUCCESS  0

/**
 * Request a power lock to prevent the system from suspending.
 *
 * @param name The name of the lock.
 * @return KAL_PM_SUCCESS on success, non-zero on failure.
 */
int32_t KalPmLockRequest(const char *name);

/**
 * Release a power lock to allow the system to suspend.
 *
 * @param name The name of the lock.
 * @return KAL_PM_SUCCESS on success, non-zero on failure.
 */
int32_t KalPmLockRelease(const char *name);

/**
 * Read the current wake lock count.
 *
 * @return The wake lock count.
 */
uint32_t KalPmReadLock(void);

/**
 * Suspend the system.
 *
 * @param wakeCount The wake lock count before suspend.
 * @return KAL_PM_SUCCESS on success, non-zero on failure.
 */
int32_t KalPmSuspend(uint32_t wakeCount);

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // POWERMGR_POWER_PM_KAL_H

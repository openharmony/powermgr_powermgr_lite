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

#include "running_lock.h"

#include <stdint.h>
#include <stdlib.h>

#include <common.h>
#include <ohos_errno.h>
#include <pthread.h>
#include <securec.h>

#include "hilog_wrapper.h"
#include "running_lock_entry.h"
#include "running_lock_inner.h"

static pthread_mutex_t g_mutex = PTHREAD_MUTEX_INITIALIZER;
static BOOL g_inited = FALSE;
static Vector g_runningLocks;

static BOOL AddRunningLock(RunningLock *lock)
{
    pthread_mutex_lock(&g_mutex);
    if (g_inited == FALSE) {
        g_runningLocks = VECTOR_Make(NULL, NULL);
        g_inited = TRUE;
    }
    int16_t pos = VECTOR_Add(&g_runningLocks, (void *)lock);
    if (pos == INVALID_INDEX) {
        POWER_HILOGE("Failed to add lock to vector for lock: %s", lock->name);
        pthread_mutex_unlock(&g_mutex);
        return FALSE;
    }
    POWER_HILOGD("Add running lock, name: %s, type: %d", lock->name, lock->type);
    pthread_mutex_unlock(&g_mutex);
    return TRUE;
}

static BOOL RemoveRunningLock(const RunningLock *lock)
{
    pthread_mutex_lock(&g_mutex);
    int16_t pos = VECTOR_Find(&g_runningLocks, (void *)lock);
    if (pos < 0) {
        POWER_HILOGE("Non-existent running lock: %s", lock->name);
        pthread_mutex_unlock(&g_mutex);
        return FALSE;
    }
    VECTOR_Swap(&g_runningLocks, pos, NULL);
    POWER_HILOGD("Remove running lock, name: %s, type: %d", lock->name, lock->type);
    pthread_mutex_unlock(&g_mutex);
    return TRUE;
}

static BOOL IsRunningLockExisted(const RunningLock *lock)
{
    if (lock == NULL) {
        POWER_HILOGE("Invalid running lock");
        return FALSE;
    }
    pthread_mutex_lock(&g_mutex);
    BOOL ret = (VECTOR_Find(&g_runningLocks, (void *)lock) >= 0) ? TRUE : FALSE;
    pthread_mutex_unlock(&g_mutex);
    return ret;
}

static RunningLockEntry *CreateRunningLockEntry(const char *name, RunningLockType type, RunningLockFlag flag)
{
    RunningLockEntry *entry = (RunningLockEntry *)malloc(sizeof(RunningLockEntry));
    if (entry == NULL) {
        POWER_HILOGE("Failed allocate running lock entry");
        return NULL;
    }
    (void)memset_s(entry, sizeof(RunningLockEntry), 0, sizeof(RunningLockEntry));
    InitIdentity(entry);
    entry->lock.type = type;
    entry->lock.flag = flag;
    if(strcpy_s(entry->lock.name, sizeof(entry->lock.name), name) != EOK) {
        free(entry);
        return NULL;
    }
    return entry;
}

static void DestroyRunningLockEntry(RunningLockEntry *entry)
{
    if (entry != NULL) {
        POWER_HILOGD("Free entry: %p", entry);
        free(entry);
    }
}

const RunningLock *CreateRunningLock(const char *name, RunningLockType type, RunningLockFlag flag)
{
    if ((name == NULL) || (type >= RUNNINGLOCK_BUTT)) {
        POWER_HILOGE("Invalid running lock name");
        return NULL;
    }

    RunningLockEntry *entry = CreateRunningLockEntry(name, type, flag);
    if (entry == NULL) {
        POWER_HILOGE("Failed create running lock entry");
        return NULL;
    }
    AddRunningLock(&entry->lock);
    POWER_HILOGI("Create %s, pid: %u, token: %llu", name, entry->identity.pid, (long long)entry->identity.token);
    return &entry->lock;
}

void DestroyRunningLock(const RunningLock *lock)
{
    if (lock == NULL) {
        POWER_HILOGE("Invalid running lock");
        return;
    }
    if (RemoveRunningLock(lock) == TRUE) {
        DestroyRunningLockEntry(GetRunningLockEntry(lock));
    }
}

BOOL AcquireRunningLock(const RunningLock *lock)
{
    if (IsRunningLockExisted(lock) == FALSE) {
        POWER_HILOGE("Non-existent running lock: %s", lock->name);
        return FALSE;
    }
    return AcquireRunningLockEntry(GetRunningLockEntry(lock), -1);
}

BOOL ReleaseRunningLock(const RunningLock *lock)
{
    if (IsRunningLockExisted(lock) == FALSE) {
        POWER_HILOGE("Non-existent running lock: %s", lock->name);
        return FALSE;
    }
    return ReleaseRunningLockEntry(GetRunningLockEntry(lock));
}

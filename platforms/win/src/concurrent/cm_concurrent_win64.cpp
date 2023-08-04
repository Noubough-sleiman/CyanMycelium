#include "concurrent/cm_concurrent.hpp"

using namespace CyanMycelium;

Mutex ::Mutex(bool initially_owned)
{
    _handle = CreateMutex(
        NULL, // default security attributes
        initially_owned,
        NULL); // unamed
}

Mutex ::~Mutex()
{
    if (_handle)
    {
        CloseHandle(_handle);
    }
}

bool Mutex ::Take(unsigned int timeout_millis)
{
    DWORD dwWaitResult = WaitForSingleObject(_handle, timeout_millis);
    return (dwWaitResult == WAIT_OBJECT_0);
}

void Mutex ::Give()
{
    if (_handle)
    {
        ReleaseMutex(_handle);
    }
}

Semaphore ::Semaphore(int initialCount, int maximumCount)
{
    _handle = CreateSemaphoreA(
        NULL, // default security attributes
        initialCount,
        maximumCount,
        NULL); // unamed
}

Semaphore ::~Semaphore()
{
    if (_handle)
    {
        CloseHandle(_handle);
    }
}

bool Semaphore ::Take(unsigned int timeout_millis)
{
    DWORD dwWaitResult = WaitForSingleObject(_handle, timeout_millis);
    return (dwWaitResult == WAIT_OBJECT_0);
}

void Semaphore ::Give(unsigned int count)
{
    if (_handle)
    {
        ReleaseSemaphore(_handle, count,
                         NULL); // not interested in previous count
    }
}
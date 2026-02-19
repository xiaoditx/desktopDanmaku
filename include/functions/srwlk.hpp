#ifndef SRWLK_HPP
#define SRWLK_HPP

#include <windows.h>

class SrwLock
{
private:
    SRWLOCK lock_{};

public:
    SrwLock() noexcept
    {
        InitializeSRWLock(&lock_);
    }

    SrwLock(const SrwLock &) = delete;
    SrwLock &operator=(const SrwLock &) = delete;
    SrwLock(SrwLock &&) = delete;
    SrwLock &operator=(SrwLock &&) = delete;

    void enterShared() noexcept { AcquireSRWLockShared(&lock_); }
    void leaveShared() noexcept { ReleaseSRWLockShared(&lock_); }
    void enterExclusive() noexcept { AcquireSRWLockExclusive(&lock_); }
    void leaveExclusive() noexcept { ReleaseSRWLockExclusive(&lock_); }
    void tryEnterShared() noexcept { TryAcquireSRWLockShared(&lock_); }
    void tryEnterExclusive() noexcept { TryAcquireSRWLockExclusive(&lock_); }
};

class SrwSharedGuard
{
private:
    SrwLock &lock_;

public:
    explicit SrwSharedGuard(SrwLock &lock) noexcept : lock_(lock)
    {
        lock_.enterShared();
    }
    ~SrwSharedGuard() noexcept
    {
        lock_.leaveShared();
    }
};

class SrwExclusiveGuard
{
private:
    SrwLock &lock_;

public:
    explicit SrwExclusiveGuard(SrwLock &lock) noexcept : lock_(lock)
    {
        lock_.enterExclusive();
    }
    ~SrwExclusiveGuard() noexcept
    {
        lock_.leaveExclusive();
    }
};

#endif // SRWLK_HPP
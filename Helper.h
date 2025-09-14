#pragma once
#include <iostream>
#include <Windows.h>
#include <sstream>
inline void ThrowIfFailed(HRESULT hr)
{
    if (FAILED(hr))
    {
        throw new std::exception();
    }
}
#pragma once
#include <iostream>
#include <Windows.h>
#include <string>
#include <mutex>

class main
{
public:
static std::atomic<bool> m_bAtomActive;
static std::condition_variable m_cvGameFinished;
static std::mutex m_muxGame;

};

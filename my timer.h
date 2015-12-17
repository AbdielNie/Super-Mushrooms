

#pragma once
#include <windows.h>

class MyTimer
{
private:
	LARGE_INTEGER m_clk;
	
	
	LONGLONG m_oldclk;
	
	

	int m_freq;
	
	
	
	
public:
	MyTimer();
	void Sleep(int ms);
	void Reset();
};


MyTimer::MyTimer()
{
	QueryPerformanceFrequency(&m_clk);
	m_freq = (int)m_clk.QuadPart / 1000;
	

	m_oldclk = 0;
}


void MyTimer::Sleep(int ms)
{
	unsigned int c = ms * m_freq;

	if (m_oldclk == 0)
	{
	
	
	
	QueryPerformanceCounter(&m_clk);
	
	m_oldclk = m_clk.QuadPart;
	

	}

	m_oldclk += c;

	QueryPerformanceCounter(&m_clk);

	if (m_clk.QuadPart > m_oldclk)
	
	m_oldclk = m_clk.QuadPart;
	else
	
	do
	
	{
	
	
	::Sleep(1);
	
	
	QueryPerformanceCounter(&m_clk);

	}
	
	while(m_clk.QuadPart < m_oldclk);
}


void MyTimer::Reset()
{
	m_oldclk = 0;
}

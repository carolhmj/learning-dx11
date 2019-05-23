#include "CpuClass.h"



CpuClass::CpuClass()
{
}

CpuClass::CpuClass(const CpuClass &)
{
}


CpuClass::~CpuClass()
{
}

void CpuClass::Initialize()
{
	PDH_STATUS status;

	m_canReadCpu = true;

	status = PdhOpenQuery(nullptr, 0, &m_queryHandle);
	if (status != ERROR_SUCCESS) {
		m_canReadCpu = false;
	}

	m_lastSampleTime = GetTickCount();

	m_cpuUsage = 0;
}

void CpuClass::Shutdown()
{
	if (m_canReadCpu)
	{
		PdhCloseQuery(m_queryHandle);
	}
}

void CpuClass::Frame()
{
	PDH_FMT_COUNTERVALUE value;
	PDH_FUNCTION result;

	if (m_canReadCpu) {
		if ((m_lastSampleTime + 1000) < GetTickCount()) {
			m_lastSampleTime = GetTickCount();

			PdhCollectQueryData(m_queryHandle);

			result = PdhGetFormattedCounterValue(m_counterHandle, PDH_FMT_LONG, nullptr, &value);
			if (result == ERROR_SUCCESS) {
				m_cpuUsage = value.longValue;
			}

		}
	}
}

int CpuClass::GetCpuPercentage()
{
	int usage;

	if (m_canReadCpu)
	{
		usage = (int)m_cpuUsage;
	}
	else
	{
		usage = 0;
	}

	return usage;

}

#include "sharememory.h"

ShareMemory::ShareMemory(const string& sMapName, unsigned int nSize) : m_sMapName(sMapName), m_pBuffer(nullptr)
{
#if defined (_WIN32) || defined(_WIN64)
	LPVOID pBuffer;
	m_hFileMap = ::OpenFileMappingA(FILE_MAP_ALL_ACCESS, 0, (LPCSTR)sMapName.c_str());

	if (NULL == m_hFileMap)
	{
		m_hFileMap = ::CreateFileMappingA(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, nSize, (LPCSTR)sMapName.c_str());
		m_pBuffer = (char*)::MapViewOfFile(m_hFileMap, FILE_MAP_ALL_ACCESS, 0, 0, 0);
		printf("Share memory [%s] created.\n", sMapName.c_str());
	}
	else
	{
		m_pBuffer = (char*)::MapViewOfFile(m_hFileMap, FILE_MAP_ALL_ACCESS, 0, 0, 0);
		printf("Share memory [%s] exists.\n", sMapName.c_str());
	}
#elif defined(_unix_) || defined(_linux_)

#endif	                                  

}


ShareMemory::~ShareMemory()
{
#if defined (_WIN32) || defined(_WIN64)
	::UnmapViewOfFile(m_pBuffer);
	::CloseHandle(m_hFileMap);
#elif defined(_unix_) || defined(_linux_)

#endif	                                  

}

char* ShareMemory::getBuffer()
{
	return m_pBuffer;
}

ShareMemory* ShareMemoryPool::RegistShareMemory(const string& sMapName, unsigned int nSize)
{
	ShareMemory* pShareMemory = nullptr;
	if (m_mpShareMem.find(sMapName) != m_mpShareMem.end())
	{
		printf("Share memory [%s] registered.\n", sMapName.c_str());
		pShareMemory = m_mpShareMem[sMapName];
		return pShareMemory;
	}
	else
	{
		pShareMemory = new ShareMemory(sMapName, nSize);
		m_mpShareMem[sMapName] = pShareMemory;
		return pShareMemory;
	}
}

char* ShareMemoryPool::getBuffer(string sMapName)
{
	if (m_mpShareMem.find(sMapName) != m_mpShareMem.end())
	{
		return m_mpShareMem[sMapName]->getBuffer();
	}
	else
	{
		return nullptr;
	}
}

#include "cplug_mysql.h"

MYSQL* MySqlConnectionPool::getMysqlConnector(int& nIndex)
{
	if (m_bClearFlag)
		return nullptr;
	try
	{
		std::lock_guard<std::mutex> lock{ m_Lock };
		nIndex = -1;
		for (auto it = m_mpUnUseConnect.begin(); it != m_mpUnUseConnect.end(); it++)
		{
			if (nullptr != it->second)
			{
				nIndex = it->first;
				MYSQL* pMySql = it->second;
				m_mpInUseConnect[nIndex] = pMySql;
				m_mpUnUseConnect[nIndex] = nullptr;
				//logInfo("当前使用数据库连接:[%d]", nIndex);
				return pMySql;
			}
		}
		logError("当前数据库无空闲连接");
		return nullptr;
	}
	catch (exception& e)
	{
		logError("%s throw an exception error: %s", __FUNCTION__, e.what());
	}
	catch (...)
	{
		logError("%s throw an unknow exception", __FUNCTION__);
	}
	return nullptr;
}

void MySqlConnectionPool::init(const string& sIP, const string& sUsr, const string& sPwd, const string& sDBName, int nConnectCnt)
{
	try
	{
		m_sIP = sIP;
		m_sUsr = sUsr;
		m_sPwd = sPwd;
		m_sDBName = sDBName;
		m_nConnectCnt = nConnectCnt;
		m_bConnected = false;
		connectMysql();

		m_reconnect = new ReconnectMysql();
		m_reconnect->start();
	}
	catch (exception& e)
	{
		logError("%s throw an exception error: %s", __FUNCTION__, e.what());
	}
	catch (...)
	{
		logError("%s throw an unknow exception", __FUNCTION__);
	}
}

void MySqlConnectionPool::freeMysqlConnector(MYSQL* pMySql, int nIndex)
{
	std::lock_guard<std::mutex> lock{ m_Lock };
	m_mpInUseConnect[nIndex] = nullptr;
	m_mpUnUseConnect[nIndex] = pMySql;

	//logInfo("当前释放数据库连接:[%d]", nIndex);
	//for (auto it = m_mpUnUseConnect.begin(); it != m_mpUnUseConnect.end(); it++)
	//{
	//	if (nullptr != it->second)
	//	{
	//		nIndex = it->first;
	//		logInfo("未用:[%d]", nIndex);
	//	}
	//}
	//for (auto it = m_mpInUseConnect.begin(); it != m_mpInUseConnect.end(); it++)
	//{
	//	if (nullptr != it->second)
	//	{
	//		nIndex = it->first;
	//		logInfo("在用:[%d]", nIndex);
	//	}
	//}
}

void MySqlConnectionPool::connectMysql()
{
	if (false == m_bConnected)
	{
		try
		{
			std::lock_guard<std::mutex> lock{ m_Lock };
			for (int i = 0; i < m_nConnectCnt; i++)
			{
				MYSQL* pMySql = new MYSQL;
				if (!pMySql)
					break;
				mysql_init(pMySql);
				//int nTimeOut = 10;
				//mysql_options(pMySql, MYSQL_OPT_CONNECT_TIMEOUT, &nTimeOut);
				if (!(mysql_real_connect(pMySql, m_sIP.c_str(), m_sUsr.c_str(), m_sPwd.c_str(), m_sDBName.c_str(), 0, NULL, 0)))
				{
					logError("Error connecting to database:%s", mysql_error(pMySql));
					pMySql = nullptr;
					m_bConnected = false;
					m_mpUnUseConnect[i] = nullptr;
				}
				else
				{
					m_bConnected = true;
					logDebug("Connected DB[%d] Success", i);
					m_mpUnUseConnect[i] = pMySql;
				}
				m_mpInUseConnect[i] = nullptr;
			}
		}
		catch (exception& e)
		{
			logError("%s throw an exception error: %s", __FUNCTION__, e.what());
		}
		catch (...)
		{
			logError("%s throw an unknow exception", __FUNCTION__);
		}
	}
}

void MySqlConnectionPool::setConnected(bool bConnected)
{
	m_bConnected = bConnected;
}

void MySqlConnectionPool::clearConnector()
{
	m_bClearFlag = true;
	bool bCleared;
	do
	{
		bCleared = true;
		for (auto it = m_mpInUseConnect.begin(); it != m_mpInUseConnect.end(); it++)
		{
			if (nullptr != it->second)
			{
				bCleared = false;
				break;
			}
		}
		std::this_thread::sleep_for(chrono::seconds(1));

	} while (!bCleared);

	std::lock_guard<std::mutex> lock{ m_Lock };
	for (auto it = m_mpUnUseConnect.begin(); it != m_mpUnUseConnect.end(); it++)
	{
		if (nullptr != it->second)
		{
			MYSQL* pMySql = it->second;
			mysql_close(pMySql);
			delete pMySql;
			m_mpUnUseConnect[it->first] = nullptr;
		}
	}
	m_bClearFlag = false;
}

bool MySqlConnectionPool::isConnected()
{
	try
	{
		DB local;
		MYSQL* pMySql;
		if (!local.Connector)
		{
			pMySql = new MYSQL;
			if (!pMySql)
				return false;
			mysql_init(pMySql);
			if (!(mysql_real_connect(pMySql, m_sIP.c_str(), m_sUsr.c_str(), m_sPwd.c_str(), m_sDBName.c_str(), 0, NULL, 0)))
			{
				logError("Error connecting to database:%s", mysql_error(pMySql));
				mysql_close(pMySql);
				pMySql = nullptr;
				return false;
			}
			else
			{
				goto TESTCONNECT;
			}
		}
		else
		{
			pMySql = local.Connector;
		}
		TESTCONNECT:
			mysql_query(pMySql, "set names gbk");
			int status;
			if (status = mysql_query(pMySql, "select 1;"))
			{
				logError("Connect failed", mysql_error(pMySql));
				return false;
			}
			else
			{
				logDebug("Connect successfully");
			}

			MYSQL_RES* res;
			if (!(res = mysql_store_result(pMySql)))
			{
				logError("Couldn't get result from %s", mysql_error(local.Connector));
				return false;
			}
			MYSQL_ROW column;
			column = mysql_fetch_row(res);
			mysql_free_result(res);
			return true;

	}
	catch (exception& e)
	{
		logError("%s throw an exception error: %s", __FUNCTION__, e.what());
	}
	catch (...)
	{
		logError("%s throw an unknow exception", __FUNCTION__);
	}
	return false;
}

MySqlConnectionPool::MySqlConnectionPool()
{
}

MySqlConnectionPool::~MySqlConnectionPool()
{
	m_reconnect->terminate();
	delete m_reconnect;
	m_reconnect = nullptr;
	clearConnector();
}

DB::DB()
{
	Connector = GET_MYSQLPOOL.getMysqlConnector(Index);
}

DB::~DB()
{
	if (-1 != Index &&  nullptr != Connector)
		GET_MYSQLPOOL.freeMysqlConnector(Connector, Index);
}

void ReconnectMysql::run()
{
	while (!isTerminate())
	{
		if (Cplug_Singleton<MySqlConnectionPool>::getInstance().isConnected())
		{
			//连接成功
			Cplug_Singleton<MySqlConnectionPool>::getInstance().connectMysql();
		}
		else
		{
			//连接失败
			Cplug_Singleton<MySqlConnectionPool>::getInstance().setConnected(false);
			Cplug_Singleton<MySqlConnectionPool>::getInstance().clearConnector();
		}
		std::this_thread::sleep_for(chrono::seconds(10));
	}   
}


void Cplug_MySql_Init(const string& sIP, const string& sUsr, const string& sPwd, const string& sDBName, int nConnectCnt)
{
	Cplug_Singleton<MySqlConnectionPool>::getInstance().init(sIP, sUsr, sPwd, sDBName, nConnectCnt);
}

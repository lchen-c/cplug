#pragma once
#include <mysql.h>
#include "../cplug_singleton.h"
#include <string>
#include "../log/cplug_log.h"
#include <map>
#include <mutex>
#include "../thread/cplug_threadbase.h"

using namespace std;

class ReconnectMysql : public ThreadBase
{
public:
	virtual void run();
};

class MySqlConnectionPool
{
	friend class Cplug_Singleton<MySqlConnectionPool>;
public:
	MYSQL* getMysqlConnector(int& nIndex);
	void init(const string& sIP, const string& sUsr, const string& sPwd, const string& sDBName, int nConnectCnt);
	void freeMysqlConnector(MYSQL* pMySql, int nIndex);
	void connectMysql();
	void setConnected(bool bConnected);
	void clearConnector();
	bool isConnected();
private:
	MySqlConnectionPool();
	~MySqlConnectionPool();

	//在用连接池
	map<int, MYSQL*> m_mpInUseConnect;

	//未用连接池
	map<int, MYSQL*> m_mpUnUseConnect;

	atomic<bool> m_bConnected;
	atomic<bool> m_bClearFlag;

	mutex m_Lock;
	string m_sIP;
	string m_sUsr;
	string m_sPwd;
	string m_sDBName;
	int m_nConnectCnt;

	ReconnectMysql* m_reconnect;
};

class DB
{
public:
	DB();
	~DB();
	MYSQL* Connector;
	int Index;
};

void Cplug_MySql_Init(const string& sIP, const string& sUsr, const string& sPwd, const string& sDBName, int nConnectCnt);

#define GET_MYSQLPOOL Cplug_Singleton<MySqlConnectionPool>::getInstance()


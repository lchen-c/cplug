#include "catch.hpp"
#include "sql/cplug_mysql.h"
#include <cplug_singleton.h>


TEST_CASE("1:Test mysql", "[mysql]")
{
	printf("\n##################Test Mysql##################\n");

#if defined (_WIN32) || defined(_WIN64)
	Cplug_Singleton<MySqlConnectionPool>::getInstance().init("127.0.0.1", "root", "111111", "algorithmdb",5);
	printf("connect info [127.0.0.1], [root], [111111], [algorithmdb] \n");
#else
	Cplug_Singleton<MySqlConnectionPool>::getInstance().init("127.0.0.1", "root", "Root_12root", "algorithmdb", 5);
	printf("connect info [127.0.0.1], [root], [Root_12root], [algorithmdb]\n");
#endif
	try
	{
		DB local;
		if (!local.Connector)
		{
			printf("connect failed (%s)\n", mysql_error(local.Connector));
			return;
		}
		mysql_query(local.Connector, "set names gbk");
		if (mysql_query(local.Connector, "select 1 as id"))
		{
			printf("select 1 failed (%s)\n", mysql_error(local.Connector));
			return;
		}
		else
		{
			printf("select 1 success\n");
		}
		MYSQL_RES* res;
		if (!(res = mysql_store_result(local.Connector)))
		{
			printf("couldn't get result from %s\n", mysql_error(local.Connector));
			return;
		}

		int field_count = res->field_count;
		map<string, int> mpField;
		for (int i = 0; i < field_count; i++)
		{
			mpField.insert(std::make_pair(mysql_fetch_field(res)->name, i));
		}
		MYSQL_ROW column;
		while (column = mysql_fetch_row(res))
		{
			if (mpField.find("id") != mpField.end())
			{
				printf("found [id] column\n");
			}
			else
			{
				printf("Field [id] does not exist\n ");
				mysql_free_result(res);
				return;
			}
		}
		mysql_free_result(res);
	}
	catch (exception& e)
	{
		printf("%s throw an exception error: %s", __FUNCTION__, e.what());
	}
	catch (...)
	{
		printf("%s throw an unknow exception", __FUNCTION__);
	}
}

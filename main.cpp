#include <iostream>
#include "connection.h"
#include "CommonConnectionPool.h"
using namespace std;
int main()
{
	/*Connection conn;
	char sql[1024] = { 0 };
	sprintf(sql, "insert into user(name,age,sex) values('%s',%d,'%s')",
		"zhang san", 20, "male");
	conn.connect("127.0.0.1", 3306, "root", "sjl0216..", "chat");
	conn.update(sql);
	*/
	clock_t begin = clock();


	thread t1([]() {
		//ConnectionPool* cp = ConnectionPool::getConnectionPool();
		Connection conn;
		conn.connect("127.0.0.1", 3306, "root", "sjl0216..", "chat");
		for (int i = 0; i < 250; ++i)
		{
			/*	char sql[1024] = { 0 };
			sprintf(sql, "insert into user(name,age,sex) values('%s',%d,'%s')",
				"zhang san", 20, "male");
			shared_ptr<Connection> conn = cp->getConnection();
			conn->update(sql);*/
			char sql[256];
			sprintf(sql, "insert into user(name,age,sex) values('%s',%d,'%s')",
				"zhang san", 20, "male");
			conn.update(sql);
		}
		});
	thread t2([]() {
		//ConnectionPool* cp = ConnectionPool::getConnectionPool();
		Connection conn;
		conn.connect("127.0.0.1", 3306, "root", "sjl0216..", "chat");
		for (int i = 0; i < 250; ++i)
		{
			/*	char sql[1024] = { 0 };
			sprintf(sql, "insert into user(name,age,sex) values('%s',%d,'%s')",
				"zhang san", 20, "male");
			shared_ptr<Connection> conn = cp->getConnection();
			conn->update(sql);*/
			char sql[256];
			sprintf(sql, "insert into user(name,age,sex) values('%s',%d,'%s')",
				"zhang san", 20, "male");
			conn.update(sql);
		}
		});
	thread t3([]() {
		//ConnectionPool* cp = ConnectionPool::getConnectionPool();
		Connection conn;
		conn.connect("127.0.0.1", 3306, "root", "sjl0216..", "chat");
		for (int i = 0; i < 250; ++i)
		{
		/*	char sql[1024] = { 0 };
			sprintf(sql, "insert into user(name,age,sex) values('%s',%d,'%s')",
				"zhang san", 20, "male");
			shared_ptr<Connection> conn = cp->getConnection();
			conn->update(sql);*/
			char sql[256];
			sprintf(sql, "insert into user(name,age,sex) values('%s',%d,'%s')",
				"zhang san", 20, "male");
			conn.update(sql);
		}
		});
	thread t4([]() {
		//ConnectionPool* cp = ConnectionPool::getConnectionPool();
		Connection conn;
		conn.connect("127.0.0.1", 3306, "root", "sjl0216..", "chat");
		for (int i = 0; i < 250; ++i)
		{
			/*char sql[1024] = { 0 };
			sprintf(sql, "insert into user(name,age,sex) values('%s',%d,'%s')",
				"zhang san", 20, "male");
			shared_ptr<Connection> conn = cp->getConnection();
			conn->update(sql);*/
			char sql[256];
			sprintf(sql, "insert into user(name,age,sex) values('%s',%d,'%s')",
				"zhang san", 20, "male");
			conn.update(sql);
		}
		});
	t1.join();
	t2.join();
	t3.join();
	t4.join();
	/* 使用事务
	shared_ptr<Connection> conn = cp->getConnection();
	conn->update("BEGIN");
	for (int i = 0; i < 5000; ++i)
	{
		char sql[1024] = { 0 };
		sprintf(sql, "insert into user(name,age,sex) values('%s',%d,'%s')",
			"zhang san", 20, "male");
		conn->update(sql);
	}
	conn->update("COMMIT");*/

	clock_t end = clock();
	cout << (end - begin) << " ms" << endl;
	
#if 0
	Connection conn;
	conn.connect("127.0.0.1", 3306, "root", "sjl0216..", "chat");
	for (int i = 0; i < 5000; ++i)
	{

		 char sql[1024] = { 0 };
		 sprintf(sql, "insert into user(name,age,sex) values('%s',%d,'%s')",
		 "zhang san", 20, "male");

		conn.update(sql);
	/*	shared_ptr<Connection> conn = cp->getConnection();
		char sql[1024] = { 0 };
		sprintf(sql, "insert into user(name,age,sex) values('%s',%d,'%s')",
			"zhang san", 20, "male");
		conn->update(sql);*/
	}
#endif;					
	
	return 0;

}
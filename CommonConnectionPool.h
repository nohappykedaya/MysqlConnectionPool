#pragma once
/*实现连接池功能模块*/
#include<string>
#include<queue>
#include <mutex>
#include "connection.h"
#include <atomic>
#include<thread>
#include<memory>
#include<functional>
#include <condition_variable>
using namespace std;
class ConnectionPool
{
public:
	//获取连接池对象实例
	static ConnectionPool* getConnectionPool();
	shared_ptr<Connection>getConnection();//给外部提供接口从连接池中获取一个可用的空间连接

private:
	void produceConnectionTask();//运行在独立的线程中 专门负责生产新连接
	ConnectionPool();//单例1 构造函数私有化
	void scannerConnectionTask();
	bool loadConfigFile();//从配置文件中加载配置项

	string _ip; //mysql ip地址
	unsigned short _port; //端口号
	string _username; //登录用户名
	string _password; // 登录密码
	int _initSize; //连接池的初始连接量
	int _maxSize; //连接池最大连接量
	int _maxIdletime; //连接池最大空闲时间
	int _connectionTimeout; //连接池获取连接的超时时间
	string _dbname;//连接的数据库名称
	queue<Connection*>_connectionQue; //存储mysql连接的队列
	mutex _queueMutex;//维护连接队列的线程安全互斥锁
	atomic_int _connectionCnt;//记录连接所创建的connection连接总数量
	condition_variable cv;//设置条件变量用于连接生产线程和连接消费线程的通信
};
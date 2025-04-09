// connectionPool.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
/**/
#include <iostream>
#include<string>
#include<queue>
#include "CommonConnectionPool.h" 
#include "public.h"


//线程安全的懒汉单例函数接口
ConnectionPool* ConnectionPool::getConnectionPool()
{
	static ConnectionPool pool;//lock 和unlock
	return &pool;
}

//从配置文件中加载配置项
bool ConnectionPool::loadConfigFile()
{
	FILE* pf = fopen("mysql.ini", "r");
	if (pf == nullptr)
	{
		LOG("mysql.ini file is not exist!");
		return false;
	}
	while (!feof(pf)) //文件到末尾
	{
		char line[1024] = { 0 };
		fgets(line, 1024, pf);
		string str = line;
		int idx = str.find('=', 0);
		if (idx == -1)//无效的配置项
		{
			continue;
		}
		//password=123456\n
		int endidx = str.find('\n', idx);
		string key = str.substr(0, idx);
		string value = str.substr(idx + 1, endidx - 1 - idx);
		if (key == "ip")
		{
			_ip = value;
		}
		else if (key == "port")
		{
			_port = atoi(value.c_str());
		}
		else if (key == "username")
		{
			_username = value;
		}
		else if (key == "password")
		{
			_password = value;
		}
		else if (key == "initSize")
		{
			_initSize= atoi(value.c_str());
		}
		else if (key == "maxSize")
		{
			_maxSize= atoi(value.c_str());
		}
		else if (key == "maxIdleTime")
		{
			_maxIdletime = atoi(value.c_str());
		}
		else if (key == "maxConnectionTimeout")
		{
			_connectionTimeout= atoi(value.c_str());
		}
		else if (key == "dbname")
		{
			_dbname = value;
		}
	}
	return true;
}

ConnectionPool::ConnectionPool()
{
	//加载配置项了
	if (!loadConfigFile())
	{
		return;
	}
	/*
		创建初始数量的连接
	*/
	for (int i = 0; i < _initSize; ++i)
	{
		Connection* p = new Connection();
		p->connect(_ip, _port, _username, _password, _dbname);
		p->refreshAliveTime(); //刷新一下开始空闲的起始时间
		_connectionQue.push(p);
		_connectionCnt++;
	}
	//启动一个新的线程作为连接的生产者
	thread produce(std::bind(&ConnectionPool::produceConnectionTask,this));//线程函数
	produce.detach();
	//启动一个新的定时线程 扫描多余的空闲连接 超过maxIdletime时间的空闲链接 进行多余的连接回收
	thread scanner(std::bind(&ConnectionPool::scannerConnectionTask, this));
	scanner.detach();
}
//运行在独立的线程中 专门负责生产新连接
void ConnectionPool:: produceConnectionTask()
{
	for (;;)
	{
		unique_lock<mutex>lock(_queueMutex);
		while (!_connectionQue.empty())
		{
			cv.wait(lock); //队列不空 生产线程进入等待状态
		}
		//连接数量没有到达上限继续创建新的连接
		if (_connectionCnt < _maxSize)
		{
			Connection* p = new Connection();
			p->connect(_ip, _port, _username, _password, _dbname);
			_connectionQue.push(p);
			_connectionCnt++;
		}
		//通知消费者线程可以消费连接了
		cv.notify_all();
	}
}

shared_ptr<Connection>ConnectionPool::getConnection()
{
	unique_lock<mutex>lock(_queueMutex);
	while (_connectionQue.empty())
	{
		//sleep
		if (cv_status::timeout == cv.wait_for(lock, chrono::milliseconds(_connectionTimeout)))
		{
			if (_connectionQue.empty())
			{
				LOG("获取空闲链接超时了。。。。获取连接失败！")
					return nullptr;
			}
		}
		
	}
	/*
	shared_ptr 智能指针析构时 会把connection资源直接delete掉相当于调用connection的
	析构函数 connection就背close掉了
	这里需要自定义shared_ptr的释放资源方式 把connection直接归还到queue中
	*/
	shared_ptr<Connection>sp(_connectionQue.front(),
		[&](Connection *pcon) {
			//这里是在服务器应用线程中调用的 一定要考虑队列的线程安全操作
			unique_lock<mutex>lock(_queueMutex);
			pcon->refreshAliveTime();
			_connectionQue.push(pcon);
		});
	_connectionQue.pop();
	cv.notify_all();//谁消费了队列的最后一个连接 谁负责通知一下生产者
	return sp;

}
//超过maxIdletime时间的空闲回收
void ConnectionPool::scannerConnectionTask()
{
	for (;;)
	{
		//通过sleep模拟定时效果
		this_thread::sleep_for(chrono::seconds(_maxIdletime));
		
		//扫描整个队列 释放多余的连接
		unique_lock<mutex>lock(_queueMutex);
		while (_connectionCnt > _initSize)
		{
			Connection* p = _connectionQue.front();
			if (p->getAliveeTime() >= (_maxIdletime * 1000))
			{
				_connectionQue.pop();
				delete p;//释放连接 调用析构函数
			}
			else
			{
				break;//队头的连接没有超过_maxIdletime其他连接肯定没有
			}
		}
	}
}
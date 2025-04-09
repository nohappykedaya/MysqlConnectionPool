#pragma once
/*ʵ�����ӳع���ģ��*/
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
	//��ȡ���ӳض���ʵ��
	static ConnectionPool* getConnectionPool();
	shared_ptr<Connection>getConnection();//���ⲿ�ṩ�ӿڴ����ӳ��л�ȡһ�����õĿռ�����

private:
	void produceConnectionTask();//�����ڶ������߳��� ר�Ÿ�������������
	ConnectionPool();//����1 ���캯��˽�л�
	void scannerConnectionTask();
	bool loadConfigFile();//�������ļ��м���������

	string _ip; //mysql ip��ַ
	unsigned short _port; //�˿ں�
	string _username; //��¼�û���
	string _password; // ��¼����
	int _initSize; //���ӳصĳ�ʼ������
	int _maxSize; //���ӳ����������
	int _maxIdletime; //���ӳ�������ʱ��
	int _connectionTimeout; //���ӳػ�ȡ���ӵĳ�ʱʱ��
	string _dbname;//���ӵ����ݿ�����
	queue<Connection*>_connectionQue; //�洢mysql���ӵĶ���
	mutex _queueMutex;//ά�����Ӷ��е��̰߳�ȫ������
	atomic_int _connectionCnt;//��¼������������connection����������
	condition_variable cv;//�������������������������̺߳����������̵߳�ͨ��
};
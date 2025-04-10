

# MySQLConnectionPool - C++ MySQL数据库连接池

该项目基于 C++11 实现了一个高性能的 MySQL 数据库连接池模块，支持多线程并发访问，解决传统数据库访问中频繁创建和释放连接带来的性能瓶颈，适用于中小型服务端项目中数据库访问优化场景。

---

## 🛠 关键技术

- MySQL C/C++ 客户端编程
- 单例模式
- 线程安全队列（std::queue + mutex）
- C++11 多线程编程：`std::thread`, `std::mutex`, `std::condition_variable`
- 原子操作：`std::atomic<int>`
- 智能指针管理：`shared_ptr` + `lambda`
- 生产者-消费者模型

---

## 📌 项目背景

在高并发场景下，频繁进行 MySQL TCP 三次握手、连接认证、资源释放等操作会带来明显性能开销。  
本项目通过连接池技术预创建数据库连接，实现连接复用，从而减少数据库操作延迟，提高系统吞吐能力。

---

## 🚀 功能特性

- **初始化连接池**：程序启动时创建固定数量的数据库连接
- **最大连接数限制**：防止连接无限创建导致资源耗尽
- **连接空闲回收**：长时间未使用的连接将被自动释放
- **连接超时等待**：并发过高时，连接池支持阻塞等待，超过时间未获取则失败
- **线程安全设计**：多线程环境下安全获取和释放连接
- **连接智能管理**：使用 `shared_ptr` + 自定义回收机制，自动归还连接

---

## 🧱 项目结构

```
├── ConnectionPool.h / .cpp     # 连接池核心逻辑
├── Connection.h / .cpp         # MySQL连接封装
├── public.h                    # 日志宏等公共定义
├── mysql.ini                   # 配置文件（数据库地址、端口、连接池参数等）
├── main.cpp                    # 测试程序，支持性能压测
```

---

## ⚙️ 配置说明

配置文件 `mysql.ini` 示例：

```ini
ip =127.0.0.1
port=3306
username=root
password=yourpassword
initSize=10
maxSize=100
maxIdleTime=60
maxConnectionTimeout=100
dbname=yourdb
```

---

## 📊 性能对比（插入数据耗时）（硬件水平的高低性能也不一样）

| 数据量 | 未使用连接池（单线程） | 未使用连接池（4线程） | 使用连接池（单线程） | 使用连接池（4线程） |
|--------|-------------------------|-------------------------|------------------------|------------------------|
| 1000   | 1891ms                  | 497ms                   | 1079ms                 | 408ms                  |
| 5000   | 10033ms                 | 2361ms                  | 5380ms                 | 2041ms                 |
| 10000  | 19403ms                 | 4589ms                  | 10522ms                | 4034ms                 |

---

## 🧪 编译与运行

### 📌 Windows（VS）

1. 引入 `libmysql.lib` 和 `mysql.h`：
   - 右键项目 → C/C++ → 附加包含目录 → 添加 MySQL 头文件路径
   - 链接器 → 附加库目录 → 添加 libmysql.lib 所在目录
   - 链接器 → 输入 → 附加依赖项：添加 `libmysql.lib`

2. 复制 `libmysql.dll` 到可执行文件目录

3. 使用 `main.cpp` 编译运行测试程序

### 📌 Linux（g++）

> 同样支持 g++ 编译，只需链接 `-lmysqlclient` 库

---

## 📂 参考资料

- [MySQL C API 文档](https://dev.mysql.com/doc/c/)
- [APUE（UNIX环境高级编程）](https://book.douban.com/subject/25900403/)
- [连接池相关原理解释（知乎）](https://zhuanlan.zhihu.com/p/600968354)
                                

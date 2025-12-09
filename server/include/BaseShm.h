#pragma once
#include <iostream>
#include <string>
#include <sys/types.h>

// BaseShm: 简单的 System V 共享内存封装
// 设计说明：
// - 构造函数（按 key 或路径）负责获得或创建共享内存段，但并不自动映射。
// - 使用者调用 `mapShm()` 将共享内存关联到进程地址空间，返回映射地址指针；
//   使用完成后应调用 `unmapShm()` 解除映射。若需要删除共享内存段，调用 `delShm()`。
// - 该类不提供跨进程同步；若多个进程或线程并发访问共享内存，必须由上层引入信号量、互斥体或文件锁来保证一致性。
// - 所有方法尽量保持幂等性（例如多次调用 `unmapShm()` 不应导致未定义行为）。
class BaseShm
{
public:
	// 通过 key 打开/标识共享内存段（System V key）
	explicit BaseShm(int key);
	// 通过 key 创建或指定大小的共享内存段（若不存在则创建）
	BaseShm(int key, int size);
	// 通过文件路径（用于 ftok）打开共享内存段
	explicit BaseShm(const std::string& name);
	// 通过路径创建/打开并指定大小
	BaseShm(const std::string& name, int size);

	// 将共享内存映射到当前进程地址空间，成功返回地址指针，失败返回 nullptr
	void* mapShm();
	// 解除映射，成功返回 0，失败返回 -1
	int unmapShm();
	// 删除共享内存段（shmctl IPC_RMID），成功返回 0，失败返回 -1
	int delShm();

	~BaseShm();

private:
	// 内部获得或创建 shm id 的辅助函数，flag 为 shmget 的 flags
	int getShmID(key_t key, int shmSize, int flag);

private:
	int m_shmID = -1; // 共享内存 id，未初始化时为 -1
protected:
	void* m_shmAddr = nullptr; // 映射地址，未映射时为 nullptr
};


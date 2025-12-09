#pragma once
#include <openssl/md5.h>
#include <openssl/sha.h>
#include <iostream>
#include <string>
#include <cstring>
using namespace std;

enum HashType
{
	T_MD5,
	T_SHA1,
	T_SHA224,
	T_SHA256,
	T_SHA384,
	T_SHA512
};

class Hash
{
public:
	Hash(HashType type);
	~Hash();

	void addData(string data);
	// 返回十六进制字符串表示的摘要（原有行为）
	string result();
	// 返回原始二进制摘要（用于签名/校验，应直接传入 RSA_sign/RSA_verify）
	string resultRaw();

private:
	// md5
	inline void md5Init() { MD5_Init(&m_md5); }
	inline void md5AddData(const char* data)
	{
		MD5_Update(&m_md5, data, strlen(data));
	}
	string md5Result();
	string md5ResultRaw();

	// sha1
	inline void sha1Init() { SHA1_Init(&m_sha1); }
	inline void sha1AddData(const char* data)
	{
		SHA1_Update(&m_sha1, data, strlen(data));
	}
	string sha1Result();
	string sha1ResultRaw();

	// sha224
	inline void sha224Init() { SHA224_Init(&m_sha224); }
	inline void sha224AddData(const char* data)
	{
		SHA224_Update(&m_sha224, data, strlen(data));
	}
	string sha224Result();
	string sha224ResultRaw();

	// sha256
	inline void sha256Init() { SHA256_Init(&m_sha256); }
	inline void sha256AddData(const char* data)
	{
		SHA256_Update(&m_sha256, data, strlen(data));
	}
	string sha256Result();
	string sha256ResultRaw();

	// sha384
	inline void sha384Init() { SHA384_Init(&m_sha384); }
	inline void sha384AddData(const char* data)
	{
		SHA384_Update(&m_sha384, data, strlen(data));
	}
	string sha384Result();
	string sha384ResultRaw();

	// sha512
	inline void sha512Init() { SHA512_Init(&m_sha512); }
	inline void sha512AddData(const char* data)
	{
		SHA512_Update(&m_sha512, data, strlen(data));
	}
	string sha512Result();
	string sha512ResultRaw();

private:
	HashType m_type;
	MD5_CTX m_md5;
	SHA_CTX m_sha1;
	SHA256_CTX m_sha224;
	SHA256_CTX m_sha256;
	SHA512_CTX m_sha384;
	SHA512_CTX m_sha512;

};


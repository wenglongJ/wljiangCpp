#pragma once
#include <iostream>
#include <memory>
#include "CodecFactory.h"
#include "RespondCodec.h"

class RespondFactory :
	public CodecFactory
{
public:
	RespondFactory(std::string enc);
	RespondFactory(RespondInfo* info);
	Codec* createCodec();

	~RespondFactory();

private:
	bool m_flag;
	std::string m_encStr;
	RespondInfo* m_info;
	std::shared_ptr<RespondCodec> m_ptr;
};


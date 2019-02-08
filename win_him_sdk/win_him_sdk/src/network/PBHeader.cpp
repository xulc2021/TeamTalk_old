#include "stdafx.h"
#include "PBHeader.h"

namespace him {

	PBHeader::PBHeader()
		:header_buff_(0)
	{
		header_buff_ = new unsigned char[HEADER_LENGTH];
	}

	PBHeader::~PBHeader()
	{
		delete header_buff_;
		header_buff_ = 0;
	}

	unsigned char* PBHeader::GetSerializeBuffer()
	{
		_Serialize();
		return header_buff_;
	}

	void PBHeader::UnSerialize(unsigned char* headerBuff, unsigned short len)
	{
		if (len != HEADER_LENGTH || 0 == headerBuff || 0 == header_buff_)
			return;
		memcpy(header_buff_, headerBuff, len);
		if (header_buff_)
		{
			CByteStream is(header_buff_, HEADER_LENGTH);
			is >> length_;
			is >> version_;
			is >> flag_;
			is >> module_id_;
			is >> command_id_;
			is >> seq_number_;
			is >> reserved_;
		}
	}

	void PBHeader::_Serialize()
	{
		CByteStream::WriteInt32(header_buff_, length_);
		CByteStream::WriteUint16(header_buff_ + 4, version_);
		CByteStream::WriteUint16(header_buff_ + 6, flag_);
		CByteStream::WriteUint16(header_buff_ + 8, module_id_);
		CByteStream::WriteUint16(header_buff_ + 10, command_id_);
		CByteStream::WriteUint16(header_buff_ + 12, seq_number_);
		CByteStream::WriteUint16(header_buff_ + 14, reserved_);
	}

	void PBHeader::Clear()
	{
		memset(header_buff_, 0, HEADER_LENGTH);
		length_ = 0;
		version_ = VERSION;
		flag_ = 0;
		module_id_ = 0;
		command_id_ = 0;
		seq_number_ = 0;
		reserved_ = 0;
	}
}
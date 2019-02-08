/** @file TTPBHeader.h
  * @brief
  * @author Captain China
  * @date 2019/02/08
  */

#ifndef _TTPBHEADER_F74DFB39_90D2_4544_9C5A_DBFDBDCCE75C_H_
#define _TTPBHEADER_F74DFB39_90D2_4544_9C5A_DBFDBDCCE75C_H_

#include "api/him_sdk_dll.h"
#include "UtilPdu.h"

namespace him {

	const static unsigned char HEADER_LENGTH = 16;
	const static unsigned short VERSION = 1;

	const unsigned int TTPBHEADER_RESERVED_MASK = 0xff00U;

	enum
	{
		RESERVED_TYPE_HISTORY_MESSAGE = TTPBHEADER_RESERVED_MASK | 0x01,		//identify send history message packet
		RESERVED_TYPE_HISTORY_SCROLLBOTTOM_MESSAGE = TTPBHEADER_RESERVED_MASK | 0x02,		//identify send history message packet which will scroll bottom
		RESERVER_TYPE_UNREADER_MESSAGE = TTPBHEADER_RESERVED_MASK | 0x03,		//identify unread message packet
	};

	/**
	 * The class <code>PBHeader</code>
	 *
	 */
	class HIM_SDK_API PBHeader final
	{
	public:
		PBHeader();
		~PBHeader();

	public:
		unsigned char* GetSerializeBuffer();
		void UnSerialize(unsigned char* headerBuff, unsigned short len);
		void Clear();
		inline void SetLength(unsigned int length) { length_ = length; }
		inline void SetModuleId(unsigned short  module_id) { module_id_ = module_id; }
		inline void SetCommandId(unsigned short  cmd_id) { command_id_ = cmd_id; }
		inline void SetSeqNumber(unsigned short  seq) { seq_number_ = seq; }
		inline void SetReserved(unsigned short  reserv_id) { reserved_ = reserv_id; }

		inline unsigned short  GetModuleId()const { return module_id_; }
		inline unsigned short  GetCommandId()const { return command_id_; }
		inline unsigned short  GetReserved()const { return reserved_; }
		inline unsigned short  GetSeqNumber()const { return seq_number_; }

	private:
		void _Serialize();

	private:
		unsigned int 		length_ = 0;
		unsigned short  	version_ = VERSION;
		unsigned short 		flag_ = 0;
		unsigned short 		module_id_ = 0;
		unsigned short 		command_id_ = 0;
		unsigned short 		seq_number_ = 0;	// °üÐòºÅ
		unsigned short 		reserved_ = 0;		// ±£Áô

		unsigned char*				header_buff_ = 0;
	};

}
#endif//_TTPBHEADER_F74DFB39_90D2_4544_9C5A_DBFDBDCCE75C_H_
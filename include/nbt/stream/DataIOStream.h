#ifndef __DATA_IOSTREAM__
#define __DATA_IOSTREAM__ 1

#ifdef __cplusplus

#include <stdint.h>
#include <fstream>
#include <ostream>
#include <string>

namespace dataIO {
	class DataOutputStream {
		public:
			DataOutputStream(std::string filePath, bool littleEndian = false):mNeedDelete(true), mLittleEndian(littleEndian) {
				std::ofstream* of = new std::ofstream();
				of->open(filePath, std::ios::binary | std::ios::out);
				stream = of;
			}

			DataOutputStream(std::ostream& input, bool needDelete = false, bool littleEndian = false) :mNeedDelete(needDelete), mLittleEndian(littleEndian) {
				stream = &input;
			}

			~DataOutputStream() {
				if(mNeedDelete)
					delete stream;
			}

			DataOutputStream(std::string filePath) : DataOutputStream(filePath, false) {}

			void writeInt(int32_t value);
			void writeLong(int64_t value);
			void writeByte(uint8_t value);
			void writeShort(int16_t value);
			void writeFloat(float value);
			void writeDouble(double value);
			void writeString(std::string value);

			bool isLittleEndian() {
				return mLittleEndian;
			}

		private:
			bool mNeedDelete, mLittleEndian;
			std::ostream* stream;
	};

	class DataInputStream {
		public :
			DataInputStream(std::string filePath, bool littleEndian = false) : mNeedDelete(true) ,mLittleEndian(littleEndian) {
				std::ifstream* is = new std::ifstream();
				is->open(filePath, std::ios::in | std::ios::binary);
				stream = is;
			}

			DataInputStream(std::istream& input, bool needDelete = false, bool littleEndian = false): mNeedDelete(needDelete), mLittleEndian(littleEndian) {
				stream = &input;
			}

			~DataInputStream() {
				if(mNeedDelete)
					delete stream;
			}

			bool isLittleEndian() {
				return mLittleEndian;
			}
			uint8_t readByte();
		       	int16_t readShort();
			int32_t readInt();
			int64_t readLong();
			float readFloat();
			double readDouble();
			std::string readString();

		private:
			bool mNeedDelete, mLittleEndian;
			std::istream* stream;
	};
};
#endif

#endif

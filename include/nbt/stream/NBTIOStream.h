#ifndef __NBTIO_STREAM_H__
#define __NBTIO_STREAM_H__ 1

#ifdef __cplusplus

#include "nbt/NBTTags.h"
#include "nbt/stream/DataIOStream.h"

namespace nbt {

	class NBTOutputStream {
		public:
			NBTOutputStream(std::string path, bool littleEndian = false): stream(path, littleEndian) {}
			
			NBTOutputStream(std::ostream input, bool needDelete = false, bool littleEndian = false)
				:stream(input, needDelete, littleEndian) {}

			void writeTag(Tag* tag, bool isInList = false);
			void writeTag(Tag& tag, bool isInList = false);

			void writeIntTag(IntTag tag);
			void writeByteTag(ByteTag tag);
			void writeShortTag(ShortTag tag);
			void writeLongTag(LongTag tag);
			void writeFloatTag(FloatTag tag);
			void writeDoubleTag(DoubleTag tag);
			void writeCompoundTag(CompoundTag tag);
			void writeListTag(ListTag tag);
			void writeIntArray(IntArrayTag tag);
			void writeLongArrayTag(LongArrayTag tag);
			void writeByteArrayTag(ByteArrayTag tag);
			void writeStringTag(StringTag tag);
		private:
			dataIO::DataOutputStream stream;
			
	};

	class NBTInputStream {
		public:
			NBTInputStream(std::string path, bool littleEndian = false) : stream(path, littleEndian) {}
			
			NBTInputStream(std::istream input, bool needDelete = false, bool littleEndian = false)
				:stream(input, needDelete, littleEndian) {}


			Tag* readTag(bool isInList = false, TagType = Unknow);
			IntTag readIntTag();
			ByteTag readByteTag();
			ShortTag readShortTag();
			LongTag readLongTag();
			FloatTag readFloatTag();
			DoubleTag readDoubleTag();
			CompoundTag readCompoundTag();
			ListTag readListTag();
			IntArrayTag readIntArray();
			LongArrayTag readLongArrayTag();
			ByteArrayTag readByteArrayTag();
			StringTag readStringTag();

		private:
			dataIO::DataInputStream stream;
	};
}

#endif
#endif

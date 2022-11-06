#include "nbt/stream/NBTIOStream.h"
#include "nbt/NBTTags.h"
#include <string>

using namespace nbt;

void NBTOutputStream::writeTag(Tag& tag, bool isInList) {
	writeTag(&tag, isInList);
}
void NBTOutputStream::writeTag(Tag* tag, bool isInList) {
	if (!isInList) {
		stream.writeByte(tag->getTagType());
		if (tag->getTagType() != End)
			stream.writeString(tag->getKey());
	}
	switch (tag->getTagType()) {
		case End: {
			break;
		}
		case Byte:{
			ByteTag* tg = castByteTag(tag);
			stream.writeByte(tg->getValue());
			break;
		}
		case Short:{
			ShortTag* ta = castShortTag(tag);
			stream.writeShort(ta->getValue());
			break;		
		}
		case Int : {
			IntTag* it = castIntTag(tag);
			stream.writeInt(it->getValue());
			break;
		}
		case Long: {
			LongTag* lt = castLongTag(tag);
			stream.writeLong(lt->getValue());
			break;
		}
		case Float: {
			FloatTag* ft = castFloatTag(tag);
			stream.writeFloat(ft->getValue());
			break;
		}
		case Double: {
			DoubleTag* dt = castDoubleTag(tag);
			stream.writeDouble(dt->getValue());
			break;
		}
		case List: {
			ListTag* lsttag = castListTag(tag);
			stream.writeByte(lsttag->getEntryKind());
			stream.writeInt(lsttag->getSize());
			for(size_t i = 0; i < lsttag->getSize(); i++) {
				writeTag(lsttag->getValueByCount(i), true);
			}
			break;
		}
		case IntArray: {
			IntArrayTag* iat = castIntArrayTag(tag);
			stream.writeInt(iat->getSize());
			for(size_t i = 0; i < iat->getSize(); i++) {
				stream.writeInt(iat->getValueByCount(i));
			}
			break;
		}
		case LongArray: {
			LongArrayTag* lat = castLongArrayTag(tag);
			stream.writeInt(lat->getSize());
			for(size_t i = 0; i < lat->getSize(); i++) {
				stream.writeLong(lat->getValueByCount(i));
			}
			break;
		}
		case ByteArray : {
			ByteArrayTag* bat = castByteArrayTag(tag);
			stream.writeInt(bat->getSize());
			for(size_t i = 0; i < bat->getSize(); i++) {
				stream.writeByte(bat->getValueByCount(i));
			}
			break;
		}
		case String: {
			StringTag* st = castStringTag(tag);
			stream.writeString(st->getValue());
			break;
		}
		case Compound: {
			CompoundTag* ct = castCompoundTag(tag);
			for(size_t i = 0; i < ct->getSize(); i++) {
				writeTag(ct->getValueByCount(i), false);
			}
			stream.writeByte(End);
			break;
		}
		default:
		     throw "Error: Unknow tag type:" + std::to_string(tag->getTagType());
	}
}

void NBTOutputStream::writeIntTag(IntTag tag) {
	this->stream.writeByte(tag.getTagType());
	this->stream.writeString(tag.getKey());
	this->stream.writeInt(tag.getValue());
}

void NBTOutputStream::writeByteTag(ByteTag tag) {	
	this->stream.writeByte(tag.getTagType());
	this->stream.writeString(tag.getKey());
	this->stream.writeByte(tag.getValue());
}
void NBTOutputStream::writeShortTag(ShortTag tag) {	
	this->stream.writeByte(tag.getTagType());
	this->stream.writeString(tag.getKey());
	this->stream.writeShort(tag.getValue());
}
void NBTOutputStream::writeLongTag(LongTag tag) {
	this->stream.writeByte(tag.getTagType());
	this->stream.writeString(tag.getKey());
	this->stream.writeLong(tag.getValue());
}
void NBTOutputStream::writeFloatTag(FloatTag tag) {	
	this->stream.writeByte(tag.getTagType());
	this->stream.writeString(tag.getKey());
	this->stream.writeFloat(tag.getValue());
}

void NBTOutputStream::writeDoubleTag(DoubleTag tag) {
	this->stream.writeByte(tag.getTagType());
	this->stream.writeString(tag.getKey());
	this->stream.writeDouble(tag.getValue());
}
void NBTOutputStream::writeCompoundTag(CompoundTag tag) {
	this->stream.writeByte(tag.getTagType());
	this->stream.writeString(tag.getKey());
	for (size_t i = 0; i < tag.getSize(); i++) {
		writeTag(tag.getValueByCount(i), false);
	}
	this->stream.writeByte(End);
}

void NBTOutputStream::writeListTag(ListTag tag) {
	this->stream.writeByte(tag.getTagType());
	this->stream.writeString(tag.getKey());
	this->stream.writeByte(tag.getEntryKind());
	this->stream.writeInt(tag.getSize());

	for(size_t i = 0; i < tag.getSize(); i++) {
		writeTag(tag.getValueByCount(i), true);
	}
}

void NBTOutputStream::writeIntArray(IntArrayTag tag) {
	this->stream.writeByte(tag.getTagType());
	this->stream.writeString(tag.getKey());
	this->stream.writeInt(tag.getSize());
	for(size_t i = 0; i < tag.getSize(); i++) {
		this->stream.writeInt(tag.getValueByCount(i));
	}
}
void NBTOutputStream::writeLongArrayTag(LongArrayTag tag) {
	this->stream.writeByte(tag.getTagType());
	this->stream.writeString(tag.getKey());
	this->stream.writeInt(tag.getSize());
	for(size_t i = 0; i < tag.getSize(); i++) {
		this->stream.writeLong(tag.getValueByCount(i));
	}

}
void NBTOutputStream::writeByteArrayTag(ByteArrayTag tag) {
	this->stream.writeByte(tag.getTagType());
	this->stream.writeString(tag.getKey());
	this->stream.writeInt(tag.getSize());
	for(size_t i = 0; i < tag.getSize(); i++) {
		this->stream.writeByte(tag.getValueByCount(i));
	}

}
void NBTOutputStream::writeStringTag(StringTag tag) {
	this->stream.writeByte(tag.getTagType());
	this->stream.writeString(tag.getKey());
	this->stream.writeString(tag.getValue());
}
		




Tag* NBTInputStream::readTag(bool isInList, TagType type) {
	TagType tt;
	std::string key("");
	if(!isInList) {
		tt = (TagType)stream.readByte();
		if(tt != End) key = stream.readString();
	} else {
		tt = type;
	}
	switch(tt) {
		case End:
			return new EndTag();
		case Byte:
			return new ByteTag(key, stream.readByte());
		case Short:
			return new ShortTag(key, stream.readShort());
		case Int :
			return new IntTag(key, stream.readInt());
		case Long:
			return new LongTag(key, stream.readLong());
		case Float:
			return new FloatTag(key, stream.readFloat());
		case Double:
			return new DoubleTag(key, stream.readDouble());
		case ByteArray: {
			int32_t len = stream.readInt();
			std::vector<uint8_t> v {};
			v.reserve(len);
			for (int32_t i = 0; i < len; i++) {
				v.push_back(stream.readByte());
			}
			return new ByteArrayTag(key, v);
		}
		case IntArray: {
			int32_t len = stream.readInt();
			std::vector<int32_t> v {};
			v.reserve(len);
			for (int32_t i = 0; i < len; i++) {
				v.push_back(stream.readInt());
			}
			return new IntArrayTag(key, v);
		}
		case LongArray:{
			int32_t len = stream.readInt();
			std::vector<int64_t> v {};
			v.reserve(len);
			for(int32_t i = 0; i < len; i++) {
				v.push_back(stream.readLong());
			}
			return new LongArrayTag(key, v);
		}
		case String: 
			return new StringTag(key, stream.readString());
		case List: {
			TagType tt = (TagType)stream.readByte();
			int32_t len = stream.readInt();
			ListTag* ret = new ListTag(key, tt);
			ret->reverse(len);
			for(int32_t i = 0; i < len; i++) {
				Tag* t = readTag(true, tt);
				ret->add(t);
				delete t;
			}
			return ret;
		}
		case Compound: {
			CompoundTag* ret = new CompoundTag(key);
			Tag* tag;
			while((tag = readTag(false))->getTagType() != End) {
				ret->add(tag);
				delete tag;
			}
			delete tag;
			return ret;
		}
		case Unknow:
			 throw "Unknow Tag Type!!! key " + key;
		default:
			 throw "Unknow Tag Type!!! key " + key + ", TagType:" + std::to_string((int) tt);
	}
	return new Tag("");
}

IntTag NBTInputStream::readIntTag() {
	stream.readByte();//a byte
	IntTag ret(stream.readString(), stream.readInt());
	return ret;
}

ByteTag NBTInputStream::readByteTag() {
	stream.readByte();
	ByteTag ret(stream.readString(), stream.readByte());
	return ret;
}

ShortTag NBTInputStream::readShortTag() {
	stream.readByte();
	ShortTag ret(stream.readString(), stream.readShort());
	return ret;
}

LongTag NBTInputStream::readLongTag() {
	stream.readByte();
	LongTag lt(stream.readString(), stream.readLong());
	return lt;
}

FloatTag NBTInputStream::readFloatTag() {
	stream.readByte();
	FloatTag ft(stream.readString(), stream.readFloat());
	return ft;
}

DoubleTag NBTInputStream::readDoubleTag() {
	stream.readByte();
	DoubleTag dt(stream.readString(), stream.readDouble());
	return dt;
}

StringTag NBTInputStream::readStringTag() {
	stream.readByte();
	return StringTag(stream.readString(), stream.readString());
}

ByteArrayTag NBTInputStream::readByteArrayTag() {
	stream.readByte();
	std::vector<uint8_t> v {};
	std::string key = stream.readString();
	for(int32_t i = 0; i < stream.readInt(); i++) {
		v.push_back(stream.readByte());
	}
	ByteArrayTag bat(key, v);
	return bat;
}

IntArrayTag NBTInputStream::readIntArray() {
	stream.readByte();
	std::string key = stream.readString();
	std::vector<int32_t> v {};
	for(int32_t i = 0; i < stream.readInt(); i++) {
		v.push_back(stream.readInt());
	}
	return IntArrayTag(key, v);
}

LongArrayTag NBTInputStream::readLongArrayTag() {
	stream.readByte();
	std::string key(stream.readString());
	std::vector<int64_t> v {};
	for (int32_t i = 0; i < stream.readInt(); i++) {
		v.push_back(stream.readLong());
	}
	return LongArrayTag(key, v);
}

ListTag NBTInputStream::readListTag() {
	stream.readByte();

	std::string key(stream.readString());
	TagType tt = (TagType)stream.readByte();
	int32_t size = stream.readInt();

	ListTag lt(key, tt);
	lt.reverse(size);

	for(int32_t i = 0; i < size; i++) {
		Tag* t = readTag(true, tt);
		lt.add(t);
		delete t;
	}
	return lt;

}

CompoundTag NBTInputStream::readCompoundTag() {
	stream.readByte();
	std::string key(stream.readString());

	CompoundTag ct(key);

	Tag* tag;
	while((tag = readTag(false))->getTagType() != End) {
		ct.add(tag);	
		delete tag;
	}
	delete tag;
	return ct;
}

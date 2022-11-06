#include "nbt/NBTTags.h"
#include <string>

namespace nbt {

	IntTag* castIntTag(Tag *t) {return dynamic_cast<IntTag*>(t);}

	#define castTag(klass) klass* cast##klass(Tag* t){return dynamic_cast<klass*>(t);}

	castTag(ByteTag);
	castTag(ShortTag);
	castTag(CompoundTag);
	castTag(LongTag);
	castTag(FloatTag);
	castTag(ByteArrayTag);
	castTag(DoubleTag);
	castTag(LongArrayTag);
	castTag(StringTag);
	castTag(ListTag);
	castTag(IntArrayTag);

	#undef castTag



}

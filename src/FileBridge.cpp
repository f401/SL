#include "main.h"
#include "FileBridge.h"
#include "nbt/stream/NBTIOStream.h"
#include "other_tools/log.h"
#include "other_tools/CompressFile.h"
#include <string>
#include <config.h>

std::string filePath("/opt/c/projects/sl/k.dat");

extern "C" {

	static int mX, mMinesCount;

	void sl_FileBridge_save(int saveStatus) {
		using namespace nbt;
		
		CompoundTag root("");

		root.add(LongTag("version", atol(VERSION)))
			.add(IntTag("X", X))
			.add(IntTag("mines_count", MINES_COUNT))
			.add(IntTag("spaces", SPACES))
			.add(IntTag("has_status", saveStatus));

		//save all data
		for(unsigned int i = 0; i < X; i++) {
			for(unsigned int j = 0; j < X; j++) {
				Block data = *(gameData + X*i + j);
				CompoundTag compoundRoot(std::to_string(X*i + j));
				std::vector<int32_t> pos_vector {(int32_t)data.pos.x, (int32_t)data.pos.y};
				IntArrayTag posTag("pos", pos_vector);
				compoundRoot.add(posTag).add(IntTag("who", data.who));
				if(saveStatus) compoundRoot.add(IntTag("status", data.status));
				root.add(compoundRoot);
			}
		}

		//write data
		NBTOutputStream* nos = new NBTOutputStream(filePath, true);
		nos->writeTag(root);
		delete nos;
		sl_compress_compressWithGzip(filePath.c_str());
	}

	void sl_FileBridge_loadArgsFromFile(const char* path) {
		using namespace nbt;
		
		if (path) filePath = path;

		sl_compress_decompressWithGzip(filePath.c_str());	

		NBTInputStream nis(filePath, true);
		CompoundTag rootTag = nis.readCompoundTag();
		LongTag versionTag = *(castLongTag(rootTag.getChildByKey("version")));
		//check version
		if (versionTag.getValue() > atol(VERSION)) {
			sl_logger_error("curr ver: %d, nbt tag version: %d", VERSION, versionTag.getValue());
		}

		sl_logger_info("nbt tag version: %d", versionTag.getValue());

		mX = X = castIntTag(rootTag.getChildByKey("X"))->getValue();
		mMinesCount = MINES_COUNT = castIntTag(rootTag.getChildByKey("mines_count"))->getValue();
		SPACES = castIntTag(rootTag.getChildByKey("spaces"))->getValue();
	}

	void sl_FileBridge_loadDataFromFile() {
		using namespace nbt;

		NBTInputStream nis(filePath, true);
		CompoundTag rootTag = nis.readCompoundTag();
		IntTag* has_status = castIntTag(rootTag.getChildByKey("has_status"));

		for(unsigned int i = 0; i < X; i++) {
			for (unsigned int j = 0; j < X; j++) {

				CompoundTag* t = castCompoundTag(rootTag.getChildByKey(std::to_string(i*X + j)));					
				Block* data = gameData + i*X + j;
				
				IntArrayTag* posArrayTag = castIntArrayTag(t->getChildByKey("pos"));
				data->pos.x = posArrayTag->getValueByCount(0);
				data->pos.y = posArrayTag->getValueByCount(1);

				IntTag* whoTag = castIntTag(t->getChildByKey("who"));
				data->who = (Who)whoTag->getValue();
				
				if(has_status->getValue()) {
					IntTag* statusTag = castIntTag(t->getChildByKey("status"));
					data->status = (Status)statusTag->getValue();
				} else {
					data->status = STATUS_CLOSE;
				}
			}
		}
		sl_compress_compressWithGzip(filePath.c_str());

	}
}

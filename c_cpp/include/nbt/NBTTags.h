#ifndef __TAGS_H__
#define __TAGS_H__ 1

#ifdef __cplusplus

#include <stdint.h>
#include <string>
#include <iostream>
#include <vector>

namespace nbt {

	typedef uint8_t byteNBT;
	typedef int16_t shortNBT;
	typedef int32_t intNBT;
	typedef int64_t longNBT;
	typedef double doubleNBT;
	typedef float floatNBT;
	typedef uint16_t stringLength;
	
	enum TagType : int8_t {
		Unknow = -1,
		End = 0,
		Byte = 1,
		Short = 2,
		Int = 3,
		Long = 4,
		Float = 5,
		Double = 6,
		ByteArray = 7,
		String = 8,
		List = 9,
		Compound = 10,
		IntArray = 11,
		LongArray = 12
	};

	class Tag {
		public :
			Tag(const std::string key) :mKey(key) {}

			virtual ~Tag() {}

			virtual TagType getTagType() const {return Unknow;}

			inline void setKey(const std::string key) noexcept {this->mKey = key;}

			inline std::string getKey() const noexcept {return mKey;}

			bool operator==(const std::string& v) {
				return mKey == v;
			}

			bool operator==(const Tag& t) {return mKey == t.mKey;}

			friend std::ostream& operator<<(std::ostream& output, const Tag& tag) {
				output << tag.toString();
				return output;
			}

			virtual Tag* getDeepCopy() const {return new Tag(mKey);}

			virtual std::string toString() const {return "Tag: key: " + mKey;}

		protected:
			std::string mKey;
	};

	template<class T>
	class BaseTag : public Tag {
		public:
			BaseTag(const std::string key, T value):Tag(key), mValue(value) {}

			using Tag::getTagType;

			inline T getValue() noexcept {return mValue;}

			inline void setValue(const T value) noexcept {this->mValue = value;}

			friend std::ostream& operator<<(std::ostream& output, const BaseTag<T>& tag) {
				output << tag.toString();
				return output;
			}

		protected:
			T mValue;
	};

	class StringTag : public BaseTag<std::string> {
		public:
			StringTag(std::string key, std::string value) : BaseTag<std::string>(key, value) {}

			virtual TagType getTagType() const override {return String;}

			virtual Tag* getDeepCopy() const override {return new StringTag(mKey, mValue);}

			virtual std::string toString() const override {return "StringTag: key" + mKey + ", value:" + mValue;}
	};

	class ByteTag : public BaseTag<byteNBT> {
		public:
			ByteTag(std::string key, byteNBT value) : BaseTag<byteNBT>(key, value) {}

			virtual TagType getTagType() const override {return Byte;}

			virtual Tag* getDeepCopy() const override {return new ByteTag(mKey, mValue);}

			virtual std::string toString() const override {return "ByteTag: key" + mKey + ", value:" + std::to_string((int) mValue);}
	};

	class EndTag : public BaseTag<int8_t> {
		public:
			EndTag() : BaseTag<int8_t>("", 0) {}

			virtual TagType getTagType() const override {return End;}

			virtual Tag* getDeepCopy() const override {return new EndTag();}
			
			virtual std::string toString() const override {return "EndTag";}

	};

	class ShortTag : public BaseTag<shortNBT> {
		public:
			using BaseTag<shortNBT>::BaseTag;
			
			virtual TagType getTagType() const override {return Short;}

			virtual Tag* getDeepCopy() const override {return new ShortTag(mKey, mValue);}

			virtual std::string toString() const override {return "ShortTag: key:" + mKey + ", value: " + std::to_string(mValue);}
	};

	class IntTag : public BaseTag<intNBT> {
		public:
			using BaseTag<intNBT>::BaseTag;

			virtual TagType getTagType() const override {return Int;}

			virtual Tag* getDeepCopy() const override {return new IntTag(mKey, mValue);}

			virtual std::string toString() const override {return "IntTag: key:" + mKey + ", value:" + std::to_string(mValue);}
	};

	class LongTag : public BaseTag<longNBT> {
		public:
			using BaseTag<longNBT>::BaseTag;

			virtual TagType getTagType() const override {return Long;}

			virtual Tag* getDeepCopy() const override {return new LongTag(mKey, mValue);}

			virtual std::string toString() const override {return "LongTag: key:" + mKey + ", value:" + std::to_string(mValue);}
	};

	class FloatTag : public BaseTag<float> {
		public :
			using BaseTag<float>::BaseTag;

			virtual TagType getTagType() const override {return Float;}

			virtual Tag* getDeepCopy() const override {return new FloatTag(mKey, mValue);}

			virtual std::string toString() const override {return "FloatTag: key:" + mKey + ",value:" + std::to_string(mValue);}
	};

	class DoubleTag : public BaseTag<double> {
		public:
			using BaseTag<double>::BaseTag;

			virtual TagType getTagType() const override {return Double;}

			virtual Tag* getDeepCopy() const override {return new DoubleTag(mKey, mValue);}

			virtual std::string toString() const override {return "DoubleTag: key:" + mKey + ",value:" + std::to_string(mValue);}
	};

	template<typename T, TagType type> class BaseArray : public BaseTag<std::vector<T>> {
		public:
			BaseArray(std::string key, std::vector<T> value) : BaseTag<std::vector<T>>(key, value) {}

			BaseArray(std::string key) {
				std::vector<T> a {};
				BaseArray(key, a);
			}
			
			typedef typename std::vector<T>::iterator iterator;
			typedef typename std::vector<T>::const_iterator const_iterator;
			typedef typename std::vector<T>::reverse_iterator reverse_iterator;
			typedef typename std::vector<T>::const_reverse_iterator const_reverse_iterator;

			inline iterator begin() noexcept {return this->mValue.begin();}
			inline iterator end() noexcept {return this->mValue.end();}
			inline const_iterator cbegin() const noexcept {return this->mValue.cbegin();}
			inline const_iterator cend() const noexcept {return this->mValue.cend();}
			inline reverse_iterator rbegin() noexcept {return this->mValue.rbegin();}
			inline reverse_iterator rend() noexcept {return this->mValue.rend();}
			inline const_reverse_iterator crbegin() const noexcept {return this->mValue.crbegin();}
			inline const_reverse_iterator crend() const noexcept {return this->mValue.crend();}

			constexpr TagType getEntryKind() const noexcept {return type;}

			virtual TagType getTagType() const override {return Unknow;}

			inline size_t getSize() const noexcept {return this->mValue.size();}

			inline void reserve(size_t size) noexcept {this->mValue.reserve(size);}

			inline BaseArray& add(const T value) noexcept {
				//this->mValue.push_back(value); 
				this->mValue.emplace_back(value);
				return *this;
			}//这里返回引用是为了多态

			BaseArray& remove(const size_t num) {
				if(num < 0 || num > this->mValue.size()) {
					std::cerr << "Error: try to delete " << num << ", max size: " << this->mValue.size() << std::endl;
				}
				this->mValue.erase(this->mValue.begin() + num);
				return *this;
			}

			inline T getValueByCount(const size_t count) const noexcept {return this->mValue[count];}

			T& operator[] (const size_t i) {return this->mValue[i];}

			friend std::ostream& operator<<(std::ostream& output, const BaseArray<T, type>& tag) {
				output << tag.toString();
				return output;
			}
	};

	class IntArrayTag : public BaseArray<intNBT, Int> {
		public:
			IntArrayTag(std::string key, std::vector<intNBT> value) : BaseArray<intNBT, Int>(key, value) {}

			virtual TagType getTagType() const override {return IntArray;}

			virtual Tag* getDeepCopy() const override {return new IntArrayTag(mKey, mValue);}

			virtual std::string toString() const override {
				std::string ret = 
				"IntArrayTag: size:" + 
				std::to_string(this->getSize()) +
				", key:" + mKey + ", value:";
				for(auto i : mValue) {
					ret.append(std::to_string(i) + ",");
				}
				return ret;
			}
	};

	class LongArrayTag : public BaseArray<longNBT, Long> {
		public:
			using BaseArray<longNBT, Long>::BaseArray;

			virtual TagType getTagType() const override {return LongArray;}

			virtual Tag* getDeepCopy() const override {return new LongArrayTag(mKey, mValue);}

			virtual std::string toString() const override {
				std::string ret = 
				"LongArrayTag: size:" + 
				std::to_string(this->getSize()) +
				", key:" + mKey + ", value:";
				for(auto i : mValue) {
					ret.append(std::to_string(i) + ",");
				}
				return ret;
			}
	};

	class ByteArrayTag : public BaseArray<byteNBT, Byte> {
		public:
			using BaseArray<byteNBT, Byte>::BaseArray;

			virtual TagType getTagType() const override {return ByteArray;}

			virtual Tag* getDeepCopy() const override {return new ByteArrayTag(mKey, mValue);}

			virtual std::string toString() const override {
				std::string ret = 
				"ByteArrayTag: size:" + 
				std::to_string(this->getSize()) +
				", key:" + mKey + ", value:";
				for(auto i : mValue) {
					ret.append(std::to_string(i) + ",");
				}
				return ret;
			}
	};

	class CompoundTag : public Tag {
		public:
			inline void reverse(size_t size) noexcept {this->children.reserve(size);}

			virtual std::string toString() const override {
				std::string ret = 
				"CompoundTag: size:" + 
				std::to_string(this->getSize()) +
				", key:" + mKey + ", value:\n";
				for(auto i : children){
					ret.append(i->toString() + "\n");
				}
				return ret;
			}
			CompoundTag(const std::string key, const std::vector<Tag*> value) : Tag(key) {
				for(size_t i = 0; i < value.size(); i++) {
					Tag* ta = value[i]->getDeepCopy();
					children.emplace_back(ta);
				}
//				printf("构造函数\n");
			}

			CompoundTag(const std::string key) : Tag(key){
				std::vector<Tag*> t {};
				CompoundTag(key, t);
			}

			CompoundTag(const CompoundTag& ct):Tag(ct.mKey) {
				for(auto t : ct.children) {
					Tag* ta = t->getDeepCopy();
					children.emplace_back(ta);
				}
//				printf("拷贝构造函数\n");
			}

			CompoundTag& operator=(const CompoundTag& ct) {
				if(&ct != this) 
					for(auto t : ct.children) {
						Tag* ta = t->getDeepCopy();
						children.emplace_back(ta);
					}
//				printf("赋值函数\n");
				return *this;
			}

			CompoundTag(CompoundTag&& ct) = default;

			virtual ~CompoundTag() override {
				for(size_t i = 0; i < children.size(); i++) {
					Tag* t = children[i];
					delete t;
				}
//				printf("析构函数\n");
			}

			typedef typename std::vector<Tag*>::iterator iterator;
			typedef typename std::vector<Tag*>::const_iterator const_iterator;
			typedef typename std::vector<Tag*>::reverse_iterator reverse_iterator;
			typedef typename std::vector<Tag*>::const_reverse_iterator const_reverse_iterator;

			inline iterator begin() noexcept {return this->children.begin();}
			inline iterator end() noexcept {return this->children.end();}
			inline const_iterator cbegin() const noexcept {return this->children.cbegin();}
			inline const_iterator cend() const noexcept {return this->children.cend();}
			inline reverse_iterator rbegin() noexcept {return this->children.rbegin();}
			inline reverse_iterator rend() noexcept {return this->children.rend();}
			inline const_reverse_iterator crbegin() const noexcept {return this->children.crbegin();}
			inline const_reverse_iterator crend() const noexcept {return this->children.crend();}		

			inline size_t getSize() const noexcept {return children.size();}

			virtual Tag* getDeepCopy() const override {return new CompoundTag(mKey, children);}

			Tag* getChildByKey(const std::string key) const noexcept {
				for(size_t i = 0;i < children.size(); i++) {
					Tag* tag = children[i];
					if(tag->getKey() == key) {
						return tag;
					}
				}
				return NULL;
			}

			virtual CompoundTag& add(const Tag* tag) {
				children.emplace_back(tag->getDeepCopy());
				return *this;
			}

			virtual CompoundTag& add(const Tag& tag) {
				children.emplace_back(tag.getDeepCopy());
				return *this;
			}

			CompoundTag& remove(const size_t num) {
				if(num > children.size()) {
					std::cerr << "Error:try to delete " << num << ", but it number is ill.Max size " << children.size() << std::endl;
				}
				Tag* t = children[num];
				children.erase(children.begin() + num);
				delete t;
				return *this;
			}

			virtual TagType getTagType() const override {return Compound;}

			virtual Tag* getValueByCount(const size_t i) noexcept {return children[i];}

			friend std::ostream& operator<<(std::ostream& output, const CompoundTag& ct) {
				//output << "Compound class: size:" << ct.getSize() << "\n";
				output << ct.toString();
				/*for(int i = 0; i < ct.getSize(); i++) {
					output << "\t" << *(ct.children[i]) << "\n";
				}*/
				return output;
			}

		protected:
			std::vector<Tag*> children {};
	};

	class ListTag : public CompoundTag {
		public:
			ListTag(const std::string key, const TagType entryKind, const std::vector<Tag*> value) : CompoundTag(key, value), mEntryKind(entryKind) {
				for(auto i : children) {
					if(i->getTagType() != mEntryKind) throw "Error!!!TagType:" + std::to_string((int)i->getTagType()) + ", it should be:" + std::to_string((int)mEntryKind);
				}
			}

			ListTag(std::string key, TagType entryKind) : CompoundTag(key) ,mEntryKind(entryKind) {
				std::vector<Tag*> t;
				ListTag(key, entryKind, t);
			}

			virtual TagType getTagType() const override {return List;}

			virtual std::string toString() const override {
				std::string ret = 
				"ListTag: size:" + 
				std::to_string(this->getSize()) +
				", key:" + mKey + ", value:\n";
				for(auto i : children){
					ret.append(i->toString() + "\n");
				}
				return ret;
			}

			virtual ListTag& add(const Tag& tag) override {
				if(tag.getTagType() != mEntryKind) {
					std::cerr << "Error: TagType: " << (int)tag.getTagType() << ", but it should be :" << (int)mEntryKind << std::endl;
					return *this;
				}
				this->children.emplace_back(tag.getDeepCopy());
				return *this;
			}

			inline TagType getEntryKind() {return mEntryKind;}

			virtual Tag* getDeepCopy() const override {return new ListTag(mKey, mEntryKind, children);}

			virtual ListTag& add(const Tag* tag) override {
				if(tag->getTagType() != mEntryKind) {
					std::cerr << "Error: TagType: " << (int)tag->getTagType() << ", but it should be :" << (int)mEntryKind << std::endl;
					return *this;
				}
				this->children.push_back(tag->getDeepCopy());
				return *this;
			}

			friend std::ostream& operator<<(std::ostream& output, const ListTag& lt) {
				output << "List class: size:" << lt.getSize() << ", entryKind:" << (int)lt.mEntryKind << "\n";
				for(size_t i = 0; i < lt.getSize(); i++) {
					output << "\t" << *(lt.children[i]) << "\n";
				}
				return output;
			}

		private:
			TagType mEntryKind;
	};     
	//#define castTag(klass) klass* cast##klass(Tag* t){return dynamic_cast<klass*>(t);}
	#define defineTagCast(klass) klass* cast##klass(Tag* t);

	IntTag* castIntTag(Tag* t); 

	defineTagCast(ByteTag);
	defineTagCast(ShortTag);
	defineTagCast(CompoundTag);
	defineTagCast(LongTag);
	defineTagCast(FloatTag);
	defineTagCast(ByteArrayTag);
	defineTagCast(DoubleTag);
	defineTagCast(LongArrayTag);
	defineTagCast(StringTag);
	defineTagCast(ListTag);
	defineTagCast(IntArrayTag);

	#undef defineTagCast


}

#endif
#endif

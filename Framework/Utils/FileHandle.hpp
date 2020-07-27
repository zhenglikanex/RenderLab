#pragma once

#include <sstream>

namespace Aurora
{
	/*
	* 文件描述符;
	*/
	class FileHandle
	{
	public:
		FileHandle()
		{

		}

		FileHandle(const std::string& name, const std::shared_ptr<std::istream>& in)
			:res_name_(name), istream_(in)
		{
			// 计算文件大小;
			auto pos = this->tellg();
			this->seekg(0, std::ios::end);
			size_ = this->tellg();
			this->seekg(pos);
		}
		~FileHandle()
		{}

		void set_res_name(const std::string& name)
		{
			res_name_ = name;
		}

		const std::string& res_name() const { return res_name_; }

		void read(void* p, size_t size)
		{
			if (istream_) {
				istream_->read(static_cast<char*>(p), static_cast<std::streamsize>(size));
			}
		}

		void seekg(std::ios::pos_type offset, std::ios_base::seekdir way)
		{
			if (istream_) {
				istream_->seekg(offset, way);
			}
		}

		void seekg(std::ios::pos_type pos)
		{
			if (istream_) {
				istream_->seekg(pos);
			}
		}

		std::ios::pos_type tellg() {
			if (istream_) {
				return istream_->tellg();
			}
			return 0;
		}

		void clear()
		{
			if (istream_) {
				istream_->clear(std::ios::goodbit);
			}
		}

		std::streamsize size() const
		{
			return size_;
		}

		operator bool() const
		{
			if (istream_) {
				return !istream_->fail();
			}
			return false;
		}

		bool operator!() const {
			if (istream_) {
				return istream_->operator!();
			}
			return false;
		}

		std::shared_ptr<std::istream> istream()
		{
			return istream_;
		}

		template<class T>
		void operator>>(T& dest)
		{
			read(&dest, sizeof(T));
		}

		std::string string() const
		{
			std::stringstream sstream;
			sstream << istream_->rdbuf();
			return sstream.str();
		}
	private:
		std::string res_name_;
		std::shared_ptr<std::istream> istream_;
		std::streamsize size_;
	};
}
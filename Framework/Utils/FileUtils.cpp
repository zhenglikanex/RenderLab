#include "FileUtils.hpp"
#include <windows.h>
#include <fstream>

namespace Aurora
{
	FileUtils::FileUtils()
	{
		current_path_ = filesystem::current_path();
		search_paths_.push_back(current_path_);
	}

	FileUtils::~FileUtils()
	{

	}

	void FileUtils::AddSearchPath(const std::string& path)
	{
		auto full_path = current_path_ / path;
		for (auto& entry : search_paths_)
		{
			if (entry == full_path)
			{
				return;
			}
		}

		search_paths_.push_back(full_path);
	}

	filesystem::path FileUtils::GetFullPath(const std::string& file_name)
	{
		// 判断是否已经是完整的路径
		if (IsExists(file_name))
		{
			return filesystem::path(file_name);
		}

		for (auto& entry : search_paths_)
		{
			filesystem::path path = entry / file_name;
			if (IsExists(path))
			{
				return path;
			}
		}

		// 返回个空的路径
		filesystem::path path;
		return path;
	}

	FileHandle FileUtils::OpenFile(const std::string& file_name, uint32_t mode /* = std::ios_base::in */)
	{
		auto full_path = GetFullPath(file_name);
		if (full_path.empty())
		{
			return FileHandle();
		}
		
		auto full_name = full_path.string();
		std::shared_ptr<std::ifstream> istream = std::make_shared<std::ifstream>(full_path.string(), mode);

		return FileHandle(full_name, istream);
	}

	bool FileUtils::IsExists(const std::string& file_name)
	{
		return filesystem::exists(file_name);
	}

	bool FileUtils::IsExists(const filesystem::path& path)
	{
		return filesystem::exists(path);
	}
}
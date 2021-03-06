#pragma once

#include <string>

#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
// C++17(����boost��filesystem)
#include <experimental/filesystem>

#include "Framework/Utils/Singleton.hpp"
#include "Framework/Utils/FileHandle.hpp"

using namespace std::experimental;

namespace Aurora
{
	class FileUtils : public Singleton<FileUtils>


	{
	public:
		FileUtils();
		~FileUtils();

		void AddSearchPath(const std::string& path);

		filesystem::path GetFullPath(const std::string& file_name);

		FileHandle OpenFile(const std::string& file_name, uint32_t mode = std::ios_base::in);

		std::string OpenFileAndReadString(const std::string& file_name);

		bool IsExists(const std::string& file_name);
		bool IsExists(const filesystem::path& path);

		const filesystem::path& CurrentPath() const { return current_path_; }
	private:
		filesystem::path current_path_;
		std::vector<filesystem::path> search_paths_;
	};
}

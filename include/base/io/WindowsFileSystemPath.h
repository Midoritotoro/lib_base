#pragma once

#include <string>


namespace base::io {
	class WindowsFileSystemPath {
	public:
		static [[nodiscard]] bool isAbsolutePath(const std::string& path);
		static [[nodiscard]] bool isRelativePath(const std::string& path);

		//!
		//! \return std::tuble(head, tail), ��� tail - ��� ���, ��� ��������� ����� ��������� ����� �����.
		//! ����� ����� ����� ���� ������.
		static [[nodiscard]] std::pair<std::string, std::string>
			split(const std::string& path);

		//!
		//! \return ���������� ��������� (directoryName) ����� ����, ������ split(path).first
		static [[nodiscard]] std::string directoryName(const std::string& path);

		//!
		//! \return ���������� �������� (baseName) ����� ����, ������ split(path).second
		static [[nodiscard]] std::string baseName(const std::string& path);
	};
} // namespace base::io
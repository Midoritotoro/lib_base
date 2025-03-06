#pragma once

#include <string>
#include <base/core/BaseNamespace.h>

__BASE_IO_NAMESPACE_BEGIN

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

__BASE_IO_NAMESPACE_END

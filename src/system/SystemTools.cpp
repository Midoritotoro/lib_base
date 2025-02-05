#include <base/system/SystemTools.h>

#include <string>
#include <algorithm>

#if defined(OS_WIN)
	#include <tlHelp32.h>
#endif

namespace base::system::utility {
	#if defined(OS_WIN) && defined(LIB_BASE_ENABLE_WINDOWS_UNICODE)

		int ConvertWCharToUnicode(
			char* buffer,
			size_t bufferlen,
			const wchar_t* input)
		{
			return WideCharToMultiByte(
				CP_UTF8, 0, input, -1,
				buffer, (int)bufferlen, NULL, NULL);
		}

		int ConvertUnicodeToWChar(
			wchar_t* buffer,
			size_t bufferlen,
			const char* input)
		{
			return MultiByteToWideChar(
				CP_UTF8, 0, input, -1,
				buffer, (int)bufferlen);
		}

	#endif

	std::string AbsolutePathFromDescriptor(FILE* descriptor) {
		static const char* err = "base::system::AbsolutePathFromDescriptor: Не удается извлечь путь из нулевого дескриптора файла. ";
		SystemAssert(descriptor != nullptr, err, "");

		int fd = _fileno(descriptor);
		SystemAssert(fd != -1, err, "");

#if defined(OS_WIN)
		HANDLE handle = (HANDLE)_get_osfhandle(fd); // Получаем HANDLE
		SystemAssert(handle != INVALID_HANDLE_VALUE, err, "");

		std::vector<wchar_t> buffer(MAX_PATH);
		DWORD length = GetFinalPathNameByHandleW(
			handle, buffer.data(), MAX_PATH,
			FILE_NAME_NORMALIZED | VOLUME_NAME_DOS);

		SystemAssert(length != 0, err, "");

		if (length > MAX_PATH) {
			buffer.resize(length);
			length = GetFinalPathNameByHandleW(handle, buffer.data(), length, FILE_NAME_NORMALIZED | VOLUME_NAME_DOS);

			SystemAssert(length != 0, err, "");
		}

		std::wstring wpath(buffer.data());

		SystemAssert(!wpath.empty(), err, "");

		int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wpath[0], (int)wpath.size(), NULL, 0, NULL, NULL);
		std::string path(size_needed, 0);

		WideCharToMultiByte(CP_UTF8, 0, &wpath[0], (int)wpath.size(), &path[0], size_needed, NULL, NULL);

		return path;
#elif defined(OS_MAC) || defined(OS_LINUX)
		char path[PATH_MAX];
#if defined(OS_MAC)
		SystemAssert(fcntl(fd, F_GETPATH, path) != -1, err, "");
#else
		char proc_path[64];
		snprintf(proc_path, sizeof(proc_path), "/proc/self/fd/%d", fd);

		ssize_t len = readlink(proc_path, path, sizeof(path) - 1);
		SystemAssert(len != -1, err, "");

		path[len] = '\0';
#endif

		return std::string(path);
#endif
		return "";
	}

	std::vector<std::string> GetAllProcesses() {
		std::vector<std::string> processes;
#if defined (OS_WIN)
		HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

		PROCESSENTRY32 entry;
		entry.dwSize = sizeof(PROCESSENTRY32);

		bool isProcessExists = Process32First(snapshot, &entry);
		while (isProcessExists) {
			std::string name = std::string(lstrlen(entry.szExeFile), 0);
			ConvertWCharToUnicode(&name[0], lstrlen(entry.szExeFile), entry.szExeFile);

			processes.push_back(name);
			isProcessExists = Process32Next(snapshot, &entry);
		}

		CloseHandle(snapshot);
#elif defined(OS_MAC)
		int mib[] = { CTL_KERN, KERN_PROC, KERN_PROC_ALL };

		size_t size;
		if (sysctl(mib, 3, NULL, &size, NULL, 0) == -1) {
			perror("sysctl");
			return processes;
		}

		std::vector<kinfo_proc> kinfo(size / sizeof(kinfo_proc));
		if (sysctl(mib, 3, kinfo.data(), &size, NULL, 0) == -1) {
			perror("sysctl");
			return processes;
		}

		for (const auto& kp : kinfo) {
			if (kp.kp_proc.p_pid > 0) {
				processes.push_back(kp.kp_proc.p_comm);
			}
		}

#elif defined(OS_LINUX)
		DIR* proc_dir = opendir("/proc");
		if (!proc_dir) {
			perror("opendir");
			return processes;
		}

		struct dirent* entry;
		while ((entry = readdir(proc_dir)) != NULL) {
			if (isdigit(entry->d_name[0])) {
				std::string pid_str = entry->d_name;
				std::string comm_path = "/proc/" + pid_str + "/comm";

				std::ifstream comm_file(comm_path.c_str());
				std::string comm;

				if (comm_file.is_open()) {
					std::getline(comm_file, comm);

					comm.erase(std::remove(comm.begin(), comm.end(), '\n'), comm.end());
					processes.push_back(comm);
				}
			}
		}
		closedir(proc_dir);
#endif

		return processes;
	}

} // namespace base::system::utility
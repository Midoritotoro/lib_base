#include <base/core/system/SystemTools.h>

#include <string>
#include <algorithm>

#if defined(OS_WIN)
	#include <tlHelp32.h>
#endif

__BASE_SYSTEM_NAMESPACE_BEGIN

std::vector<std::string> GetAllProcesses() {
	/*std::vector<std::string> processes;
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

	return processes;*/
	return {};
}

__BASE_SYSTEM_NAMESPACE_END

#include <iostream>
#include "sha1.h"
#include "Utility.h"
#include <fstream>
#include <iostream>

static const char* f31704hash = "6D09781426A5C61AED59ADDEC130A8009849E3C7";
static const char* f31703modhash = "F43F16CD4785D974ADD0E9DA08B7C7F523C1538C";

void GetSHA1File(const char* filePath, char* outHash)
{
	FileStream sourceFile;
	if (!sourceFile.Open(filePath)) return;

	SHA1 sha;

	HANDLE handle = sourceFile.GetHandle();

	char buffer[0x400];
	unsigned int offset = 0;
	DWORD length;

	while (!sourceFile.HitEOF())
	{
		ReadFile(handle, buffer, 0x400, &length, NULL);
		offset += length;
		sourceFile.SetOffset(offset);
		sha.addBytes(buffer, length);
	}
	sourceFile.Close();
	unsigned char* digest = sha.getDigest();

	for (byte idx = 0; idx < 0x14; idx++, outHash += 2)
		sprintf_s(outHash, 3, "%02X", digest[idx]);
}

int main()
{
    char outHash[0x29];
	char filename[MAX_PATH];
	char commandline[MAX_PATH];

	GetModuleFileNameA(NULL, filename, MAX_PATH);
	std::string::size_type pos = std::string(filename).find_last_of("\\/");
	std::string path = std::string(filename).substr(0, pos);
	const char* cPath = path.c_str();
	std::string exe_path = path + "\\Fallout3.exe";
	std::string backup_path = path + "\\Fallout3_backup.exe";

    GetSHA1File(exe_path.c_str(), outHash);

	if (strcmp(outHash, f31704hash) == 0) {

		std::cout << "Hash checks completed.\n";

		std::ifstream input(exe_path, std::ios::binary);
		std::ofstream output(backup_path, std::ios::binary);
		std::copy(
			std::istreambuf_iterator<char>(input),
			std::istreambuf_iterator<char>(),
			std::ostreambuf_iterator<char>(output));
		input.close();
		output.close();
		std::cout << "\nBackup created.\n\n";

		sprintf(commandline, "%s\\xdelta3.exe -v -d -f -s \"%s\\Fallout3_backup.exe\" \"%s\\patch.vcdiff\" \"%s\\Fallout3.exe\"", cPath, cPath, cPath, cPath);
		system(commandline);

		GetSHA1File(exe_path.c_str(), outHash);

		if (strcmp(outHash, f31703modhash) == 0) {

			std::cout << "\nDowngrading completed successfully.\n";
			system("@pause");

		}
	}
	else if (strcmp(outHash, f31703modhash) == 0) {

		std::cout << "The game is already downgraded.\n";
		system("@pause");

	}
	else {
		std::cout << "Invalid executable\n";
		system("@pause");
	}
	
}


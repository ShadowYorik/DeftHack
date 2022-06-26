#include <Windows.h>

#include <filesystem>
#include <fstream>

#include "Logging.h"
#include "DeftHackImage.h"
#include "PatchedGameImage.h"
#include "OriginalGameImage.h"


bool getUnturnedDirectory(std::filesystem::path& unturnedDirectory)
{
	char buffer[MAX_PATH]{};
	DWORD bufferSize = sizeof(buffer);
	uint32_t errorCode = RegGetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Steam App 304930", "InstallLocation", RRF_RT_REG_SZ, nullptr, buffer, &bufferSize);
	if (errorCode != ERROR_SUCCESS)
	{
		LOG_ERROR_WITH_CODE("Unable to read unturned directory path from registry!", errorCode);
		return false;
	}

	unturnedDirectory = buffer;
	return true;
}

bool setupDeftHack(const std::filesystem::path& patchedGameImagePath, const std::filesystem::path& deftHackImagePath)
{
	std::ofstream patchedGameImage(patchedGameImagePath, std::ofstream::binary);
	if (!patchedGameImage)
	{
		LOG_ERROR("Unable to create original game image!");
		return false;
	}

	patchedGameImage.write(reinterpret_cast<const char*>(patchedGameImage_raw), sizeof(patchedGameImage_raw));

	std::ofstream deftHackImage(deftHackImagePath, std::ofstream::binary);
	if (!deftHackImage)
	{
		LOG_ERROR("Unable to create deft hack image!");
		return false;
	}

	deftHackImage.write(reinterpret_cast<const char*>(deftHackImage_raw), sizeof(deftHackImage_raw));
	return true;
}

bool removeDeftHack(const std::filesystem::path& originalGameImagePath, const std::filesystem::path& deftHackImagePath)
{
	std::ofstream originalGameImage(originalGameImagePath, std::ofstream::binary);
	if (!originalGameImage)
	{
		LOG_ERROR("Unable to create original game image!");
		return false;
	}

	originalGameImage.write(reinterpret_cast<const char*>(originalGameImage_raw), sizeof(originalGameImage_raw));

	std::error_code stdErrorCode;
	if (!std::filesystem::remove(deftHackImagePath, stdErrorCode))
	{
		LOG_ERROR_WITH_CODE("Unable to remove deft hack image!", stdErrorCode.value());
		return false;
	}

	return true;
}

int main()
{
	std::filesystem::path unturnedDirectory;
	if (!getUnturnedDirectory(unturnedDirectory))
	{
		std::cout << "Enter the unturned directory path: ";
		std::cin >> unturnedDirectory;
	}

	std::error_code stdErrorCode;
	std::filesystem::path tempDirectory = std::filesystem::temp_directory_path();
	if (stdErrorCode.value() != 0)
	{
		LOG_ERROR_WITH_CODE("Unable to get temp directory path!", stdErrorCode.value());
		system("pause");
		return 0;
	}

	std::filesystem::path gameImagePath = unturnedDirectory / "Unturned_Data\\Managed\\UnityEngine.TextRenderingModule.dll";
	std::filesystem::path deftHackImagePath = tempDirectory / "88bbecc3-9a33-4164-b07a-100ce6e1444n.tmp";

	uint32_t optionNumber;
	std::cout << "DeftHack options:" << std::endl << "1. Setup" << std::endl << "2. Remove" << std::endl << std::endl << ">> ";
	std::cin >> optionNumber;
	std::cout << std::endl;

	switch (optionNumber)
	{
	case 1:
		if (setupDeftHack(gameImagePath, deftHackImagePath))
		{
			LOG_INFO("Setup completed!");
			LOG_INFO("Please, start unturned!");
		}

		break;

	case 2:
		if (removeDeftHack(gameImagePath, deftHackImagePath))
		{
			LOG_INFO("Removing completed!");
		}

		break;

	default:
		LOG_ERROR("Unknown option!");
		break;
	}

	system("pause");
	return 0;
}

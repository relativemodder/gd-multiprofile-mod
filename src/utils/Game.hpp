#pragma once

#include <Geode/Geode.hpp>
#include "Profiles.hpp"
#include <filesystem>
#include <string>

using namespace geode::prelude;

namespace profilesutils {

	class GameAsync {
	public:
		void trySaveAsync(float dt) {
			AppDelegate::sharedApplication()->trySaveGame();
		}
	};

	void restartGame() {
		STARTUPINFO si;
		PROCESS_INFORMATION pi;

		ZeroMemory(&si, sizeof(si));
		si.cb = sizeof(si);
		ZeroMemory(&pi, sizeof(pi));

		CreateProcess(NULL,   // No module name (use command line)
			LPSTR("GeometryDash.exe"),        // Command line
			NULL,           // Process handle not inheritable
			NULL,           // Thread handle not inheritable
			FALSE,          // Set handle inheritance to FALSE
			0,              // No creation flags
			NULL,           // Use parent's environment block
			NULL,           // Use parent's starting directory 
			&si,            // Pointer to STARTUPINFO structure
			&pi);

		exit(0);
	}

	void loadIntoNew(std::string profileToLoad, bool isNew = false) {
		AppDelegate::sharedApplication()->trySaveGame();

		std::string username_old = readCurrentProfile();
		std::string saves_path = profilesutils::getSaveDirectoryPath();

		std::string CCGameManager_path_old = saves_path + std::string("\\CCGameManager.dat");
		std::string CCLocalLevels_path_old = saves_path + std::string("\\CCLocalLevels.dat");

		std::string CCGameManager_path_template = saves_path + std::string("\\GDProfilesTemplate\\CCGameManager.dat");
		std::string CCLocalLevels_path_template = saves_path + std::string("\\GDProfilesTemplate\\CCLocalLevels.dat");

		std::string profiles_path = profilesutils::getProfilesDirectoryPath();
		std::string profile_new_dir_path = profiles_path + std::string("\\") + profileToLoad;
		std::string profile_load_dir_path = profiles_path + std::string("\\") + username_old;

		std::string CCGameManager_path_new = profile_new_dir_path + std::string("\\CCGameManager.dat");
		std::string CCLocalLevels_path_new = profile_new_dir_path + std::string("\\CCLocalLevels.dat");

		std::string CCGameManager_path_to_save = profile_load_dir_path + std::string("\\CCGameManager.dat");
		std::string CCLocalLevels_path_to_save = profile_load_dir_path + std::string("\\CCLocalLevels.dat");

		log::info("Checking if it's first time of saving profile");

		bool exists = dirExists(profile_load_dir_path);
		bool exists1 = dirExists(profile_new_dir_path);

		if (!exists) {
			log::info("{}", "Save Profile directory doesn't exist. Creating...");
			CreateDirectory(profile_load_dir_path.c_str(), NULL);
		}

		if ((!exists1) && isNew) {
			log::info("{}", "New Profile directory doesn't exist. Creating...");
			CreateDirectory(profile_new_dir_path.c_str(), NULL);
		}

		log::info("Saving old profile to its own folder");

		std::filesystem::copy_file(CCGameManager_path_old, CCGameManager_path_to_save, std::filesystem::copy_options::overwrite_existing);
		std::filesystem::copy_file(CCLocalLevels_path_old, CCLocalLevels_path_to_save, std::filesystem::copy_options::overwrite_existing);


		if (isNew) {
			std::filesystem::copy_file(CCGameManager_path_template, CCGameManager_path_new, std::filesystem::copy_options::overwrite_existing);
			std::filesystem::copy_file(CCLocalLevels_path_template, CCLocalLevels_path_new, std::filesystem::copy_options::overwrite_existing);
		}


		log::info("Removing old save files");

		std::filesystem::remove(CCGameManager_path_old);
		std::filesystem::remove(CCLocalLevels_path_old);

		log::info("Copying files from new profile to GD saves folder");

		std::filesystem::copy_file(CCGameManager_path_new, CCGameManager_path_old, std::filesystem::copy_options::overwrite_existing);
		std::filesystem::copy_file(CCLocalLevels_path_new, CCLocalLevels_path_old, std::filesystem::copy_options::overwrite_existing);
		

		writeCurrentProfile(profileToLoad);

		restartGame();
	}
}
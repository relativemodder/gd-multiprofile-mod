#pragma once

#include <filesystem>
#include <windows.h>
#include <vector>
#include <string>
#include <Geode/Geode.hpp>

using namespace geode::prelude;

namespace profilesutils {

    void replaceAll(std::string& str, const std::string& from, const std::string& to) {
        if (from.empty())
            return;
        size_t start_pos = 0;
        while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
            str.replace(start_pos, from.length(), to);
            start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
        }
    }

    bool dirExists(const std::string& dirName_in)
    {
        DWORD ftyp = GetFileAttributesA(dirName_in.c_str());
        if (ftyp == INVALID_FILE_ATTRIBUTES)
            return false;

        if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
            return true;

        return false;
    }

    std::string getSaveDirectoryPath() {
        std::string appdata = std::string(getenv("LOCALAPPDATA"));
        appdata += "\\GeometryDash";

        log::info("{}", appdata);

        return appdata;
    }

    std::string getProfilesDirectoryPath() {
        std::string save_dir = getSaveDirectoryPath();
        save_dir += "\\GDProfiles";

        return save_dir;
    }

    std::vector<std::string> tokenize(std::string const& str, const char delim){
        std::vector<std::string> out{};
        std::stringstream ss(str);

        std::string s;
        while (std::getline(ss, s, delim)) {
            out.push_back(s);
        }
        return out;
    }
    
    std::vector<std::string> getProfileList(std::string dir_path) {
        std::vector<std::string> profile_list {};

        for (const auto& entry : std::filesystem::directory_iterator(dir_path)) {
            std::string path_to_profile = entry.path().relative_path().generic_string();
            auto tokens = tokenize(path_to_profile, '/');

            auto profile = tokens.back();
            log::info("{}", profile);

            profile_list.push_back(profile);
        }

        return profile_list;
    }

    std::string checkDirExistsOrCreate() {
        std::string profiles_dir = getProfilesDirectoryPath();

        log::info("Fetching the directory {}", profiles_dir);

        bool exists = dirExists(profiles_dir);

        if (!exists) {
            log::info("{}", "Profiles directory doesn't exist. Creating...");
            CreateDirectory(profiles_dir.c_str(), NULL);
        }

        return profiles_dir;
    }

    void writeStringToFile(std::string path, std::string content) {
        std::ofstream ofs; // output file stream

        ofs.open(path, std::ios::out | std::ios::trunc);

        if (ofs)
        {
            ofs << content.c_str();
            ofs.close();
        }
    }

    std::string readStringFromFile(std::string path) {
        std::ifstream f(path);
        std::string str;

        if (f) {
            std::ostringstream ss;
            ss << f.rdbuf();
            str = ss.str();

            return str;
        }

        return "Unable to read.";

    }

    std::string readCurrentProfile() {
        std::string profileFilePath = getSaveDirectoryPath();
        profileFilePath += "\\profile.txt";

        std::string profileContents;

        if (std::filesystem::exists(profileFilePath)) {
            profileContents = readStringFromFile(profileFilePath);
        }
        else {
            profileContents = GameManager::sharedState()->m_playerName;
        }

        return profileContents;
    }

    void writeCurrentProfile(std::string profile) {
        std::string profileFilePath = getSaveDirectoryPath();
        profileFilePath += "\\profile.txt";
        
        writeStringToFile(profileFilePath, profile);
    }

    void renameProfile(std::string oldName, std::string newName) {
        std::string current = readCurrentProfile();
        std::string old_folder = getProfilesDirectoryPath() + "\\" + oldName;
        std::string new_folder = getProfilesDirectoryPath() + "\\" + newName;

        std::filesystem::rename(old_folder, new_folder);

        if (oldName == current) { // Need to change the name in profile.txt file to work properly
            profilesutils::writeCurrentProfile(newName);
        }
    }

    void createTemplates() {
        std::string saves_path(profilesutils::getSaveDirectoryPath());
        std::string templates_dir(saves_path + "\\GDProfilesTemplate");

        bool exists = dirExists(templates_dir);

        if (!exists) {
            log::info("{}", "Profiles templates directory doesn't exist. Creating...");
            CreateDirectory(templates_dir.c_str(), NULL);
        }
        unsigned long bufferSize = 0;

        unsigned char* gm_template_contents = CCFileUtils::sharedFileUtils()->getFileData("CCGameManager.dat"_spr, "r", &bufferSize);
        unsigned char* ll_template_contents = CCFileUtils::sharedFileUtils()->getFileData("CCLocalLevels.dat"_spr, "r", &bufferSize);

        std::ofstream gm_wf(templates_dir + "\\CCGameManager.dat", std::ios::out | std::ios::binary);
        gm_wf << gm_template_contents;
        gm_wf.close();

        std::ofstream ll_wf(templates_dir + "\\CCLocalLevels.dat", std::ios::out | std::ios::binary);
        ll_wf << ll_template_contents;
        ll_wf.close();


    }

}
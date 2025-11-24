#include "ScoreManager.h"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <map>
#include <algorithm>

const std::string ScoreManager::LEADERBOARD_FILE = "leaderboard.txt";

void ScoreManager::updateOrAddScore(const std::string& username, const std::string& difficulty, int timeInSeconds) {
    std::map<std::pair<std::string, std::string>, int> scoreMap;

    std::ifstream inFile(LEADERBOARD_FILE);
    if (inFile.is_open()) {
        std::string line;
        while (std::getline(inFile, line)) {
            std::istringstream ss(line);
            std::string user, diff, timeStr;
            if (std::getline(ss, user, ',') &&
                std::getline(ss, diff, ',') &&
                std::getline(ss, timeStr)) {
                int time = std::stoi(timeStr);
                auto key = std::make_pair(user, diff);

                if (scoreMap.find(key) == scoreMap.end() || time < scoreMap[key]) {
                    scoreMap[key] = time;
                }
            }
        }
        inFile.close();
    }

    auto key = std::make_pair(username, difficulty);
    if (scoreMap.find(key) == scoreMap.end() || timeInSeconds < scoreMap[key]) {
        scoreMap[key] = timeInSeconds;
    }

    std::ofstream outFile(LEADERBOARD_FILE);
    if (outFile.is_open()) {
        for (const auto& entry : scoreMap) {
            outFile << entry.first.first << ","
                << entry.first.second << ","
                << entry.second << "\n";
        }
        outFile.close();
    }
}

std::vector<std::pair<std::string, int>> ScoreManager::getScoresForDifficulty(const std::string& difficulty) {
    std::vector<std::pair<std::string, int>> scores;

    std::ifstream file(LEADERBOARD_FILE);
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            std::istringstream ss(line);
            std::string user, diff, timeStr;
            if (std::getline(ss, user, ',') &&
                std::getline(ss, diff, ',') &&
                std::getline(ss, timeStr)) {
                if (diff == difficulty) {
                    int time = std::stoi(timeStr);
                    scores.push_back(std::make_pair(user, time));
                }
            }
        }
        file.close();
    }

    std::sort(scores.begin(), scores.end(),
        [](const auto& a, const auto& b) {
            return a.second < b.second;
        });

    return scores;
}

std::string ScoreManager::formatTime(int seconds) {
    int mins = seconds / 60;
    int secs = seconds % 60;
    std::ostringstream oss;
    oss << std::setfill('0') << std::setw(2) << mins << ":"
        << std::setfill('0') << std::setw(2) << secs;
    return oss.str();
}
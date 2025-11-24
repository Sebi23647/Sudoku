#pragma once
#include <string>
#include <vector>
#include <utility>

class ScoreManager {
public:
    static void updateOrAddScore(const std::string& username, const std::string& difficulty, int timeInSeconds);
    static std::vector<std::pair<std::string, int>> getScoresForDifficulty(const std::string& difficulty);
    static std::string formatTime(int seconds);

private:
    static const std::string LEADERBOARD_FILE;
};
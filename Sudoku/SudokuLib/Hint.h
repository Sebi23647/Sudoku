//#pragma once
//#include <vector>
//#include <string>
//#include <optional>
//
//enum class HintType {
//    NakedSingle,
//    HiddenSingle,
//    CandidateList,
//    SafeCell,
//    LocalReasoning
//};
//
//struct Hint {
//    int row = -1;
//    int col = -1;
//    int value = 0; // 0 if no direct value suggested (soft hint)
//    HintType type = HintType::CandidateList;
//    std::vector<int> candidates; // optional list of candidates for soft hints
//    std::string message; // human-readable explanation
//};


#pragma once
#include <vector>
#include <string>

enum class HintType {
    NakedSingle,
    HiddenSingle,
    CandidateList,
    SafeCell,
    LocalReasoning
};

struct Hint {
    int row = -1;
    int col = -1;
    int value = 0; // 0 if no direct value suggested (soft hint)
    HintType type = HintType::CandidateList;
    std::vector<int> candidates; // optional list of candidates for soft hints (numbers)
    std::string message; // human-readable explanation
};
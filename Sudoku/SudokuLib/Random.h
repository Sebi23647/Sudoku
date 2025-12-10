#pragma once
#pragma once
#include <random>
#include <chrono>

// Mic helper: un engine global (seed-uit o singur? data).
// Folose?te Random::getEngine() pentru toate opertaiile de shuffle/rng.
namespace Random {
    inline std::mt19937& getEngine() {
        static std::mt19937 engine{ static_cast<std::mt19937::result_type>(
            std::random_device{}() ^ (static_cast<std::mt19937::result_type>(
                std::chrono::high_resolution_clock::now().time_since_epoch().count()))
        ) };
        return engine;
    }
}
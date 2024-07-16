#pragma once

#include "DataBase.h"

DataBase::DataBase(const std::vector<Agent>& data) {
    for (const auto& i : data) {
        array_A.push_back({ i.id, i.A });
        array_B.push_back({ i.id, i.B });
        result[i.id] = 0;
    }
}

std::unordered_map<int, bool> DataBase::verdict() {
    std::sort(array_A.begin(), array_A.end(), [](const auto& a, const auto& b) {
        return a.second < b.second;
        });
    for (int i = 0; i < 5; ++i) {
        result[array_A[i].first] = 1;
    }

    /*std::cout << std::endl;
    for (const auto& i : array_A) {
        std::cout << i.first << " - " << i.second << std::endl;
    }
    std::cout << std::endl;*/

    std::sort(array_B.begin(), array_B.end(), [](const auto& a, const auto& b) {
        return a.second > b.second;
        });
    for (int i = 0; i < 5; ++i) {
        result[array_B[i].first] = 1;
    }

    /*for (const auto& i : array_B) {
        std::cout << i.first << " - " << i.second << std::endl;
    }
    std::cout << std::endl;*/

    db_log("../data/log.csv", result);
    return result;
}
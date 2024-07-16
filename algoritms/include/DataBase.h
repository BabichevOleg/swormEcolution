#pragma once


#include "Logger.h"

#include <algorithm>
#include <stdexcept>



class DataBase {
private:
    std::vector<std::pair<int, double>> array_A;
    std::vector<std::pair<int, double>> array_B;

    void save_middle_result(bool is_A);
    std::unordered_map<int, bool> result;

public:

    DataBase(const std::vector<Agent>& data);
    std::unordered_map<int, bool> verdict();
};


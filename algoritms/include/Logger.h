#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>
#include <vector>
#include <iomanip>

const std::string FILE_PATH = "../data/log.csv";
const std::string TEST_FILE_PATH = "../algoritm/test/log.csv";

struct Agent {
    int id;
    double A;
    double B;
};

void db_log(const std::string& filepath, const std::unordered_map<int, bool>& data);
void server_log(const std::string& filepath, const std::vector<Agent>& data);
void main_log(const std::string& filepath, size_t iter_number);






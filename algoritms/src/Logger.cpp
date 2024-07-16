#pragma once

#include "Logger.h"

void db_log(const std::string& filepath, const std::unordered_map<int, bool>& data) {
    std::ofstream file(filepath, std::ios::app);

    if (!file.eof()) {
        file << std::endl;
    }

    std::stringstream csv;
    for (const auto& iter : data) {
        csv  << std::setw(10) << iter.first << " | "
             << std::setw(10) << iter.second << std::endl;
    }

    file << csv.str();
    file.close();
}


void server_log(const std::string& filepath, const std::vector<Agent>& data) {
    std::ofstream file(filepath, std::ios::app);

    if (!file.eof()) {
        file << std::endl;
    }

    std::stringstream csv;
    csv << std::setw(20) << "id" << " | "
        << std::setw(20) << "A value" << " | "
        << std::setw(20) << "B value" << std::endl;

    for (const auto& iter : data) {
        csv << std::setw(20) << iter.id << " | "
            << std::setw(20) << iter.A << " | "
            << std::setw(20) << iter.B << std::endl;
    }

    file << csv.str();
    file.close();
}

void main_log(const std::string& filepath, size_t iter_number) {

    std::ifstream tmp_file("../data/data.csv");
    if (!tmp_file.is_open()) throw std::logic_error("Open file error");

    // Создание векторов для хранения данных
    std::vector<double> A;
    std::vector<double> B;
    std::string line;

    while (std::getline(tmp_file, line)) {
        std::stringstream ss(line);
        std::string data;
        while (std::getline(ss, data, '|')) {
            A.push_back(std::stod(data));
        }

        getline(tmp_file, line);
        ss.str(line);
        ss.clear();
        while (std::getline(ss, data, '|')) {
            B.push_back(std::stod(data));
        }
    }

    tmp_file.close();

    std::ofstream file(filepath, std::ios::app);

    if (!file.eof()) {
        file << std::endl;
    }

    std::stringstream csv;
    csv << "iteration number: " << iter_number << std::endl << std::endl;
    for (const auto& iter : A) {
        csv << std::setw(5) << iter << " | ";
    }
    csv << std::endl;

    for (const auto& iter : B) {
        csv << std::setw(5) << iter << " | ";
    }
    csv << std::endl;
    

    file << csv.str();
    file.close();
}

#pragma once

#include "Evolution.h"

void fill(std::vector<double>& v,const bool& is) {
    std::sort(v.begin(), v.end(), [&](const auto& a, const auto& b) {
        return is ? a < b : a > b;
        });
    auto iter = v.begin();
    auto i = v.end() - 2;
    auto j = v.end() - 1;

    while (*iter != 0) iter++;
    while (*iter == 0) {
        *iter = is ? (*i + *j) / 2 + 0.01 : (*i + *j) / 2 - 0.01;
        iter++;
    }
}


void evolution() {
    std::ifstream file("../data/evolution_values.csv");
    if (!file.is_open()) throw std::logic_error("Open file error");

    std::vector<double> A;
    std::vector<double> B;
    std::string line;

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string v_A, v_B;
        std::getline(ss, v_A, '|');
        std::getline(ss, v_B, '|');

        if (v_A != " ") {
            A.push_back(std::stod(v_A));
        }
        else {
            A.push_back(0);
        }
        if (v_B != " ") {
            B.push_back(std::stod(v_B));
        }
        else {
            B.push_back(0);
        }
    }
    file.close();
    std::ofstream file_("../data/evolution_values.csv", std::ios::trunc);
    if (!file_.is_open()) throw std::logic_error("Open file error");
    file_.close();

    while (A.size() < 20) {
        A.push_back(0);
    }
    while (B.size() < 20) {
        B.push_back(0);
    }

    fill(A, 1);
    fill(B, 0);

    std::ofstream main_file("../data/data.csv");
    if (!main_file.is_open()) throw std::logic_error("Open file error");

    std::stringstream csv;
    for (const auto& iter : A) {
        csv << std::setw(5) << iter << " | ";
    }
    if (!csv.str().empty()) {
        csv.seekp(-3, std::ios_base::end);
        csv << "  ";
    }
    csv << std::endl;

    for (const auto& iter : B) {
        csv << std::setw(5) << iter << " | ";
    }
    if (!csv.str().empty()) {
        csv.seekp(-3, std::ios_base::end);
        csv << "  ";
    }
    main_file << csv.str();
    main_file.close();
}
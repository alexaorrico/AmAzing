#include "../include/Layout.hpp"
#include <iostream>
#include <fstream>

Layout::Layout(std::string filename, Vector2d &pos) {

    std::string line;
    std::ifstream map_file(filename);
    if (map_file.is_open()) {
        rows = 0;
        columns = 0;
        bool start_exists = false;
        while (std::getline(map_file, line)) {
            rows++;
            std::vector<int> row;
            uint32_t i;
            for (i = 0; line[i] != '\n' && line[i] != '\0'; i++) {
                if (!start_exists && line[i] == '0') {
                    pos(0) = rows - 1;
                    pos(1) = i;
                    start_exists = true;
                }
                if (line[i] == 'x') {
                    start_exists = true;
                    pos(0) = rows - 1;
                    pos(1) = i;
                }
                if (!std::isdigit(line[i])) {
                    line[i] = '0';
                }
                row.push_back(line[i] - '0');
            }
            if (i > columns)
                columns = i;
            map.push_back(row);
        }
        map_file.close();
        if (!start_exists) {
            std::cout << "No valid start location possible! Check your map!!" << std::endl;
            exit(EXIT_FAILURE);
        }
        for (uint32_t i = 0; i < this->rows; i++) {
            map[i].resize(columns, 0);
            map[i].insert(map[i].begin(), 1);
            map[i].push_back(1);
        }
        rows += 2;
        columns += 2;
        pos(0) += 1.5;
        pos(1) += 1.5;
        map.insert(map.begin(), std::vector<int>(columns, 1));
        map.push_back(std::vector<int>(columns, 1));
    } else {
        std::cout << "Could not open file: " << filename << std::endl;
        exit(EXIT_FAILURE);
    }
}

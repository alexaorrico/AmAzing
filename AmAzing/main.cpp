#include "include/app.hpp"
#include <iostream>

int main(int argc, const char * argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <map_filename>" << std::endl;
        exit(EXIT_FAILURE);
    }
    App app;
    app.run(argv[1]);
    return 0;
}


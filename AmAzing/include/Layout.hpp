#ifndef Layout_hpp
#define Layout_hpp

#include <vector>
#include <string>
#include <Dense>

using namespace Eigen;

class Layout {
public:
    Layout(std::string filename, Vector2d &pos);
    std::vector<std::vector<int>> map;
    uint32_t columns;
    uint32_t rows;
};

#endif /* Layout_hpp */

#include <iostream>

#include <iostream>
#include <vector>

using map = std::vector<std::vector<bool>>;

int count_trees(map &m, int x, int y)
{
    const int width = m[0].size();
    const int y_size = m.size();

    int num_trees = 0;
    int y_pos = 1;
    int x_pos = 0;

    for(; y_pos<y_size; y_pos+=y) {
        x_pos = (x_pos + x) % width;
        num_trees += !!(m[y_pos][x_pos]);
    }

    return num_trees;
}

int main()
{
    int i = 0;
    std::string line;
    map m;

    while(std::getline(std::cin, line)) {
        m.emplace_back();
        for(auto &c : line) {
            m[i].emplace_back(c=='#');
        }

        ++i;
    }

    std::cout << "number of trees: " << count_trees(m, 3, 1) << std::endl;
    return 0;
}


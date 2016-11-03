//
//  tests.cpp
//  Avalgo-tsp
//
//  Created by Edvard Ahlsén on 31/10/16.
//  Copyright © 2016 Edvard Ahlsén. All rights reserved.
//

#include "tests.hpp"
#include <cassert>

namespace tsp {
    bool validateTour(std::vector<int> &tour, tsp::instance &map) {
        std::vector<bool> visited = std::vector<bool>(map.size, false);
        for (int i = 0; i<tour.size(); i++) {
            if (visited[tour[i]]) {
                std::cerr << "Invalid tour: " << i << " visited more than once\n";
                exit(1);
            }
            visited[tour[i]] = true;
        }
        assert(tour.size() == map.size);
        if (tour.size() != map.size) {
            exit(1);
        }
        return false;
    }
    
    bool validateEdges(tsp::instance &map) {
        for (int i = 0; i < map.size; i++) {
            for (int j = 0; j < map.size; j++) {
                if (map.D[i][j] != map.D[j][i]) {
                    return false;
                }
            }
        }
        return true;
    }
}
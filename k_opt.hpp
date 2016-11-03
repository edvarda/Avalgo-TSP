//
//  k_opt.hpp
//  Avalgo-tsp
//
//  Created by Edvard Ahlsén on 01/11/16.
//  Copyright © 2016 Edvard Ahlsén. All rights reserved.
//

#ifndef k_opt_hpp
#define k_opt_hpp

#include <stdio.h>
#include <chrono>
#include "instance.hpp"
#include "tests.hpp"

namespace tsp {
    int getWeight(tsp::instance &map, std::vector<int> &tour);
    
    void two_opt(tsp::instance &map, std::vector<int> &tour);
    
    void fast_two_opt(tsp::instance &map, std::vector<int> &tour);
    void fast_three_opt(tsp::instance &map, std::vector<int> &tour,
                        const std::chrono::time_point<std::chrono::high_resolution_clock>& deadline);
    
    void swap_two(tsp::instance &map, std::vector<int> &tour, int begin, int end);
    
    void updateMaxLink(tsp::instance &map, std::vector<int> &tour);
    void updateWhichSlot(tsp::instance &map, std::vector<int> &tour);
    void tourOrderEdges(tsp::instance &map, std::vector<int> &tour,
                        int& u, int& v, int& w, int& z, int& x, int& y,
                        int& k, int& l, int& m, int& n, int& o, int& p);
}
#endif /* k_opt_hpp */

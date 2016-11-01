//
//  twoopt.hpp
//  Avalgo-tsp
//
//  Created by Edvard Ahlsén on 29/10/16.
//  Copyright © 2016 Edvard Ahlsén. All rights reserved.
//

#ifndef twoopt_hpp
#define twoopt_hpp

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <algorithm>
#include <sys/time.h>
#include <iostream>
#include <fstream>
#include "instance.hpp"

#endif /* twoopt_hpp */
namespace tsp {

    void twoopter(std::vector<int> &tour, instance map, long startTime);
    std::vector<int> twooptswap(std::vector<int> &tour, int &i, int &k);
    int evaluateTwoOpt(std::vector<int> &tour, tsp::instance &map, int &city1, int &city2);
}

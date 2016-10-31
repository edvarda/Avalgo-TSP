//
//  simann.hpp
//  Avalgo-tsp
//
//  Created by Edvard Ahlsén on 29/10/16.
//  Copyright © 2016 Edvard Ahlsén. All rights reserved.
//

#ifndef simann_hpp
#define simann_hpp

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <algorithm>
#include <sys/time.h>
#include <iostream>
#include <fstream>
#include "instance.hpp"

#endif /* simann_hpp */
namespace tsp {

    void sa(std::vector<int> &tour, instance map, long startTime);
    int getCurrTime();
    void reverse(std::vector<int> &tour, int city1, int city2);
    int evaluate(std::vector<int> &tour, tsp::instance &map, int &city1, int &city2);
}

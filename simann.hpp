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
#include <vector>
#include <algorithm>
#include <sys/time.h>
#include "instance.hpp"

#endif /* simann_hpp */
namespace tsp {

    void sa(std::vector<int> tour, instance map, long startTime);
    int getCurrTime();
}

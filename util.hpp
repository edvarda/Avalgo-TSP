//
//  util.cpp
//  Avalgo-tsp
//
//  Created by Edvard Ahlsén on 03/11/16.
//  Copyright © 2016 Edvard Ahlsén. All rights reserved.
//
#ifndef util_hpp
#define util_hpp


#include <stdio.h>
#include "instance.hpp"
namespace tsp {

    void printTour(std::vector<int> &tour) { // TODO just for testing early
        for (int i = 0; i < tour.size(); i++) {
            std::cout << tour[i] << std::endl;
        }
    }

}

#endif /* util_hpp */
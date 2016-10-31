//
//  tests.hpp
//  Avalgo-tsp
//
//  Created by Edvard Ahlsén on 31/10/16.
//  Copyright © 2016 Edvard Ahlsén. All rights reserved.
//

#ifndef tests_hpp
#define tests_hpp
#include <vector>
#include "instance.hpp"
namespace tsp {
    bool validateTour(std::vector<int> &tour, tsp::instance &map);
}
#endif /* tests_hpp */



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
#include "instance.hpp"
#include "tests.hpp"

namespace tsp {
    void two_opt(tsp::instance &map, std::vector<int> &tour);
    
    void fast_two_opt(tsp::instance &map, std::vector<int> &tour);
}
#endif /* k_opt_hpp */

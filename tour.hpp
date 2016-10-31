//
//  tour.hpp
//  Avalgo-tsp
//
//  Created by Edvard Ahlsén on 28/10/16.
//  Copyright © 2016 Edvard Ahlsén. All rights reserved.
//

#ifndef tour_hpp
#define tour_hpp

#include <iostream>
#include <vector>

namespace tsp {
    class tour {
        
    public:
        struct stop {
            int location;
            stop * prev;
            stop * next;
        };
        
        tour(size_t max) : max_size(max), size(0), stops(std::vector<stop>(max)) {}
        
        
    private:
        std::vector<int>;
        size_t max_size;
        size_t size;
        std::vector<stop> stops;
        
    };
}


#endif /* tour_hpp */

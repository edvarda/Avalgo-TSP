//
//  instance.hpp
//  Avalgo-tsp
//
//  Created by Edvard Ahlsén on 28/10/16.
//  Copyright © 2016 Edvard Ahlsén. All rights reserved.
//

#ifndef instance_hpp
#define instance_hpp

#include <iostream>
#include <vector>
#include <unordered_map>
#include "point.hpp"


namespace tsp {
    class instance {
        
    public:
        instance(size_t n) :    size(n),
                                cities(std::vector<point>(n)),
                                distances(std::vector<std::vector<int> >(n)),
                                nbhd(std::vector<std::vector<int>>(n)) {}
        
        void readCities(std::istream& in);
        void computeDistances();
        void nneighbour(std::vector<int> &tour);
        std::vector<std::vector<int>> distances;
        std::vector<std::vector<int>> nbhd;
        size_t size;
        std::vector<point> cities;
        int min_link, max_link;
        
    private:
        
        
    };
}

#endif /* instance_hpp */

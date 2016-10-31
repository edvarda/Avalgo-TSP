//
//  christofides.hpp
//  Avalgo-tsp
//
//  Created by Edvard Ahlsén on 31/10/16.
//  Copyright © 2016 Edvard Ahlsén. All rights reserved.
//

#ifndef christofides_hpp
#define christofides_hpp
#include "kruskal.hpp"

#endif /* christofides_hpp */

namespace tsp {

    void christofides(tsp::instance &map, std::vector<int> &tour);
    void matchOdds(std::vector<tsp::edge> *mst, tsp::instance &map);
    int getNeighbour(std::vector<std::vector<bool>> edges, int v);
    void getHamiltonTour(std::vector<tsp::edge> *mst, tsp::instance &map, std::vector<int> &tour);
}
//
// Created by pierfied on 11/20/18.
//

#ifndef STUPID_CIC_GRID_H
#define STUPID_CIC_GRID_H


#include "grid.h"

class cic_grid : public grid {
public:
    cic_grid(int nx, int ny, int nz, particle_list &plist) : grid(nx, ny, nz, plist) {}

    void populate_mass_grid() override;
};


#endif //STUPID_CIC_GRID_H

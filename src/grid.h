//
// Created by pierfied on 11/19/18.
//

#ifndef STUPID_GRID_H
#define STUPID_GRID_H


#include <iostream>
#include <fftw3.h>
#include "multidim_array.h"
#include "particle_list.h"
#include "cosmology.h"
#include <math.h>
#include <omp.h>

class grid {
public:
    const long n;

    particle_list *plist;

    const double avg_particle_density;

    cosmology cosmo;

    array_3d<double> real_grid;
    array_3d<fftw_complex> fourier_grid;

    fftw_plan forward_plan;
    fftw_plan backward_plan;

    grid(long n, particle_list &plist, cosmology cosmo) :
            n(n), real_grid(n, n, n), fourier_grid(n, n, (n / 2 + 1)), plist(&plist),
            cosmo(cosmo), avg_particle_density((double) plist.num_particles / (n * n * n)) {
        fftw_init_threads();
        fftw_plan_with_nthreads(omp_get_max_threads());
        forward_plan = fftw_plan_dft_r2c_3d(n, n, n, real_grid.data, fourier_grid.data, FFTW_MEASURE);
        backward_plan = fftw_plan_dft_c2r_3d(n, n, n, fourier_grid.data, real_grid.data, FFTW_MEASURE);
    }

    ~grid() {
        fftw_destroy_plan(forward_plan);
        fftw_destroy_plan(backward_plan);
        fftw_cleanup_threads();
    }

    inline long modulo(long a, long b) {
        return (a % b + b) % b;
    }

    inline double modulo(double a, long b) {
        return fmod(fmod(a, b) + b, b);
    }

    virtual void populate_delta_grid() = 0;

    void fft();

    void ifft();

    void apply_greens_func(double a);

    void compute_potential(double a);

    inline double grid_accel(long i, long j, long k, long dim) {
        long plus_neighbor;
        long minus_neighbor;

        switch (dim) {
            case 0:
                plus_neighbor = (i + 1) % n;
                minus_neighbor = modulo(i - 1, n);

                return -(real_grid(plus_neighbor, j, k) - real_grid(minus_neighbor, j, k)) / 2;
            case 1:
                plus_neighbor = (j + 1) % n;
                minus_neighbor = modulo(j - 1, n);;

                return -(real_grid(i, plus_neighbor, k) - real_grid(i, minus_neighbor, k)) / 2;
            default:
                plus_neighbor = (k + 1) % n;
                minus_neighbor = modulo(k - 1, n);;

                return -(real_grid(i, j, plus_neighbor) - real_grid(i, j, minus_neighbor)) / 2;
        }
    }

    virtual double particle_accel(long particle_ind, long dim) = 0;

    void rebound_positions();
};


#endif //STUPID_GRID_H

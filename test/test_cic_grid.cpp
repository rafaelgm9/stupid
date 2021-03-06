//
// Created by pierfied on 11/21/18.
//

#include <gtest/gtest.h>
#include <multidim_array.h>
#include <cic_grid.h>

TEST(cic_grid, flat_density) {
    long n = 10;

    long num_particles = n * n * n;
    long num_dims = 3;

    array_2d<double> x(num_particles, num_dims);
    array_2d<double> p(num_particles, num_dims);

    long cur_particle = 0;
    for (long i = 0; i < n; ++i) {
        for (long j = 0; j < n; ++j) {
            for (long k = 0; k < n; ++k) {
                x(cur_particle, 0) = i;
                x(cur_particle, 1) = j;
                x(cur_particle, 2) = k;

                cur_particle++;
            }
        }
    }

    particle_list plist(x, p);
    cic_grid grid(n, plist, cosmology(0, 0, 0, 0));

    grid.populate_delta_grid();

    for (long i = 0; i < n; ++i) {
        for (long j = 0; j < n; ++j) {
            for (long k = 0; k < n; ++k) {
                EXPECT_DOUBLE_EQ(0, grid.real_grid(i, j, k));
            }
        }
    }
}

TEST(cic_grid, flat_density_half_offset) {
    long n = 10;

    long num_particles = n * n * n;
    long num_dims = 3;

    array_2d<double> x(num_particles, num_dims);
    array_2d<double> p(num_particles, num_dims);

    long cur_particle = 0;
    for (long i = 0; i < n; ++i) {
        for (long j = 0; j < n; ++j) {
            for (long k = 0; k < n; ++k) {
                x(cur_particle, 0) = i + 0.5;
                x(cur_particle, 1) = j + 0.5;
                x(cur_particle, 2) = k + 0.5;

                cur_particle++;
            }
        }
    }

    particle_list plist(x, p);
    cic_grid grid(n, plist, cosmology(0, 0, 0, 0));

    grid.populate_delta_grid();

    for (long i = 0; i < n; ++i) {
        for (long j = 0; j < n; ++j) {
            for (long k = 0; k < n; ++k) {
                EXPECT_DOUBLE_EQ(0, grid.real_grid(i, j, k));
            }
        }
    }
}

TEST(cic_grid, alternating_density) {
    long n = 10;

    long num_particles = n * n * n;
    long num_dims = 3;

    array_2d<double> x(num_particles, num_dims);
    array_2d<double> p(num_particles, num_dims);

    long cur_particle = 0;
    for (long i = 0; i < n; ++i) {
        for (long j = 0; j < n; ++j) {
            for (long k = 0; k < n; ++k) {
                x(cur_particle, 0) = i - i % 2;
                x(cur_particle, 1) = j - j % 2;
                x(cur_particle, 2) = k - k % 2;

                cur_particle++;
            }
        }
    }

    particle_list plist(x, p);
    cic_grid grid(n, plist, cosmology(0, 0, 0, 0));

    grid.populate_delta_grid();

    for (long i = 0; i < n; ++i) {
        for (long j = 0; j < n; ++j) {
            for (long k = 0; k < n; ++k) {
                if (i % 2 == 0 && j % 2 == 0 && k % 2 == 0) {
                    EXPECT_DOUBLE_EQ(7, grid.real_grid(i, j, k));
                } else {
                    EXPECT_DOUBLE_EQ(-1, grid.real_grid(i, j, k));
                }
            }
        }
    }
}

TEST(cic_grid, fft_recover) {
    long n = 10;

    long num_particles = n * n * n;
    long num_dims = 3;

    array_2d<double> x(num_particles, num_dims);
    array_2d<double> p(num_particles, num_dims);

    long cur_particle = 0;
    for (long i = 0; i < n; ++i) {
        for (long j = 0; j < n; ++j) {
            for (long k = 0; k < n; ++k) {
                x(cur_particle, 0) = i - i % 2;
                x(cur_particle, 1) = j - j % 2;
                x(cur_particle, 2) = k - k % 2;

                cur_particle++;
            }
        }
    }

    particle_list plist(x, p);
    cic_grid grid(n, plist, cosmology(0, 0, 0, 0));

    grid.populate_delta_grid();

    grid.fft();
    grid.real_grid.reset_zero();
    grid.ifft();

    for (long i = 0; i < n; ++i) {
        for (long j = 0; j < n; ++j) {
            for (long k = 0; k < n; ++k) {
                if (i % 2 == 0 && j % 2 == 0 && k % 2 == 0) {
                    EXPECT_DOUBLE_EQ(7, grid.real_grid(i, j, k));
                } else {
                    EXPECT_DOUBLE_EQ(-1, grid.real_grid(i, j, k));
                }
            }
        }
    }
}

TEST(cic_grid, flat_potential) {
    long n = 10;

    long num_particles = n * n * n;
    long num_dims = 3;

    array_2d<double> x(num_particles, num_dims);
    array_2d<double> p(num_particles, num_dims);

    long cur_particle = 0;
    for (long i = 0; i < n; ++i) {
        for (long j = 0; j < n; ++j) {
            for (long k = 0; k < n; ++k) {
                x(cur_particle, 0) = i;
                x(cur_particle, 1) = j;
                x(cur_particle, 2) = k;

                cur_particle++;
            }
        }
    }

    cosmology cosmo(1, 0, 0, 0);

    particle_list plist(x, p);
    cic_grid grid(n, plist, cosmo);

    double a = 1;
    grid.compute_potential(a);

    for (long i = 0; i < n; ++i) {
        for (long j = 0; j < n; ++j) {
            for (long k = 0; k < n; ++k) {
                EXPECT_DOUBLE_EQ(0, grid.real_grid(i, j, k));
            }
        }
    }
}

TEST(cic_grid, alternating_potential) {
    long n = 10;

    long num_particles = n * n * n;
    long num_dims = 3;

    array_2d<double> x(num_particles, num_dims);
    array_2d<double> p(num_particles, num_dims);

    long cur_particle = 0;
    for (long i = 0; i < n; ++i) {
        for (long j = 0; j < n; ++j) {
            for (long k = 0; k < n; ++k) {
                x(cur_particle, 0) = i - i % 2;
                x(cur_particle, 1) = j - j % 2;
                x(cur_particle, 2) = k - k % 2;

                cur_particle++;
            }
        }
    }

    cosmology cosmo(1, 0, 0, 0);

    particle_list plist(x, p);
    cic_grid grid(n, plist, cosmo);

    double a = 1;
    grid.compute_potential(a);

    double center_potential = grid.real_grid(0, 0, 0);
    double face_potential = grid.real_grid(0, 0, 1);
    double edge_potential = grid.real_grid(0, 1, 1);
    double corner_potential = grid.real_grid(1, 1, 1);

    EXPECT_LT(center_potential, face_potential);
    EXPECT_LT(face_potential, edge_potential);
    EXPECT_LT(edge_potential, corner_potential);

    for (long i = 0; i < n; ++i) {
        for (long j = 0; j < n; ++j) {
            for (long k = 0; k < n; ++k) {
                long num_off = i % 2 + j % 2 + k % 2;

                switch (num_off) {
                    case 0:
                        EXPECT_DOUBLE_EQ(center_potential, grid.real_grid(i, j, k));
                        break;
                    case 1:
                        EXPECT_DOUBLE_EQ(face_potential, grid.real_grid(i, j, k));
                        break;
                    case 2:
                        EXPECT_DOUBLE_EQ(edge_potential, grid.real_grid(i, j, k));
                        break;
                    default:
                        EXPECT_DOUBLE_EQ(corner_potential, grid.real_grid(i, j, k));
                        break;
                }
            }
        }
    }
}

TEST(cic_grid, accel_2body_x) {
    long n = 10;

    long num_particles = 2;
    long num_dims = 3;

    array_2d<double> x(num_particles, num_dims);
    array_2d<double> p(num_particles, num_dims);

    x(1, 0) = 1;

    cosmology cosmo(1, 0, 0, 0);

    particle_list plist(x, p);
    cic_grid grid(n, plist, cosmo);

    double a = 1;
    grid.compute_potential(a);

    double accel_0_x = grid.particle_accel(0, 0);
    double accel_0_y = grid.particle_accel(0, 1);
    double accel_0_z = grid.particle_accel(0, 2);

    double accel_1_x = grid.particle_accel(1, 0);
    double accel_1_y = grid.particle_accel(1, 1);
    double accel_1_z = grid.particle_accel(1, 2);

    EXPECT_GT(accel_0_x, 0);
    EXPECT_NEAR(accel_0_x, -accel_1_x, 1e-8);
    EXPECT_NEAR(0, accel_0_y, 1e-8);
    EXPECT_NEAR(0, accel_0_z, 1e-8);
    EXPECT_NEAR(0, accel_1_y, 1e-8);
    EXPECT_NEAR(0, accel_1_z, 1e-8);
}

TEST(cic_grid, accel_2body_y) {
    long n = 10;

    long num_particles = 2;
    long num_dims = 3;

    array_2d<double> x(num_particles, num_dims);
    array_2d<double> p(num_particles, num_dims);

    x(1, 1) = 1;

    cosmology cosmo(1, 0, 0, 0);

    particle_list plist(x, p);
    cic_grid grid(n, plist, cosmo);

    double a = 1;
    grid.compute_potential(a);

    double accel_0_x = grid.particle_accel(0, 0);
    double accel_0_y = grid.particle_accel(0, 1);
    double accel_0_z = grid.particle_accel(0, 2);

    double accel_1_x = grid.particle_accel(1, 0);
    double accel_1_y = grid.particle_accel(1, 1);
    double accel_1_z = grid.particle_accel(1, 2);

    EXPECT_GT(accel_0_y, 0);
    EXPECT_NEAR(accel_0_y, -accel_1_y, 1e-8);
    EXPECT_NEAR(0, accel_0_z, 1e-8);
    EXPECT_NEAR(0, accel_0_x, 1e-8);
    EXPECT_NEAR(0, accel_1_z, 1e-8);
    EXPECT_NEAR(0, accel_1_x, 1e-8);
}

TEST(cic_grid, accel_2body_z) {
    long n = 10;

    long num_particles = 2;
    long num_dims = 3;

    array_2d<double> x(num_particles, num_dims);
    array_2d<double> p(num_particles, num_dims);

    x(1, 2) = 1;

    cosmology cosmo(1, 0, 0, 0);

    particle_list plist(x, p);
    cic_grid grid(n, plist, cosmo);

    double a = 1;
    grid.compute_potential(a);

    double accel_0_x = grid.particle_accel(0, 0);
    double accel_0_y = grid.particle_accel(0, 1);
    double accel_0_z = grid.particle_accel(0, 2);

    double accel_1_x = grid.particle_accel(1, 0);
    double accel_1_y = grid.particle_accel(1, 1);
    double accel_1_z = grid.particle_accel(1, 2);

    EXPECT_GT(accel_0_z, 0);
    EXPECT_NEAR(accel_0_z, -accel_1_z, 1e-8);
    EXPECT_NEAR(0, accel_0_y, 1e-8);
    EXPECT_NEAR(0, accel_0_x, 1e-8);
    EXPECT_NEAR(0, accel_1_y, 1e-8);
    EXPECT_NEAR(0, accel_1_x, 1e-8);
}
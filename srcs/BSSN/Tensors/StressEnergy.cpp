#include <Geodesics.h>

/* void Grid::update_energy_momentum_tensor(int i, int j, int k) { */
/*     Cell2D &cell = globalGrid[i][j][k]; */
/*  */
/*     float vx = cell.matter.vx; */
/*     float vy = cell.matter.vy; */
/*     float vz = cell.matter.vz; */
/*  */
/*     float v2 = vx * vx + vy * vy + vz * vz; */
/*     float gamma_lorentz = 1.0 / sqrt(1.0 - v2); */
/*  */
/*     float rho = cell.matter.rho; */
/*     float p = cell.matter.p; */
/*     float h = rho + p; */
/*  */
/*     float alpha = cell.gauge.alpha; */
/*     float beta[3] = { cell.gauge.beta[0], cell.gauge.beta[1], cell.gauge.beta[2] }; */
/*     float gamma[3][3]; */
/*     for (int a = 0; a < 3; ++a) */
/*         for (int b = 0; b < 3; ++b) */
/*             gamma[a][b] = cell.geom.gamma[a][b]; */
/*  */
/*     float u_spatial[3] = { gamma_lorentz * vx, gamma_lorentz * vy, gamma_lorentz * vz }; */
/*  */
/*     float u0 = gamma_lorentz / alpha; */
/*  */
/*     float u_cov[4]; */
/*     u_cov[0] = -alpha * u0; */
/*     for (int i = 0; i < 3; ++i) { */
/*         u_cov[i + 1] = 0.0; */
/*         for (int j = 0; j < 3; ++j) { */
/*             u_cov[i + 1] += gamma[i][j] * u_spatial[j]; */
/*         } */
/*         u_cov[i + 1] -= alpha * beta[i] * u0; */
/*     } */
/*  */
/*     float u_contra[4]; */
/*     u_contra[0] = u0; */
/*     for (int i = 0; i < 3; ++i) { */
/*         u_contra[i + 1] = u_spatial[i] - beta[i] * u0; */
/*     } */
/*  */
/*     for (int mu = 0; mu < 4; ++mu) { */
/*         for (int nu = 0; nu < 4; ++nu) { */
/*             cell.matter.T[mu][nu] = h * u_contra[mu] * u_contra[nu] + p * ((mu == nu) ? 1.0 : 0.0); */
/*         } */
/*     } */
/* } */
/*  */
/* void Grid::compute_energy_momentum_evolution(int i, int j, int k, float dt) { */
/*     Cell2D &cell = globalGrid[i][j][k]; */
/*  */
/*     float dTdx[4][4], dTdy[4][4], dTdz[4][4]; */
/*     for (int mu = 0; mu < 4; ++mu) { */
/*         for (int nu = 0; nu < 4; ++nu) { */
/*             dTdx[mu][nu] = (globalGrid[i+1][j][k].matter.T[mu][nu] - globalGrid[i-1][j][k].matter.T[mu][nu]) / (2.0 * DX); */
/*             dTdy[mu][nu] = (globalGrid[i][j+1][k].matter.T[mu][nu] - globalGrid[i][j-1][k].matter.T[mu][nu]) / (2.0 * DY); */
/*             dTdz[mu][nu] = (globalGrid[i][j][k+1].matter.T[mu][nu] - globalGrid[i][j][k-1].matter.T[mu][nu]) / (2.0 * DZ); */
/*         } */
/*     } */
/*  */
/*     float ChristoffelTerm[4][4] = {0.0}; */
/*     for (int mu = 0; mu < 4; ++mu) { */
/*         for (int nu = 0; nu < 4; ++nu) { */
/*             for (int alpha = 0; alpha < 4; ++alpha) { */
/*                 for (int beta = 0; beta < 4; ++beta) { */
/*                     ChristoffelTerm[mu][nu] += cell.conn.Christoffel[mu][alpha][beta] * cell.matter.T[alpha][beta]; */
/*                 } */
/*             } */
/*         } */
/*     } */
/*  */
/*     float dtT[4][4]; */
/*     for (int mu = 0; mu < 4; ++mu) { */
/*         for (int nu = 0; nu < 4; ++nu) { */
/*             float divergence = dTdx[mu][nu] + dTdy[mu][nu] + dTdz[mu][nu]; */
/*             dtT[mu][nu] = -divergence - ChristoffelTerm[mu][nu]; */
/*         } */
/*     } */
/*  */
/*     for (int mu = 0; mu < 4; ++mu) { */
/*         for (int nu = 0; nu < 4; ++nu) { */
/*             cell.matter.T[mu][nu] += dt * dtT[mu][nu]; */
/*         } */
/*     } */
/* } */

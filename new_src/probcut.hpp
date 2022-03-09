#pragma once
#include <iostream>
#include "board.hpp"
#include "evaluate.hpp"
#include "search.hpp"
#include "midsearch.hpp"

using namespace std;
/*
constexpr int mpcd[61] = {
    0, 1, 0, 1, 2, 3, 2, 3, 2, 3, 
    4, 3, 4, 3, 4, 5, 4, 5, 6, 5, 
    6, 5, 6, 7, 6, 7, 8, 7, 8, 7, 
    8, 9, 8, 9, 10, 9, 10, 9, 10, 11, 
    10, 11, 12, 11, 12, 11, 12, 13, 12, 13, 
    14, 13, 14, 13, 14, 15, 14, 15, 16, 15,
    16
};
*/
/*
constexpr int mpcd[61] = {
    0, 1, 0, 1, 0, 1, 2, 1, 2, 1, 
    2, 3, 2, 3, 2, 3, 4, 3, 4, 3, 
    4, 5, 4, 5, 4, 5, 6, 5, 6, 5, 
    6, 7, 6, 7, 6, 7, 8, 7, 8, 7, 
    8, 9, 8, 9, 8, 9, 10, 9, 10, 9, 
    10, 11, 10, 11, 10, 11, 12, 11, 12, 11,
    12
};
*/
/*
constexpr int mpcd[61] = {
    0, 1, 0, 1, 0, 1, 2, 1, 2, 1, 
    2, 1, 2, 3, 2, 3, 2, 3, 4, 3, 
    4, 3, 4, 3, 4, 5, 4, 5, 4, 5, 
    6, 5, 6, 5, 6, 5, 6, 7, 6, 7, 
    6, 7, 8, 7, 8, 7, 8, 7, 8, 9, 
    8, 9, 8, 9, 10, 9, 10, 9, 10, 9,
    10
};
*/

constexpr int mpcd[61] = {
    0, 1, 0, 1, 0, 1, 0, 1, 2, 1, 
    2, 1, 2, 1, 2, 3, 2, 3, 2, 3, 
    2, 3, 4, 3, 4, 3, 4, 5, 4, 5, 
    4, 5, 4, 5, 6, 5, 6, 5, 6, 7, 
    6, 7, 6, 7, 6, 7, 8, 7, 8, 7, 
    8, 9, 8, 9, 8, 9, 8, 9, 10, 9,
    10
};


constexpr double probcut_sigmas[N_PHASES][N_PHASES] = {
    {32.0833, 31.7961, 31.4724, 30.7864, 29.8365, 30.105, 29.5813, 28.6756, 28.097, 27.4648, 26.8071, 26.1217, 25.5883, 24.9074, 24.3483, 23.9103, 23.5966, 23.4757, 23.4832, 23.4631, 23.514, 23.4841, 23.4965, 23.5738, 23.5965, 23.5004, 23.4416, 23.3096, 23.1082, 22.8473},
    {31.7961, 31.5058, 31.1784, 30.4846, 29.5236, 29.7954, 29.2654, 28.3486, 27.7626, 27.122, 26.455, 25.7596, 25.218, 24.5261, 23.9575, 23.5118, 23.1924, 23.0693, 23.077, 23.0565, 23.1083, 23.0779, 23.0905, 23.1692, 23.1923, 23.0945, 23.0346, 22.9001, 22.6948, 22.4288},   
    {31.4724, 31.1784, 30.847, 30.1444, 29.1709, 29.4462, 28.9092, 27.9796, 27.3851, 26.7348, 26.0574, 25.3503, 24.7993, 24.0947, 23.5152, 23.0605, 22.7345, 22.6087, 22.6166, 22.5956, 22.6486, 22.6175, 22.6304, 22.7108, 22.7344, 22.6345, 22.5733, 22.4359, 22.2261, 21.9542},
    {30.7864, 30.4846, 30.1444, 29.4228, 28.4222, 28.7053, 28.153, 27.1958, 26.5828, 25.9113, 25.2107, 24.4781, 23.9061, 23.1733, 22.5693, 22.0944, 21.7534, 21.6217, 21.63, 21.6081, 21.6635, 21.631, 21.6445, 21.7286, 21.7533, 21.6487, 21.5847, 21.4407, 21.2208, 20.9354},   
    {29.8365, 29.5236, 29.1709, 28.4222, 27.3825, 27.6769, 27.1024, 26.105, 25.4649, 24.7622, 24.0272, 23.2564, 22.6528, 21.8772, 21.2355, 20.7296, 20.3653, 20.2244, 20.2332, 20.2097, 20.2691, 20.2343, 20.2487, 20.3387, 20.3651, 20.2533, 20.1847, 20.0305, 19.7947, 19.488}, 
    {30.105, 29.7954, 29.4462, 28.7053, 27.6769, 27.968, 27.4, 26.4142, 25.7819, 25.0883, 24.3633, 23.6038, 23.0095, 22.2465, 21.6159, 21.1192, 20.7618, 20.6237, 20.6324, 20.6093, 20.6675, 20.6334, 20.6476, 20.7358, 20.7617, 20.652, 20.5848, 20.4337, 20.2026, 19.9023},     
    {29.5813, 29.2654, 28.9092, 28.153, 27.1024, 27.4, 26.8193, 25.8106, 25.1628, 24.4513, 23.7065, 22.9248, 22.3121, 21.524, 20.8714, 20.3563, 19.9851, 19.8415, 19.8505, 19.8265, 19.887, 19.8516, 19.8663, 19.9581, 19.985, 19.8709, 19.801, 19.6438, 19.4032, 19.0902},       
    {28.6756, 28.3486, 27.9796, 27.1958, 26.105, 26.4142, 25.8106, 24.76, 24.0835, 23.3387, 22.5568, 21.7334, 21.0859, 20.2498, 19.5544, 19.0033, 18.605, 18.4506, 18.4603, 18.4345, 18.4996, 18.4615, 18.4773, 18.576, 18.6049, 18.4823, 18.407, 18.2377, 17.9782, 17.6398},     
    {28.097, 27.7626, 27.3851, 26.5828, 25.4649, 25.7819, 25.1628, 24.0835, 23.3873, 22.6194, 21.8116, 20.9589, 20.2866, 19.416, 18.6894, 18.112, 17.6936, 17.5311, 17.5414, 17.5142, 17.5827, 17.5426, 17.5593, 17.6631, 17.6934, 17.5645, 17.4853, 17.3069, 17.0332, 16.6756},  
    {27.4648, 27.122, 26.7348, 25.9113, 24.7622, 25.0883, 24.4513, 23.3387, 22.6194, 21.8245, 20.986, 20.0982, 19.3961, 18.4835, 17.7187, 17.1086, 16.6649, 16.4923, 16.5032, 16.4744, 16.5472, 16.5045, 16.5222, 16.6325, 16.6648, 16.5278, 16.4436, 16.2538, 15.962, 15.5798},  
    {26.8071, 26.455, 26.0574, 25.2107, 24.0272, 24.3633, 23.7065, 22.5568, 21.8116, 20.986, 20.1127, 19.1844, 18.4475, 17.4855, 16.675, 16.0251, 15.5506, 15.3655, 15.3771, 15.3462, 15.4243, 15.3785, 15.3976, 15.5158, 15.5504, 15.4035, 15.3132, 15.1092, 14.7948, 14.3816},  
    {26.1217, 25.7596, 25.3503, 24.4781, 23.2564, 23.6038, 22.9248, 21.7334, 20.9589, 20.0982, 19.1844, 18.209, 17.4309, 16.4093, 15.5428, 14.8434, 14.3298, 14.1287, 14.1414, 14.1077, 14.1927, 14.1429, 14.1636, 14.2921, 14.3297, 14.1701, 14.0718, 13.8495, 13.5059, 13.052}, 
    {25.5883, 25.218, 24.7993, 23.9061, 22.6528, 23.0095, 22.3121, 21.0859, 20.2866, 19.3961, 18.4475, 17.4309, 16.6163, 15.5414, 14.6235, 13.8779, 13.3271, 13.1107, 13.1243, 13.088, 13.1796, 13.126, 13.1482, 13.2865, 13.3269, 13.1552, 13.0493, 12.8093, 12.437, 11.9425},   
    {24.9074, 24.5261, 24.0947, 23.1733, 21.8772, 22.2465, 21.524, 20.2498, 19.416, 18.4835, 17.4855, 16.4093, 15.5414, 14.3863, 13.3895, 12.5709, 11.9601, 11.7184, 11.7337, 11.6931, 11.7954, 11.7355, 11.7604, 11.9148, 11.9599, 11.7682, 11.6497, 11.3802, 10.9594, 10.3949}, 
    {24.3483, 23.9575, 23.5152, 22.5693, 21.2355, 21.6159, 20.8714, 19.5544, 18.6894, 17.7187, 16.675, 15.5428, 14.6235, 13.3895, 12.3122, 11.4166, 10.7404, 10.4706, 10.4877, 10.4423, 10.5567, 10.4897, 10.5176, 10.69, 10.7402, 10.5263, 10.3937, 10.0907, 9.61363, 8.96476},  
    {23.9103, 23.5118, 23.0605, 22.0944, 20.7296, 21.1192, 20.3563, 19.0033, 18.112, 17.1086, 16.0251, 14.8434, 13.8779, 12.5709, 11.4166, 10.4446, 9.7008, 9.40119, 9.42023, 9.36961, 9.49703, 9.4225, 9.45353, 9.64496, 9.70052, 9.46319, 9.31542, 8.97611, 8.43627, 7.68867},  
    {23.5966, 23.1924, 22.7345, 21.7534, 20.3653, 20.7618, 19.9851, 18.605, 17.6936, 16.6649, 15.5506, 14.3298, 13.3271, 11.9601, 10.7404, 9.7008, 8.89508, 8.56734, 8.58822, 8.53267, 8.6724, 8.59072, 8.62474, 8.83415, 8.89477, 8.63533, 8.47313, 8.09861, 7.49584, 6.64324},
    {23.4757, 23.0693, 22.6087, 21.6217, 20.2244, 20.6237, 19.8415, 18.4506, 17.5311, 16.4923, 15.3655, 14.1287, 13.1107, 11.7184, 10.4706, 9.40119, 8.56734, 8.22655, 8.2483, 8.19044, 8.33591, 8.25089, 8.28632, 8.50406, 8.56701, 8.29734, 8.12839, 7.7372, 7.10383, 6.19754},
    {23.4832, 23.077, 22.6166, 21.63, 20.2332, 20.6324, 19.8505, 18.4603, 17.5414, 16.5032, 15.3771, 14.1414, 13.1243, 11.7337, 10.4877, 9.42023, 8.58822, 8.2483, 8.26999, 8.21228, 8.35737, 8.27257, 8.3079, 8.52509, 8.5879, 8.31889, 8.1504, 7.76032, 7.129, 6.22637},
    {23.4631, 23.0565, 22.5956, 21.6081, 20.2097, 20.6093, 19.8265, 18.4345, 17.5142, 16.4744, 15.3462, 14.1077, 13.088, 11.6931, 10.4423, 9.36961, 8.53267, 8.19044, 8.21228, 8.15417, 8.30027, 8.21489, 8.25047, 8.46913, 8.53235, 8.26153, 8.09184, 7.6988, 7.06198, 6.14952},
    {23.514, 23.1083, 22.6486, 21.6635, 20.2691, 20.6675, 19.887, 18.4996, 17.5827, 16.5472, 15.4243, 14.1927, 13.1796, 11.7954, 10.5567, 9.49703, 8.6724, 8.33591, 8.35737, 8.30027, 8.44384, 8.35993, 8.39489, 8.60989, 8.67207, 8.40577, 8.23904, 7.85337, 7.23018, 6.34197},
    {23.4841, 23.0779, 22.6175, 21.631, 20.2343, 20.6334, 19.8516, 18.4615, 17.5426, 16.5045, 15.3785, 14.1429, 13.126, 11.7355, 10.4897, 9.4225, 8.59072, 8.25089, 8.27257, 8.21489, 8.35993, 8.27516, 8.31048, 8.52761, 8.59039, 8.32147, 8.15302, 7.76308, 7.132, 6.22981},
    {23.4965, 23.0905, 22.6304, 21.6445, 20.2487, 20.6476, 19.8663, 18.4773, 17.5593, 16.5222, 15.3976, 14.1636, 13.1482, 11.7604, 10.5176, 9.45353, 8.62474, 8.28632, 8.3079, 8.25047, 8.39489, 8.31048, 8.34565, 8.56188, 8.62442, 8.35659, 8.18887, 7.80071, 7.17295, 6.27665},
    {23.5738, 23.1692, 22.7108, 21.7286, 20.3387, 20.7358, 19.9581, 18.576, 17.6631, 16.6325, 15.5158, 14.2921, 13.2865, 11.9148, 10.69, 9.64496, 8.83415, 8.50406, 8.52509, 8.46913, 8.60989, 8.52761, 8.56188, 8.77279, 8.83383, 8.57255, 8.40913, 8.03163, 7.42342, 6.56142},
    {23.5965, 23.1923, 22.7344, 21.7533, 20.3651, 20.7617, 19.985, 18.6049, 17.6934, 16.6648, 15.5504, 14.3297, 13.3269, 11.9599, 10.7402, 9.70052, 8.89477, 8.56701, 8.5879, 8.53235, 8.67207, 8.59039, 8.62442, 8.83383, 8.89445, 8.63501, 8.4728, 8.09826, 7.49546, 6.64282},
    {23.5004, 23.0945, 22.6345, 21.6487, 20.2533, 20.652, 19.8709, 18.4823, 17.5645, 16.5278, 15.4035, 14.1701, 13.1552, 11.7682, 10.5263, 9.46319, 8.63533, 8.29734, 8.31889, 8.26153, 8.40577, 8.32147, 8.35659, 8.57255, 8.63501, 8.36752, 8.20002, 7.81242, 7.18568, 6.29119},
    {23.4416, 23.0346, 22.5733, 21.5847, 20.1847, 20.5848, 19.801, 18.407, 17.4853, 16.4436, 15.3132, 14.0718, 13.0493, 11.6497, 10.3937, 9.31542, 8.47313, 8.12839, 8.1504, 8.09184, 8.23904, 8.15302, 8.18887, 8.40913, 8.4728, 8.20002, 8.02903, 7.63275, 6.98992, 6.06663},
    {23.3096, 22.9001, 22.4359, 21.4407, 20.0305, 20.4337, 19.6438, 18.2377, 17.3069, 16.2538, 15.1092, 13.8495, 12.8093, 11.3802, 10.0907, 8.97611, 8.09861, 7.7372, 7.76032, 7.6988, 7.85337, 7.76308, 7.80071, 8.03163, 8.09826, 7.81242, 7.63275, 7.21474, 6.53089, 5.53151},
    {23.1082, 22.6948, 22.2261, 21.2208, 19.7947, 20.2026, 19.4032, 17.9782, 17.0332, 15.962, 14.7948, 13.5059, 12.437, 10.9594, 9.61363, 8.43627, 7.49584, 7.10383, 7.129, 7.06198, 7.23018, 7.132, 7.17295, 7.42342, 7.49546, 7.18568, 6.98992, 6.53089, 5.76651, 4.60409},
    {22.8473, 22.4288, 21.9542, 20.9354, 19.488, 19.9023, 19.0902, 17.6398, 16.6756, 15.5798, 14.3816, 13.052, 11.9425, 10.3949, 8.96476, 7.68867, 6.64324, 6.19754, 6.22637, 6.14952, 6.34197, 6.22981, 6.27665, 6.56142, 6.64282, 6.29119, 6.06663, 5.53151, 4.60409, 3.02369}
};

constexpr double w_probcut_sigma[61] = {
    1.00, 0.99, 0.98, 0.97, 0.96, 0.95, 0.94, 0.93, 0.92, 0.91, 
    0.90, 0.89, 0.88, 0.87, 0.86, 0.85, 0.84, 0.83, 0.82, 0.81, 
    0.80, 0.79, 0.78, 0.77, 0.76, 0.75, 0.74, 0.73, 0.72, 0.71, 
    0.70, 0.69, 0.68, 0.67, 0.66, 0.65, 0.64, 0.63, 0.62, 0.61, 
    0.60, 0.59, 0.58, 0.57, 0.56, 0.55, 0.54, 0.53, 0.52, 0.51, 
    0.50, 0.49, 0.48, 0.47, 0.46, 0.45, 0.44, 0.43, 0.42, 0.41, 
    0.40
};

#define W_PROBCUT_SIGMA_ALL 0.7

inline double probcut_sigma(int n_stones, int depth){
    int x1 = min(N_PHASES - 1, (n_stones + depth - 4) / PHASE_N_STONES);
    int x2 = min(N_PHASES - 1, (n_stones + mpcd[depth] - 4) / PHASE_N_STONES);
    //return probcut_sigmas[x1][x2] * sqrt(w_probcut_sigma[x1] * w_probcut_sigma[x2]); // * W_PROBCUT_SIGMA_ALL;
    return probcut_sigmas[x1][x2] * W_PROBCUT_SIGMA_ALL;
    //return probcut_sigmas[x1][x2] * exp(-0.0075 * (x1 + x2));
}

int nega_alpha_eval1(Search *search, int alpha, int beta, bool skipped);
int nega_alpha(Search *search, int alpha, int beta, int depth, bool skipped);
int nega_alpha_ordering_nomemo(Search *search, int alpha, int beta, int depth, bool skipped, uint64_t legal);

inline bool mpc_higher(Search *search, int beta, int depth, uint64_t legal){
    if (depth <= 8)
        return false;
    int bound = beta + ceil(search->mpct * probcut_sigma(search->board.n, depth));
    if (bound > HW2)
        bound = HW2; //return false;
    bool res;
    switch(mpcd[depth]){
        case 0:
            res = mid_evaluate(&search->board) >= bound;
            break;
        case 1:
            res = nega_alpha_eval1(search, bound - 1, bound, false) >= bound;
            break;
        default:
            if (mpcd[depth] <= MID_FAST_DEPTH)
                res = nega_alpha(search, bound - 1, bound, mpcd[depth], false) >= bound;
            else{
                //double mpct = search->mpct;
                //search->mpct = 1.18;
                //search->use_mpc = false;
                    res = nega_alpha_ordering_nomemo(search, bound - 1, bound, mpcd[depth], false, legal) >= bound;
                //search->use_mpc = true;
                //search->mpct = mpct;
            }
            break;
    }
    return res;
}

inline bool mpc_lower(Search *search, int alpha, int depth, uint64_t legal){
    if (depth <= 8)
        return false;
    int bound = alpha - ceil(search->mpct * probcut_sigma(search->board.n, depth));
    if (bound < -HW2)
        bound = -HW2; //return false;
    bool res;
    switch(mpcd[depth]){
        case 0:
            res = mid_evaluate(&search->board) <= bound;
            break;
        case 1:
            res = nega_alpha_eval1(search, bound, bound + 1, false) <= bound;
            break;
        default:
            if (mpcd[depth] <= MID_FAST_DEPTH)
                res = nega_alpha(search, bound, bound + 1, mpcd[depth], false) <= bound;
            else{
                //double mpct = search->mpct;
                //search->mpct = 1.18;
                //search->use_mpc = false;
                    res = nega_alpha_ordering_nomemo(search, bound, bound + 1, mpcd[depth], false, legal) <= bound;
                //search->use_mpc = true;
                //search->mpct = mpct;
            }
            break;
    }
    return res;
}
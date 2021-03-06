#pragma once
#include <iostream>
#include <fstream>
#include <algorithm>
#include "setting.hpp"
#include "common.hpp"
#include "board.hpp"
#include "evaluate.hpp"
#include "transpose_table.hpp"

using namespace std;

#ifndef N_SYMMETRY_PATTERNS
    #define N_SYMMETRY_PATTERNS 62
#endif
/*
#ifndef N_ADDITIONAL_SYMMETRY_PATTERNS
    #define N_ADDITIONAL_SYMMETRY_PATTERNS 12
#endif
*/
#define MID_FAST_DEPTH 1
#define END_FAST_DEPTH 7
#define MID_TO_END_DEPTH 13
#define CUDA_YBWC_SPLIT_MAX_DEPTH 10
#define USE_TT_DEPTH_THRESHOLD 10

#define SCORE_UNDEFINED -INF

#define N_CELL_WEIGHT_MASK 5

constexpr int cell_weight[HW2] = {
    18,  4,  16, 12, 12, 16,  4, 18,
     4,  2,   6,  8,  8,  6,  2,  4,
    16,  6,  14, 10, 10, 14,  6, 16,
    12,  8,  10,  0,  0, 10,  8, 12,
    12,  8,  10,  0,  0, 10,  8, 12,
    16,  6,  14, 10, 10, 14,  6, 16,
     4,  2,   6,  8,  8,  6,  2,  4,
    18,  4,  16, 12, 12, 16,  4, 18
};

constexpr uint64_t cell_weight_mask[N_CELL_WEIGHT_MASK] = {
    0b10000001'00000000'00000000'00000000'00000000'00000000'00000000'10000001ULL, // corner
    0b00011000'00000000'00000000'10000001'10000001'00000000'00000000'00011000ULL, // B
    0b01100110'10000001'10000001'00000000'00000000'10000001'10000001'01100110ULL, // A & C
    0b00000000'01000010'00000000'00000000'00000000'00000000'01000010'00000000ULL, // X
    0b00000000'00111100'01111110'01111110'01111110'01111110'00111100'00000000ULL  // other
};

/*
from https://github.com/abulmo/edax-reversi/blob/1ae7c9fe5322ac01975f1b3196e788b0d25c1e10/src/search.c
modified by Nyanyan
*/
constexpr int nws_stability_threshold[61] = {
    99, 99, 99,  4,  6,  8, 10, 12,
    14, 16, 20, 22, 24, 26, 28, 30,
    32, 34, 36, 38, 40, 42, 44, 46,
    48, 48, 50, 50, 52, 52, 54, 54,
    56, 56, 58, 58, 60, 60, 62, 62,
    64, 64, 64, 64, 64, 64, 64, 64,
    99, 99, 99, 99, 99, 99, 99, 99,
    99, 99, 99, 99, 99
};
/*
end of modification
*/

struct Search_result{
    int policy;
    int value;
    int depth;
    int nps;
    uint64_t time;
    uint64_t nodes;
};

struct Search{
    Board board;
    bool use_mpc;
    double mpct;
    uint64_t n_nodes;
    int eval_features[N_SYMMETRY_PATTERNS];
    uint_fast8_t eval_feature_reversed;
};

struct Parallel_task{
    int value;
    uint64_t n_nodes;
    uint_fast8_t cell;
};

inline void register_tt(Search *search, uint32_t hash_code, int first_alpha, int v, int best_move, int l, int u, int alpha, int beta, const bool *searching){
    #if USE_END_TC
        if (search->board.n <= HW2 - USE_TT_DEPTH_THRESHOLD && (*searching)){
            if (first_alpha < v && best_move != TRANSPOSE_TABLE_UNDEFINED)
                child_transpose_table.reg(&search->board, hash_code, best_move);
            if (first_alpha < v && v < beta)
                parent_transpose_table.reg(&search->board, hash_code, v, v);
            else if (beta <= v && l < v)
                parent_transpose_table.reg(&search->board, hash_code, v, u);
            else if (v <= alpha && v < u)
                parent_transpose_table.reg(&search->board, hash_code, l, v);
        }
    #endif
}


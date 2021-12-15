#pragma once
#include <iostream>
#include <fstream>
#include "setting.hpp"
#include "common.hpp"
#include "board.hpp"

using namespace std;

typedef float eval_type;

#define n_patterns 11
#define n_dense0 32
#define n_dense1 32
#define n_dense2 32
#define n_add_input 3
#define n_add_dense0 8
#define n_add_dense1 8
#define n_add_dense2 8
#define max_canput 40
#define max_surround 80
#define max_canput_2 20
#define max_surround_2 40
#define max_evaluate_idx 59049

#define sc_w 6400
#define step 100

#define p31 3
#define p32 9
#define p33 27
#define p34 81
#define p35 243
#define p36 729
#define p37 2187
#define p38 6561
#define p39 19683
#define p310 59049
#define p31m 2
#define p32m 8
#define p33m 26
#define p34m 80
#define p35m 242
#define p36m 728
#define p37m 2186
#define p38m 6560
#define p39m 19682
#define p310m 59048

int count_black_arr[n_line];
int count_both_arr[n_line];
int mobility_arr[2][n_line];
int mobility_arr2[2][n_line * n_line];
int surround_arr[2][n_line];
int stability_edge_arr[2][n_line];
int stability_corner_arr[2][n_line];
int pattern_arr[n_phases][2][n_patterns][max_evaluate_idx];
int add_arr[n_phases][2][max_canput_2][max_surround_2][max_surround_2];

inline void init_evaluation_base() {
    int idx, place, b, w;
    bool full_black, full_white;
    for (idx = 0; idx < n_line; ++idx) {
        b = create_one_color(idx, 0);
        w = create_one_color(idx, 1);
        mobility_arr[black][idx] = 0;
        mobility_arr[white][idx] = 0;
        surround_arr[black][idx] = 0;
        surround_arr[white][idx] = 0;
        count_black_arr[idx] = 0;
        count_both_arr[idx] = 0;
        stability_edge_arr[black][idx] = 0;
        stability_edge_arr[white][idx] = 0;
        stability_corner_arr[black][idx] = 0;
        stability_corner_arr[white][idx] = 0;
        for (place = 0; place < hw; ++place) {
            if (1 & (b >> place)){
                ++count_black_arr[idx];
                ++count_both_arr[idx];
            } else if (1 & (w >> place)){
                --count_black_arr[idx];
                ++count_both_arr[idx];
            }
            if (place > 0) {
                if ((1 & (b >> (place - 1))) == 0 && (1 & (w >> (place - 1))) == 0) {
                    if (1 & (b >> place))
                        ++surround_arr[black][idx];
                    else if (1 & (w >> place))
                        ++surround_arr[white][idx];
                }
            }
            if (place < hw - 1) {
                if ((1 & (b >> (place + 1))) == 0 && (1 & (w >> (place + 1))) == 0) {
                    if (1 & (b >> place))
                        ++surround_arr[black][idx];
                    else if (1 & (w >> place))
                        ++surround_arr[white][idx];
                }
            }
        }
        for (place = 0; place < hw; ++place) {
            if (legal_arr[black][idx][place])
                ++mobility_arr[black][idx];
            if (legal_arr[white][idx][place])
                ++mobility_arr[white][idx];
        }
        if (count_both_arr[idx] == hw){
            stability_edge_arr[black][idx] = (count_black_arr[idx] + hw) / 2;
            stability_edge_arr[white][idx] += hw - stability_edge_arr[black][idx];
        } else{
            full_black = true;
            full_white = true;
            for (place = 0; place < hw; ++place){
                full_black &= 1 & (b >> place);
                full_white &= 1 & (w >> place);
                stability_edge_arr[black][idx] += full_black;
                stability_edge_arr[white][idx] += full_white;
            }
            full_black = true;
            full_white = true;
            for (place = hw_m1; place >= 0; --place){
                full_black &= 1 & (b >> place);
                full_white &= 1 & (w >> place);
                stability_edge_arr[black][idx] += full_black;
                stability_edge_arr[white][idx] += full_white;
            }
            if (1 & b){
                --stability_edge_arr[black][idx];
                ++stability_corner_arr[black][idx];
            }
            if (1 & (b >> hw_m1)){
                --stability_edge_arr[black][idx];
                ++stability_corner_arr[black][idx];
            }
            if (1 & w){
                --stability_edge_arr[white][idx];
                ++stability_corner_arr[white][idx];
            }
            if (1 & (w >> hw_m1)){
                --stability_edge_arr[white][idx];
                ++stability_corner_arr[white][idx];
            }
        }
    }
}

inline void init_evaluation_calc(){
    ifstream ifs("C:/github/egaroucid/Egaroucid5/src/resources/param.txt");
    if (ifs.fail()){
        cerr << "evaluation file not exist" << endl;
        exit(1);
    }
    string line;
    int phase_idx, player_idx, pattern_idx, pattern_elem, canput, sur0, sur1;
    const int pattern_sizes[n_patterns] = {8, 8, 8, 5, 6, 7, 8, 10, 10, 10, 10};
    for (phase_idx = 0; phase_idx < n_phases; ++phase_idx){
        for (player_idx = 0; player_idx < 2; ++player_idx){
            cerr << "=";
            for (pattern_idx = 0; pattern_idx < n_patterns; ++pattern_idx){
                for (pattern_elem = 0; pattern_elem < pow3[pattern_sizes[pattern_idx]]; ++pattern_elem){
                    getline(ifs, line);
                    pattern_arr[phase_idx][player_idx][pattern_idx][pattern_elem] = stoi(line);
                }
            }
            for (canput = 0; canput < max_canput_2; ++canput){
                for (sur0 = 0; sur0 < max_surround_2; ++sur0){
                    for (sur1 = 0; sur1 < max_surround_2; ++sur1){
                        getline(ifs, line);
                        add_arr[phase_idx][player_idx][canput][sur0][sur1] = stoi(line);
                    }
                }
            }
        }
    }
    cerr << endl;
}

inline void evaluate_init(){
    init_evaluation_base();
    #if !EVAL_MODE
        init_evaluation_calc();
    #endif
    cerr << "evaluation function initialized" << endl;
}

inline int sfill5(int b){
    return pop_digit[b][2] != 2 ? b - p35m : b;
}

inline int sfill4(int b){
    return pop_digit[b][3] != 2 ? b - p34m : b;
}

inline int sfill3(int b){
    return pop_digit[b][4] != 2 ? b - p33m : b;
}

inline int sfill2(int b){
    return pop_digit[b][5] != 2 ? b - p32m : b;
}

inline int sfill1(int b){
    return pop_digit[b][6] != 2 ? b - p31m : b;
}

inline int calc_canput(const board *b){
    return 
        mobility_arr[b->p][b->b[0]] + mobility_arr[b->p][b->b[1]] + mobility_arr[b->p][b->b[2]] + mobility_arr[b->p][b->b[3]] + 
        mobility_arr[b->p][b->b[4]] + mobility_arr[b->p][b->b[5]] + mobility_arr[b->p][b->b[6]] + mobility_arr[b->p][b->b[7]] + 
        mobility_arr[b->p][b->b[8]] + mobility_arr[b->p][b->b[9]] + mobility_arr[b->p][b->b[10]] + mobility_arr[b->p][b->b[11]] + 
        mobility_arr[b->p][b->b[12]] + mobility_arr[b->p][b->b[13]] + mobility_arr[b->p][b->b[14]] + mobility_arr[b->p][b->b[15]] + 
        mobility_arr[b->p][b->b[16] - p35m] + mobility_arr[b->p][b->b[26] - p35m] + mobility_arr[b->p][b->b[27] - p35m] + mobility_arr[b->p][b->b[37] - p35m] + 
        mobility_arr[b->p][b->b[17] - p34m] + mobility_arr[b->p][b->b[25] - p34m] + mobility_arr[b->p][b->b[28] - p34m] + mobility_arr[b->p][b->b[36] - p34m] + 
        mobility_arr[b->p][b->b[18] - p33m] + mobility_arr[b->p][b->b[24] - p33m] + mobility_arr[b->p][b->b[29] - p33m] + mobility_arr[b->p][b->b[35] - p33m] + 
        mobility_arr[b->p][b->b[19] - p32m] + mobility_arr[b->p][b->b[23] - p32m] + mobility_arr[b->p][b->b[30] - p32m] + mobility_arr[b->p][b->b[34] - p32m] + 
        mobility_arr[b->p][b->b[20] - p31m] + mobility_arr[b->p][b->b[22] - p31m] + mobility_arr[b->p][b->b[31] - p31m] + mobility_arr[b->p][b->b[33] - p31m] + 
        mobility_arr[b->p][b->b[21]] + mobility_arr[b->p][b->b[32]];
}

inline int calc_surround(const board *b, int p){
    return 
        surround_arr[p][b->b[0]] + surround_arr[p][b->b[1]] + surround_arr[p][b->b[2]] + surround_arr[p][b->b[3]] + 
        surround_arr[p][b->b[4]] + surround_arr[p][b->b[5]] + surround_arr[p][b->b[6]] + surround_arr[p][b->b[7]] + 
        surround_arr[p][b->b[8]] + surround_arr[p][b->b[9]] + surround_arr[p][b->b[10]] + surround_arr[p][b->b[11]] + 
        surround_arr[p][b->b[12]] + surround_arr[p][b->b[13]] + surround_arr[p][b->b[14]] + surround_arr[p][b->b[15]] + 
        surround_arr[p][sfill5(b->b[16])] + surround_arr[p][sfill5(b->b[26])] + surround_arr[p][sfill5(b->b[27])] + surround_arr[p][sfill5(b->b[37])] + 
        surround_arr[p][sfill4(b->b[17])] + surround_arr[p][sfill4(b->b[25])] + surround_arr[p][sfill4(b->b[28])] + surround_arr[p][sfill4(b->b[36])] + 
        surround_arr[p][sfill3(b->b[18])] + surround_arr[p][sfill3(b->b[24])] + surround_arr[p][sfill3(b->b[29])] + surround_arr[p][sfill3(b->b[35])] + 
        surround_arr[p][sfill2(b->b[19])] + surround_arr[p][sfill2(b->b[23])] + surround_arr[p][sfill2(b->b[30])] + surround_arr[p][sfill2(b->b[34])] + 
        surround_arr[p][sfill1(b->b[20])] + surround_arr[p][sfill1(b->b[22])] + surround_arr[p][sfill1(b->b[31])] + surround_arr[p][sfill1(b->b[33])] + 
        surround_arr[p][b->b[21]] + surround_arr[p][b->b[32]];
}

inline eval_type edge_2x(int phase_idx, const board *b, int x, int y){
    return pattern_arr[phase_idx][b->p][7][pop_digit[b->b[x]][1] * p39 + b->b[y] * p31 + pop_digit[b->b[x]][6]];
}

inline eval_type triangle0(int phase_idx, const board *b, int w, int x, int y, int z){
    return pattern_arr[phase_idx][b->p][8][b->b[w] / p34 * p36 + b->b[x] / p35 * p33 + b->b[y] / p36 * p31 + b->b[z] / p37];
}

inline eval_type triangle1(int phase_idx, const board *b, int w, int x, int y, int z){
    return pattern_arr[phase_idx][b->p][8][reverse_board[b->b[w]] / p34 * p36 + reverse_board[b->b[x]] / p35 * p33 + reverse_board[b->b[y]] / p36 * p31 + reverse_board[b->b[z]] / p37];
}

inline eval_type edge_block(int phase_idx, const board *b, int x, int y){
    return pattern_arr[phase_idx][b->p][9][pop_digit[b->b[x]][0] * p39 + pop_mid[b->b[x]][6][2] * p35 + pop_digit[b->b[x]][7] * p34 + pop_mid[b->b[y]][6][2]];
}

inline eval_type cross(int phase_idx, const board *b, int x, int y, int z){
    return 
        pattern_arr[phase_idx][b->p][10][b->b[x] / p34 * p36 + b->b[y] / p35 * p33 + b->b[z] / p35] + 
        pattern_arr[phase_idx][b->p][10][reverse_board[b->b[x]] / p34 * p36 + pop_mid[reverse_board[b->b[y]]][7][4] * p33 + pop_mid[reverse_board[b->b[z]]][7][4]];
}

inline eval_type calc_pattern(int phase_idx, const board *b){
    return 
        pattern_arr[phase_idx][b->p][0][b->b[1]] + pattern_arr[phase_idx][b->p][0][b->b[6]] + pattern_arr[phase_idx][b->p][0][b->b[9]] + pattern_arr[phase_idx][b->p][0][b->b[14]] + 
        pattern_arr[phase_idx][b->p][1][b->b[2]] + pattern_arr[phase_idx][b->p][1][b->b[5]] + pattern_arr[phase_idx][b->p][1][b->b[10]] + pattern_arr[phase_idx][b->p][1][b->b[13]] + 
        pattern_arr[phase_idx][b->p][2][b->b[3]] + pattern_arr[phase_idx][b->p][2][b->b[4]] + pattern_arr[phase_idx][b->p][2][b->b[11]] + pattern_arr[phase_idx][b->p][2][b->b[12]] + 
        pattern_arr[phase_idx][b->p][3][b->b[18] / p33] + pattern_arr[phase_idx][b->p][3][b->b[24] / p33] + pattern_arr[phase_idx][b->p][3][b->b[29] / p33] + pattern_arr[phase_idx][b->p][3][b->b[35] / p33] + 
        pattern_arr[phase_idx][b->p][4][b->b[19] / p32] + pattern_arr[phase_idx][b->p][4][b->b[23] / p32] + pattern_arr[phase_idx][b->p][4][b->b[30] / p32] + pattern_arr[phase_idx][b->p][4][b->b[34] / p32] + 
        pattern_arr[phase_idx][b->p][5][b->b[20] / p31] + pattern_arr[phase_idx][b->p][5][b->b[22] / p31] + pattern_arr[phase_idx][b->p][5][b->b[31] / p31] + pattern_arr[phase_idx][b->p][5][b->b[33] / p31] + 
        pattern_arr[phase_idx][b->p][6][b->b[21]] + pattern_arr[phase_idx][b->p][6][b->b[32]] + 
        edge_2x(phase_idx, b, 1, 0) + edge_2x(phase_idx, b, 6, 7) + edge_2x(phase_idx, b, 9, 8) + edge_2x(phase_idx, b, 14, 15) + 
        triangle0(phase_idx, b, 0, 1, 2, 3) + triangle0(phase_idx, b, 7, 6, 5, 4) + triangle0(phase_idx, b, 15, 14, 13, 12) + triangle1(phase_idx, b, 15, 14, 13, 12) + 
        edge_block(phase_idx, b, 0, 1) + edge_block(phase_idx, b, 7, 6) + edge_block(phase_idx, b, 8, 9) + edge_block(phase_idx, b, 15, 14) + 
        cross(phase_idx, b, 21, 20, 22) + cross(phase_idx, b, 32, 31, 33);
}

inline int mid_evaluate(const board *b){
    int phase_idx = calc_phase_idx(b), canput, sur0, sur1;
    canput = min(max_canput_2 - 1, calc_canput(b) / 2);
    sur0 = min(max_surround_2 - 1, calc_surround(b, black) / 2);
    sur1 = min(max_surround_2 - 1, calc_surround(b, white) / 2);
    return calc_pattern(phase_idx, b) + add_arr[phase_idx][b->p][canput][sur0][sur1];
}

inline int end_evaluate(const board *b){
    int count = (b->p ? -1 : 1) * 
        (count_black_arr[b->b[0]] + count_black_arr[b->b[1]] + count_black_arr[b->b[2]] + count_black_arr[b->b[3]] + 
        count_black_arr[b->b[4]] + count_black_arr[b->b[5]] + count_black_arr[b->b[6]] + count_black_arr[b->b[7]]);
    int empty = hw2 - 
        count_both_arr[b->b[0]] - count_both_arr[b->b[1]] - count_both_arr[b->b[2]] - count_both_arr[b->b[3]] - 
        count_both_arr[b->b[4]] - count_both_arr[b->b[5]] - count_both_arr[b->b[6]] - count_both_arr[b->b[7]];
    if (count > 0)
        count += empty;
    else if (count < 0)
        count -= empty;
    return count * step;
}
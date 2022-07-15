#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include "new_util/board.hpp"

using namespace std;

#define MAX_N_LINE 4
#define POPULATION 100
#define N_DATA 10000
#define SCORING_SIZE_THRESHOLD 5

#define INF 100000.0

constexpr uint64_t diagonal_lines[22] = {
    0x0000000000010204ULL, 0x0000000001020408ULL, 0x0000000102040810ULL, 0x0000010204081020ULL, 0x0001020408102040ULL, 
    0x0102040810204080ULL, 
    0x0204081020408000ULL, 0x0408102040800000ULL, 0x0810204080000000ULL, 0x1020408000000000ULL, 0x2040800000000000ULL, 
    0x0000000000804020ULL, 0x0000000080402010ULL, 0x0000008040201008ULL, 0x0000804020100804ULL, 0x0080402010080402ULL,
    0x8040201008040201ULL, 
    0x4020100804020100ULL, 0x2010080402010000ULL, 0x1008040201000000ULL, 0x0804020100000000ULL, 0x0402010000000000ULL
};

int n_use_cell;
int n_use_line;
int n_possible_state;

struct Gene{
    uint64_t cell;
    uint64_t line[MAX_N_LINE];
    double score;
};

Gene genes[POPULATION];

struct Datum{
    Board board;
    double score;
};

Datum data[N_DATA];

struct Feature{
    uint64_t cell_player;
    uint64_t cell_opponent;
    bool line_player[MAX_N_LINE];
    bool line_opponent[MAX_N_LINE];
    bool line_empty[MAX_N_LINE];

    bool operator==(const Feature& other) const {
        bool res = false;
        if (cell_player == other.cell_player && cell_opponent == other.cell_opponent){
            res = true;
            for (int i = 0; i < n_use_line; ++i){
                res &= (line_player[i] == other.line_player[i]);
                res &= (line_opponent[i] == other.line_opponent[i]);
                res &= (line_empty[i] == other.line_empty[i]);
            }
        }
        return res;
    };

    bool operator<(const Feature& other) {
        if (cell_player < other.cell_player)
            return true;
        if (cell_opponent < other.cell_opponent)
            return true;
        for (int i = 0; i < n_use_line; ++i){
            if (!line_player[i] && other.line_player[i])
                return true;
            if (!line_opponent[i] && other.line_opponent[i])
                return true;
            if (!line_empty[i] && other.line_empty[i])
                return true;
        }
        return false;
    };
};

struct Feature_hash {
    size_t operator()(const Feature& x) const noexcept {
        size_t hash = 
            hash_rand_player[0][0b1111111111111111 & x.cell_player] ^ 
            hash_rand_player[1][0b1111111111111111 & (x.cell_player >> 16)] ^ 
            hash_rand_player[2][0b1111111111111111 & (x.cell_player >> 32)] ^ 
            hash_rand_player[3][0b1111111111111111 & (x.cell_player >> 48)] ^ 
            hash_rand_opponent[0][0b1111111111111111 & x.cell_opponent] ^ 
            hash_rand_opponent[1][0b1111111111111111 & (x.cell_opponent >> 16)] ^ 
            hash_rand_opponent[2][0b1111111111111111 & (x.cell_opponent >> 32)] ^ 
            hash_rand_opponent[3][0b1111111111111111 & (x.cell_opponent >> 48)];
        return hash;
    };
};

void init(){
    int i, j, k, line, offset;
    bool overlap;
    for (i = 0; i < POPULATION; ++i){
        genes[i].score = INF;
        genes[i].cell = 0ULL;
        while (pop_count_ull(genes[i].cell) != n_use_cell)
            genes[i].cell = myrand_ull();
        for (j = 0; j < n_use_line; ++j){
            line = myrandrange(0, 38);
            if (line < 8)
                genes[i].line[j] = 0xFF00000000000000ULL >> (line * HW); // horizontal
            else if (line < 16)
                genes[i].line[j] = 0x8080808080808080ULL >> (line - 8); // vertical
            else
                genes[i].line[j] = diagonal_lines[line - 16];
            overlap = false;
            for (k = 0; k < j; ++k)
                overlap |= (genes[i].line[j] == genes[i].line[k]);
            if (overlap)
                --j;
        }
    }
}

Datum convert_data(string str){
    int i, j;
    unsigned long long bk = 0, wt = 0;
    char elem;
    Board b;
    b.n = 0;
    b.parity = 0;
    for (i = 0; i < HW; ++i){
        for (j = 0; j < HW; ++j){
            elem = str[i * HW + j];
            if (elem != '.'){
                bk |= (unsigned long long)(elem == '0') << (i * HW + j);
                wt |= (unsigned long long)(elem == '1') << (i * HW + j);
                ++b.n;
            }
        }
    }
    int ai_player, score;
    ai_player = (str[65] == '0' ? 0 : 1);
    if (ai_player == 0){
        b.player = bk;
        b.opponent = wt;
    } else{
        b.player = wt;
        b.opponent = bk;
    }
    score = stoi(str.substr(67));
    if (ai_player == 1)
        score = -score;
    Datum res = {b, (double)(score + 64) / 128.0};
    return res;
}

void input_data(string dir, int start_file, int end_file){
    int i, t = 0;
    for (i = start_file; i < end_file && t < N_DATA; ++i){
        cerr << "=";
        ostringstream sout;
        sout << setfill('0') << setw(7) << i;
        string file_name = sout.str();
        ifstream ifs("data/" + dir + "/" + file_name + ".txt");
        if (ifs.fail()){
            cerr << "evaluation file not exist" << endl;
            return;
        }
        string line;
        while (getline(ifs, line)){
            data[t++] = convert_data(line);
            if (t >= N_DATA)
                break;
        }
        if (i % 20 == 19)
            cerr << endl;
    }
    cerr << endl << t << " data" << endl;
}

double calc_sd(vector<double> &lst){
    double avg = 0.0;
    for (const double &elem: lst)
        avg += elem / lst.size();
    double distribution = 0.0;
    for (const double &elem: lst)
        distribution += (avg - elem) * (avg - elem) / lst.size();
    return sqrt(distribution);
}

void scoring(Gene *gene){
    unordered_map<Feature, vector<double>, Feature_hash> features;
    int i, j;
    Feature feature;
    for (i = 0; i < N_DATA; ++i){
        feature.cell_player = data[i].board.player & gene->cell;
        feature.cell_opponent = data[i].board.opponent & gene->cell;
        for (j = 0; j < n_use_line; ++j){
            feature.line_player[j] = (data[i].board.player & ~gene->cell & gene->line[j]) > 0;
            feature.line_opponent[j] = (data[i].board.opponent & ~gene->cell & gene->line[j]) > 0;
            feature.line_empty[j] = (~(data[i].board.player | data[i].board.opponent) & ~gene->cell & gene->line[j]) > 0;
        }
        if (features.find(feature) != features.end())
            features[feature].emplace_back(data[i].score);
        else
            features.emplace(feature, (vector<double>){data[i].score});
    }
    int n_appear_state = 0;
    double avg_sd = 0.0;
    for (auto itr = features.begin(); itr != features.end(); ++itr){
        if (itr->second.size() >= SCORING_SIZE_THRESHOLD){
            ++n_appear_state;
            avg_sd += 1.0 - calc_sd(itr->second);
        }
    }
    avg_sd /= n_appear_state;
    gene->score = (double)n_appear_state / n_possible_state * avg_sd;
    //cerr << n_appear_state << " " << (double)n_appear_state / n_possible_state << " " << avg_sd << " " << gene->score << endl;
}

pair<int, double> scoring_all(){
    int i;
    double max_score = -INF;
    int max_idx = -1;
    for (i = 0; i < POPULATION; ++i){
        scoring(&genes[i]);
        cerr << "\r" << i << " " << genes[i].score;
        if (max_score < genes[i].score){
            max_score = genes[i].score;
            max_idx = i;
        }
    }
    cerr << endl;
    return make_pair(max_idx, max_score);
}

void mate(Gene *parent0, Gene *parent1, Gene *child0, Gene *child1){
    uint64_t cell_mask0 = myrand_ull(); // BUG ON THIS LINE
    uint64_t cell_mask1 = ~cell_mask0;
    child0->cell = (parent0->cell & cell_mask0) | (parent1->cell & cell_mask1);
    child1->cell = (parent0->cell & cell_mask1) | (parent1->cell & cell_mask0);
    child0->line[0] = parent0->line[0];
    child0->line[1] = parent1->line[1];
    child0->line[2] = parent0->line[2];
    child0->line[3] = parent1->line[3];
    child1->line[0] = parent1->line[0];
    child1->line[1] = parent0->line[1];
    child1->line[2] = parent1->line[2];
    child1->line[3] = parent0->line[3];
}

bool cmp_gene(Gene &x, Gene &y){
    return x.score > y.score;
}

pair<int, double> ga(pair<int, double> best){
    int parent0_idx = myrandrange(0, POPULATION);
    int parent1_idx = myrandrange(0, POPULATION);
    while (parent0_idx == parent1_idx)
        parent1_idx = myrandrange(0, POPULATION);
    Gene child0, child1;
    mate(&genes[parent0_idx], &genes[parent1_idx], &child0, &child1);
    scoring(&child0);
    scoring(&child1);
    vector<Gene> lst = {genes[parent0_idx], genes[parent1_idx], child0, child1};
    sort(lst.begin(), lst.end(), cmp_gene);
    genes[parent0_idx] = lst[0];
    genes[parent1_idx] = lst[1];
    if (lst[0].score > best.second){
        cerr << lst[0].score << endl;
        return make_pair(parent0_idx, lst[0].score);
    }
    return best;
}

int main(int argc, char *argv[]){
    n_use_cell = atoi(argv[1]);
    n_use_line = atoi(argv[2]);
    uint64_t tl = (uint64_t)atoi(argv[3]) * 60 * 1000; // minutes
    n_possible_state = pow(3, n_use_cell) * pow(8, n_use_line);
    cerr << "cell: " << n_use_cell << " line: " << n_use_line << " population: " << POPULATION << " possible_state: " << n_possible_state << " tl: " << argv[3] << " minutes" << endl;

    cerr << "initializing" << endl;
    init();
    cerr << "initialized" << endl;

    input_data("records3", 0, 10);

    pair<int, double> idx_score = scoring_all();
    cerr << endl;
    cerr << idx_score.first << " " << idx_score.second << endl;
    cerr << genes[idx_score.first].cell << " " << genes[idx_score.first].line[0] << " " << genes[idx_score.first].line[1] << " " << genes[idx_score.first].line[2] << " " << genes[idx_score.first].line[3] << endl;

    uint64_t strt = tim();
    while (tim() - strt < tl)
        idx_score = ga(idx_score);
    cerr << endl;
    cerr << idx_score.first << " " << idx_score.second << endl;
    cerr << genes[idx_score.first].cell << " " << genes[idx_score.first].line[0] << " " << genes[idx_score.first].line[1] << " " << genes[idx_score.first].line[2] << " " << genes[idx_score.first].line[3] << endl;

    return 0;
}
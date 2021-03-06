#pragma once
#include "setting.hpp"
#include "common.hpp"
#include "board.hpp"
#include "thread_pool.hpp"
#include <future>
#include <atomic>

using namespace std;

#define TRANSPOSE_TABLE_SIZE 8388608 //2097152
#define TRANSPOSE_TABLE_MASK 8388607 //2097151

#define CACHE_SAVE_EMPTY 10

#define TRANSPOSE_TABLE_UNDEFINED -INF

class Node_child_transpose_table{
    private:
        atomic<uint64_t> player;
        atomic<uint64_t> opponent;
        atomic<int> best_move;

    public:

        inline void init(){
            player.store(0ULL);
            opponent.store(0ULL);
            best_move.store(0);
        }

        inline void register_value_with_board(const Board *board, const int policy){
            player.store(board->player);
            opponent.store(board->opponent);
            best_move.store(policy);
        }

        inline void register_value_with_board(Node_child_transpose_table *from){
            player.store(from->player.load());
            opponent.store(from->opponent.load());
            best_move.store(from->best_move.load());
        }

        inline int get(const Board *board){
            int res = best_move.load(memory_order_relaxed);
            if (board->player != player.load(memory_order_relaxed) || board->opponent != opponent.load(memory_order_relaxed))
                res = TRANSPOSE_TABLE_UNDEFINED;
            return res;
        }

        inline int n_stones(){
            return pop_count_ull(player.load(memory_order_relaxed) | opponent.load(memory_order_relaxed));
        }
};


void init_child_transpose_table(Node_child_transpose_table table[], int s, int e){
    for(int i = s; i < e; ++i){
        //if (!table[i].is_null())
        table[i].init();
    }
}

void copy_child_transpose_table(Node_child_transpose_table from[], Node_child_transpose_table to[], int s, int e){
    for(int i = s; i < e; ++i){
        /*
        if (!from[i].is_null()){
            if (from[i].n_stones() < HW2 - CACHE_SAVE_EMPTY){
                if (to[i].is_null())
                    to[i].set();
                to[i].register_value_with_board(from[i].node);
            }
        } else{
            if (!to[i].is_null())
                to[i].init();
        }
        */
        to[i].register_value_with_board(&from[i]);
    }
}

class Child_transpose_table{
    private:
        Node_child_transpose_table table[TRANSPOSE_TABLE_SIZE];

    public:
        inline void first_init(){
            for(int i = 0; i < TRANSPOSE_TABLE_SIZE; ++i)
                table[i].init();
            //table[i].set_null();
        }

        inline void init(){
            int thread_size = thread_pool.size();
            int delta = (TRANSPOSE_TABLE_SIZE + thread_size - 1) / thread_size;
            int s = 0, e;
            vector<future<void>> tasks;
            for (int i = 0; i < thread_size; ++i){
                e = min(TRANSPOSE_TABLE_SIZE, s + delta);
                tasks.emplace_back(thread_pool.push(bind(&init_child_transpose_table, table, s, e)));
                s = e;
            }
            for (future<void> &task: tasks)
                task.get();
        }

        inline void reg(const Board *board, const uint32_t hash, const int policy){
            table[hash].register_value_with_board(board, policy);
        }

        inline int get(const Board *board, const uint32_t hash){
            return table[hash].get(board);
        }

        inline void copy(Child_transpose_table *to){
            int thread_size = thread_pool.size();
            int delta = (TRANSPOSE_TABLE_SIZE + thread_size - 1) / thread_size;
            int s = 0, e;
            vector<future<void>> tasks;
            for (int i = 0; i < thread_size; ++i){
                e = min(TRANSPOSE_TABLE_SIZE, s + delta);
                tasks.emplace_back(thread_pool.push(bind(&copy_child_transpose_table, table, to->table, s, e)));
                s = e;
            }
            for (future<void> &task: tasks)
                task.get();
        }
};


class Node_parent_transpose_table{
    private:
        atomic<uint64_t> player;
        atomic<uint64_t> opponent;
        atomic<int> lower;
        atomic<int> upper;

    public:

        inline void init(){
            player.store(0ULL);
            opponent.store(0ULL);
            lower.store(-INF);
            upper.store(INF);
        }

        inline void register_value_with_board(const Board *board, const int l, const int u){
            player.store(board->player);
            opponent.store(board->opponent);
            lower.store(l);
            upper.store(u);
        }

        inline void register_value_with_board(Node_parent_transpose_table *from){
            player.store(from->player);
            opponent.store(from->opponent);
            lower.store(from->lower);
            upper.store(from->upper);
        }

        inline void get(const Board *board, int *l, int *u){
            *l = lower.load(memory_order_relaxed);
            *u = upper.load(memory_order_relaxed);
            if (board->player != player.load(memory_order_relaxed) || board->opponent != opponent.load(memory_order_relaxed)){
                *l = -INF;
                *u = INF;
            }
        }

        inline bool contain(const Board *board){
            return board->player == player.load(memory_order_relaxed) && board->opponent == opponent.load(memory_order_relaxed);
        }

        inline int n_stones() const{
            return pop_count_ull(player.load(memory_order_relaxed) | opponent.load(memory_order_relaxed));
        }
};

void init_parent_transpose_table(Node_parent_transpose_table table[], int s, int e){
    for(int i = s; i < e; ++i){
        //if (!table[i].is_null())
        table[i].init();
    }
}

void copy_parent_transpose_table(Node_parent_transpose_table from[], Node_parent_transpose_table to[], int s, int e){
    for(int i = s; i < e; ++i){
        /*
        if (!from[i].is_null()){
            if (from[i].n_stones() < HW2 - CACHE_SAVE_EMPTY){
                if (to[i].is_null())
                    to[i].set();
                to[i].register_value_with_board(from[i].node);
            }
        } else{
            if (!to[i].is_null())
                to[i].init();
        }
        */
        to[i].register_value_with_board(&from[i]);
    }
}

class Parent_transpose_table{
    private:
        Node_parent_transpose_table table[TRANSPOSE_TABLE_SIZE];

    public:
        inline void first_init(){
            for(int i = 0; i < TRANSPOSE_TABLE_SIZE; ++i)
                table[i].init();
        }

        inline void init(){
            int thread_size = thread_pool.size();
            int delta = (TRANSPOSE_TABLE_SIZE + thread_size - 1) / thread_size;
            int s = 0, e;
            vector<future<void>> tasks;
            for (int i = 0; i < thread_size; ++i){
                e = min(TRANSPOSE_TABLE_SIZE, s + delta);
                tasks.emplace_back(thread_pool.push(bind(&init_parent_transpose_table, table, s, e)));
                s = e;
            }
            for (future<void> &task: tasks)
                task.get();
        }

        inline void reg(const Board *board, const uint32_t hash, const int l, const int u){
            table[hash].register_value_with_board(board, l, u);
        }

        inline void get(const Board *board, const uint32_t hash, int *l, int *u){
            table[hash].get(board, l, u);
        }

        inline bool contain(const Board *board, const uint32_t hash){
            return table[hash].contain(board);
        }

        inline void copy(Parent_transpose_table *to){
            int thread_size = thread_pool.size();
            int delta = (TRANSPOSE_TABLE_SIZE + thread_size - 1) / thread_size;
            int s = 0, e;
            vector<future<void>> tasks;
            for (int i = 0; i < thread_size; ++i){
                e = min(TRANSPOSE_TABLE_SIZE, s + delta);
                tasks.emplace_back(thread_pool.push(bind(&copy_parent_transpose_table, table, to->table, s, e)));
                s = e;
            }
            for (future<void> &task: tasks)
                task.get();
        }
};

Parent_transpose_table parent_transpose_table;
Parent_transpose_table bak_parent_transpose_table;
Child_transpose_table child_transpose_table;
Child_transpose_table bak_child_transpose_table;

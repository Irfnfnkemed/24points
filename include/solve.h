#ifndef INC_24POINTS_SOLVE_H
#define INC_24POINTS_SOLVE_H

#include <sstream>
#include <cstring>
#include <vector>

enum OP {
    ADD, SUB, MUL, DIV, NOP
};

class bitset {

private:
    u_int8_t size = 0;//选中的个数(1-based)
    u_int8_t select[32]; // select[i]==-1 表示未选中，其他 select[i]==j 表示选中的下标为i的数对应全局的bitset的下标
    int traverse_index = 1;

public:
    explicit bitset(int index_);

    ~bitset() = default;

    inline void beg_traverse() { traverse_index = 1; }

    inline void next_traverse() { ++traverse_index; }

    inline bool no_more_traverse() const { return traverse_index > (1 << size); }

    inline int get_size() const { return size; }

    inline int get_select(int index_select) const { return select[index_select]; }

    int index_traverse();
};

struct expr {
    long long val = 0;
    expr *lhs = nullptr;
    expr *rhs = nullptr;
    OP op = NOP;

    explicit expr(long long val_);

    explicit expr(expr *lhs_, expr *rhs_, OP op_);

    ~expr() = default;

    friend std::ostringstream &operator<<(std::ostringstream &os, const expr &obj);
};

class sol {
private:
    std::vector<expr *> expr_list; // 所有的合法表达式
    int index = 1; // 当前遍历到的下标

public:
    inline void beg_expr() { index = 1; }

    inline void next_expr() { ++index; }

    inline bool no_more_expr() const { return expr_list.size() < index; }

    inline expr *get_expr() { return expr_list[index - 1]; }

    inline void push_expr(expr *new_expr) { expr_list.push_back(new_expr); }

    friend std::ostringstream &operator<<(std::ostringstream &os, const sol &obj);
};

class solve_problem {
private:
    std::istringstream iss;
    std::ostringstream oss;
    int n = 0;
    int *num = nullptr;
    sol **sol_list = nullptr;
    std::vector<expr *> expr_list;

public:
    solve_problem(int n_, const char *input);

    ~solve_problem();

    void init();//接受输入

    void find_sol(int index);//找到对应的所有解，index对应bitset下选中的所有数字的下标

    void get_all_solution(char *output);//将结果写入output中
};


void solve(char *input, char *output);

#endif
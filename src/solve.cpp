#include "../include/solve.h"
#include <iostream>

bitset::bitset(int index_) {
    size = 0;
    traverse_index = 1;
    for (int i = 0; i < 32; ++i) {
        if (index_ & (1 << i)) {
            select[size++] = i;
        }
    }
}

int bitset::index_traverse() {
    int sum = 0;
    for (int i = 0; i < size; ++i) {
        if (traverse_index & (1 << i)) {
            sum |= (1 << select[i]);
        }
    }
    return sum;
}

expr::expr(long long val_) {
    val = val_;
}

expr::expr(expr *lhs_, expr *rhs_, OP op_) {
    lhs = lhs_;
    rhs = rhs_;
    op = op_;
    switch (op_) {
        case ADD:
            val = lhs_->val + rhs_->val;
            break;
        case SUB:
            val = lhs_->val - rhs_->val;
            break;
        case MUL:
            val = lhs_->val * rhs_->val;
            break;
        case DIV:
            val = lhs_->val / rhs_->val;
            break;
        default:
            val = 0;
    }
}

std::ostringstream &operator<<(std::ostringstream &os, const expr &obj) {
    if (obj.lhs == nullptr) {
        os << obj.val;
    } else {
        os << "( ";
        os << *(obj.lhs);
        switch (obj.op) {
            case ADD:
                os << " + ";
                break;
            case SUB:
                os << " - ";
                break;
            case MUL:
                os << " * ";
                break;
            case DIV:
                os << " / ";
                break;
            default:
                break;
        }
        os << *(obj.rhs);
        os << " )";
    }
    return os;
}

std::ostringstream &operator<<(std::ostringstream &os, const sol &obj) {
    bool flag = false;
    for (auto &exp: obj.expr_list) {
        if (exp->val == 24) {
            flag = true;
            os << exp->val << " = ";
            os << *exp << '\n';
        }
    }
    if (!flag) {
        os << "无解\n";
    }
    return os;
}

solve_problem::solve_problem(int n_, const char *input) : iss(input) {
    n = n_;
    num = new int[n_];
    sol_list = new sol *[1 << n_];
    for (int i = 0; i < (1 << n_); ++i) {
        sol_list[i] = nullptr;
    }
}

solve_problem::~solve_problem() {
    delete[]num;
    for (int i = 0; i < (1 << n); ++i) {
        if (sol_list[i] != nullptr) {
            delete sol_list[i];
        }
    }
    delete[]sol_list;
    for (expr *exp: expr_list) {
        delete exp;
    }
}

void solve_problem::init() {
    for (int i = 0; i < n; ++i) {
        if (!(iss >> num[i])) {
            throw std::exception();
        }
    }
    int tmp;
    if ((iss >> tmp) || !iss.eof()) {
        throw std::exception();
    }
}

void solve_problem::find_sol(int index) {
    if (sol_list[index] != nullptr) {
        return; // 已经搜索过
    }
    sol_list[index] = new sol;
    bitset bitset_new(index);
    if (bitset_new.get_size() == 1) {// 表明此时仅仅一个数，结束查找
        expr *expr_new = new expr(num[bitset_new.get_select(bitset_new.get_size() - 1)]);
        sol_list[index]->push_expr(expr_new);
        expr_list.push_back(expr_new);
        return;
    }
    int traverse_index = 0, traverse_complement_index = 0;
    sol *traverse_sol = nullptr, *traverse_complement_sol = nullptr;
    expr *expr_new = nullptr;
    bitset_new.beg_traverse();
    while (!bitset_new.no_more_traverse()) {
        traverse_index = bitset_new.index_traverse();
        traverse_complement_index = ~traverse_index & index;
        find_sol(traverse_index);
        find_sol(traverse_complement_index);
        traverse_sol = sol_list[traverse_index];
        traverse_complement_sol = sol_list[traverse_complement_index];
        traverse_sol->beg_expr();
        while (!traverse_sol->no_more_expr()) {
            traverse_complement_sol->beg_expr();
            while (!traverse_complement_sol->no_more_expr()) {
                expr_new = new expr(traverse_sol->get_expr(), traverse_complement_sol->get_expr(), ADD);
                sol_list[index]->push_expr(expr_new);
                expr_list.push_back(expr_new);
                expr_new = new expr(traverse_sol->get_expr(), traverse_complement_sol->get_expr(), SUB);
                sol_list[index]->push_expr(expr_new);
                expr_list.push_back(expr_new);
                expr_new = new expr(traverse_sol->get_expr(), traverse_complement_sol->get_expr(), MUL);
                sol_list[index]->push_expr(expr_new);
                expr_list.push_back(expr_new);
                if (traverse_complement_sol->get_expr()->val != 0 &&
                    traverse_sol->get_expr()->val % traverse_complement_sol->get_expr()->val == 0) {
                    expr_new = new expr(traverse_sol->get_expr(), traverse_complement_sol->get_expr(), DIV);
                    sol_list[index]->push_expr(expr_new);
                    expr_list.push_back(expr_new);
                }
                traverse_complement_sol->next_expr();
            }
            traverse_sol->next_expr();
        }
        bitset_new.next_traverse();
    }
}

void solve_problem::get_all_solution(char *output) {
    oss << *sol_list[(1 << n) - 1];
    std::strcpy(output, oss.str().c_str());
}

void solve(char *input, char *output) {
    solve_problem problem(4, input);
    problem.init();
    problem.find_sol(15);
    problem.get_all_solution(output);
}

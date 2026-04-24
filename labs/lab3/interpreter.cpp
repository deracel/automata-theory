#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include "ast.hpp"
#include <nlohmann/json.hpp>
#include <map>
#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include <stdexcept>
#include <cstdio>
#include <cstring>
#include <random>

using json = nlohmann::json;

extern std::map<std::string, struct SymbolInfo> global_symbols;
extern std::map<std::string, FuncDecl> functions;


struct SymbolInfo {
    VarDecl decl_;
    std::vector<int> array_value_;
    bool initialzed_ = false;
};

class Interpreter {
private:
    FILE* go_stdin = nullptr;

    std::mt19937 rng;

    int robot_x = 0;
    int robot_y = 0;
    int drone_count = 10;
    bool robot_broken = false;


    static const int FIELD_WIDTH = 10;
    static const int FIELD_HEIGHT = 10;

    std::vector<std::vector<int>> field;

    void send_to_go(const json& cmd) const {
        if (go_stdin) {
            std::string cmd_str = cmd.dump();
            fprintf(go_stdin, "%s\n", cmd_str.c_str());
            fflush(go_stdin);
        }
    }

    struct Value {
        enum Type { INT, BOOL, CELL, UNDEF };
        Type type_;
        int int_val_ = 0;
        bool bool_val_ = false;
        CellValue cell_val_ = CellValue::UNDEF;
    };
    Value eval_expr(expr_ptr node) {
        if (!node) {
            throw std::runtime_error("Expression == NULL");
        }
        switch (node->type_) {
        case ExprNode::INT_LIT: {
            Value val;
            val.type_ = Value::INT;
            val.int_val_ = node->int_value_;
            return val;
        }
        case ExprNode::BOOL_LIT: {
            Value v;
            v.type_ = Value::BOOL;
            v.bool_val_ = node->bool_value_;
            return v;
        }

        case ExprNode::CELL_LIT: {
            Value v;
            v.type_ = Value::CELL;
            v.cell_val_ = node->cell_value_;
            return v;
        }
        case ExprNode::VAR_REF:
            return eval_var_ref(node);
        }
        case ExprNode::ADD: {
            Value result;
            result.type_ = Value::INT;
            result.int_val_ = 0;
            for (auto& arg : node->args_) {
                result.int_val_ += eval_expr(arg).int_val_;
            }
            return result;
        }
        case ExprNode::MUL: {
            Value result;
            result.type_ = Value::INT;
            result.int_val_ = 1;
            for (auto& arg : node->args_) {
                result.int_val_ *= eval_expr(arg).int_val_;
            }
            return result;
        }
        case ExprNode::SUB: {
            Value v1 = eval_expr(node->args_[0]);
            Value v2 = eval_expr(node->args_[1]);
            Value result;
            result.type_ = Value::INT;
            result.int_val_ = v1.int_val_ - v2.int_val_;
            return result;
        }
        case ExprNode::DIV: {
            Value v1 = eval_expr(node->args_[0]);
            Value v2 = eval_expr(node->args_[1]);
            Value result;
            result.type_ = Value::INT;
            result.int_val_ = v1.int_val_ / v2.int_val_;
            return result;
        }
        case ExprNode::AND: {
            Value result;
            result.type_ = Value::BOOL;
            result.bool_val_ = true;
            for (auto& arg : node->args_) {
                if (!eval_expr(arg).bool_val_) {
                    result.bool_val_ = false;
                    break;
                }
            }
            return result;
        }
        case ExprNode::OR: {
            Value result;
            result.type_ = Value::BOOL;
            result.bool_val_ = false;
            for (auto& arg : node->args_) {
                if (eval_expr(arg).bool_val_) {
                    result.bool_val_ = true;
                    break;
                }
            }
            return result;
        }
        case ExprNode::NOT: {
            Value v = eval_expr(node->args_.front());
            Value result;
            result.type_ = Value::BOOL;
            result.bool_val_ = !v.bool_val_;
            return result;
        }
    }
};


#endif
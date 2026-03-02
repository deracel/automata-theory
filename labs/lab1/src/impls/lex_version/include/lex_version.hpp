#pragma once
#include "i_lexer.hpp"
#include <string>
#include <vector>
#include <utility>
#include <memory>
#include <iostream>  // для отладки

struct yy_buffer_state;

enum class LexState {
    INITIAL,
    CREATE,
    NAME,
    AS,
    JOIN,
    LPAREN,
    ATTR,
    COMMA,
    RPAREN,
    AFT_REXP,
    ERROR,
    SUCCESS
};

// Глобальные переменные из lex файла
extern int yylex();
extern void yy_switch_to_buffer(yy_buffer_state*);
extern yy_buffer_state* yy_scan_string(const char*);
extern void yy_delete_buffer(yy_buffer_state*);

extern std::string current_token;        // из lex файла
extern LexState current_state_;           // из lex файла
extern std::vector<std::string> attributes; // из lex файла
extern STATE predicted_state_;             // из lex файла
extern std::string exp_name_;

class lex_version : public i_lexer {
private:
    // Локальные поля класса (не путать с глобальными переменными!)
    std::string name_;
    LexState state_;
    std::vector<std::string> attributes_;
    STATE predicted_;

    void reset() {
        // Сбрасываем локальные поля
        name_.clear();
        attributes_.clear();
        state_ = LexState::INITIAL;
        predicted_ = STATE::NO;

        // ВАЖНО: также сбрасываем глобальные переменные!
        current_token.clear();
        current_state_ = LexState::INITIAL;
        attributes.clear();
        predicted_state_ = STATE::NO;
        exp_name_.clear();
    }

public:
    lex_version() : state_(LexState::INITIAL), predicted_(STATE::NO) {}

    std::pair<STATE, std::pair<std::string, std::vector<std::string>>>
    lexline(const std::string& line) override {

        // Сбрасываем состояние
        reset();
        // Отладка: показываем входящую строку
        //std::cout << "[DEBUG] Анализ строки: '" << line << "'" << std::endl;

        // Создаем буфер Flex
        yy_buffer_state* buffer = yy_scan_string(line.c_str());

        // Запускаем лексический анализ
        int res;
        int token_count = 0;
        while ((res = yylex()) > 0) {
            token_count++;
            //std::cout << "[DEBUG] Токен " << token_count << ": " << current_token << std::endl;
        }

        // Очищаем буфер
        yy_delete_buffer(buffer);

        //std::cout << "[DEBUG] Всего токенов: " << token_count << std::endl;
        //std::cout << "[DEBUG] Финальное состояние: " << static_cast<int>(current_state_) << std::endl;
        //std::cout << "[DEBUG] Собрано атрибутов: " << attributes.size() << std::endl;

        // Формируем результат ИЗ ГЛОБАЛЬНЫХ ПЕРЕМЕННЫХ
        std::pair<std::string, std::vector<std::string>> result_data;

        if (current_state_ == LexState::SUCCESS) {
            // Успешный разбор
            result_data.first = exp_name_;
            result_data.second = attributes;

            // Сохраняем в локальные поля (опционально)
            name_ = current_token;
            attributes_ = attributes;
            state_ = current_state_;
            predicted_ = predicted_state_;

            //std::cout << "[DEBUG] УСПЕХ!" << std::endl;
        } else {
            result_data.first = "";
            result_data.second = {};
            predicted_state_ = STATE::NO;

            // ВАЖНО: Явно сбрасываем глобальные переменные после ошибки!
            current_state_ = LexState::INITIAL;
            current_token.clear();
            attributes.clear();
            exp_name_.clear();
        }


        // Возвращаем результат
        return {predicted_state_, result_data};
    }
};

%{
#include <string>
#include <vector>
#include <iostream>
#include "../include/lex_version.hpp"

// Глобальные переменные для взаимодействия с C++
std::string current_token;
LexState current_state_ = LexState::INITIAL;
std::vector<std::string> attributes;
STATE predicted_state_ = STATE::NO;
std::string exp_name_;
// Вспомогательная функция для обновления токена
void set_token(const char* text) {
    current_token = text;
}

%}

%option noyywrap
%x St_CREATE St_NAME St_AS St_JOIN St_LPAREN St_ATTR St_COMMA St_RPAREN St_R_ATTR
%%

    /* Начальное состояние - ожидаем CREATE */
<INITIAL>{
    (?i:create)[ \t]+    {
        set_token(yytext);
        current_state_ = LexState::CREATE;
        BEGIN(St_CREATE);
        return 1;
    }
    (?i:create)    {
            set_token(yytext);
            current_state_ = LexState::ERROR;
            BEGIN(INITIAL);
            return 0;
    }

    [ \t]+         { /* ignore */ }
    \n             { /* ignore */ }
    .              {
        set_token(yytext);
        current_state_ = LexState::ERROR;
        BEGIN(INITIAL);
        return 0;
    }
}

    /* После CREATE - ожидаем имя отношения */
<St_CREATE>{
    [ \t]+         { /* ignore */ }
    [a-zA-Z._][a-zA-Z0-9._]* {
        set_token(yytext);
        current_state_ = LexState::NAME;
        exp_name_ = yytext;
        BEGIN(St_NAME);
        return 1;
    }
    .              {

        set_token(yytext);
        current_state_ = LexState::ERROR;
        BEGIN(INITIAL);
        return 0;
    }
}

    /* После имени - может быть ( или AS */
<St_NAME>{
    [ \t]+         { /* ignore */ }
    "("            {
        set_token(yytext);
        current_state_ = LexState::LPAREN;
        predicted_state_ = STATE::NEW_EXP;
        BEGIN(St_LPAREN);
        return 1;
    }
    (?i:as)[ \t]        {
        set_token(yytext);
        current_state_ = LexState::AS;
        predicted_state_ = STATE::COMBINE_EXP;
        BEGIN(St_AS);
        return 1;
    }
    .              {
        set_token(yytext);
        current_state_ = LexState::ERROR;
        BEGIN(INITIAL);
        return 0;
    }
}

    /* В режиме CREATE TABLE - после ( */
<St_LPAREN>{
    [ \t]+         { /* ignore */ }
    [a-zA-Z._][a-zA-Z0-9._]* {
        set_token(yytext);
        attributes.push_back(yytext);
        current_state_ = LexState::ATTR;
        BEGIN(St_ATTR);
        return 1;
    }
    .              {
        set_token(yytext);
        current_state_ = LexState::ERROR;
        BEGIN(INITIAL);
        return 0;
    }
}

    /* После атрибута - ожидаем , или ) */
<St_ATTR>{
    [ \t]+         { /* ignore */ }
    ","            {
        set_token(yytext);
        current_state_ = LexState::COMMA;
        BEGIN(St_COMMA);
        return 1;
    }
    ")"            {
        set_token(yytext);
        current_state_ = LexState::RPAREN;
        BEGIN(St_RPAREN);
        return 1;
    }
    .              {
        set_token(yytext);
        current_state_ = LexState::ERROR;
        BEGIN(INITIAL);
        return 0;
    }
}

    /* После запятой - ожидаем следующий атрибут */
<St_COMMA>{
    [ \t]+         { /* ignore */ }
    [a-zA-Z._][a-zA-Z0-9._]* {
        set_token(yytext);
        attributes.push_back(yytext);
        current_state_ = LexState::ATTR;
        BEGIN(St_ATTR);
        return 1;
    }
    .              {
        set_token(yytext);
        current_state_ = LexState::ERROR;
        BEGIN(INITIAL);
        return 0;
    }
}


<St_RPAREN>{
        [ \t]+         { /* игнорируем пробелы */ }
        \n             {
            set_token("COMPLETE");
            current_state_ = LexState::SUCCESS;
            BEGIN(INITIAL);
            return 1;
        }
        <<EOF>>        {
            set_token("COMPLETE");
            current_state_ = LexState::SUCCESS;
            BEGIN(INITIAL);
            return 1;
        }
        .              {
            set_token(yytext);
            current_state_ = LexState::ERROR;
            BEGIN(INITIAL);
            return 0;
        }
}

    /* В режиме CREATE JOIN - после AS */
<St_AS>{
    [ \t]+         { /* ignore */ }
    [a-zA-Z._][a-zA-Z0-9._]* {
        set_token(yytext);
        attributes.push_back(yytext);  // Левое отношение
        current_state_ = LexState::JOIN;
        BEGIN(St_JOIN);
        return 1;
    }
    .              {
        set_token(yytext);
        current_state_ = LexState::ERROR;
        BEGIN(INITIAL);
        return 0;
    }
}

    /* После левого отношения - ожидаем JOIN */
<St_JOIN>{
    [ \t]+         { /* ignore */ }
    (?i:join)[ \t]      {
        set_token(yytext);
        current_state_ = LexState::ATTR;  // Для правого отношения
        BEGIN(St_R_ATTR);
        return 1;
    }
    .              {
        set_token(yytext);
        // Если после JOIN нет правого отношения, это ошибка
        current_state_ = LexState::ERROR;
        BEGIN(INITIAL);
        return 0;
    }
}

    /* После левого отношения - ожидаем JOIN */
<St_R_ATTR>{
    [ \t]+         { /* ignore */ }
    [a-zA-Z._][a-zA-Z0-9._]* {
        set_token(yytext);
        attributes.push_back(yytext);  // Левое отношение
        current_state_ = LexState::AFT_REXP;
        BEGIN(St_RPAREN);
        return 1;
    }
    .              {
        set_token(yytext);
        current_state_ = LexState::ERROR;
        BEGIN(INITIAL);
        return 0;
    }
}

%%

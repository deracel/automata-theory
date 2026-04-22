#ifndef ROBOTLEXER_H
#define ROBOTLEXER_H

#if !defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

#include <string>
#include <iostream>
#include <sstream>
#include "robot.tab.hpp"


// ВАЖНО: класс в глобальном пространстве для Flex
namespace yy {
    class RobotLexer : public yyFlexLexer {
    public:
        RobotLexer(std::istream& input) : yyFlexLexer(&input) {}
        RobotLexer(const std::string& input) : yyFlexLexer() {
            iss_ = new std::istringstream(input);
            switch_streams(iss_);
        }
        ~RobotLexer() {
            if (iss_) delete iss_;
        }
        int yylex(yy::RobotParser::semantic_type* yylval, yy::location* loc);



    private:
        std::istringstream* iss_ = nullptr;
    };

}

#endif
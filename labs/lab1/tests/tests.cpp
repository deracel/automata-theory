#include <fstream>
#include <gtest/gtest.h>
#include <memory>

#include "regex_version.hpp"

class parser_test : public ::testing::Test {
    public:
        static std::unique_ptr<regex_version> regex_parser_;
};
class parser_valid_test : public parser_test, public ::testing::WithParamInterface<std::string>{};
class parser_invalid_test : public parser_test, public ::testing::WithParamInterface<std::string>{};

std::unique_ptr<regex_version> parser_test::regex_parser_ = std::make_unique<regex_version>();

TEST_P(parser_valid_test, valid_strings) {
    std::string valid_input = GetParam();
    regex_parser_->terminal_parser(valid_input);
    EXPECT_TRUE(regex_parser_->get_parser_state());
}

TEST_P(parser_invalid_test, invalid_strings) {
    std::string invalid_input = GetParam();
    regex_parser_->terminal_parser(invalid_input);
    EXPECT_FALSE(regex_parser_->get_parser_state());
}


INSTANTIATE_TEST_SUITE_P(
    valid_strings,
    parser_valid_test,
    ::testing::Values(
        "create deracel (wfed, d, werf, g)",
        "create somth (ef1,)",
        "create abs123 (a,1,2,3,4)",
        "create n (a, d1, t, d2, wf, r2, f2, f2, a, b, c, d, e_, q, t, e)",
        "create n_o_1 (a)",
        "create ollolo (     a,       r,      q,      t,)",
        "create hallo         (a,b,                   r,d,        q)",
        "create              tutut (a,b,c,sd,      e,   q,  t_)",
        "create                       totot                   (a,            e,               t,          q)",
        "create r_r (a,b,c,d,e,a,e,)",
        "create ,, (a)",
        "create __ (__, ___, _____)"
    )
);

INSTANTIATE_TEST_SUITE_P(
    invalid_strings,
    parser_invalid_test,
    ::testing::Values(
        "lev (1,2,3)",
        "createe __ (fwew)",
        "create 1wfefw (a)",
        "create wfe? (a)",
        "create we@ (a)",
        "create wew- (a)",
        "create wdqwwe+ (a)",
        "create aefwe= (a)",
        "create fe~ (a),"
        "create wfe`wfew (a)",
        "create ef* (a)",
        "create wde! (a)",
        "create wfe# (fwfe)",
        "create ewf$ (a)",
        "create wfe% (A)",
        "create ewf; (a)",
        "create erg: (a)",
        "create wef^ (a)",
        "create wef& (a)",
        "create wef( (a)",
        "create ewfw) (a)",
        "create afew\" (a)",
        "create abs| (a)",
        "create a/efw (a)",
        "create abs[ (a)",
        "create [a (a)",
        "create [ (a)",
        "create a] (a)",
        "create pwew{ (a)",
        "create wefw} (a)",
        "create wfef { (a)",
        "create abc(a)",
        "createabs (a)",
        "create abs (1a)",
        "create abs (a\"e)",
        "create abs (a, b, c`)",
        "create abs (a~f)",
        "create abs (a!)",
        "create abs (a@)",
        "create ans (a\")",
        "create ",
        "create as (\")",
        "create wf wef, a, f",
        "create bas (a#)",
        "create bas (a№)",
        "create ewf (wf;)",
        "create ewrf (ar$)",
        "create ewf )ewqfw(",
        "create wef (wef%)",
        "create ewfe (ewf^)",
        "create efwf (aerwf:)",
        "create wef (wef?)",
        "create gerg (wef&)",
        "create ewfew (wfe*)",
        "create ewf ()",
        "create ewf (    )",
        "create wefr (ewf()",
        "create wef (wef))",
        "create wef (wqdq-)",
        "create gre (wefwf=)",
        "create erwf (wfwferw+)",
        "create wefw (adwed|)",
        "create reg (wef/)",
        "create wefw (wefw)_",
        "create wef (wef)wfe",
        "create wefew> (weef)",
        "create wefw< (wefw)",
        "create wefew (wdf>)",
        "create ewfe (wef<ewf)",
        "create ewf (wfe, a, bv, e, q, ewf>)"
    )
);
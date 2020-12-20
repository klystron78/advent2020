#include <iostream>
#include <sstream>
#include <variant>
#include <vector>
#include <map>
#include <assert.h>
#include <tuple>
#include <regex>

namespace {
    using index_t = int;
    using char_t = char;
    using message_t = std::string;
    using rule_single_t = std::tuple<index_t>;
    using rule_double_t = std::tuple<index_t, index_t>;
    using rule_single_or_t = std::tuple<rule_single_t, rule_single_t>;
    using rule_double_or_t = std::tuple<rule_double_t, rule_double_t>;
    using rule_t = std::variant<rule_double_or_t, rule_single_or_t, rule_double_t, rule_single_t, char_t>;
    using rules_t = std::map<index_t, rule_t>;
    using messages_t = std::vector<std::string>;

    template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
    template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

    int debug_counter=0;

    std::string build_regex_rule(const rules_t &rules, index_t rule)
    {
        std::string ret;
        auto it = rules.find(rule);
        assert(it!=rules.end());
        std::visit(overloaded {
            [&ret](char arg) {
                ret += arg;
            },

            [&ret, &rules](const rule_single_t &arg) {
                ret += '(';
                ret += build_regex_rule(rules, std::get<0>(arg));
                ret += ')';
            },

            [&ret, &rules, rule](const rule_double_t &arg) {
                if(rule==11) {
                    /* bare minimum effort: special case for rule 11 */
                    ret += '(';
                    for(int i=1; i<4; ++i) {
                        ret += '(';
                        for(int j=0; j<i; ++j) ret += build_regex_rule(rules, std::get<0>(arg));
                        ret += "((";
                        ret += build_regex_rule(rules, std::get<0>(arg));
                        ret += build_regex_rule(rules, std::get<1>(arg));
                        ret += ")*)";
                        for(int j=0; j<i; ++j) ret += build_regex_rule(rules, std::get<1>(arg));
                        ret += ")|";
                    }
                    ret[ret.size()-1] = ')'; /* clobber the final | */
                }
                else if(rule==8) {
                    /* bare minimum effort: special case for rule 8 */
                    ret += "((";
                    ret += build_regex_rule(rules, std::get<0>(arg));
                    ret += ")+)";
                }
                else {
                    ret += '(';
                    ret += build_regex_rule(rules, std::get<0>(arg));
                    ret += build_regex_rule(rules, std::get<1>(arg));
                    ret += ')';
                }
            },

            [&ret, &rules](const rule_single_or_t &arg) {
                ret += "((";
                ret += build_regex_rule(rules, std::get<0>(std::get<0>(arg)));
                ret += ")|(";
                ret += build_regex_rule(rules, std::get<0>(std::get<1>(arg)));
                ret += "))";
            },

            [&ret, &rules](const rule_double_or_t &arg) {
                ret += "((";
                ret += build_regex_rule(rules, std::get<0>(std::get<0>(arg)));
                ret += build_regex_rule(rules, std::get<1>(std::get<0>(arg)));
                ret += ")|(";
                ret += build_regex_rule(rules, std::get<0>(std::get<1>(arg)));
                ret += build_regex_rule(rules, std::get<1>(std::get<1>(arg)));
                ret += "))";
            }
        }, it->second);
        return ret;
    }

    std::string build_regex(const rules_t &rules)
    {
        std::string ret("^");

        ret += build_regex_rule(rules, 0);
        return ret;
    }
}

int main()
{
    std::string line;
    rules_t rules;
    messages_t messages;

    messages.reserve(1024); /* seems like enough */

    /* parse the rules */
    while(std::getline(std::cin, line)&&line.size()) {
        std::istringstream istr(line);
        index_t index;

        istr >> index;

        if(line[line.size()-1]=='"') {
            rules[index] = {line[line.size()-2]};
        }
        else {
            index_t values[5];
            size_t i=0;
            size_t pipe_index=0;

            istr.ignore(2);

            while(i<5) {
                istr >> values[i++];
                istr.ignore();
                if(!istr.good()) break;
                if(istr.peek()=='|') {
                    pipe_index=i;
                    istr.ignore(2);
                }
            }

            assert((pipe_index==0)||(pipe_index==1)||(pipe_index==2));
            if(pipe_index) {
                /* double rule (or) */
                assert((i>=2));
                if(i==2) {
                    rules[index] = std::make_tuple(std::make_tuple(values[0]), std::make_tuple(values[1]));
                }
                else if(i==3) {
                    /* special case for rule 8 */
                    assert(index==8);
                    rules[index] = std::make_tuple(values[0], 8);
                }
                else if(i==4) {
                    rules[index] = std::make_tuple(std::make_tuple(values[0], values[1]), std::make_tuple(values[2], values[3]));
                }
                else if(i==5) {
                    /* special case for rule 11 */
                    assert(index==11);
                    rules[index] = std::make_tuple(values[0], values[1]);
                }
            }
            else {
                /* single rule */
                assert((i==1)||(i==2));
                if(i==1) {
                    rules[index] = std::make_tuple(values[0]);
                }
                else { /* could check for 2 here, but assert will catch in debug build */
                    rules[index] = std::make_tuple(values[0], values[1]);
                }
            }
        }
    }

    /* parse the messages */
    while(std::getline(std::cin, line)) {
        messages.emplace_back(line);
    }

    /* build regex */
    std::regex r_s(build_regex(rules));

    int sum=0;
    for(const auto &m : messages) {
        sum += std::regex_match(m, r_s);
    }

    std::cout << "sum is " << sum << std::endl;
    return 0;
}


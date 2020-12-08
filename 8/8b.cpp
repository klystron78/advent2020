#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <cassert>

namespace {
enum opcode_t : std::uint16_t {
    nop,
    jmp,
    acc
};
using operand_t = std::int16_t;
using is_t = std::vector<std::tuple<opcode_t, operand_t, int /* number of times executed */>>;
}

int main()
{
    const std::map<std::string, opcode_t> asm_map{{"nop", nop}, {"jmp", jmp}, {"acc", acc}};
    const std::map<char, std::int16_t> sign_map{{'+', 1}, {'-', -1}};
    std::string line;
    is_t is;

    while(std::getline(std::cin, line)) {
        std::istringstream istr(line);
        std::string opc;
        char sign;
        std::int16_t operand;

        istr >> opc >> sign >> operand;
        is.emplace_back(asm_map.find(opc)->second, sign_map.find(sign)->second*operand, 0);
    }

    /* run until the first time an instruction is executed twice */
    for(int swap=0;swap<is.size();++swap) {
        opcode_t o = std::get<0>(is[swap]);
        if(o==opcode_t::acc) continue;
        is_t ist(is);

        std::get<0>(ist[swap]) = (o==opcode_t::jmp)?opcode_t::nop:opcode_t::jmp;

        std::int16_t acc=0;
        int pc=0;

        for(;;) {
            if(pc==is.size()) {
                /* reached the end! */
                std::cout << "acc is " << acc << std::endl;
                return 0;
            }

            auto &t = ist[pc];
            auto &c = std::get<2>(t);

            if(c) {
                /* executing a second time */
                break;
            }

            c = 1;

            switch(std::get<0>(t)) {
                case opcode_t::jmp:
                    pc += std::get<1>(t);
                    break;
                case opcode_t::acc:
                    acc += std::get<1>(t);
                    /* fall through */
                case opcode_t::nop:
                    ++pc;
                    break;
            }
        }
    }

    return 1;
}


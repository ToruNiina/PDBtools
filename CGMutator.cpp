#include "includes/CGMutator.hpp"
using namespace arabica;

int main(int argc, char *argv[])
{
    std::ifstream ifs(argv[1]);
    std::ofstream ofs("out.pdb");

    std::cout << "PDB CG(CafeMol) style Mutator" << std::endl;            
    std::cout << "please input sequence that you want." << std::endl;            
    std::cout << "letter = means the residue does not change" << std::endl;
    std::cout << "sequence of input PDB file indicated below." << std::endl;
    std::cout << "Note: only mutation is supported. insertion, elongation, deletion is not supported yet." << std::endl;
    std::cout << std::endl;

    CGChnSptr chain(new CGChain);
    chain->read_block(ifs);
    std::string input_sequence(chain->get_sequence());

    std::string mutated_sequence;

    if(input_sequence.size() < 80)
    {
        std::cout << "<<<< " << input_sequence << std::endl;
        std::cout << ">>>> ";
        std::cin >> mutated_sequence;
    }
    else
    {
        std::string test;
        size_t current_pos(0);
        std::string buffer;
        for(int full_length(input_sequence.size()); full_length > 80; full_length -= 80)
        {
            std::cout << "<<<< " << input_sequence.substr(current_pos, 80) << std::endl;
            std::cout << ">>>> ";
            std::cin  >> buffer;
            mutated_sequence += buffer;
            test += input_sequence.substr(current_pos, 80);
            current_pos += 80;
        }
        std::cout << "<<<< " << input_sequence.substr(current_pos) << std::endl;
        std::cout << ">>>> ";
        std::cin  >> buffer;
        mutated_sequence += buffer;
        test += input_sequence.substr(current_pos);
        if(test != input_sequence) throw std::invalid_argument("test != input_sequence");
    }

    CGMutator mut(chain, mutated_sequence);
    std::cout << "mutator constructed" << std::endl;
    mut.mutateDNA();
    std::cout << "mutation completed" << std::endl;
    mut.output(ofs);
    std::cout << "output completed" << std::endl;

    return 0;
}

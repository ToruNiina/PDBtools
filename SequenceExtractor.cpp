#include "includes/PDBAtom.hpp"
#include <boost/regex.hpp>
//g++ -o seqextr -std=c++11 SequenceExtractor.cpp -lboost_regex
using namespace arabica;

int main(int argc, char* argv[])
{
    if(argc != 2 && argc != 3)
    {
        std::cout << "Usage : ./seqextr [OPTION] [filename].pdb" << std::endl;
        std::cout << "Option: -cg : read cg style pdb file" << std::endl;
        return -1;
    }

    std::ifstream pdbfile(argv[1]);

    int current_resSeq(-1);
    std::string seq;
    while(!pdbfile.eof())
    {
        AtomSptr atom(new PDBAtom);
        LINE_TYPE lt(atom->getAtomLine(pdbfile));
        if(lt == ATOM)
        {
            boost::regex seqdna("D[ACGT]");//, std::regex_constants::grep
            std::string resName(atom->get_resName());
            int resSeq(atom->get_resSeq());
            if(current_resSeq == resSeq) continue;
            else current_resSeq = resSeq;
            
            if(boost::regex_search(resName, seqdna))
            {
                size_t dpos(resName.find('D'));
                seq += resName[dpos+1];
            }else{
                resName += ' ';
                seq += resName;
            }
        }
        else if(lt == TER)
        {
            std::cout << "Chain: " << seq << std::endl;
            std::cout << "TER" << std::endl;
            seq.erase();
        }
        else if(lt == END)
        {
            std::cout << "Chain: " << seq << std::endl;
            std::cout << "END" << std::endl;
            seq.erase();
        }
        else
        {
            continue;
        }
    }
    return 0;
}

#ifndef ARABICA_CG_MUTATOR
#define ARABICA_CG_MUTATOR
#include "CGChain.hpp"

namespace arabica
{
    class CGMutator
    {
        std::string mutated_seq;

        CGChnSptr input;
        CGChnSptr mutated;

    public:

        CGMutator(CGChnSptr chain): input(chain) {}
        CGMutator(CGChnSptr chain, std::string seq)
            : input(chain), mutated_seq(seq) {}
        ~CGMutator(){}

        void set_seq(std::string seq){mutated_seq = seq;}
        std::string get_seq() const {return mutated_seq;}

        void mutateDNA();
        void output(std::ofstream& ofs){mutated->write_block(ofs);}

    private:

        void no_mut(std::vector<BeadSptr>::iterator& iter,
                    std::vector<BeadSptr>::iterator& end);
        void mut_to_A(std::vector<BeadSptr>::iterator& iter,
                      std::vector<BeadSptr>::iterator& end);
        void mut_to_C(std::vector<BeadSptr>::iterator& iter,
                      std::vector<BeadSptr>::iterator& end);
        void mut_to_G(std::vector<BeadSptr>::iterator& iter,
                      std::vector<BeadSptr>::iterator& end);
        void mut_to_T(std::vector<BeadSptr>::iterator& iter,
                      std::vector<BeadSptr>::iterator& end);
        void mut_prot(const char aacode,
                      std::vector<BeadSptr>::iterator& start,
                      std::vector<BeadSptr>::iterator& end);
    };

    void CGMutator::mutateDNA()
    {
        if(mutated_seq.empty())
        {
            std::cout << "CGMutator has no input sequence" << std::endl;
            throw std::invalid_argument("invalid sequence");
        }
        
        std::vector<BeadSptr> chain(input->get_chain());

        if(mutated_seq.size() != input->get_ResNum())
        {
            std::cout << "mut_seq: " << mutated_seq.size() << std::endl;
            std::cout << "chain  : " << chain.size() << std::endl;
            std::cout << "mut    : " << mutated_seq << std::endl;
            std::cout << "input  : " << input->get_sequence() << std::endl;
            
            throw std::invalid_argument(
                    "length of sequene and chain size is not equal");
        }

        std::vector<BeadSptr>::iterator iter = chain.begin();
        std::vector<BeadSptr>::iterator end = chain.end();

        for(size_t current_seq(0); current_seq < mutated_seq.size();
                ++current_seq)
        {
            switch(mutated_seq[current_seq])
            {
            case '=':
                no_mut(iter, end);
                break;
            case 'A':
                mut_to_A(iter, end);
                break;
            case 'C':
                mut_to_C(iter, end);
                break;
            case 'G':
                mut_to_G(iter, end);
                break;
            case 'T':
                mut_to_T(iter, end);
                break;
            }
        }

        mutated = std::shared_ptr<CGChain>(new CGChain(chain));

        return;
    }

    void CGMutator::no_mut(std::vector<BeadSptr>::iterator& iter,
                           std::vector<BeadSptr>::iterator& end)
    {
        int residue_number((*iter)->get_iResNum());
        while(true)
        {
            if(iter == end) return;

            if((*iter)->get_iResNum() == residue_number)
            {
                if(iter != end) ++iter;
            }else{
                return;
            }
        }
    }

    void CGMutator::mut_to_A(std::vector<BeadSptr>::iterator& iter,
                             std::vector<BeadSptr>::iterator& end)
    {
        int residue_number((*iter)->get_iResNum());
        while(true)
        {
            if(iter == end) return;

            if((*iter)->get_iResNum() == residue_number)
            {
                (*iter)->set_seq("DA");
                if(iter != end) ++iter;
            }else{
                return;
            }
        }
    }

    void CGMutator::mut_to_C(std::vector<BeadSptr>::iterator& iter,
                             std::vector<BeadSptr>::iterator& end)
    {
        int residue_number((*iter)->get_iResNum());
        while(true)
        {
            if(iter == end) return;

            if((*iter)->get_iResNum() == residue_number)
            {
                (*iter)->set_seq("DC");
                if(iter != end) ++iter;
            }else{
                return;
            }
        }
    }

    void CGMutator::mut_to_G(std::vector<BeadSptr>::iterator& iter,
                             std::vector<BeadSptr>::iterator& end)
    {
        int residue_number((*iter)->get_iResNum());
        while(true)
        {
            if(iter == end) return;

            if((*iter)->get_iResNum() == residue_number)
            {
                (*iter)->set_seq("DG");
                if(iter != end) ++iter;
            }else{
                return;
            }
        }
    }

    void CGMutator::mut_to_T(std::vector<BeadSptr>::iterator& iter,
                             std::vector<BeadSptr>::iterator& end)
    {
        int residue_number((*iter)->get_iResNum());
        while(true)
        {
            if(iter == end) return;

            if((*iter)->get_iResNum() == residue_number)
            {
                (*iter)->set_seq("DT");
                if(iter != end) ++iter;
            }else{
                return;
            }
        }
    }

}
#endif//ARABICA_CG_MUTATOR

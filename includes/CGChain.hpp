#ifndef ARABICA_CG_CHAIN
#define ARABICA_CG_CHAIN
#include <vector>
#include <algorithm>
#include <boost/regex.hpp>
#include "CGBeads.hpp"
#include "ProteinSeq.hpp"

namespace arabica
{
    class CGChain
    {
        MOLECULE_TYPE mol_type;
        char ChainID;
        int iunit;
        std::vector<BeadSptr> residue;
        std::string sequence;
        ProteinSeq seqmap;

    public:
        CGChain(){}

        CGChain(const BeadSptr& bead) : ChainID(bead->get_ChainID()), seqmap()
        {
            residue.push_back(bead);
        }

        CGChain(std::vector<BeadSptr>& beads)
            : ChainID(beads.at(0)->get_ChainID()), residue(beads), seqmap()
        {
            ;
        }

        ~CGChain(){}

        void push_back(BeadSptr& bead);

        void read_block(std::ifstream& ifs);
        void write_block(std::ofstream& ofs);

        std::vector<BeadSptr> get_chain(){return residue;}

        void sort_imp();

        char get_ChainID() const {return ChainID;}
        int get_iunit() const {return iunit;}
        int get_ResNum() const {return (*(residue.end()-1))->get_iResNum();}
        std::string get_sequence() const {return sequence;}
        MOLECULE_TYPE get_moltype() const {return mol_type;}

    private:

        void read_sequence();
        
    };

    void CGChain::push_back(BeadSptr& bead)
    {
        if(bead->get_ChainID() != ChainID)
        {
            std::cout << "Error: trying to push_back bead having different ChainID"
                      << std::endl;
            throw std::invalid_argument("invalid ChainID");
        }
        residue.push_back(bead);
        return;
    }

    void CGChain::read_block(std::ifstream& ifs)
    {
        bool block_found(false);
        bool read_id(false);
        while(!ifs.eof())
        {
            std::string line;
            std::getline(ifs, line);

            if(line.substr(0,6) == "ATOM  " ||
               line.substr(0,6) == "HEATOM")
            {
                BeadSptr bead(new CGBead);
                bead->get_line(line);
                if(!read_id)
                {
                    ChainID = bead->get_ChainID();
                    read_id = true;
                }
                push_back(bead);
                continue;
            }

            if(line.substr(0,2) == "<<")
            {
                if(block_found)
                    throw std::invalid_argument(
                            "CG PDB block is not closed correctly");
                block_found = true;
                continue;
            }

            if(line.substr(0,2) == ">>")
            {
                read_sequence();
                return;
            }

            if(line.substr(0,6) == "ENDMDL")
            {
                if(block_found)
                {
                    std::cout << "ENDMDL found." << std::endl;
                    throw std::invalid_argument(
                            "CG PDB block is not closed correctly");
                }
            }

            if(line.substr(0,5) == "MODEL")
            {
                if(block_found)
                {
                    std::cout << "MODEL found." << std::endl;
                    throw std::invalid_argument(
                            "CG PDB block is not closed correctly");
                }
            }
#ifdef _DEBUG
            std::cout << "unknown line: " << line << std::endl;
#endif//_DEBUG
        }

        if(block_found)
        {
            std::cout << "cannot find end of block >>" << std::endl;
            throw std::invalid_argument("CG PDB block does not close correctly");
        }
        read_sequence();
        return;
    }

    void CGChain::sort_imp()
    {
        std::sort(residue.begin(), residue.end(), less_imp);
    }


    void CGChain::write_block(std::ofstream& ofs)
    {
        for(std::vector<BeadSptr>::iterator iter = residue.begin();
            iter != residue.end(); ++iter)
        {
            ofs << *(*iter) << std::endl;
        }
        return;
    }

    void CGChain::read_sequence()
    {
        if(!sequence.empty())
        {
            std::cout << "Warning: trying to re-read sequence. do nothing."
                      << std::endl;
            return;
        }

        int current_resseq(-1);
        for(std::vector<BeadSptr>::iterator iter = residue.begin();
            iter != residue.end(); ++iter)
        {
            boost::regex seqdna("D[ACGT]");
            std::string seqname((*iter)->get_seq());

            if(current_resseq == (*iter)->get_iResNum()) continue;
            else current_resseq = (*iter)->get_iResNum();

            if(boost::regex_search(seqname, seqdna))
            {//DNA
                mol_type = DNA;
                size_t dpos(seqname.find('D'));
                sequence += seqname[dpos+1];
            }
            else if(seqmap.find(seqname))
            {//protein
                mol_type = PROTEIN;
                sequence += seqmap.pseq_3to1(seqname);
            }
            else
            {//unknown. RNA?
                mol_type = UNKNOWN;
                std::cout << "Unknown sequence: " << seqname << std::endl;
            }
        }
        return;
    }

    typedef std::shared_ptr<CGChain> CGChnSptr;
}

#endif

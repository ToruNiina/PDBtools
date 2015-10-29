#ifndef ARABICA_CG_BEAD
#define ARABICA_CG_BEAD
#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <string>
#include <memory>
#include "PDBdef.hpp"

namespace arabica
{
    class CGBead
    {
        char ChainID;
        int imp;//mass particle number
        int iResNum;//residue sequence number
        double x, y, z;
        std::string head;//hedder. like ATOM, HEATOM
        std::string bead;//bead type. like CA(carbon alpha), DB(DNA base), etc.
        std::string seq;//sequence. like ALA(alanine), DG(DNA guanine), etc.

    public:
        CGBead(){}
        ~CGBead(){}

        void get_line(std::string& line);

        char        get_ChainID()  const {return ChainID;}
        int         get_imp()      const {return imp;}
        int         get_iResNum()  const {return iResNum;}
        double      get_coordx()   const {return x;}
        double      get_coordy()   const {return y;}
        double      get_coordz()   const {return z;}
        std::string get_beadname() const {return bead;}
        std::string get_seq()      const {return seq;}

        void set_ChainID (char id)       {ChainID = id;}
        void set_imp     (int im)        {imp = im;}
        void set_iResNum (int rn)        {iResNum = rn;}
        void set_coordx  (double _x)     {x = _x;}
        void set_coordy  (double _y)     {y = _y;}
        void set_coordz  (double _z)     {z = _z;}
        void set_beadname(std::string bn){bead = bn;}
        void set_seq     (std::string sq){seq = sq;}
        void set_headder (std::string hd){head = hd;}

        friend std::ostream& operator<<(std::ostream& os, const CGBead& a);

    private:

        void line_input(std::string line);

    };

    void CGBead::get_line(std::string& line)
    {
        if(line.substr(0,6) == "ATOM  " ||
           line.substr(0,6) == "HEATOM")
        {
            line_input(line);
            return;
        }

#ifdef _DEBUG
        std::cout << "unknown line: " << line << std::endl;
#endif //_DEBUG
        throw std::invalid_argument("CGBead: invalid linetype");
    }

    void CGBead::line_input(std::string line)
    {
        std::istringstream ls(line);
        ls >> head >> imp >> bead >> seq >> ChainID >> iResNum;
        ls >> x >> y >> z;
//         this->head       = line.substr(0,6);
//         this->imp        = std::stoi(line.substr(6,5));
//         this->bead       = line.substr(11,4);
//         this->seq        = line.substr(16,4);
//         this->ChainID    = line.at(21);
//         this->iResNum    = std::stoi(line.substr(22, 4));
//         this->x          = std::stod(line.substr(30, 8));
//         this->y          = std::stod(line.substr(38, 8));
//         this->z          = std::stod(line.substr(46, 8));

        return;
    }

    typedef std::shared_ptr<CGBead> BeadSptr;

    bool less_imp(const BeadSptr& lhs, const BeadSptr& rhs)
    {//lhs.imp < rhs.imp
        return lhs->get_imp() < rhs->get_imp();
    }


    std::ostream& operator<<(std::ostream& os, const CGBead& a)
    {
        os << std::setw(6) << a.head;
        os << std::setw(5) << a.imp;
        os << std::setw(4) << a.bead;
        os << std::setw(4) << a.seq;
        os << std::setw(3) << a.ChainID;
        os << std::setw(4) << a.iResNum;
        os << "    ";
        os << std::setw(8) << std::fixed << std::setprecision(3) << a.x;
        os << std::setw(8) << std::fixed << std::setprecision(3) << a.y;
        os << std::setw(8) << std::fixed << std::setprecision(3) << a.z;
        return os;
    }

}

#endif

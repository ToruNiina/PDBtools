#ifndef ARABICA_PDB_ATOM
#define ARABICA_PDB_ATOM
#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <string>
#include <memory>

namespace arabica
{
    class PDBAtom
    {
    public:
        enum LINE_TYPE
        {
            ATOM,
            TER,
            END,
            EMPTY,
            UNKNOWN
        };

    public:

        PDBAtom();
        ~PDBAtom();

        LINE_TYPE getAtomLine(std::ifstream& ifs);
        void read_line(std::string line);

        int get_resSeq() const {return resSeq;}
        int get_serial() const {return serial;}
        std::string get_name() const {return name;}
        std::string get_element() const {return element;}
        std::string get_charge() const {return charge;}
        std::string get_resName() const {return resName;}
        char get_altLoc() const {return altLoc;}
        char get_chainID() const {return chainID;}
        char get_iCode() const {return iCode;}
        double get_x() const {return x;}
        double get_y() const {return y;}
        double get_z() const {return z;}
        double get_occupancy() const {return occupancy;}
        double get_tempFactor() const {return tempFactor;}

        void set_resSeq(int rs){resSeq = rs;}
        void set_name(std::string n){name = n;}
        void set_resName(std::string rn){resName = rn;}
        void set_element(std::string el){element = el;}
        void set_charge(std::string ch){charge = ch;}
        void set_chainID(char cid){chainID = cid;}
        void set_x(double _x){x = _x;}
        void set_y(double _y){y = _y;}
        void set_z(double _z){z = _z;}

        friend std::ostream& operator<<(std::ostream& os, const PDBAtom& a);

    private:

        int serial, resSeq;
        std::string name, resName, element, charge;
        char altLoc, chainID, iCode;
        double x, y, z;
        double occupancy, tempFactor;
    };

    PDBAtom::PDBAtom(){}
    PDBAtom::~PDBAtom(){}

    PDBAtom::LINE_TYPE PDBAtom::getAtomLine(std::ifstream& ifs)
    {
        std::string line;
        getline(ifs, line);
        if(line.empty()) return EMPTY;

        if(line.substr(0, 6) != "ATOM  ")
        {
            if(line.substr(0,3) == "TER")
            {
                return TER;
            }
            if(line.substr(0,3) == "END")
            {
                return END;
            }
#ifdef _DEBUG
            std::cout << "unknown line: " << line << std::endl;
#endif //_DEBUG
            return UNKNOWN;
        }
        
        read_line(line);
        return ATOM;
    }

    void PDBAtom::read_line(std::string line)
    {
        this->serial     = stoi(line.substr(6, 5));
        this->name       = line.substr(12, 4);
        this->altLoc     = line[16];
        this->resName    = line.substr(17,3);
        this->chainID    = line[21];
        this->resSeq     = stoi(line.substr(22, 4));
        this->iCode      = line[26];
        this->x          = stod(line.substr(30, 8));
        this->y          = stod(line.substr(38, 8));
        this->z          = stod(line.substr(46, 8));
        this->occupancy  = stod(line.substr(54, 6));
        this->tempFactor = stod(line.substr(60, 6));
        this->element    = line.substr(76,2);
        this->charge     = line.substr(78,2);

        return;
    }

    std::ostream& operator<<(std::ostream& os, const PDBAtom& a)
    {
        os << "ATOM  ";
        os << std::setw(5) << a.serial;
        os << " ";
        os << std::setw(4) << a.name;
        os << std::setw(1) << a.altLoc;
        os << std::setw(3) << a.resName;
        os << " ";
        os << std::setw(1) << a.chainID;
        os << std::setw(4) << a.resSeq;
        os << std::setw(1) << a.iCode;
        os << "   ";
        os << std::setw(8) << std::fixed << std::setprecision(3) << a.x;
        os << std::setw(8) << std::fixed << std::setprecision(3) << a.y;
        os << std::setw(8) << std::fixed << std::setprecision(3) << a.z;
        os << std::setw(6) << std::fixed << std::setprecision(2) << a.occupancy;
        os << std::setw(6) << std::fixed << std::setprecision(2) << a.tempFactor;
        os << "          ";
        os << std::setw(2) << a.element;
        os << std::setw(2) << a.charge;
        return os;
    }

    typedef std::shared_ptr<PDBAtom> AtomSptr;
}
#endif // ARABICA_PDB_ATOM

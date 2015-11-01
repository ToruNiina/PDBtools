#ifndef ARABICA_DCD_READER
#define ARABICA_DCD_READER
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <memory>
#include "RealVec.hpp"

namespace arabica
{
    class DCDReader
    {
        public:

            typedef typename std::vector<Realvec> SnapShot;

        public:
            DCDReader(){}
            DCDReader(const std::string& filename)
            : dcdfile(filename, std::ios::binary)
            {
                ;
            }
            ~DCDReader(){}

            void read_file();
            void read_file(const std::string& filename);

            int get_nset() const {return nset;}
            int get_istart() const {return istart;}
            int get_nstep_save() const {return nstep_save;}
            int get_nstep() const {return nstep;}
            int get_nunit() const {return nunit;}
            int get_npart() const {return nparticle;}
            double get_delta_t() const {return delta_t;}
            SnapShot& get_snapshot(int i){return data.at(i);}
            std::vector<SnapShot>& get_all_data(int i){return data;}

        private:

            void read_header();
            void read_core();
            void read_head_block1();
            void read_head_block2();
            void read_head_block3();
            std::vector<double> read_x();
            std::vector<double> read_y();
            std::vector<double> read_z();

        private:

            int nset;// number of frame
            int istart;// initial value of isteps
            int nstep_save;// saving frequency
            int nstep;// total step number
            int nunit;// total unit number
            int verCHARMM;
            int nparticle;// total number of particle
            double delta_t;
            std::vector<SnapShot> data;
            std::ifstream dcdfile;

        private:

            static const int size_int;
            static const int size_float;
            static const int size_char;
    };

    const int DCDReader::size_int = sizeof(int);
    const int DCDReader::size_float = sizeof(float);
    const int DCDReader::size_char = sizeof(char);

    void DCDReader::read_file(const std::string& filename)
    {
        if(dcdfile.is_open())
        {
            std::cout << "Warning: DCDReader already open a dcd file. "
                      << "read opened file. not " << filename << std::endl;
            read_file();
            return;
        }
        dcdfile.open(filename, std::ios::binary);
        read_file();
        return;
    }

    void DCDReader::read_file()
    {
        read_header();
        read_core();
        std::cout << "read_file completed" << std::endl;
        return;
    }

    void DCDReader::read_header()
    {
        read_head_block1();
        read_head_block2();
        read_head_block3();
        return;
    }

    void DCDReader::read_head_block1()
    {
        char *cbytes = new char[size_int];
        dcdfile.read(cbytes, size_int);
        int byte = *reinterpret_cast<int*>(cbytes);
        std::cout << "header block 1 : " << byte << "bytes" << std::endl;
        delete [] cbytes;

        char *ccord = new char [4];
        dcdfile.read(ccord, 4);
        std::string cord(ccord);
        if(cord == "CORD")
        {
            std::cout << "Message: start to read dcd file." << std::endl;
        }
        else if(cord == "VELD")
        {
            std::cout << "Message: start to read vdcd file." << std::endl;
        }
        else
        {
            std::cout << "Warning: unknown file : " << cord << std::endl;
        }
        delete [] ccord;

        char *cnset = new char[size_int];
        dcdfile.read(cnset, size_int);
        nset = *reinterpret_cast<int*>(cnset);
        delete [] cnset;
        
        char *cistart = new char[size_int];
        dcdfile.read(cistart, size_int);
        istart = *reinterpret_cast<int*>(cistart);
        delete [] cistart;

        char *cnstep_save = new char[size_int];
        dcdfile.read(cnstep_save, size_int);
        nstep_save = *reinterpret_cast<int*>(cnstep_save);
        delete [] cnstep_save;

        std::cout << "nstep_save: " << nstep_save << std::endl;

        char *cnstep = new char[size_int];
        dcdfile.read(cnstep, size_int);
        nstep = *reinterpret_cast<int*>(cnstep);
        delete [] cnstep;

        std::cout << "nstep: " << nstep << std::endl;
        std::cout << "number of saved snapshot: " << nstep / nstep_save << std::endl;

        char *cnunit = new char[size_int];
        dcdfile.read(cnunit, size_int);
        nunit = *reinterpret_cast<int*>(cnunit);
        delete [] cnunit;

        char *null_1 = new char[16];
        dcdfile.read(null_1, 16);
        delete [] null_1;
        
        char cdelta[size_int];
        dcdfile.read(cdelta, size_int);
        float fdelta = *(float*)(cnunit);
        delta_t = static_cast<double>(fdelta);

        char *null_2 = new char[36];
        dcdfile.read(null_2, 36);
        delete [] null_2;

        char *cverCHARMM = new char[size_int];
        dcdfile.read(cverCHARMM, size_int);
        verCHARMM = *reinterpret_cast<int*>(cverCHARMM);
        delete [] cverCHARMM;

        char *cbytes_f = new char[size_int];
        dcdfile.read(cbytes_f, size_int);
        if(byte != *reinterpret_cast<int*>(cbytes_f))
        {
            std::cout << "Warning: header block1 is invalid" << std::endl;
        }
        delete cbytes_f;
        return;
    }

    void DCDReader::read_head_block2()
    {
        char *cbytes = new char[size_int];
        dcdfile.read(cbytes, size_int);
        int bytes = *reinterpret_cast<int*>(cbytes);
        delete [] cbytes;
        
        char *clines = new char[size_int];
        dcdfile.read(clines, size_int);
        int lines = *reinterpret_cast<int*>(clines);
        std::cout << "there are " << lines << "lines exist" << std::endl;
        delete [] clines;

        if((80 * lines + 4) != bytes)
        {
            std::cout << "Warning: header block2 has invalid size" << std::endl;
        }

        std::cout << "header: " << std::endl;
        for(int i(0); i<lines; ++i)
        {
            char *line = new char[80];
            dcdfile.read(line, 80);
            std::cout << line << std::endl;
            delete [] line;
        }

        char *cbytes_f = new char[size_int];
        dcdfile.read(cbytes_f, size_int);
        if(bytes != *reinterpret_cast<int*>(cbytes_f))
        {
            std::cout << "Warning: header block2 has invalid size information"
                      << std::endl;
        }
        delete [] cbytes_f;

    }

    void DCDReader::read_head_block3()
    {
        char *cbytes = new char[size_int];
        dcdfile.read(cbytes, size_int);
        int bytes = *reinterpret_cast<int*>(cbytes);
        delete [] cbytes;
        
        char *cnpart = new char[size_int];
        dcdfile.read(cnpart, size_int);
        nparticle = *reinterpret_cast<int*>(cnpart);
        delete [] cnpart;

        std::cout << "there are " << nparticle << " particles in this file" << std::endl;

        char *cbytes_f = new char[size_int];
        dcdfile.read(cbytes_f, size_int);
        if(bytes != *reinterpret_cast<int*>(cbytes_f))
        {
            std::cout << "Warning: header block2 has invalid size information"
                      << std::endl;
        }
        std::cout << "header block3 reading completed" << std::endl;
        return;
    }

    void DCDReader::read_core()
    {
        int saved(nstep / nstep_save);
        data.reserve(saved);

        for(int i(0); i<saved; ++i)
        {
            std::vector<double> x(read_x());
            std::vector<double> y(read_y());
            std::vector<double> z(read_z());

            SnapShot temp_snapshot(nparticle);
            for(int c(0); c != x.size(); ++c)
            {
                Realvec coord(x.at(c), y.at(c), z.at(c));
                temp_snapshot.at(c) = coord;
            }

            data.push_back(temp_snapshot);
            if(i % 1000 == 0)
            {
                std::cout << i << "step read" << std::endl;
            }
        }

        std::cout <<"data size: " << data.size() <<std::endl;

        return;
    }

    std::vector<double> DCDReader::read_x()
    {
        char *cbytes = new char[size_int];
        dcdfile.read(cbytes, size_int);
        int bytes(*reinterpret_cast<int*>(cbytes));
        if(bytes / size_int != nparticle)
        {
            std::cout << "Warning: "
                      << "dcd file x coordinate has invalid byte-information"
                      << std::endl;
        }
        delete [] cbytes;

        std::vector<double> retval(nparticle);
        for(int i(0); i<nparticle; ++i)
        {
            char *cfloat = new char[size_float];
            dcdfile.read(cfloat, size_float);
            double x(static_cast<double>(*reinterpret_cast<float*>(cfloat)));
            retval.at(i) = x;
            delete [] cfloat;
        }

        char *cbytes_f = new char[size_int];
        dcdfile.read(cbytes_f, size_int);
        if(bytes != *reinterpret_cast<int*>(cbytes_f))
        {
            std::cout << "Warning: "
                      << "dcd file x coordinate header-byte-information "
                      << "is not same number as footer-byte-information"
                      <<std::endl;
            throw std::invalid_argument("stop");
        }
        delete [] cbytes_f;

        return retval;
    }

    std::vector<double> DCDReader::read_y()
    {
        char *cbytes = new char[size_int];
        dcdfile.read(cbytes, size_int);
        int bytes(*reinterpret_cast<int*>(cbytes));
        if(bytes / size_int != nparticle)
        {
            std::cout << "Warning: "
                      << "dcd file x coordinate has invalid byte-information"
                      << std::endl;
        }
        delete [] cbytes;

        std::vector<double> retval(nparticle);
        for(int i(0); i<nparticle; ++i)
        {
            char *cfloat = new char[size_float];
            dcdfile.read(cfloat, size_float);
            double y(static_cast<double>(*reinterpret_cast<float*>(cfloat)));
            retval.at(i) = y;
            delete [] cfloat;
        }

        char *cbytes_f = new char[size_int];
        dcdfile.read(cbytes_f, size_int);
        if(bytes != *reinterpret_cast<int*>(cbytes_f))
        {
            std::cout << "Warning: "
                      << "dcd file x coordinate header-byte-information "
                      << "is not same number as footer-byte-information"
                      <<std::endl;
            throw std::invalid_argument("stop");
        }
        delete [] cbytes_f;

        return retval;
    }

    std::vector<double> DCDReader::read_z()
    {
        char *cbytes = new char[size_int];
        dcdfile.read(cbytes, size_int);
        int bytes(*reinterpret_cast<int*>(cbytes));
        if(bytes / size_int != nparticle)
        {
            std::cout << "Warning: "
                      << "dcd file x coordinate has invalid byte-information"
                      << std::endl;
            throw std::invalid_argument("stop");
        }
        delete [] cbytes;

        std::vector<double> retval(nparticle);
        for(int i(0); i < nparticle; ++i)
        {
            char *cfloat = new char[size_float];
            dcdfile.read(cfloat, size_float);
            double z(static_cast<double>(*reinterpret_cast<float*>(cfloat)));
            retval.at(i) = z;
            delete [] cfloat;
        }

        char *cbytes_f = new char[size_int];
        dcdfile.read(cbytes_f, size_int);
        if(bytes != *reinterpret_cast<int*>(cbytes_f))
        {
            std::cout << "Warning: "
                      << "dcd file x coordinate header-byte-information "
                      << "is not same number as footer-byte-information"
                      <<std::endl;
            throw std::invalid_argument("stop");
        }
        delete [] cbytes_f;

        return retval;
    }

    typedef std::shared_ptr<DCDReader> DCDRderSptr;

}//end namespace arabica

#endif //ARABICA_DCD_READER
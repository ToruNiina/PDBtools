#ifndef ARABICA_CGPDB_READER
#define ARABICA_CGPDB_READER
#include "CGModel.hpp"

namespace arabica
{
    class CGReader
    {
        public:
            CGReader()
            {
                ;
            }

            CGReader(const std::string& filename)
                : cgfile(filename)
            {
                ;
            }

            ~CGReader()
            {
                cgfile.close();
            }

            void read_file();
            void read_file(const std::string& filename);

            CGMdlSptr& get_chain(const int i)
            {
                return models.at(i);
            }

            int get_chain_num() const
            {
                return models.size();
            }

        private:

            std::ifstream cgfile;
            std::vector<CGMdlSptr> models;
    };


    void CGReader::read_file(const std::string& filename)
    {
        if(cgfile.is_open())
        {
            std::cout << "file already open" << std::endl;
        }
        cgfile.open(filename);
        read_file();
        return;
    }

    void CGReader::read_file()
    {
        while(!cgfile.eof())
        {
            CGMdlSptr temp(new CGModel);

            temp->read_file(cgfile);

            if(temp->empty()) break;

            models.push_back(temp);
        }
        return;
    }

}

#endif //ARABICA_CGPDB_READER

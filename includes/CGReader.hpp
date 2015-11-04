#ifndef ARABICA_CGPDB_READER
#define ARABICA_CGPDB_READER
#include "CGChain.hpp"

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

            CGChnSptr& get_chain(const int i)
            {
                return chains.at(i);
            }

            int get_chain_num() const
            {
                return chains.size();
            }

            CGChnSptr& find(const char ID);
            int find_id(const char ID);

        private:

            std::ifstream cgfile;
            std::vector<CGChnSptr> chains;
    };


    CGChnSptr& CGReader::find(const char ID)
    {
        for(auto iter = chains.begin(); iter != chains.end(); ++iter)
        {
            if((*iter)->get_chainID() == ID)
                return *iter;
        }
        std::cout << "cannot find ID: " << ID << std::endl;
        return *(chains.end());
    }

    int CGReader::find_id(const char ID)
    {
        for(int i(0); i<chains.size(); ++i)
        {
            if(chains.at(i)->get_chainID() == ID)
                return i;
        }
        std::cout << "cannot find ID: " << ID << std::endl;
        return -1;
    }

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
            CGChnSptr temp(new CGChain);

            temp->read_block(cgfile);

            if(temp->empty()) break;

            chains.push_back(temp);
        }
        return;
    }

}

#endif //ARABICA_CGPDB_READER

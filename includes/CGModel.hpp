#ifndef ARABICA_CGPDB_READER
#define ARABICA_CGPDB_READER
#include "CGChain.hpp"

namespace arabica
{
    class CGModel
    {
        public:
            CGModel()
            {
                ;
            }
            CGModel(const std::string& filename)
                : cgfile(filename)
            {
                ;
            }
            ~CGModel()
            {
                ;
            }

            void read_file();
            void read_file(const std::string& filename);

            bool empty(){return chains.empty();}
            int size(){return chains.size();}
            CGChnSptr& at(const int i){return chains.at(i);}
            std::vector<CGChnSptr>& get_data(){return chains;}

        private:

            int model_ID;
            int step;
            std::ifstream cgfile;
            std::vector<CGChnSptr> chains;
    };

    void CGModel::read_file(const std::string& filename)
    {
        if(cgfile.is_open())
        {
            std::cout << "Warning: already open" << std::endl;
        }
        cgfile.open(filename);
        read_file();
        return;
    }

    void CGModel::read_file()
    {
        bool model_found(false);
        while(!cgfile.eof())
        {
            std::string line;
            std::getline(cgfile, line);

            if(line.substr(0,5) == "MODEL")
            {
                std::cout << "model found." << std::endl;
                model_found = true;
                std::istringstream ls(line);
                std::string temp;
                ls >> temp >> model_ID;
                ls >> temp >> step;
                std::cout << line << std::endl;
            }

            if(line.substr(0,6) == "ENDMDL" && model_found)
            {
                std::cout << "end of model block founded" << std::endl;
                return;
            }
            CGChnSptr chain(new CGChain);
            chain->read_block(cgfile);
            chains.push_back(chain);
        }

        if(!model_found)
        {
            std::cout << "model description not found" << std::endl;
            return;
        }

        throw std::invalid_argument("model(or end of model) not found");
    }

}

#endif //ARABICA_CGPDB_READER

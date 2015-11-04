#ifndef ARABICA_CGPDB_MODEL
#define ARABICA_CGPDB_MODEL
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
            ~CGModel()
            {
                ;
            }

            void read_file(std::ifstream& pdbfile);
            void write_file(std::ofstream& outfile)
            {//TODO
                std::cout << "not supported yet" << std::endl;
            }

            bool empty(){return chains.empty();}
            int size(){return chains.size();}
            CGChnSptr& at(const int i){return chains.at(i);}
            CGChnSptr& find(const char ID);
            int find_id(const char ID);
            std::vector<CGChnSptr>& get_data(){return chains;}

        private:

            int model_ID;
            int step;
            std::vector<CGChnSptr> chains;
    };

    void CGModel::read_file(std::ifstream& pdbfile)
    {
        if(!chains.empty())
        {
            throw std::invalid_argument("already read");
        }

        bool model_found(false);
        while(!pdbfile.eof())
        {
            std::string line;
            std::getline(pdbfile, line);

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
            chain->read_block(pdbfile);
            chains.push_back(chain);
        }

        if(!model_found)
        {
            std::cout << "model description not found" << std::endl;
            return;
        }

        throw std::invalid_argument("model(or end of model) not found");
    }

    CGChnSptr& CGModel::find(const char ID)
    {
        for(auto iter = chains.begin(); iter != chains.end(); ++iter)
        {
            if((*iter)->get_chainID() == ID)
                return *iter;
        }
        std::cout << "cannot find ID: " << ID << std::endl;
        return *(chains.end());
    }

    int CGModel::find_id(const char ID)
    {
        for(int i(0); i<chains.size(); ++i)
        {
            if(chains.at(i)->get_chainID() == ID)
                return i;
        }
        std::cout << "cannot find ID: " << ID << std::endl;
        return -1;
    }

    typedef std::shared_ptr<CGModel> CGMdlSptr;
}

#endif //ARABICA_CGPDB_MODEL

#include "CGModel.hpp"
using namespace arabica;

int main(int argc, char *argv[])
{
    std::string filename(argv[1]);
    CGModel model(filename);
    model.read_file();
    std::cout << model.size() << std::endl;
    for(int i(0); i<model.size(); ++i)
        std::cout << model.at(i)->get_sequence() << std::endl;
    return 0;
}

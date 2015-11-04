#include "includes/DCDReader.hpp"
#include "includes/RMSDcalc.hpp"
#include "includes/CGReader.hpp"
using namespace arabica;

int main(int argc, char *argv[])
{
    std::ifstream ifs(argv[1]);

    CGChain chain;
    chain.read_block(ifs);
    std::vector<Eigen::Vector3d> coord(chain.get_coord());

    RMSDCalculator rmsdc;
    rmsdc.set_data(coord, coord);
    std::cout << "RMSD(zero): " << rmsdc.get_RMSD() << std::endl;
    return 0;
}

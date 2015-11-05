#include "includes/DCDReader.hpp"
#include "includes/RMSDCalculator.hpp"
#include "includes/CGReader.hpp"
using namespace arabica;

int main(int argc, char *argv[])
{
    if(argc != 3)
    {
        std::cout << "RMSDCalculator" << std::endl;
        std::cout << "USAGE: ./rmsd <filename> <chainIDs>" << std::endl;
        std::cout << "       input the filename without extension" << std::endl;
        std::cout << "       input IDs(alphabet) of the chain you want to calculate RMSD"
                  << std::endl;
        std::cout << "       this reads pdb(cg style) and dcd file." << std::endl;
        std::cout << "       please confirm these files exist and have same name." << std::endl;
    }

    std::string filename(argv[1]);

    std::string cgpdb(filename + ".pdb");
    CGReader cgreader(cgpdb);
    cgreader.read_file();
    CGMdlSptr model(cgreader.get_model(0));

    std::string IDs(argv[2]);
    std::transform(IDs.cbegin(), IDs.cend(), IDs.begin(), toupper);

    std::cout << "model->size" << model->size() << std::endl;
    //this contains how many mass points the chain has
    std::vector<int> imps;
    for(int i(0); i<model->size(); ++i)
    {
        imps.push_back(model->at(i)->get_size());
        std::cout<< "model->at->get_size" << model->at(i)->get_size() << std::endl;
    }
    for(int i(0); i<imps.size(); ++i)
        std::cout << imps.at(i) << std::endl;

    //this contains what is the ID of the chain to calculate RMSD
    std::vector<int> imp_ID;
    for(auto iter = IDs.begin(); iter != IDs.end(); ++iter)
    {
        imp_ID.push_back(model->find_id(*iter));
        std::cout << "model->find_id" << model->find_id(*iter) << std::endl;
    }
    for(int i(0); i<imp_ID.size(); ++i)
        std::cout << imp_ID.at(i) << std::endl;

    std::string dcd(filename + ".dcd");
    DCDReader dcdreader;
    dcdreader.read_file(dcd);

    std::string output(IDs + "_RMSD.ts");
    std::ofstream ofs(output);

    std::cout << "rmsdcalc" << std::endl;
    RMSDCalculator rmsdcalc;

    if(dcdreader.get_size() < 2)
    {
        std::cout << "there is only" << dcdreader.get_size() << "snapshots." << std::endl;
        std::cout << "it is not enough to calculate RMSD." << std::endl;
        return 1;
    }

    std::pair<DCDReader::SnapShot, double> initial
        (dcdreader.get_snapshot(0));

    std::pair<DCDReader::SnapShot, double> second_
        (dcdreader.get_snapshot(1));

    std::cout << "initial snapshot" << std::endl;
//TODO
    int end_ID(0);
    for(int i(0); i < *(imp_ID.end() - 1); ++i)
    {
        end_ID += imps.at(i);
    }
    std::cout << "end_ID " << end_ID << std::endl;

    DCDReader::SnapShot init(initial.first.begin(), (initial.first.begin() + end_ID));
    DCDReader::SnapShot seco(initial.first.begin(), (initial.first.begin() + end_ID));
    std::cout << "init size " << init.size() << std::endl;
    std::cout << "seco size " << seco.size() << std::endl;

    rmsdcalc.set_data(init, seco);

    ofs << 0e0 << " " << 0e0 << std::endl;
    ofs << second_.second << " " << rmsdcalc.get_RMSD() << std::endl;

    for(int i(2); i < dcdreader.get_size(); ++i)
    {
        std::pair<DCDReader::SnapShot, double> snapshot
            (dcdreader.get_snapshot(i));
        DCDReader::SnapShot sshot(snapshot.first.begin(), (snapshot.first.begin()+end_ID));

        rmsdcalc.set_data2(sshot);
        ofs << snapshot.second << " " << rmsdcalc.get_RMSD() << std::endl;
    }

    return 0;
}

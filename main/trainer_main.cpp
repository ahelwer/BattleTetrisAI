#include <trainer/tetris_agent_eval.hpp>
#include <trainer/generated_game.hpp>
#include <core/game_state_eval.hpp>
#include <core/harmony_search.hpp>
#include <core/harmony.hpp>
#include <cstdlib>
#include <sstream>
#include <fstream>

int main(int argc, char* argv[]) {
    srand(time(NULL));
    std::ifstream in;
    std::string outName;
    bool initHarmonies = false;
    if (argc == 3) {
        in.open(argv[1]);
        initHarmonies = in.is_open();
        outName = std::string(argv[2]);
    }

    int const varCount = GetVarCount();
    int const memorySize = 5;
    float const r_accept = 0.95;
    float const r_pa = 0.99;
    float const r_range = 0.1;

    std::vector<Harmony> init;
    if (initHarmonies) {
        for (int i = 0; i < memorySize; ++i) {
            Harmony h;
            in.ignore(1);
            for (int i = 0; i < varCount; ++i) {
                float weight = 0.0;    
                in >> weight;
                h.push_back(weight);
                in.ignore(1);
            }
            in.ignore(1);
            in.ignore(1);
            init.push_back(h);
        }
    }
    in.close();

    int const gameLength = 1000;
    int const iterationCount = 50;

    HarmonyRanges const* ranges = GetRanges();
    HarmonyFactory const factory (varCount, *ranges);
    GeneratedGame generator (gameLength);
    TetrisPointsEarned f (generator);
    HarmonyCompareMax maxComp (f);
    HarmonyCompareWrapper comp (maxComp);
    HarmonySearch search (comp, factory, varCount, memorySize,
                            r_accept, r_pa, r_range);
    if (initHarmonies)
        search.InitializeHarmonies(init);

    for (int i = 0; i < iterationCount; ++i) {
        search.Iterate();
        generator.GenerateNewGame();
        search.EraseHarmonyCaches();
        std::cout << "COMPLETED ITERATION " << i << std::endl;
    }
    delete ranges;

    std::vector<Harmony const*> memory;
    for (int i = 0; i < memorySize; ++i)
        memory.push_back(search.GetRanked(i));

    std::stringstream ss;
    std::ofstream out (outName.c_str(), std::ios::app);
    for (int i = 0; i < memorySize; ++i)
        ss << *(memory.at(i)) << std::endl << std::endl;

    ss << std::endl;
    out << ss.str();
    out.close();

    for (int i = 0; i < memorySize; ++i)
        delete memory.at(i);

    std::cout << "Completed!" << std::endl;

    return 0;
}


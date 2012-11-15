#include <trainer/generated_game.hpp>
#include <trainer/tetris_rows_cleared.hpp>
#include <core/game_state_eval.hpp>
#include <core/harmony_search.hpp>
#include <core/harmony.hpp>
#include <cstdlib>
#include <sstream>
//#include <omp>

int main(int argc, char* argv[]) {
    srand(time(NULL));
    int threadCount = 1;
    if (argc == 2)
        std::istringstream(argv[1]) >> threadCount;
    std::cout << "Launching " << threadCount << " trainers." << std::endl;
    int const gameLength = 1000;
    int const iterationCount = 1000;

    int const varCount = GetVarCount();
    HarmonyRanges const* ranges = GetRanges();
    int const memorySize = 5;
    float const r_accept = 0.95;
    float const r_pa = 0.99;
    float const r_range = 0.1;
    HarmonyFactory factory (varCount, *ranges);

    //#pragma omp parallel for num_threads(threadCount)
    //for (int t = 0; t < threadCount; ++t) {
        GeneratedGame generator (gameLength);
        TetrisRowsCleared f (generator);
        HarmonyCompareMax maxComp (f);
        HarmonyCompareWrapper comp (maxComp);
        HarmonySearch search (comp, factory, varCount, memorySize,
                                r_accept, r_pa, r_range);
        for (int i = 0; i < iterationCount; ++i) {
            search.Iterate();
            generator.GenerateNewGame();
            search.EraseHarmonyCaches();
            std::cout << "COMPLETED ITERATION " << i << std::endl;
        }
    //}
    std::vector<Harmony const*> memory;
    for (int i = 0; i < memorySize; ++i)
        memory.push_back(search.GetRanked(i));
    for (int i = 0; i < memorySize; ++i)
        std::cout << *(memory.at(i)) << std::endl;
    for (int i = 0; i < memorySize; ++i)
        delete memory.at(i);
    delete ranges;

    return 0;
}


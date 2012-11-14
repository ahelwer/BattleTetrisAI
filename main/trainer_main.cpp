#include <trainer/generated_game.hpp>
#include <trainer/tetris_rows_cleared.hpp>
#include <core/game_state_eval.hpp>
#include <core/harmony_search.hpp>
#include <core/harmony.hpp>

int main() {
    int const gameLength = 100;
    int const iterationCount = 100;

    int const varCount = GameStateEval::GetVarCount();
    int const memorySize = 5;
    float const r_accept = 0.95;
    float const r_pa = 0.99;
    float const r_range = 0.1;

    HarmonyRanges const* ranges = GameStateEval::GetRanges();
    HarmonyFactory factory (varCount, *ranges);
    GeneratedGame generator (gameLength);
    TetrisRowsCleared f (generator);
    HarmonyCompareMax maxComp (f);
    HarmonyCompareWrapper comp (maxComp);
    HarmonySearch search (comp, factory, varCount, memorySize,
                            r_accept, r_pa, r_range);
    for (int i = 0; i < iterationCount; ++i) {

        generator.GenerateNewGame();
    }
    delete ranges;

    return 0;
}


#include <iostream>
#include <string>

#include "uci/uci.h"
#include "engine/engine.h"

#include "tests/checkmate_eval_test/CheckmateEvalTester.h"
#include "opening_book/opening_book.h"

int main() {
    Engine engine = Engine();
    UCIReader uci_reader = UCIReader(&engine);

    OpeningBook::init();
    //uci_reader.read_input();
}
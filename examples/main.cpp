#include "basic/game.h"
#include "tests/test_keychain.h"

int main()
{
    // run tests first
    test::keychain();

    // run a basic game for 10 iterations
    basic::run(10);

    return 0;
}

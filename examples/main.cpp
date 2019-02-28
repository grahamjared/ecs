#include "basic/game.h"
#include "tests/test_keychain.h"
#include "tests/test_world.h"

int main()
{
    // run tests first
    test::keychain();
    test::world();

    // run a basic game for 10 iterations
    basic::run(10);

    return 0;
}

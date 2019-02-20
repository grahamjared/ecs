#include "test_keychain.h"

namespace
{
    void print_components(const ecs::key & key)
    {
        std::cout << "key components: " << key.get() << "\n";
    }

    enum comp
    {
        none = 0,
        first = 1 << 0,
        second = 1 << 1,
        third = 1 << 2,
    };

    void key_add_component_t()
    {
        ecs::key key;

        for (auto i = 0; i < 3; ++i)
        {
            key.add(first | second);
            logic_assert(key.get() == (first | second), { "components: " + std::to_string(key.get()) + " | expected: " + std::to_string(first | second) });
        }

        key.add(first | second | third);
        logic_assert(key.get() == (first | second | third), { "components: " + std::to_string(key.get()) + " | expected: " + std::to_string(first | second | third) });
    }

    void key_sub_component_t()
    {
        ecs::key key;

        key.add(first | second | third);

        for (auto i = 0; i < 3; ++i)
        {
            key.sub(first);
            logic_assert(key.get() == (second | third), { "components: " + std::to_string(key.get()) + " | expected: " + std::to_string(second | third) });
        }

        key.sub(first | second);
        logic_assert(key.get() == (third), { "components: " + std::to_string(key.get()) + " | expected: " + std::to_string(third) });
    }
}

void test::keychain()
{
    key_add_component_t();
    key_sub_component_t();

    std::cout << "test::keychain() all tests passed\n";
}

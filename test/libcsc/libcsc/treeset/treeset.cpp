#include <treeset/treeset.h>
#include <gtest/gtest.h>
#include <vector>
#include <set>


TEST(first, first) {
    my_set::TreeSet set;
    std::vector items = {7, 2, 6, 4, 5, 3, -1, 0};
    for (auto &i: items) {
        set.insert(i);
    }
    std::cout << set.size() << '\n';
    set.remove(2);
    set.print();
    std::cout << "contains: " << set.contains(4) << '\n';
    std::cout << set.size() << '\n';

    auto node = set.getMin(set.getRoot());

    for (std::size_t i = 0; i < set.size() - 1; i++) {
        std::cout << node->key << '\n';
        node = set.increment(node);
    }
    set.clear();

    ASSERT_EQ(0, set.size());
}

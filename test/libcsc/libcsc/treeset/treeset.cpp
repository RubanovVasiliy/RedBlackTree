#include <treeset/treeset.h>
#include <gtest/gtest.h>
#include <vector>


TEST(first, first) {
    my_set::TreeSet set;
    std::vector items = {7, 2, 6, 4, 5, 3, 1, 0};
    for (auto &i: items) {
        set.insert(i);
    }
    std::cout << set.size() << '\n';
    set.remove(2);
    set.print();
    std::cout << "contains: " << set.contains(4) << '\n';
    std::cout << set.size() << '\n';
    set.clear();
    std::cout << "contains: " << set.contains(4) << '\n';
    std::cout << set.size() << '\n';

    ASSERT_EQ(1, 1);
}

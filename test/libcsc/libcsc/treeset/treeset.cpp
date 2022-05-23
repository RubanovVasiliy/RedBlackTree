#include <treeset/treeset.h>
#include <gtest/gtest.h>
#include <vector>
#include <set>

TEST(foreach, null) {
    my_set::TreeSet set;

    for (auto it = set.begin(); it != set.end(); it++) {}
    ASSERT_EQ(0, set.size());
}

TEST(foreach, one_element) {
    my_set::TreeSet set;
    set.insert(1);

    for (auto it = set.begin(); it != set.end(); it++) {}

    ASSERT_EQ(1, set.size());
}

TEST(foreach, full_tree) {
    my_set::TreeSet set;
    std::vector items = {7, 2, 6, 4, 5, 3, -1, 0};
    for (auto &i: items) {
        set.insert(i);
    }

    for (auto it = set.begin(); it != set.end(); it++) {
        set.remove(*it);
    }

    ASSERT_EQ(0, set.size());
}

TEST(remove, false) {
    my_set::TreeSet set;
    auto res = set.remove(4);
    ASSERT_EQ(false, res);
}

TEST(remove, true) {
    my_set::TreeSet set;
    set.insert(4);
    auto res = set.remove(4);
    ASSERT_EQ(true, res);
}

TEST(contains, false) {
    my_set::TreeSet set;
    auto res = set.contains(4);
    ASSERT_EQ(false, res);
}

TEST(contains, true) {
    my_set::TreeSet set;
    set.insert(4);
    auto res = set.contains(4);
    ASSERT_EQ(true, res);
}
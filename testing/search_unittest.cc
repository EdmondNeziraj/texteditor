
#include "gtest/gtest.h"
#include "../src/search.cc"

using namespace std;

// g++ -o search search_unittest.cc -lgtest -lgtest_main -pthread

TEST(TestName, general_case) {
    // arrange
    //                       1
    // indices     0123456789012345
    char text[] = "AABAACAADABTAABA";
    char pattern[] = "AABA";
    vector<int> expected{0, 12};

    // act
    vector<int> found = SearchDFA(pattern, text);

    // assert
    ASSERT_EQ(found.size(), expected.size());
    for (int i = 0; i < found.size(); i++) {
        EXPECT_EQ(found[i], expected[i]) << "vectors differ at: " << i;
    }
}

TEST(TestName, results_overlap) {
    // arrange
    //                       1
    // indices     0123456789012345
    char text[] = "AABAABAADABTAABA";
    char pattern[] = "AABA";
    vector<int> expected{0, 3, 12};

    // act
    vector<int> found = SearchDFA(pattern, text);

    // assert
    ASSERT_EQ(found.size(), expected.size());
    for (int i = 0; i < found.size(); i++) {
        EXPECT_EQ(found[i], expected[i]) << "vectors differ at: " << i;
    }
}

TEST(TestName, not_found) {
    // arrange
    char text[] = "AABAACAADABTAABA";
    char pattern[] = "AED";
    vector<int> expected; // expected empty vector

    // act
    vector<int> found = SearchDFA(pattern, text);

    // assert
    ASSERT_EQ(found.size(), expected.size());
}

TEST(TestName, empty_pattern) {
    // arrange
    char text[] = "AABAACAADABTAABA";
    char pattern[] = "";
    vector<int> expected; // expected empty vector

    // act
    vector<int> found = SearchDFA(pattern, text);

    // assert
    ASSERT_EQ(found.size(), expected.size());
}

TEST(TestName, empty_text) {
    // arrange
    char text[] = "";
    char pattern[] = "AAB";
    vector<int> expected; // expected empty vector

    // act
    vector<int> found = SearchDFA(pattern, text);

    // assert
    ASSERT_EQ(found.size(), expected.size());
}


int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
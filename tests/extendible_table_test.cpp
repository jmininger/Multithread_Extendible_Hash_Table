#include "gtest/gtest.h"
#include "extendible_hash.h"
#include "test_utils.h"

#include <memory>
#include <string>
#include <thread>
#include <functional>

TEST(Table, can_insert_elements_one_at_a_time)
{
	ExtendibleHash<int, std::string, TestHash> table{3};

}

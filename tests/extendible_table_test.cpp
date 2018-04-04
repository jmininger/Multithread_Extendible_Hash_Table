#include "gtest/gtest.h"
#include "extendible_hash.h"
#include "test_utils.h"

#include <memory>
#include <string>
#include <thread>
#include <functional>

//#include <iostream>


TEST(Table, can_insert_elements_one_at_a_time)
{
	ExtendibleHash<int, std::string, TestHash> table{2};
	EXPECT_EQ(1, table.m_global);

	table.Insert(0, "HELLO");
	table.Insert(1, "WORLD");
	table.Insert(2, "FOO");
	table.Insert(3, "BAR");
	table.Insert(4, "BAZ");
	table.Insert(5, "ARMADILLO");
	table.Insert(6, "BJARNE STROUSTRUP");
	table.Insert(7, "RANDOM STRING");
	table.Insert(8, "RANDOM STRING 2");

	EXPECT_EQ(3, table.m_global);
	EXPECT_EQ(table.m_table[1], table.m_table[5]);
	EXPECT_EQ(table.m_table[2], table.m_table[6]);
	EXPECT_EQ(table.m_table[3], table.m_table[7]);
	EXPECT_EQ(1, (int)table.m_table[0]->GetIndices().size());
	EXPECT_EQ(1, (int)table.m_table[4]->GetIndices().size());

}

TEST(Table, can_find_elements)
{
	ExtendibleHash<int, std::string, TestHash> table{2};

	table.Insert(0, "HELLO");
	table.Insert(1, "WORLD");
	table.Insert(2, "FOO");
	table.Insert(3, "BAR");
	table.Insert(4, "BAZ");
	table.Insert(5, "ARMADILLO");

	std::string s;
	table.Find(2, s);
	EXPECT_EQ("FOO", s);
	
	table.Find(4, s);
	EXPECT_EQ("BAZ", s);
	
	table.Find(3, s);
	EXPECT_EQ("BAR", s);
	
	table.Find(5, s);
	EXPECT_EQ("ARMADILLO", s);
	
	table.Find(1, s);
	EXPECT_EQ("WORLD", s);

	EXPECT_FALSE(table.Find(6, s));
}

TEST(Table, can_insert_elements_concurrently)
{
	ExtendibleHash<int, std::string, TestHash> table{3};
	const int num_runs = 50;
  	const int num_threads = 5;
  // Run concurrent test multiple times to guarantee correctness.
  for (int run = 0; run < num_runs; run++) 
  {
    std::vector<std::thread> threads;
    for (int tid = 0; tid < num_threads; tid++) 
    {
      threads.push_back(std::thread([tid, &table]() {
	        table.Insert(tid, std::to_string(tid));
	      }));
    }
    for (int i = 0; i < num_threads; i++) 
    {
      threads[i].join();
    }
    for (int i = 0; i < num_threads-1; i++) 
    {
      std::string val;
      EXPECT_TRUE(table.Find(i, val));
      EXPECT_EQ(val, std::to_string(i));
    }
    std::string val;
    EXPECT_FALSE(table.Find(num_threads, val));
  }
}

#include "gtest/gtest.h"
#include "extendible_hash.cpp"

#include <memory>
#include <string>
#include <thread>

using Bucket = ExtendibleHash<int,std::string>::Bucket;
using Bucket_Element = ExtendibleHash<int,std::string>::Bucket_Element;

// class Bucket_Test : public ::testing::Test
// {
// 	virtual void SetUp()
// 	{

// 	}


// };

TEST(Bucket, can_insert_elements_one_at_a_time)
{
	Bucket bucket{3};
	auto p_bucket = &bucket;
	std::string s = "HELLO";
	int i = 123;
	Bucket_Element elem;

	elem = {i, s};
	EXPECT_EQ(p_bucket->Insert(elem), true);
	elem = {234, "WOrlD"};
	EXPECT_EQ(p_bucket->Insert(elem), true);
	elem = {5, "FOO"};
	EXPECT_EQ(p_bucket->Insert(elem), true);
	elem = {90000, "BAR"};
	EXPECT_EQ(p_bucket->Insert(elem), false);
	elem = {100, "foo"};
	EXPECT_EQ(p_bucket->Insert(elem), false);
	elem = {5, "BAZ"};
	EXPECT_EQ(p_bucket->Insert(elem), true);

}

TEST(Bucket, can_single_threaded_search)
{
	Bucket bucket{3};
	auto p_bucket = &bucket;
	Bucket_Element elem1{1, "Hello"};
	Bucket_Element elem2{2, "World"};
	Bucket_Element elem3{3, "Foo"};
	p_bucket->Insert(elem1);
	p_bucket->Insert(elem2);
	p_bucket->Insert(elem3);

	std::string val;

	EXPECT_TRUE(p_bucket->Find(1, val));
	EXPECT_EQ("Hello", val);
	EXPECT_TRUE(p_bucket->Find(3, val));
	EXPECT_EQ("Foo", val);
	EXPECT_TRUE(p_bucket->Find(2, val));
	EXPECT_EQ("World", val);
	EXPECT_FALSE(p_bucket->Find(4, val));

};

TEST(Bucket, can_insert_concurrently)
{
	const int num_runs = 50;
  	const int num_threads = 5;
  // Run concurrent test multiple times to guarantee correctness.
  for (int run = 0; run < num_runs; run++) 
  {
    std::shared_ptr<Bucket> p_bucket{new Bucket(49)};
    std::vector<std::thread> threads;
    for (int tid = 0; tid < num_threads; tid++) 
    {
      threads.push_back(std::thread([tid, &p_bucket]() {
	        p_bucket->Insert(Bucket_Element{tid, std::to_string(tid)});
	      }));
    }
    for (int i = 0; i < num_threads; i++) 
    {
      threads[i].join();
    }
    for (int i = 0; i < num_threads-1; i++) 
    {
      std::string val;
      EXPECT_TRUE(p_bucket->Find(i, val));
      EXPECT_EQ(val, std::to_string(i));
    }
    std::string val;
    EXPECT_FALSE(p_bucket->Find(num_threads, val));
  }
};


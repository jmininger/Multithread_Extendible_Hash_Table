#include "gtest/gtest.h"
#include "extendible_hash.h"
#include "test_utils.h"

#include <memory>
#include <string>
#include <thread>
#include <functional>

//#include <iostream>


TEST(Bucket, can_insert_elements_one_at_a_time)
{
	using Bucket = ExtendibleHash<int,std::string, TestHash>::Bucket;
	using Bucket_Element = ExtendibleHash<int,std::string,TestHash>::Bucket_Element;
	
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
	using Bucket = ExtendibleHash<int,std::string, TestHash>::Bucket;
	using Bucket_Element = ExtendibleHash<int,std::string,TestHash>::Bucket_Element;
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
	using Bucket = ExtendibleHash<int,std::string, TestHash>::Bucket;
	using Bucket_Element = ExtendibleHash<int,std::string,TestHash>::Bucket_Element;
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

// TEST(Bucket, can_search_concurrently)
// {
// 	const int num_runs = 50;
//   	const int num_threads = 5;
//   // Run concurrent test multiple times to guarantee correctness.
//   for (int run = 0; run < num_runs; run++) 
//   {
//     std::shared_ptr<Bucket> p_bucket{new Bucket(49)};
//     std::vector<std::thread> threads;
//     for (int tid = 0; tid < num_threads; tid++) 
//     {
//       threads.push_back(std::thread([tid, &p_bucket]() {
// 	        p_bucket->Insert(Bucket_Element{tid, std::to_string(tid)});
// 	      }));
//     }
//     for (int i = 0; i < num_threads; i++) 
//     {
//       threads[i].join();
//     }
//     for (int i = 0; i < num_threads-1; i++) 
//     {
//       std::string val;
//       EXPECT_TRUE(p_bucket->Find(i, val));
//       EXPECT_EQ(val, std::to_string(i));
//     }
//     std::string val;
//     EXPECT_FALSE(p_bucket->Find(num_threads, val));
//   }

// };
TEST(Bucket, can_remove)
{
	using Bucket = ExtendibleHash<int,std::string, TestHash>::Bucket;
	using Bucket_Element = ExtendibleHash<int,std::string,TestHash>::Bucket_Element;
	Bucket bucket{5};
	auto p_bucket = &bucket;
	Bucket_Element elem1{1, "Hello"};
	Bucket_Element elem2{2, "World"};
	Bucket_Element elem3{3, "Foo"};
	Bucket_Element elem4{4, "Bar"};
	Bucket_Element elem5{5, "Baz"};
	p_bucket->Insert(elem1);
	p_bucket->Insert(elem2);
	p_bucket->Insert(elem3);
	p_bucket->Insert(elem4);
	p_bucket->Insert(elem5);

	std::string s;

	p_bucket->Remove(4);
	EXPECT_FALSE(p_bucket->Find(4, s));
	
	EXPECT_TRUE(p_bucket->Find(5, s));
	EXPECT_EQ(s, "Baz");
	
	p_bucket->Remove(1);
	EXPECT_FALSE(p_bucket->Find(1, s));
	
	p_bucket->Remove(3);
	EXPECT_FALSE(p_bucket->Find(3, s));


}
TEST(Bucket, can_add_and_retrieve_indices)
{
	using Bucket = ExtendibleHash<int,std::string, TestHash>::Bucket;

	Bucket bucket{2};
	auto p_bucket = &bucket;
	p_bucket->AddIndex({0,1,2,3});
	auto indices = p_bucket->GetIndices();
	EXPECT_NE(indices.end(), find(indices.begin(), indices.end(), 1));
	EXPECT_NE(indices.end(), find(indices.begin(), indices.end(), 2));
	EXPECT_NE(indices.end(), find(indices.begin(), indices.end(), 3));
}

TEST(Bucket, can_split_bucket)
{
	using Bucket = ExtendibleHash<int,std::string, TestHash>::Bucket;
	using Bucket_Element = ExtendibleHash<int,std::string,TestHash>::Bucket_Element;
	Bucket bucket{2};
	auto p_bucket = &bucket;
	p_bucket->AddIndex({0,1});
	Bucket_Element elem1{0, "Hello"};
	Bucket_Element elem2{1, "World"};
	p_bucket->Insert(elem1);
	p_bucket->Insert(elem2);
 
	auto p_new_bucket = p_bucket->SplitBucket();
	EXPECT_EQ(0, p_bucket->CompareDepths(1));
	EXPECT_EQ(0, p_new_bucket->CompareDepths(1));

	std::string s;
	
	EXPECT_EQ(true, p_bucket->Find(0, s));
	EXPECT_EQ(false, p_bucket->Find(1, s));

	EXPECT_EQ(true, p_new_bucket->Find(1, s));
	EXPECT_EQ(false, p_new_bucket->Find(0, s));

	EXPECT_EQ(1, (int)p_bucket->m_chain.size());
	EXPECT_EQ(1, (int)p_new_bucket->m_chain.size());

	EXPECT_EQ(0, (int)p_bucket->m_indices[0]);
	EXPECT_EQ(1, (int)p_new_bucket->m_indices[0]);
	EXPECT_EQ(1, (int)p_bucket->m_indices.size());
	EXPECT_EQ(1, (int)p_new_bucket->m_indices.size());


};


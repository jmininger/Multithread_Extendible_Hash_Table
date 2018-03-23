#include "gtest/gtest.h"
#include "extendible_hash.cpp"

#include <memory>
#include <string>

using Bucket = ExtendibleHash<int,std::string>::Bucket;
using Bucket_Element = ExtendibleHash<int,std::string>::Bucket_Element;

TEST(Bucket, can_insert_elements)
{
	Bucket b{3};
	auto pb = &b;
	std::string s = "HELLO";
	int i = 123;
	EXPECT_EQ(true, pb->Insert(Bucket_Element{i,s}));
	EXPECT_EQ(true, pb->Insert(Bucket_Element{234, "WOrlD"}));
	EXPECT_EQ(true, pb->Insert(Bucket_Element{5, "FOO"}));
	EXPECT_EQ(false, pb->Insert(Bucket_Element{90000, "BAR"}));
	EXPECT_EQ(false, pb->Insert(Bucket_Element{100, "foo"}));
	EXPECT_EQ(true, pb->Insert(Bucket_Element{5, "BAZ"}));

}
#include "gtest/gtest.h"
#include "extendible_hash.h"

#include <memory>

using Bucket = ExtendibleHash<int,std::string>::Bucket;
TEST(Bucket, can_get_depth)
{
	for(int i=0; i<10; i++)
	{
		std::unique_ptr<Bucket> bp{new Bucket(3, i)};
		auto depth = bp->GetDepth();
		EXPECT_EQ(i, depth);
	}
}


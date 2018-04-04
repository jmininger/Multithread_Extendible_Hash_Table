/*
 * extendible_hash.h : implementation of in-memory hash table using extendible
 * hashing
 *
 * Functionality: The buffer pool manager must maintain a page table to be able
 * to quickly map a PageId to its corresponding memory location; or alternately
 * report that the PageId does not match any currently-buffered page.
 */

#ifndef EXTENDIBLE_HASH_H
#define EXTENDIBLE_HASH_H

#include <cstdlib>
#include <vector>
#include <string>
#include <shared_mutex>
#include <mutex>
#include <initializer_list>
#include <atomic>
#include "gtest/gtest_prod.h"

template <typename K, typename V, typename Hash=std::hash<K>>
class ExtendibleHash {

private: 
  FRIEND_TEST(Table, can_insert_elements_one_at_a_time);
  FRIEND_TEST(Bucket, can_insert_elements_one_at_a_time);
  FRIEND_TEST(Bucket, can_single_threaded_search);
  FRIEND_TEST(Bucket, can_insert_concurrently);
  FRIEND_TEST(Bucket, can_remove);
  FRIEND_TEST(Bucket, can_split_bucket);
  FRIEND_TEST(Bucket, can_add_and_retrieve_indices);
private:
  class Bucket;
  struct Bucket_Element;

public:
  // constructor
  ExtendibleHash(size_t size);
  // destructor  
  //~ExtendibleHash();
  // helper function to generate hash addressing
  size_t HashKey(const K &key);
  // helper function to get global & local depth
  int GetGlobalDepth() const;
  

  int GetLocalDepth(int bucket_id) const;
  int GetNumBuckets() const;
  // lookup and modifier
  bool Find(const K &key, V &value);
  bool Remove(const K &key);
  void Insert(const K &key, const V &value);

private:
  void DoubleTable();
  // add your own member variables here

  std::vector<std::shared_ptr<Bucket>> m_table;
  std::atomic<int> m_global;
  size_t m_max_bucket_size; 
  mutable std::shared_timed_mutex m_table_mutex;

private:
  class Bucket
  {
    FRIEND_TEST(Bucket, can_insert_elements_one_at_a_time);
    FRIEND_TEST(Bucket, can_single_threaded_search);
    FRIEND_TEST(Bucket, can_insert_concurrently);
    FRIEND_TEST(Bucket, can_remove);
    FRIEND_TEST(Bucket, can_split_bucket);
    FRIEND_TEST(Bucket, can_add_and_retrieve_indices);
    public:      
      explicit Bucket(size_t size);
      bool Find(const K &key, V &value) const;//override
      bool Insert(Bucket_Element);//override
      bool Remove(const K &key);//override 
      std::shared_ptr<Bucket> SplitBucket();
      int CompareDepths(int global);//Returns global-local
      void AddIndex(std::initializer_list<size_t>);
      std::vector<size_t> GetIndices();

    private:
      const size_t m_max_size;

      size_t m_shared_bits;

      int m_depth;

      std::vector<size_t> m_indices;
    
      std::vector<Bucket_Element> m_chain;
    
    private:
      mutable std::shared_timed_mutex m_chain_mutex; 
  };
  struct Bucket_Element
  {
    K key;
    V value;
  };
};

#endif
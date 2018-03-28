/*
 * extendible_hash.h : implementation of in-memory hash table using extendible
 * hashing
 *
 * Functionality: The buffer pool manager must maintain a page table to be able
 * to quickly map a PageId to its corresponding memory location; or alternately
 * report that the PageId does not match any currently-buffered page.
 */

#pragma once

#include <cstdlib>
#include <vector>
#include <string>
#include <shared_mutex>
#include <mutex>
#include <unordered_map>
#include <set>


template <typename K, typename V>
class ExtendibleHash {
  //friend class Bucket;
  public:
  class Bucket;
  class BucketManager;
  struct Bucket_Element;
public:
  // constructor
  ExtendibleHash(size_t size);
  // destructor  
  ~ExtendibleHash();
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
  // add your own member variables here
  size_t m_max_bucket_size; 

public:
  class Bucket
  {
    public:      
      explicit Bucket(size_t size);
      bool Find(const K &key, V &value) const;//override
      bool Insert(Bucket_Element);//override
      void Remove(const K &key);//override 
      Bucket* SplitBucket();
      int CompareDepths(int global);//Returns global-local
      void AddIndex(size_t index);

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

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
  struct Bucket_Element;
public:
  // constructor
//   ExtendibleHash(size_t size);
//   // destructor  
//   ~ExtendibleHash();
//   // helper function to generate hash addressing
//   size_t HashKey(const K &key);
//   // helper function to get global & local depth
//   int GetGlobalDepth() const;
//   int GetLocalDepth(int bucket_id) const;
//   int GetNumBuckets() const;
//   // lookup and modifier
//   bool Find(const K &key, V &value);
//   bool Remove(const K &key);
//   void Insert(const K &key, const V &value);

// private:
//   size_t GetIndex(size_t hash);
//   Bucket* GetBucket(const size_t hash_val);
//   Bucket* SplitBucket(Bucket* p_bucket);


// private:
//   // add your own member variables here
//   size_t m_max_bucket_size;

//   int m_depth;
//   mutable std::mutex m_depth_mutex;
  
//   std::vector<Bucket*> m_table;
//   mutable std::shared_timed_mutex m_table_mutex;

//   std::unordered_map<Bucket*, std::vector<size_t>> m_bucket_to_index;
  class BucketManager
  {
    /* Returns a proper bucket ptr based on a hash*/
    /* GetBucket(), GrowTable(), SplitBucket() */

    // holds a shared lock
    // holds an atomic global so changing the global and GetGlobal don't interfere with each other
    // On table growth, writers lock on the table
    // If bucket insert fails, lock the BM, try again, and then grow or split
  };
  class Bucket
  {
    //friend Bucket* ExtendibleHash::SplitBucket(Bucket* p_bucket);
    public:      
      explicit Bucket(size_t size);
      /*  only EH::insert() will use this func after global 
          has been locked, and only thread can lock global at a 
          time, so there is no worry of m_depth being in a 
          datarace as long as it is not used elsewhere 
      */
      // bool IsDepthsEqual(int global) const;
      // bool GetElem(const K &key, V &val);
      // bool Remove(const K &key);
      bool Insert(Bucket_Element);
    private:
     // Bucket(std::vector<Bucket_Element>&& chain, 
     //        size_t max_size, 
     //        int depth);//should only be called in 

    private:
      int m_id;
      size_t m_max_size;
      // int m_depth;
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

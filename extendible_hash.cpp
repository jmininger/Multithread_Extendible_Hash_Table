#include <list>
#include <algorithm>
#include <functional>
#include <utility>

#include "extendible_hash.h"


/************************************************************************************/
/*
    Public constructor
*/
template <typename K, typename V>
ExtendibleHash<K, V>::Bucket::Bucket(size_t max_size)
:m_max_size(max_size), m_shared_bits(0), m_depth(0)
{
  m_indices.push_back(0);
  m_indices.push_back(1);
  m_chain = std::vector<Bucket_Element>();
  m_chain.reserve(max_size);
}

/*
   Insert an element;
*/
template <typename K, typename V>
bool ExtendibleHash<K, V>::Bucket::Insert(
  typename ExtendibleHash<K, V>::Bucket_Element elem)
{
  m_chain_mutex.lock();
  /* Enter Critical Section */
  bool ret = false;
  K key = elem.key;
  auto iter = find_if(std::begin(m_chain), std::end(m_chain),
                    [&key](Bucket_Element e){
                      return (e.key == key);
                    });
  bool isUpdate = std::end(m_chain) != iter;
  if(isUpdate)
  {
    iter->value = elem.value;
    ret = true;
  }
  else if(m_chain.size() < m_max_size)
  {
    m_chain.push_back(elem);
    ret = true;
  }
  /* Exit Critical Section */
  m_chain_mutex.unlock();
  return ret;
}

/*
   Find an element;
*/
template <typename K, typename V>
bool ExtendibleHash<K, V>::Bucket::Find(const K& key, V& value) const
{
  m_chain_mutex.lock_shared();
  auto iter = find_if(std::begin(m_chain), std::end(m_chain),
                    [&key](Bucket_Element e){
                      return (e.key == key);
                    });
  bool isFound = iter != std::end(m_chain);
  if(isFound)
  {
    value = iter->value;
  }
  m_chain_mutex.unlock_shared();
  return isFound;
}

/*
   Remove an element;
*/
template <typename K, typename V>
void ExtendibleHash<K, V>::Bucket::Remove(const K &key)
{
    m_chain_mutex.lock();
    auto iter = find_if(std::begin(m_chain), std::end(m_chain),
                    [&key](Bucket_Element e){
                      return (e.key == key);
                    });
    if(iter != std::end(m_chain))
    {
      m_chain.erase(iter);
    }
    m_chain_mutex.unlock();
}


template <typename K, typename V>
typename ExtendibleHash<K, V>::Bucket* ExtendibleHash<K, V>::Bucket::SplitBucket()
{
  using Bucket = typename ExtendibleHash<K, V>::Bucket;
  using Bucket_Element = typename ExtendibleHash<K, V>::Bucket_Element;

  auto p_new_bucket = new Bucket(m_max_size);
  p_new_bucket->m_shared_bits = m_shared_bits & (1 << m_depth);
  p_new_bucket->m_depth = m_depth;

  size_t bitmask = (1 << m_depth)-1;
  size_t new_shared_bits = p_new_bucket->m_shared_bits;
  
  auto chn_prtn_iter = std::partition(std::begin(m_chain), std::end(m_chain), 
                  [bitmask, new_shared_bits](Bucket_Element elem)
                  {
                      std::hash<K> hash{};
                      return((hash(elem.key) & bitmask) != new_shared_bits);
                  });
  std::copy(chn_prtn_iter, std::end(m_chain),back_inserter(p_new_bucket->m_chain));
  m_chain.erase(std::begin(m_chain), chn_prtn_iter);

  auto ind_prtn_iter = std::partition(std::begin(m_indices), std::end(m_indices), 
                  [bitmask, new_shared_bits](size_t index)
                  {
                      return((index & bitmask) != new_shared_bits);
                  });
  std::copy(ind_prtn_iter, std::end(m_indices),back_inserter(p_new_bucket->m_indices));
  m_indices.erase(std::begin(m_indices), ind_prtn_iter);
  
  m_depth++;
  p_new_bucket->m_depth++;
  return p_new_bucket;
}


template <typename K, typename V>
int ExtendibleHash<K, V>::Bucket::CompareDepths(int global)
{
  return (global - m_depth);
}

// /************************************************************************************/
// /*
//  * constructor
//  * array_size: fixed array size for each bucket
//  */
// template <typename K, typename V>
// ExtendibleHash<K, V>::ExtendibleHash(size_t size): m_max_bucket_size(size) 
// {
	

// }

// template <typename K, typename V>
// ExtendibleHash<K, V>::~ExtendibleHash() 
// {
 
// }

// /*
//  * helper function to calculate the hashing address of input key
//  */
// template <typename K, typename V>
// size_t ExtendibleHash<K, V>::HashKey(const K &key) {
//   return std::hash<K>{}(key);
// }

// template <typename K, typename V>
// size_t ExtendibleHash<K, V>::GetIndex(size_t hash)
// {
  
// }

// /*
//  * helper function to return global depth of hash table
//  * NOTE: you must implement this function in order to pass test
//  */
// template <typename K, typename V>
// int ExtendibleHash<K, V>::GetGlobalDepth() const {
// }

// /*
//  * helper function to return local depth of one specific bucket
//  * NOTE: you must implement this function in order to pass test
//  */
// template <typename K, typename V>
// int ExtendibleHash<K, V>::GetLocalDepth(int bucket_id) const {
//   //Note: must put read lock on chain of bucket
// }

// /*
//  * helper function to return current number of bucket in hash table
//  */
// template <typename K, typename V>
// int ExtendibleHash<K, V>::GetNumBuckets() const {
// }

// template <typename K, typename V>
// typename ExtendibleHash<K, V>::Bucket* 
// ExtendibleHash<K, V>::GetBucket(const size_t hash_val) {
  
// }

// /*
//  * lookup function to find value associate with input key
//  */
// template <typename K, typename V>
// bool ExtendibleHash<K, V>::Find(const K &key, V &value) {
  
// }

// /*
//    Split/Rehash a full bucket
// */
// template <typename K, typename V>
// typename ExtendibleHash<K, V>::Bucket* ExtendibleHash<K, V>::SplitBucket(typename ExtendibleHash<K, V>::Bucket* p_bucket)
// {
  
// }

//  /* 
//   * delete <key,value> entry in hash table
//   * Shrink & Combination is not required for this project
//   */ 
 
// template <typename K, typename V>
// bool ExtendibleHash<K, V>::Remove(const K &key) {
  
// }

// /*
//  * insert <key,value> entry in hash table
//  * Split & Redistribute bucket when there is overflow and if necessary increase
//  * global depth
//  */
// template <typename K, typename V>
// void ExtendibleHash<K, V>::Insert(const K &key, const V &value) {
  
// }

// test purpose
template class ExtendibleHash<int, std::string>;
template class ExtendibleHash<int, std::list<int>::iterator>;
template class ExtendibleHash<int, int>;
template class ExtendibleHash<std::string, int>;

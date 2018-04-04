#include <list>
#include <algorithm>
#include <functional>
#include <utility>

#include "extendible_hash.h"
#include "test_utils.h"

//#include <iostream>

/************************************************************************************/
/*
    Public constructor
*/
template <typename K, typename V, typename Hash>
ExtendibleHash<K, V, Hash>::Bucket::Bucket(size_t max_size)
:m_max_size(max_size), m_shared_bits(0), m_depth(0)
{
  m_chain = std::vector<Bucket_Element>();
  m_chain.reserve(max_size);
}

/*
   Insert an element;
*/
template <typename K, typename V, typename Hash>
bool ExtendibleHash<K, V, Hash>::Bucket::Insert(
  typename ExtendibleHash<K, V, Hash>::Bucket_Element elem)
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
template <typename K, typename V, typename Hash>
bool ExtendibleHash<K, V, Hash>::Bucket::Find(const K& key, V& value) const
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
template <typename K, typename V, typename Hash>
bool ExtendibleHash<K, V, Hash>::Bucket::Remove(const K &key)
{
    m_chain_mutex.lock();
    auto iter = find_if(std::begin(m_chain), std::end(m_chain),
                    [&key](Bucket_Element e){
                      return (e.key == key);
                    });
    bool isFound = iter != std::end(m_chain);
    if(isFound)
    {
      m_chain.erase(iter);
    }
    m_chain_mutex.unlock();
    return isFound;
}


template <typename K, typename V, typename Hash>
std::shared_ptr<typename ExtendibleHash<K, V, Hash>::Bucket> 
ExtendibleHash<K, V, Hash>::Bucket::SplitBucket()
{
  using Bucket = typename ExtendibleHash<K, V, Hash>::Bucket;
  using Bucket_Element = typename ExtendibleHash<K, V, Hash>::Bucket_Element;
  
  m_chain_mutex.lock();

  auto p_new_bucket = std::shared_ptr<Bucket>{new Bucket(m_max_size)};

  m_depth++;
  p_new_bucket->m_depth = m_depth;
  p_new_bucket->m_shared_bits = m_shared_bits | (1 << (m_depth-1));


  size_t bitmask = (1 << m_depth)-1;
  size_t new_shared_bits = p_new_bucket->m_shared_bits;
  
  auto chn_prtn_iter = std::partition(std::begin(m_chain), std::end(m_chain), 
                  [bitmask, new_shared_bits](Bucket_Element elem)
                  {
                      Hash hash{};
                      return((hash(elem.key) & bitmask) != new_shared_bits);
                  });
  std::copy(chn_prtn_iter, std::end(m_chain),back_inserter(p_new_bucket->m_chain));
  m_chain.erase(chn_prtn_iter, std::end(m_chain));

  auto ind_prtn_iter = std::partition(std::begin(m_indices), std::end(m_indices), 
                  [bitmask, new_shared_bits](size_t index)
                  {
                      return((index & bitmask) != new_shared_bits);
                  });
  std::copy(ind_prtn_iter, std::end(m_indices),back_inserter(p_new_bucket->m_indices));
  m_indices.erase(ind_prtn_iter, std::end(m_indices));

  m_chain_mutex.unlock();  
  return p_new_bucket;
}


template <typename K, typename V, typename Hash>
int ExtendibleHash<K, V, Hash>::Bucket::CompareDepths(int global)
{
  return (global - m_depth);
}
template <typename K, typename V, typename Hash>
void ExtendibleHash<K, V, Hash>::Bucket::AddIndex(std::initializer_list<size_t> init_list)
{
  //assert init_list.size() > acceptable#of inidices given the depth
  m_indices.insert(std::end(m_indices), init_list);
}

template <typename K, typename V, typename Hash>
std::vector<size_t>  ExtendibleHash<K, V, Hash>::Bucket::GetIndices()
{
  return m_indices;
}
/************************************************************************************/
/*
 * constructor
 * array_size: fixed array size for each bucket
 */
template <typename K, typename V, typename Hash>
ExtendibleHash<K, V, Hash>::ExtendibleHash(size_t size): 
m_global(1), m_max_bucket_size(size)
{
  std::shared_ptr<Bucket> b1{new Bucket(m_max_bucket_size)};
  b1->AddIndex({0});
  m_table.push_back(std::move(b1));

  std::shared_ptr<Bucket> b2{new Bucket(m_max_bucket_size)};
  b2->AddIndex({1});
  m_table.push_back(std::move(b2));

}

// template <typename K, typename V>
// ExtendibleHash<K, V, Hash>::~ExtendibleHash() 
// }
 
// }

/*
 * helper function to calculate the hashing address of input key
 */
template <typename K, typename V, typename Hash>
size_t ExtendibleHash<K, V, Hash>::HashKey(const K &key) 
{
  int global = m_global;
  int bitmask = ((1 << global)-1);
  return bitmask & Hash{}(key);
}

/*
 * helper function to return global depth of hash table
 * NOTE: you must implement this function in order to pass test
 */
template <typename K, typename V, typename Hash>
int ExtendibleHash<K, V, Hash>::GetGlobalDepth() const 
{
  return m_global;
}

/*
 * helper function to return local depth of one specific bucket
 * NOTE: you must implement this function in order to pass test
//  */
// template <typename K, typename V>
// int ExtendibleHash<K, V>::GetLocalDepth(int bucket_id) const 
// {
//   //Note: must put read lock on chain of bucket
// }

// /*
//  * helper function to return current number of bucket in hash table
//  */
// template <typename K, typename V>
// int ExtendibleHash<K, V>::GetNumBuckets() const {
// }

/*
 * lookup function to find value associate with input key
 */
template <typename K, typename V, typename Hash>
bool ExtendibleHash<K, V, Hash>::Find(const K &key, V &value) 
{
  m_table_mutex.lock_shared();
  auto bucket = m_table[HashKey(key)];
  bool isSuccess = bucket->Find(key, value);
  m_table_mutex.unlock_shared();
  return isSuccess;
}

//  /* 
//   * delete <key,value> entry in hash table
//   * Shrink & Combination is not required for this project
//   */ 
 
template <typename K, typename V, typename Hash>
bool ExtendibleHash<K, V, Hash>::Remove(const K &key)
{
  m_table_mutex.lock_shared();
  auto bucket = m_table[HashKey(key)];
  bool isFound = bucket->Remove(key);
  m_table_mutex.unlock_shared();
  return isFound;
}

/*
 * insert <key,value> entry in hash table
 * Split & Redistribute bucket when there is overflow and if necessary increase
 * global depth
 */
template <typename K, typename V, typename Hash>
void ExtendibleHash<K, V, Hash>::Insert(const K &key, const V &value) 
{
  m_table_mutex.lock_shared();
  auto bucket = m_table[HashKey(key)];
  bool isFailure = !bucket->Insert(Bucket_Element{key,value});
  m_table_mutex.unlock_shared();
  if(isFailure)
  {
    m_table_mutex.lock();
    bucket = m_table[HashKey(key)];
    if(bucket->Insert(Bucket_Element{key,value}))
    {
      m_table_mutex.unlock();
      return;
    }
    if(0 == bucket->CompareDepths(m_global))
    {
      DoubleTable();
    }
    auto new_bucket = bucket->SplitBucket();
    for(auto i: new_bucket->GetIndices())
    {
      m_table[i] = new_bucket;
    }
    m_table_mutex.unlock();
    Insert(key, value);
  }

}

template <typename K, typename V, typename Hash>
void ExtendibleHash<K, V, Hash>::DoubleTable()
{
  //Assumes a lock has already been taken out
  size_t old_size = m_table.size();
  m_global++;
  m_table.resize(1 << m_global);
  for(size_t i = 0; i<old_size; ++i)
  {
    m_table[old_size + i] = m_table[i];
    m_table[i]->AddIndex({old_size + i});
  }
}

// test purpose


template class ExtendibleHash<int, std::string, TestHash>;
template class ExtendibleHash<int, std::string>;
template class ExtendibleHash<int, std::list<int>::iterator>;
template class ExtendibleHash<int, int>;
template class ExtendibleHash<std::string, int>;

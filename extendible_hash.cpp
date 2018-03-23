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
ExtendibleHash<K, V>::Bucket::Bucket(size_t max_size, int depth)
:m_depth(depth)
{
  m_chain = std::vector<Bucket_Element>();
  m_chain.reserve(max_size);
}

/*
    Private constructor makes a Bucket out of another bucket
*/

// template <typename K, typename V>
// ExtendibleHash<K, V>::Bucket::Bucket(
//     std::vector<Bucket_Element>&& chain,
//     size_t max_size, 
//     int depth):
// m_depth(depth)
// {
//   m_chain = chain;
//   m_chain.reserve(max_size);

// }

// /*
//     Compare the local and global depths
// */
// template <typename K, typename V>
// bool ExtendibleHash<K, V>::Bucket::IsDepthsEqual(int global) const
// {
// }

// /*
//    Search for an element within a bucket
// */
// template <typename K, typename V>
// bool ExtendibleHash<K, V>::Bucket::GetElem(const K &key, V &val)
// {
// }

// /*
//    Attempt to insert an element;
// */
// template <typename K, typename V>
// bool ExtendibleHash<K, V>::Bucket::Insert(const K &key, const V &value)
// {

 
// }

template <typename K, typename V>
int ExtendibleHash<K, V>::Bucket::GetDepth()
{
  return m_depth;  
}

// template <typename K, typename V>
// bool ExtendibleHash<K, V>::Bucket::Remove(const K &key)
// {
 
// }
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

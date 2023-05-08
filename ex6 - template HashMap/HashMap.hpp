#ifndef _HASHMAP_I_HPP_
#define _HASHMAP_I_HPP_

#include <stdexcept>
#include <algorithm>
#include <vector>
#include <cmath>
#include <set>

#define MIN_FACTOR 0.25
#define MAX_FACTOR 0.75
#define INITIAL_CAPACITY 16

template <class KeyT, class ValueT>
class HashMap
{
 protected:
  std::vector<std::pair<KeyT,ValueT>> * _hashtable;
  int _capacity;
  int _size;

  /**
   * hash func
   * @param key
   * @return the right index
   */
  int hash_function(const KeyT& key) const
  {
    std::hash<KeyT> hash;
    return hash(key) & (_capacity - 1);
  }

  /**
   * calculates the new capacity needed
   * @return new capacity
   */
  int calculate_new_capacity()
  {
    if(_size == 0) {return 1;}
    return (get_load_factor() < MIN_FACTOR) ? _capacity/2 : _capacity*2;
  }

  /**
   * rehashing the hash table with new capacity
   */
  void rehash()
  {
    while ((get_load_factor() < MIN_FACTOR || get_load_factor() > MAX_FACTOR))
    {
      int old_capacity = _capacity;
      _capacity = calculate_new_capacity();
      auto temp_hashtable = new std::vector<std::pair<KeyT,ValueT>>[_capacity];
      for(int i = 0; i < old_capacity; ++i)
      {
        for (const auto & elem : _hashtable[i])
        {
          temp_hashtable[hash_function (elem.first)].emplace_back
              (std::pair<KeyT,ValueT>(elem.first, elem.second));
        }
        _hashtable[i].clear();
        _hashtable[i].shrink_to_fit();
      }
      delete[] _hashtable;
      _hashtable = temp_hashtable;
      if(_capacity == 1) {break;}
    }
  }

 public:
  /**
   * default constructor
   */
  HashMap()
  {
    _capacity = INITIAL_CAPACITY;
    _size = 0;
    _hashtable = new std::vector<std::pair<KeyT,ValueT>>[_capacity];
  }

  /**
   * vectors constructor
   * @param keys
   * @param values
   */
  HashMap(const std::vector<KeyT>& keys, const std::vector<ValueT>& values)
  {
    if(keys.size() != values.size())
    {
      throw std::length_error("not the same size!");
    }
    _capacity = INITIAL_CAPACITY;
    _size = 0;
    _hashtable = new std::vector<std::pair<KeyT,ValueT>>[_capacity];
    for (int i = keys.size()-1; i >= 0; --i)
    {
      insert (keys[i], values[i]);
    }
  }

  /**
   * copy constructor
   * @param hash1
   */
  HashMap(const HashMap &hash1)
  {
    _capacity = hash1._capacity;
    _size = hash1._size;
    _hashtable = new std::vector<std::pair<KeyT,ValueT>>[_capacity];
    for (int i = 0; i < _capacity; ++i)
    {
      std::copy (hash1._hashtable[i].begin(), hash1._hashtable[i].end(),
                 std::back_inserter (_hashtable[i]));
    }
  }

  /**
   * destructor
   */
  virtual ~HashMap()
  {
    for (int i = 0; i < _capacity; ++i)
    {
      _hashtable[i].clear();
      _hashtable[i].shrink_to_fit();
    }
    delete[] _hashtable;
  }

  //************ getters ************

  /**
   * size of hash map
   * @return num of elements in hash map
   */
  int size() const
  {
    return _size;
  }

  /**
   * capacity of hash map
   * @return capacity
   */
  int capacity() const
  {
    return _capacity;
  }

  /**
   * is empty
   * @return true if empty, false else
   */
  bool empty() const
  {
    return _size == 0;
  }

  /**
   * calculates load factor
   * @return
   */
  double get_load_factor() const
  {
    return ((double)_size / (double)_capacity);
  }

  //************ methods ************

  /**
   * clearing all hash map
   */
  void clear()
  {
    for (int i = 0; i < _capacity; ++i)
    {
      _hashtable[i].clear();
    }
    _size = 0;
  }

  /**
   * checks if key in hash map
   * @param key
   * @return true if so, else false
   */
  bool contains_key(const KeyT& key) const
  {
    int idx = hash_function (key);
    for (const auto& elem : _hashtable[idx])
    {
      if (elem.first == key)
      {
        return true;
      }
    }
    return false;
  }

  /**
   * inserting new element to hash map
   * @param key
   * @param value
   * @return true if succeed, else false
   */
  bool insert(const KeyT& key, const ValueT& value)
  {
    if (contains_key(key))
    {
      return false;
    }
    _hashtable[hash_function(key)].emplace_back(key, value);
    ++_size;
    if(get_load_factor() > MAX_FACTOR) {rehash();}
    return true;
  }

  /**
   * erasing element by key
   * @param key
   * @return true if succeed, else false
   */
  virtual bool erase(const KeyT & key)
  {
    if (!contains_key(key))
    {
      return false;
    }
    int index = hash_function (key);
    for (auto iter = _hashtable[index].begin(); iter !=
    _hashtable[index].end(); iter++)
    {
      if (iter->first == key)
      {
        _hashtable[index].erase(iter);
        _size--;
        break;
      }
    }
    if(get_load_factor() < MIN_FACTOR) {rehash();}
    return true;
  }

  /**
   * bucket size of the table that contains key
   * @param key
   * @return the size
   */
  int bucket_size(const KeyT& key) const
  {
    if (!contains_key (key))
    {
      throw std::out_of_range("this key isn't in hash table");
    }
    int index = hash_function (key);
    return _hashtable[index].size();
  }

  /**
   * the bucket of key index
   * @param key
   * @return index
   */
  int bucket_index(const KeyT& key) const
  {
    if (!contains_key (key))
    {
      throw std::out_of_range("this key isn't in hash table");
    }
    return hash_function (key);
  }

  //************ access ************

  ValueT& at(const KeyT& key)
  {
    int idx = hash_function (key);
    if (!contains_key (key))
    {
      throw std::out_of_range("key isn't in hash map");
    }
    auto it = _hashtable[idx].begin();
    for (;it != _hashtable[idx].end(); ++it)
    {
      if (it->first == key)
      {
        break;
      }
    }
    return it->second;
  }

  const ValueT& at(const KeyT& key) const
  {
    if (!contains_key (key))
    {
      throw std::out_of_range("key isn't in hash map");
    }
    int idx = hash_function (key);
    auto it = _hashtable[idx].begin();
    for (;it != _hashtable[idx].end(); ++it)
    {
      if (it->first == key)
      {
        break;
      }
    }
    return it->second;
  }

  ValueT operator[](const KeyT& key) const
  {
    int index = hash_function (key);
    auto it = _hashtable[index].begin();
    for (;it != _hashtable[index].end(); ++it)
    {
      if (it->first == key)
      {
        break;
      }
    }
    return it->second;
  }

  ValueT& operator[](const KeyT& key)
  {
    if (!contains_key (key))
    {
      this->insert (key, ValueT());
    }
    int index = hash_function (key);
    auto it = _hashtable[index].begin();
    for (;it != _hashtable[index].end(); it++)
    {
      if (it->first == key)
      {
        break;
      }
    }
    return it->second;
  }

  //************ operators ************

  HashMap<KeyT,ValueT>& operator=(const HashMap<KeyT,ValueT>& other)
  {
    if (&other != this)
    {
      HashMap<KeyT,ValueT> temp(other);
      swap (*this,temp);
    }
    return *this;
  }

  friend void swap(HashMap<KeyT,ValueT>& lhs_hash,HashMap<KeyT,ValueT>&
      rhs_hash)
  {
    using std::swap;
    swap (lhs_hash._size, rhs_hash._size);
    swap (lhs_hash._capacity, rhs_hash._capacity);
    swap (lhs_hash._hashtable,rhs_hash._hashtable);
  }

  bool operator==(const HashMap<KeyT,ValueT>& other) const
  {
    if (_size != other._size)
    {
      return false;
    }
    for (int i = 0; i <_capacity; ++i)
    {
      for (const auto & elem : _hashtable[i])
      {
        KeyT key = elem.first;
        ValueT value = elem.second;
        if (!(other.contains_key (key)) || other[key] != value)
        {
          return false;
        }
      }
    }
    return true;
  }

  bool operator!=(const HashMap<KeyT,ValueT>& other) const
  {
    return !operator== (other);
  }

  class ConstIterator
  {
    friend class HashMap;
   private:
    const HashMap<KeyT,ValueT>& hash_map;
    size_t table_idx;
    size_t pair_idx;

   public:
    typedef const std::pair<KeyT,ValueT> value_type;
    typedef const value_type & reference;
    typedef const value_type * pointer;
    typedef std::ptrdiff_t difference_type; // irrelevant here,
    // as we have no difference - but still required
    typedef std::forward_iterator_tag iterator_category;

   private:
    ConstIterator(const HashMap<KeyT,ValueT>& hash_map, size_t table_idx,
                  size_t pair_idx): hash_map(hash_map), table_idx(table_idx)
                  , pair_idx(pair_idx) {}
   public:
    ConstIterator& operator++()
    {
      const std::vector<std::pair<KeyT,ValueT>> & cur_table = hash_map
          ._hashtable[table_idx];
      if (++pair_idx >= cur_table.size())
      {
        while (hash_map._hashtable[++table_idx].size() == 0 && table_idx <
        hash_map._capacity){}
        pair_idx = 0;
      }
      return *this;
    }

    ConstIterator operator++(int)
    {
      ConstIterator temp (*this);
      ++(*this);
      return temp;
    }

    bool operator==(const ConstIterator & rhs) const
    {
        return (hash_map._hashtable == rhs.hash_map._hashtable && table_idx
        == rhs.table_idx && pair_idx == rhs.pair_idx);
    }

    bool operator!= (const ConstIterator &rhs) const
    {
      return !operator== (rhs);
    }

    reference operator* () const
    {
      return hash_map._hashtable[table_idx].at(pair_idx);
    }

    pointer operator-> () const
    {
      return &(operator* ());
    }
  };
  using const_iterator = ConstIterator;

  //************ iterator functions ************

  const_iterator cbegin() const
  {
    if (_size == 0)
    {
      return end();
    }
    int i = 0;
    for (; i < _capacity; ++i)
    {
      if(_hashtable[i].size() > 0)
      {
        break;
      }
    }
    return const_iterator(*this, i, 0);
  }

  const_iterator cend() const
  {
    return const_iterator(*this, _capacity, 0);
  }

  const_iterator end() const
  {
    return cend();
  }

  const_iterator begin() const
  {
    return cbegin();
  }

};

#endif //_HASHMAP_I_HPP_

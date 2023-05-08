#ifndef _HASHMAP_I_HPP_
#define _HASHMAP_I_HPP_

#include <stdexcept>
#include "vector"
#include "list"
#include <cmath>

//template <class int, class int>
class HashMap
{
 private:
  std::vector<std::pair<int,int>> * _hashtable;
  int _capacity;
  int _size;

  int hash_function(const int& key) const
  {
    std::hash<int> hash;
    return hash(key) & (_capacity - 1); //TODO: add std::hash for generic
  }

  bool key_in_vec(const std::vector<std::pair<int,int>>& vec, const int&
  key) const
  {
    for (const auto & elem : vec)
    {
      if (elem.first == key)
      {
        return true;
      }
    }
    return false;
  }

 public:
  /**
   * default constructor
   */
  HashMap()
  {
    _capacity = 16;
    _size = 0;
    _hashtable = new std::vector<std::pair<int,int>>[_capacity];
  }

  HashMap(const std::vector<int>& keys, const std::vector<int>& values)
  {
    if(keys.size() != values.size())
    {
      throw std::runtime_error("not the same size!");
    }
    _capacity = (int)pow(2,ceil(sqrt(keys.size())));
    _size = (int)values.size();
    _hashtable = new std::vector<std::pair<int,int>>[_capacity];
    for (int i = 0; i < _size; ++i)
    {
      int idx = hash_function(keys[i]);
      _hashtable[idx].emplace_back(std::pair<int,int>(keys[i],values[i]));
    }
  }

  HashMap(const HashMap &hash1)
  {
    _capacity = hash1._capacity;
    _size = hash1._size;
    _hashtable = new std::vector<std::pair<int,int>>[_capacity];
    for (int i = 0; i < _capacity; ++i)
    {
      std::copy (hash1._hashtable[i].begin(), hash1._hashtable[i].end(),
                 std::back_inserter (_hashtable[i]));
    }
  }

  ~HashMap()
  {
    for (int i = 0; i < _capacity; ++i)
    {
      _hashtable[i].clear();
      _hashtable[i].shrink_to_fit();
    }
    delete[] _hashtable;
  }



  int size() const
  {
    return _size;
  }

  int capacity() const
  {
    return _capacity;
  }

  bool empty() const
  {
    return _size == 0;
  }

  void clear()
  {
    for (int i = 0; i < _capacity; ++i)
    {
      _hashtable[i].clear();
    }
    _size = 0;
  }

  double get_load_factor() const
  {
    return ((double)_size / (double)_capacity);
  }

  bool contains_key(const int& key) const
  {
    for (int i = 0; i < _capacity; ++i)
    {
      if (key_in_vec(_hashtable[i], key))
      {
        return true;
      }
    }
    return false;
  }

  bool insert(const int& key, const int& value)
  {
    if (contains_key(key))
    {
      return false;
    }
    int index = hash_function (key);
    _hashtable[hash_function(key)].emplace_back(key, value);
    ++_size;
    if (get_load_factor() > 0.75)
    {
      _capacity *= 2;
      auto temp_hashtable = new std::vector<std::pair<int,int>> (_capacity);
      for(int i = 0; i < _capacity/2; ++i) //TODO: need to hashing again
      {
        for (const auto & elem : _hashtable[i])
        {
          temp_hashtable[hash_function (elem.first)].emplace_back
              (std::pair<int,int>(elem.first, elem.second));
        }
        _hashtable[i].clear();
        _hashtable[i].shrink_to_fit();
      }
      delete[] _hashtable;
      _hashtable = temp_hashtable;
    }
    return true;
  }

  bool erase(const int & key)
  {
    int index = hash_function (key);
    for (auto iter = _hashtable[index].begin(); iter !=
                                                       _hashtable[index].end(); iter++)
    {
      if (iter->first == key)
      {
        _hashtable[index].erase(iter);
        return true;
      }
    }
    return false;
  }

  int at(const int& key) const
  {
    if (!contains_key (key))
    {
      throw std::out_of_range("key isn't in hash map");
    }
    int idx = hash_function (key);
    for (const auto & elem : _hashtable[idx])
    {
      if (elem.first == key)
      {
        return elem.second;
      }
    }
  }

  int bucket_size(const int& key) const
  {
    int index = hash_function (key);
    return _hashtable[index].size();
  }

  int bucket_index(const int& key) const
  {
    return hash_function (key);
  }

  int operator[](const int& key) const
  {
    int index = hash_function (key);
    for (const auto& elem : _hashtable[index])
    {
      if (elem.first == key)
      {
        return elem.second;
      }
    }
  }

  int& operator[](const int& key)
  {
    int index = hash_function (key);
    if (!contains_key (key))
    {
      this->insert (key, int());
    }
    for (auto & elem : _hashtable[index])
    {
      if (elem.first == key)
      {
        return elem.second;
      }
    }
  }

  HashMap<int,int>& operator=(const HashMap<int,int>& hash1)
  {
    HashMap<int,int> temp(hash1);
    swap (temp,hash1);
  }

  friend void swap(HashMap<int,int> lhs_hash,HashMap<int,int>
  rhs_hash)
  {
    using std::swap;
    swap (lhs_hash._size, rhs_hash._size);
    swap (lhs_hash._capacity, rhs_hash._capacity);
    swap (lhs_hash._hashtable,rhs_hash._hashtable);
  }
};


#endif //_HASHMAP_I_HPP_

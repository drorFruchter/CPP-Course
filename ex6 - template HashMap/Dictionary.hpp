#ifndef _DICTIONARY_HPP_
#define _DICTIONARY_HPP_

#include "HashMap.hpp"

class InvalidKey : public std::invalid_argument
{
 public:
  InvalidKey(): std::invalid_argument ("The key is invalid") {}
  explicit InvalidKey(const std::string& message):
      std::invalid_argument (message) {}
};

 class Dictionary : public HashMap<std::string,std::string>
 {
  public:
   /**
    * default constructor
    */
  Dictionary() = default;

  /**
   * vectors constructor
   * @param keys
   * @param values
   */
  Dictionary(const std::vector<std::string>& keys,
             const std::vector<std::string>& values)
             : HashMap<std::string, std::string> (keys, values) {}

  /**
   * erasing element by key, throws exception if key is invalid
   * @param key
   * @return true if succeed
   */
  bool erase(const std::string & key) override
  {
    if (!contains_key(key))
    {
      throw InvalidKey();
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
   * updating the dictionary
   * @tparam T1
   * @param begin
   * @param end
   */
  template<typename T1>
  void update(T1 begin, T1 end)
  {
    if (begin == end) {return;}
    for (auto & elem = begin; elem != end; elem++)
    {
      (*this)[elem->first] = elem->second;
    }
  }
 };

#endif //_DICTIONARY_HPP_

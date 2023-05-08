#include "RecommenderSystem.h"
#include <set>
#include <math.h>

bool RecommenderSystem::movies_comp (const sp_movie& movie1, const sp_movie&
movie2)
{
  return (*movie1 < *movie2);
}

RecommenderSystem::RecommenderSystem()
{
  database_map d(movies_comp);
  _database = d;
}

sp_movie RecommenderSystem::add_movie(const std::string& name,int year,const
std::vector<double>& features)
{
  sp_movie movie_p = std::make_shared<Movie>(name, year);
  _database[movie_p] = features;
  return movie_p;
}

/**
 * calculates the average of user ranks
 * @param ranks
 * @return double which is the average
 */
double get_avg(rank_map& ranks)
{
  double sum = 0;
  int counter = 0;
  for(const auto& val : ranks)
  {
    sum += val.second;
    counter++;
  }
  return sum / counter;
}

std::vector<double> RecommenderSystem::calculate_featurs(rank_map& cur_ranks)
{
  sp_movie first_movie = cur_ranks.begin()->first;
  unsigned long int length = _database[first_movie].size();
  std::vector<double> final_features(length, 0);
  for (auto& movie : cur_ranks)
  {
    std::vector<double> cur_features = _database[movie.first];
    for (auto& elem : cur_features)
    {
      elem *= movie.second;
    }
    for (unsigned int i = 0; i < length; ++i)
    {
      final_features[i] += cur_features[i];
    }
  }
  return final_features;
}


double RecommenderSystem::calculate_diff(const std::vector<double> &
    movie_features, const std::vector<double> & user_features)
{
  double final_diff = 0, norm1 = 0, norm2 = 0;
  unsigned long int length = movie_features.size();
  for (unsigned int i = 0; i < length; ++i)
  {
    final_diff += (user_features[i] * movie_features[i]);
    norm1 += (user_features[i] * user_features[i]);
    norm2 += (movie_features[i] * movie_features[i]);
  }
  norm1 = std::sqrt (norm1);
  norm2 = std::sqrt (norm2);
  return final_diff / (norm1 * norm2);
}

sp_movie RecommenderSystem::recommend_by_content(const RSUser& user)
{
  rank_map cur_ranks = user.get_ranks();                          //step 1
  double avg = get_avg(cur_ranks);
  for (auto& val : cur_ranks)
  {
    val.second -= avg;
  }
  std::vector<double> user_features = calculate_featurs (cur_ranks); //step 2
  sp_movie recommend;
  double diff = 0, max_diff = 0;
  bool is_first = true;
  for(const auto& movie: _database)
  {
    if (cur_ranks.find (movie.first) != cur_ranks.end())
    {
      continue;
    }
    diff = calculate_diff(movie.second, user_features);
    if (diff > max_diff || is_first)
    {
      is_first = false;
      max_diff = diff;
      recommend = movie.first;
    }
  }
  return recommend;
}


double RecommenderSystem::predict_movie_score(const RSUser &user, const
sp_movie &movie, int k)
{
  std::multimap<double, sp_movie, std::greater<>> diff_map;
  for (const auto& sec_movie: user.get_ranks())
  {
    double movie_diff = calculate_diff(_database[movie], _database[sec_movie
    .first]);
    diff_map.insert (std::pair<double, sp_movie>(movie_diff, sec_movie.first));
  }
  auto it = diff_map.begin();
  double user_score = 0, diff_sum = 0;
  for (int i = 0; i < k; ++i)
  {
    user_score += ((it->first) * (user.get_ranks()[it->second]));
    diff_sum += it->first;
    it++;
  }
  return user_score / diff_sum;
}



sp_movie RecommenderSystem::recommend_by_cf(const RSUser& user, int k)
{
  double movie_prediction = 0 , max_predict = 0;
  sp_movie recommend;
  for (const auto & movie : _database)
  {
    if (user.get_ranks().find (movie.first) != user.get_ranks().end())
    {
      continue;
    }
    movie_prediction = predict_movie_score (user, movie.first, k);
    if (movie_prediction > max_predict)
    {
      max_predict = movie_prediction;
      recommend = movie.first;
    }
  }
  return recommend;
}


sp_movie RecommenderSystem::get_movie(const std::string &name, int year) const
{
  sp_movie movie_p = std::make_shared<Movie>(name, year);
  if (_database.find (movie_p) != _database.end())
  {
    return movie_p;
  }
  return nullptr;
}

std::ostream& operator<<(std::ostream& s, RecommenderSystem&
rc_system)
{
  for (const auto & movie: rc_system._database)
  {
    s << *(movie.first);
  }
  return s;
}
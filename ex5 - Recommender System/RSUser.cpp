//
// Created on 2/20/2022.
//

// don't change those includes
#include "RSUser.h"
#include "RecommenderSystem.h"
#include <algorithm>
#include <utility>


RSUser::RSUser(const std::string& name, rank_map& ranks,
               std::shared_ptr<RecommenderSystem> rc_system):
               _name(name), _ranks (ranks),
               _RC_system (std::move(rc_system)) {}

const std::string& RSUser::get_name() const
{
  return _name;
}

void RSUser::add_movie_to_rs(const std::string &name, int year,
                     const std::vector<double> &features,
                     double rate)
{
  if(rate < LOWEST_RANK || rate > HIGHEST_RANK)
  {
    throw std::runtime_error("invalid rate for the movie");
  }
  _RC_system->add_movie (name, year, features);
  _ranks[_RC_system->get_movie (name, year)] = rate;
}

rank_map RSUser::get_ranks() const
{
  return _ranks;
}

sp_movie RSUser::get_recommendation_by_content() const
{
  return _RC_system->recommend_by_content ((*this));
}

sp_movie RSUser::get_recommendation_by_cf(int k) const
{
  return _RC_system->recommend_by_cf(*this, k);
}

double RSUser::get_prediction_score_for_movie(const std::string& name, int
year, int k) const
{
  sp_movie movie = _RC_system->get_movie (name, year);
  if (movie == nullptr)
  {
    throw std::runtime_error("Movie isn't in system");
  }
  return _RC_system->predict_movie_score(*this, movie , k);
}

std::ostream& operator<<(std::ostream& s, RSUser& user)
{
  s << "name: " << user._name << std::endl;
  s << *(user._RC_system) << std::endl;
  return s;
}
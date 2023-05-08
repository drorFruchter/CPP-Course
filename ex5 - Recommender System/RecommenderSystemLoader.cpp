#include "RecommenderSystemLoader.h"
#include <fstream>
#include <sstream>


std::unique_ptr<RecommenderSystem>
    RecommenderSystemLoader::create_rs_from_movies_file(const std::string&
    movies_file_path) noexcept(false)
{
  auto rec_sys = std::make_unique<RecommenderSystem>();
  std::ifstream movies_file(movies_file_path);
  if (movies_file)
  {
    std::string one_line;
    while (std::getline (movies_file, one_line))
    {
      std::istringstream buffer (one_line);
      std::string name;
      int year;
      std::getline (buffer, name, '-');
      buffer >> year;
      std::vector<double> features;
      double feature;
      while (buffer >> feature)
      {
        if (feature < LOWEST_RANK || feature > HIGHEST_RANK)
        {
          movies_file.close();
          throw std::runtime_error("wrong features");
        }
        features.push_back (feature);
      }
      (*rec_sys).add_movie (name, year, features);
    }
  }
  else
  {
    throw std::runtime_error ("invalid file");
  }
  movies_file.close();
  return rec_sys;
}
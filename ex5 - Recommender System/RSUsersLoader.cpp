#include "RSUsersLoader.h"
#include "fstream"
#include "sstream"

#define PRIME_NUM 11


std::vector<sp_movie> create_movie_vec (std::istringstream & buffer)
{
  std::string movie;
  std::vector<sp_movie> movies_vec;
  while(buffer >> movie)
  {
    std::string name = movie.substr (0, movie.find ('-'));
    int year = std::stoi(movie.substr (movie.find ('-')+1, movie.length()));
    sp_movie new_movie = std::make_shared<Movie>(name, year);
    movies_vec.push_back (new_movie);
  }
  return movies_vec;
}

rank_map create_ranks_for_user(std::istringstream & buffer,
                               std::vector<sp_movie>& movies_vec)
{
  std::string rate;
  int counter = 0;
  rank_map ranks(PRIME_NUM, sp_movie_hash, sp_movie_equal);
  while (buffer >> rate)
  {
    if (rate == "NA")
    {
      ++counter;
      continue;
    }
    double rank = std::stod (rate);
    if (rank < LOWEST_RANK || rank > HIGHEST_RANK)
    {
      throw std::runtime_error("invalid rate");
    }
    ranks[movies_vec[counter]] = rank;
    ++counter;
  }
  return ranks;
}

std::vector<sp_movie> handle_first_line(std::ifstream& users_file)
{
  std::string new_line;
  std::istringstream buffer (new_line);
  std::vector<sp_movie> movies_vec;
  if (std::getline (users_file, new_line))
  {
    buffer = std::istringstream(new_line);
    movies_vec = create_movie_vec (buffer);
  }
  return movies_vec;
}

std::vector<RSUser> handle_table(std::ifstream& users_file,
                                 std::vector<sp_movie>& movies_vec,
                                 const std::shared_ptr<RecommenderSystem>&
                                     rs_ptr)
{
  std::vector<RSUser> users_vec;
  std::string new_line;
  std::istringstream buffer (new_line);
  while(std::getline (users_file, new_line))
  {
    buffer = std::istringstream(new_line);
    std::string name_user;
    buffer >> name_user;
    try
    {
      rank_map ranks = create_ranks_for_user(buffer, movies_vec);
      RSUser new_user(name_user, ranks, rs_ptr);
      users_vec.push_back (new_user);
    }
    catch (std::runtime_error& e)
    {
      users_file.close ();
      throw e;
    }
  }
  return users_vec;
}


std::vector<RSUser> RSUsersLoader::create_users_from_file(const std::string&
users_file_path, std::unique_ptr<RecommenderSystem> rs) noexcept(false)
{
  std::vector<RSUser> users_vec;
  std::shared_ptr<RecommenderSystem> rs_ptr(std::move (rs));
  std::ifstream users_file(users_file_path);
  if (users_file)
  {
    std::vector<sp_movie> movies_vec = handle_first_line (users_file);
    users_vec = handle_table (users_file, movies_vec, rs_ptr);
    users_file.close();
  }
  else
  {
    throw std::runtime_error("invalid file");
  }
  return users_vec;
}
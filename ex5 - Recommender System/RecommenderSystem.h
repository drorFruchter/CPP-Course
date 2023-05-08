//
// Created on 2/20/2022.
//

#ifndef SCHOOL_SOLUTION_RECOMMENDERSYSTEM_H
#define SCHOOL_SOLUTION_RECOMMENDERSYSTEM_H
#include "RSUser.h"
#include <map>

typedef std::map<sp_movie, std::vector<double>, equal_func> database_map;

class RecommenderSystem
{
 private:
   database_map _database;
   /**
    * calculates the user features for recommendation by content
    * @param cur_ranks
    * @return user features
    */
   std::vector<double> calculate_featurs(rank_map& cur_ranks);
   /**
    * calculate the difference between user features vec to movie
    * @param movie_features
    * @param user_features
    * @return double value which represent the difference
    */
   double calculate_diff(const std::vector<double> & movie_features,
                        const std::vector<double> & user_features);
   /**
    * movies comparator for ordered map
    * @param movie1
    * @param movie2
    * @return true or false
    */
   static bool movies_comp(const sp_movie& movie1, const sp_movie& movie2);

public:

	explicit RecommenderSystem();
    /**
     * adds a new movie to the system
     * @param name name of movie
     * @param year year it was made
     * @param features features for movie
     * @return shared pointer for movie in system
     */
	sp_movie add_movie(const std::string& name,int year,
                       const std::vector<double>& features);


    /**
     * a function that calculates the movie with
     * highest score based on movie features
     * @param ranks user ranking to use for algorithm
     * @return shared pointer to movie in system
     */
	sp_movie recommend_by_content(const RSUser& user);

    /**
     * a function that calculates the movie with highest predicted
     * score based on ranking of other movies
     * @param ranks user ranking to use for algorithm
     * @param k
     * @return shared pointer to movie in system
     */
	sp_movie recommend_by_cf(const RSUser& user, int k);


    /**
     * Predict a user rating for a movie given argument using
     * item cf procedure with k most similar movies.
     * @param user_rankings: ranking to use
     * @param movie: movie to predict
     * @param k:
     * @return score based on algorithm as described in pdf
     */
	double predict_movie_score(const RSUser &user, const sp_movie &movie,
												  int k);

	/**
	 * gets a shared pointer to movie in system
	 * @param name name of movie
	 * @param year year movie was made
	 * @return shared pointer to movie in system
	 */
	sp_movie get_movie(const std::string &name, int year) const;


	friend std::ostream& operator<<(std::ostream& s, RecommenderSystem&
    rc_system);

};


#endif //SCHOOL_SOLUTION_RECOMMENDERSYSTEM_H

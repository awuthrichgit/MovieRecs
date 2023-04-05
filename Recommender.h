#ifndef RECOMMENDER_INCLUDED
#define RECOMMENDER_INCLUDED

#include <string>
#include <vector>
#include <unordered_map>

class UserDatabase;
class MovieDatabase;
class Recommender;
class Movie;

struct MovieAndRank
{
    MovieAndRank(const std::string& id, int score)
     : movie_id(id), compatibility_score(score)
    {}
    
    MovieAndRank(const std::string& id, int score, const MovieDatabase& mdb)
    : movie_id(id), compatibility_score(score), m_ptrMDB(&mdb)
    {}

    std::string movie_id;
    int compatibility_score;
    
    bool operator<(const MovieAndRank& rhs) const;
    //bool operator==(const MovieAndRank& rhs);
    
    //bool operator()(const MovieAndRank& x, const MovieAndRank& y) const;
private:
    const MovieDatabase* m_ptrMDB;
};

class Recommender
{
  public:
    Recommender(const UserDatabase& user_database,
                const MovieDatabase& movie_database);
    std::vector<MovieAndRank> recommend_movies(const std::string& user_email,
                                               int movie_count) const;

  private:
    //const bool order(const MovieAndRank& firstM, const MovieAndRank& secondM) const;
    void updateDS(std::unordered_map<std::string, int>& DS, std::vector<Movie*>& relatedMovies, std::vector<std::string>& watchHist, int amount) const; //two vectors, datastructure, and amount
    
    const UserDatabase* m_ptrUserDB;
    const MovieDatabase* m_ptrMovieDB;
};

//inline
//bool compareMaR(const MovieAndRank& lhs, const MovieAndRank& rhs)
//{
//    return lhs < rhs;
//}
//
//inline
//bool compareMaRPtr(const MovieAndRank* lhs, const MovieAndRank* rhs)
//{
//    // TODO: You implement this.  Using the same criteria as compareStudent,
//    //       compare two Students POINTED TO by lhs and rhs.  Think about
//    //       how you can do it in one line by calling compareStudent.
//    return compareMaR(*lhs, *rhs);
//}


#endif // RECOMMENDER_INCLUDED

#ifndef MOVIEDATABASE_INCLUDED
#define MOVIEDATABASE_INCLUDED

#include "treemm.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

class Movie;

class MovieDatabase
{
  public:
    MovieDatabase();
    bool load(const std::string& filename);
    Movie* get_movie_from_id(const std::string& id) const;
    std::vector<Movie*> get_movies_with_director(const std::string& director) const;
    std::vector<Movie*> get_movies_with_actor(const std::string& actor) const;
    std::vector<Movie*> get_movies_with_genre(const std::string& genre) const;

  private:
    void partitionLinebyComma(const std::string& line, std::vector<std::string>& container);
    //template<typename treemultimap>
    //void individualInsert(const std::vector<std::string>& container);
    
    TreeMultimap<std::string, Movie*> m_idMap;
    TreeMultimap<std::string, Movie*> m_directorMap; //FIXME: second type might have to be a vector...
    TreeMultimap<std::string, Movie*> m_actorMap;
    TreeMultimap<std::string, Movie*> m_genreMap;
    
    std::vector<Movie*> m_movies; //for deletion of movies upon termination of program
};

#endif // MOVIEDATABASE_INCLUDED

#include "MovieDatabase.h"
#include "Movie.h"

#include <string>
#include <vector>
#include <algorithm>
using namespace std;

MovieDatabase::MovieDatabase()
{
    // Replace this line with correct code.
}

bool MovieDatabase::load(const string& filename)
{
    //open file
    ifstream infile(filename);
    if ( ! infile )                // Did opening the file fail?
    {
        cerr << "Error: Cannot open " << filename << "!" << endl;
        return false;
    }
    
    //variables to load in movies
    std::string line;
    
    //specific to each movie
    std::string currId;
    std::string currTitle;
    std::string currReleaseYr;
    std::vector<std::string> currDirectors;
    std::vector<std::string> currActors;
    std::vector<std::string> currGenres;
    float currRating = -1;
    
    int counter = 0; //to keep track of the lines
    bool moreToRead;
    bool completeMovie;
    
    do
    {
        //outer loop dynamically allocates a movie each iteration
        
        moreToRead = false;
        completeMovie = false;
        //reads in a line and processes until end of file
        while (!completeMovie && getline(infile, line))
        {
            moreToRead = true;
            completeMovie = false;
            
            if(line == "") //in between movies
            {
                counter = -1; //reset the counter
            }
    
            switch (counter)
            {
                case -1: //blank line so continue
                    completeMovie = true; //signal new Movie
                    break;
                case 0: //ID
                    currId = line;
                    break;
                case 1: //email
                    currTitle = line;
                    break;
                case 2: //number of Movies
                    currReleaseYr = line;
                    break;
                case 3: //director(s)
                    partitionLinebyComma(line, currDirectors);
                    break;
                case 4: //actors(s)
                    partitionLinebyComma(line, currActors);
                    break;
                case 5: //genre(s)
                    partitionLinebyComma(line, currGenres);
                    break;
                case 6: //rating
                    currRating = std::stof(line);
                    break;
    
                default: //don't think this should ever be called?
                    std::cerr << "Error in constructing a movie! Passed count 6." << std::endl;
                    break;
            }
    
            counter++; //incriment counter
        }
        
        if(currId != "" && currRating >= 0) //all info for movie attained
        {
            //create a new movie with the info and insert into tree
            Movie* p = new Movie(currId, currTitle, currReleaseYr, currDirectors, currActors, currGenres, currRating);
            m_movies.push_back(p);
            m_idMap.insert(currId, p);
            
            int largest = max({currDirectors.size(), currActors.size(), currGenres.size()});
            for(int i = 0; i < largest; i++)
            {
                if(i < currDirectors.size())
                    m_directorMap.insert(currDirectors.at(i), p);
                if(i < currActors.size())
                    m_actorMap.insert(currActors.at(i), p);
                if(i < currGenres.size())
                    m_genreMap.insert(currGenres.at(i), p);
            }
            
            //reset all of the variables
            currId = "";
            currTitle = "";
            currReleaseYr = "";
            currDirectors.clear();
            currActors.clear();
            currGenres.clear();
            currRating = -1;
        }
        
    } while(moreToRead);
    
    if(m_movies.size() > 0) //check if any users were created
        return true;
    else
        return false;  // default
}

Movie* MovieDatabase::get_movie_from_id(const string& id) const
{
    TreeMultimap<std::string, Movie*>::Iterator it = m_idMap.find(id);
    if(it.is_valid())
        return it.get_value();
    return nullptr;
}

vector<Movie*> MovieDatabase::get_movies_with_director(const string& director) const
{
    std::vector<Movie*>* ptrToVecMovies = nullptr;
    TreeMultimap<std::string, Movie*>::Iterator it = m_directorMap.find(director);
    TreeMultimap<std::string, Movie*>::Iterator temp(it, ptrToVecMovies);
    if(temp.is_valid() && ptrToVecMovies != nullptr)
        return *ptrToVecMovies;
    return vector<Movie*>();  // Replace this line with correct code.
}

vector<Movie*> MovieDatabase::get_movies_with_actor(const string& actor) const
{
    std::vector<Movie*>* ptrToVecActors = nullptr;
    TreeMultimap<std::string, Movie*>::Iterator it = m_actorMap.find(actor);
    TreeMultimap<std::string, Movie*>::Iterator temp(it, ptrToVecActors);
    if(temp.is_valid() && ptrToVecActors != nullptr)
        return *ptrToVecActors;
    return vector<Movie*>();  // Replace this line with correct code.
}

vector<Movie*> MovieDatabase::get_movies_with_genre(const string& genre) const
{
    std::vector<Movie*>* ptrToVecGenre = nullptr;
    TreeMultimap<std::string, Movie*>::Iterator it = m_genreMap.find(genre);
    TreeMultimap<std::string, Movie*>::Iterator temp(it, ptrToVecGenre);
    if(temp.is_valid() && ptrToVecGenre != nullptr)
        return *ptrToVecGenre;
    return vector<Movie*>();  // Replace this line with correct code.
}

void MovieDatabase::partitionLinebyComma(const std::string& line, std::vector<std::string>& container)
{
    int i = 0;
    std::string name = "";
    while(line[i] != '\0')
    {
        if(line[i] == ',')
        {
            //finished name
            container.push_back(name);
            //reset name
            name = "";
        }
        else
            name+=line[i]; //add character to current name
        i++;
    }
    //pushback final name
    container.push_back(name);
}

//void MovieDatabase::individualInsert(const std::vector<std::string>& container)
//{
//
//}

#include "Recommender.h"
#include "UserDatabase.h"
#include "MovieDatabase.h"
#include "Movie.h"

#include <string>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

Recommender::Recommender(const UserDatabase& user_database,
                         const MovieDatabase& movie_database)
{
    // Replace this line with correct code.
    m_ptrUserDB = &user_database;
    m_ptrMovieDB = &movie_database;
}

vector<MovieAndRank> Recommender::recommend_movies(const string& user_email, int movie_count) const
{
    //data structure for ordering MovieAndRank at end
    priority_queue<MovieAndRank> m_MovieAndRank;
    
    //hash table data structure with o(1) operations. Used for lookups, insertions, editing of db info
    std::unordered_map<string, int> m_components;
    
    //get a pointer to the current User using the recommender
    User* currUser = m_ptrUserDB->get_user_from_email(user_email);
    if(currUser == nullptr)
        return vector<MovieAndRank>();
    
    //iterate through currWatchHist for each movie id
    vector<std::string> currWatchHist = currUser->get_watch_history();
    for(int i = 0; i < currWatchHist.size(); i++)
    {
        //query movieDB for movie in watch hist
        Movie* currMovie = m_ptrMovieDB->get_movie_from_id(currWatchHist.at(i));
        
        if(currMovie == nullptr)
            return vector<MovieAndRank>();
    
        //for a given movie, check other movies w those directors
        for(int j = 0; j < currMovie->get_directors().size(); j++)
        {
            //each j is a compatible director from that movie
            //vector pointing to movies with that director
            vector<Movie*> temp = m_ptrMovieDB->get_movies_with_director(currMovie->get_directors().at(j));

            //update the unordered map DS based director overlap
            updateDS(m_components, temp, currWatchHist, 20);
        }
        
        //for a given movie, check other movies w those actors
        for(int j = 0; j < currMovie->get_actors().size(); j++)
        {
            //each j is a compatible actor from that movie
            //vector pointing to movies with that actor
            vector<Movie*> temp = m_ptrMovieDB->get_movies_with_actor(currMovie->get_actors().at(j));

            //update the unordered map DS based actor overlap
            updateDS(m_components, temp, currWatchHist, 30);
        }
        
        //for a given movie, check other movies w those genres
        for(int j = 0; j < currMovie->get_genres().size(); j++)
        {
            //each j is a compatible genre from that movie
            //vector pointing to movies with that genre
            vector<Movie*> temp = m_ptrMovieDB->get_movies_with_genre(currMovie->get_genres().at(j));

            //update the unordered map DS based genre overlap
            updateDS(m_components, temp, currWatchHist, 1);
        }
    }
    
    //to be returned
    vector<MovieAndRank> result;
    
    //construct a priority queue (max heap) from the unordered map (hash table)
    //this creates a MovieAndRank pair for each movie and then orders them by compatability score (and other specified criteria in the spec)
    for(unordered_map<string, int>::iterator it = m_components.begin(); it != m_components.end(); it++)
    {
        m_MovieAndRank.push(MovieAndRank((*it).first,(*it).second,*m_ptrMovieDB));
    }
    
    //return movie_count or the max number of MovieAndRank pairs which ever comes first
    int temp = m_MovieAndRank.size();
    int size = std::min(movie_count, temp);
    for(int i = 0; i < size; i++)
    {
        result.push_back(m_MovieAndRank.top());
        m_MovieAndRank.pop();
    }
    
    return result;
}


bool MovieAndRank::operator<(const MovieAndRank& rhs) const
{
    if(rhs.compatibility_score > this->compatibility_score)
        return true;
    else if (rhs.compatibility_score < compatibility_score)
        return false;
    else
    {
        float rating1 = m_ptrMDB->get_movie_from_id(rhs.movie_id)->get_rating();
        float rating2 = m_ptrMDB->get_movie_from_id(movie_id)->get_rating();
        //equal so compare rating
        if(rating1 > rating2)
            return true;
        else if(rating1 < rating2)
            return false;
        else
        {
            //compare name w alphabetical ordering
            if(rhs.movie_id > movie_id)
                return true;
            else
                return false;
        }
    }
    
    return true; //default
}

void Recommender::updateDS(std::unordered_map<std::string, int>& DS, std::vector<Movie*>& relatedMovies, std::vector<std::string>& watchHist, int amount) const
{
    //iterate through the vector and create a MovieAndRank for each movie w score + 20
    for(int k = 0; k < relatedMovies.size(); k++)
    {
        //check if movie has not been watched
        if(std::find(watchHist.begin(), watchHist.end(), relatedMovies.at(k)->get_id()) == watchHist.end())
        {
            //check if movie already in unordered map
            string s = relatedMovies.at(k)->get_id();
            unordered_map<string, int>::iterator it = DS.find(s);
            if(it != DS.end())
            {
                //movie already in unordered map, so update the rank score by 30 for actors
                (*it).second+=amount;
            }
            else
                //insert into unordered map
                DS.insert({relatedMovies.at(k)->get_id(), amount});
        }
    }
}

//bool MovieAndRank::operator==(const MovieAndRank& rhs)
//{
//    if(movie_id == rhs.movie_id)
//        return true;
//    return false;
//}

//bool MovieAndRank::operator()(const MovieAndRank& x, const MovieAndRank& rhs) const
//{
//    if(rhs.compatibility_score > compatibility_score)
//        return true;
//    else if (rhs.compatibility_score < compatibility_score)
//        return false;
//    else
//    {
//        float rating1 = m_ptrMDB->get_movie_from_id(rhs.movie_id)->get_rating();
//        float rating2 = m_ptrMDB->get_movie_from_id(movie_id)->get_rating();
//        //equal so compare rating
//        if(rating1 > rating2)
//            return true;
//        else if(rating1 < rating2)
//            return false;
//        else
//        {
//            //compare name w alphabetical ordering
//            if(rhs.movie_id > movie_id)
//                return true;
//            else
//                return false;
//        }
//    }
//
//    return false; //default
//}



//for a given movie, check other movies w those directors
//            for(int j = 0; j < currMovie->get_directors().size(); j++)
//            {
//                //each j is a compatible director
//                //vector pointing to movies with that director
//                vector<Movie*> temp = m_ptrMovieDB->get_movies_with_director(currMovie->get_directors().at(j));
//
//                //iterate through the vector and create a MovieAndRank for each movie w score + 20
//                for(int k = 0; k < temp.size(); k++)
//                {
//                    //check if movie has already been watched
//                    if(std::find(currWatchHist.begin(), currWatchHist.end(), temp.at(k)->get_id()) == currWatchHist.end())
//                    {
//                        //insert MovieAndRank for that unwatched movie
//                        m_MovieAndRank.push_back(MovieAndRank(temp.at(k)->get_id(), 20)); //FIXME: faster w pointers
//                    }
//                }
//            }
//
//            //check other movies w that movies actors
//            for(int j = 0; j < currMovie->get_actors().size(); j++)
//            {
//                //vector pointing to movies with that actor
//                vector<Movie*> temp = m_ptrMovieDB->get_movies_with_actor(currMovie->get_actors().at(j));
//
//                //iterate through the vector and create a MovieAndRank for each movie w score or add 30
//                for(int k = 0; k < temp.size(); k++)
//                {
//                    //check if movie has already been watched
//                    if(std::find(currWatchHist.begin(), currWatchHist.end(), temp.at(k)->get_id()) == currWatchHist.end())
//                    {
//                        //check if movie already has associated score
//                        //if(std::find(m_MovieAndRank.begin(), m_MovieAndRank.end(), temp.at(k)) == m_MovieAndRank.end()) //FIXME: problm
//                        {
//                            //if so add 30 to compatibility score
//                            m_MovieAndRank.at(k).compatibility_score+=30;
//                        }
//                        //insert MovieAndRank for that unwatched movie
//                        m_MovieAndRank.push_back(MovieAndRank(temp.at(k)->get_id(), 30));
//                    }
//                }
//            }
//            //check other movies with that movies genres
//            for(int j = 0; j < currMovie->get_genres().size(); j++)
//            {
//                //vector pointing to movies with that genre
//                vector<Movie*> temp = m_ptrMovieDB->get_movies_with_genre(currMovie->get_genres().at(j));
//
//                //iterate through the vector and create a MovieAndRank for each movie w score or add 1
//                for(int k = 0; k < temp.size(); k++)
//                {
//                    //check if movie has already been watched
//                    if(std::find(currWatchHist.begin(), currWatchHist.end(), temp.at(k)->get_id()) == currWatchHist.end())
//                    {
//                        //check if movie already has associated score
//                        //if(std::find(m_MovieAndRank.begin(), m_MovieAndRank.end(), temp.at(k)) == m_MovieAndRank.end()) //FIXME: problm
//                        {
//                            //if so add 1 to compatibility score
//                            m_MovieAndRank.at(k).compatibility_score+=1;
//                        }
//                        //insert MovieAndRank for that unwatched movie
//                        m_MovieAndRank.push_back(MovieAndRank(temp.at(k)->get_id(), 1));
//                    }
//                }
//            }

//    if(m_MovieAndRank.empty() == false)
//    {
//        //pop first movie_count reccs off the queue and put into vector, return that
//        int temp = m_MovieAndRank.size();
//        //either return movies desired or if more reccs desired than there are, return entire reccs list
//        int size = std::min(movie_count, temp);
//        for(int i = 0; i < size; i++)
//        {
//            result.push_back(m_MovieAndRank.top());
//            m_MovieAndRank.pop();
//        }
//
//    }

//    if(m_MovieAndRank.empty() == false)
//    {
//        int temp = m_MovieAndRank.size();
//        //either return movies desired or if more reccs desired than there are, return entire reccs list
//        int size = std::min(movie_count, temp);
//        multimap<MovieAndRank, Movie*>::iterator it = m_MovieAndRank.begin();
//        for(int i = 0; i < size; i++)
//        {
//            result.push_back((it)->first);
//            it++;
//        }
//    }

//vector<MovieAndRank*> ptrToMaR;
//priority_queue<MovieAndRank> m_priority;

//    for(list<MovieAndRank>::iterator it = m_MovieAndRank.begin(); it != m_MovieAndRank.end(); it++)
//    {
//        m_priority.push(*it);
//        if((*it).compatibility_score > 30)
//            cerr << "Yes!" << endl;
//    }

//        ptrToMaR.push_back(&m_MovieAndRank.at(i));

// Sort the vector of pointers using the STL sort algorithm
//sort(ptrToMaR.begin(), ptrToMaR.end(), compareMaRPtr);
//sort(m_MovieAndRank.begin(), m_MovieAndRank.end());

//sort the vector m_MovieAndRank
//std::sort(m_MovieAndRank.begin(), m_MovieAndRank.end(), order);

//select on the first movie_count to return
//vector<MovieAndRank> temp(m_MovieAndRank.begin(), m_MovieAndRank.begin()+movie_count);

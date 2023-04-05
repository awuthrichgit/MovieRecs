#include "UserDatabase.h"
#include "User.h"
#include "MovieDatabase.h"
#include "Movie.h"
#include <iostream>
#include <string>
#include "treemm.h"//FIXME: REMOVE
#include "Recommender.h"
using namespace std;

//////////////////////////i/////////////////////////////////////////////////////
//
// You are free to do whatever you want with this file, since you won't
// be turning it in.  Presumably, you will make changes to help you test
// your classes.  For example, you might try to implement only some User
// member functions to start out with, and so replace our main routine with
// one that simply creates a User and verifies that the member functions you
// implemented work correctly.
//
//////////////////////////i/////////////////////////////////////////////////////


  // If your program is having trouble finding these files. replace the
  // string literals with full path names to the files.  Also, for test
  // purposes, you may want to create some small, simple user and movie
  // data files to makde debuggiing easier, so you can replace the string
  // literals with the names of those smaller files.

const string USER_DATAFILE  = "/Users/austin/Desktop/CS32/Project4/Project4/users.txt";
const string MINI_USER_DATAFILE  = "/Users/austin/Desktop/CS32/Project4/Project4/miniusers.txt";
const string MOVIE_DATAFILE = "/Users/austin/Desktop/CS32/Project4/Project4/movies.txt";
const string MINI_MOVIE_DATAFILE = "/Users/austin/Desktop/CS32/Project4/Project4/minimovies.txt";

const string CAREY_MOVIE_DATAFILE = "moviecarey.txt";
const string CAREY_USER_DATAFILE = "usercarey.txt";

void findMatches(const Recommender& r, const MovieDatabase& md, const string& user_email,
                 int num_recommendations);

int main()
{
    MovieDatabase mdb;
    UserDatabase udb;
    if (!mdb.load(MOVIE_DATAFILE) || !udb.load(USER_DATAFILE))  // In skeleton, load always return false
    {
        cout << "Failed to load user data file " << USER_DATAFILE << "!" << endl;
        return 1;
    }

    Recommender r = Recommender(udb, mdb);
    
    string email = "AbFow2483@charter.net";
    //AbFow2483@charter.net
    
    findMatches(r, mdb, email, 100);

}

void findMatches(const Recommender& r, const MovieDatabase& md, const string& user_email,
int num_recommendations)
{
    // get up to ten movie recommendations for the user
    vector<MovieAndRank> recommendations = r.recommend_movies(user_email, num_recommendations);
    if (recommendations.empty())
        cout << "We found no movies to recommend :(.\n";
    else
    {
        for (int i = 0; i < recommendations.size(); i++)
        {
            const MovieAndRank& mr = recommendations[i];
            Movie* m = md.get_movie_from_id(mr.movie_id);
            cout << i << ". " << m->get_title() << " (" << m->get_release_year() << ")\n Rating: " << m->get_rating() << "\n Compatibility Score: " << mr.compatibility_score << "\n";
        }
    }
}

//    vector<string> hist;
//    for(int i = 0; i < 5; i++)
//        hist.push_back(to_string(i));
//    User Jim = User("Jim", "jwuthrich", hist);
//
//    for(int i = 0; i < 3; i++)
//        cout << Jim.get_watch_history()[i] << endl;
//
//

//cout << "SUCCESS" << endl;
//for (;;)
//{
//    cout << "Enter a [category] (or quit): ";
//    string category;
//    getline(cin, category);
//    if (category == "quit")
//        return 0;
//    //std::vector<Movie*> u = mdb.get_movies_with_director(category);
//    std::vector<Movie*> u = mdb.get_movies_with_genre(category);
//    //std::vector<Movie*> u = mdb.get_movies_with_actor(category);
//    if (u.size() == 0)
//        cout << "No movie in the database has that [category]." << endl;
//    else
//        {
//        for(int i = 0; i < u.size(); i++)
//            cout << (*u[i]).get_title() << endl;
//        }
//}

    
//    TreeMultimap<std::string, int> tmm;
//
//    tmm.insert("carey", 5);
//    tmm.insert("carey", 6);
//    tmm.insert("carey", 7);
//    tmm.insert("david", 25);
//    tmm.insert("david", 425);
//
//    TreeMultimap<std::string,int>::Iterator it = tmm.find("carey");
//    // prints 5, 6, and 7 in some order
//    while (it.is_valid()) {
//        std::cout << it.get_value() << std::endl;
//        it.advance();
//    }
//
//    it = tmm.find("david");
//    if (!it.is_valid())
//        std::cout << "david is not in the multimap!\n";
//    else
//    {
//        while (it.is_valid()) {
//            std::cout << it.get_value() << std::endl;
//            it.advance();
//        }
//    }
//
//    it = tmm.find("Carey");
//    if (!it.is_valid())
//        std::cout << "Carey is not in the multimap!\n";
//
//    tmm.insert("Carey", 10);
//    it = tmm.find("Carey");
//    if (it.is_valid())
//        std::cout << it.get_value() << std::endl;
//
//    it = tmm.find("laura");
//    if (!it.is_valid())
//        std::cout << "laura is not in the multimap!\n";

//UserDatabase udb;
//if (!udb.load(MINI_USER_DATAFILE))  // In skeleton, load always return false
//{
//    cout << "Failed to load user data file " << USER_DATAFILE << "!" << endl;
//    return 1;
//}
//for (;;)
//{
//    cout << "Enter user email address (or quit): ";
//    string email;
//    getline(cin, email);
//    if (email == "quit")
//        return 0;
//    User* u = udb.get_user_from_email(email);
//    if (u == nullptr)
//        cout << "No user in the database has that email address." << endl;
//    else
//        cout << "Found " << u->get_full_name() << endl;
//}

//cout << "Enter a movie director (or quit): ";
//string director;
//getline(cin, director);
//if (director == "quit")
//    return 0;
//std::vector<Movie*> u = mdb.get_movies_with_director(director);
//if (u.size() == 0)
//    cout << "No movie in the database has that director." << endl;
//else
//    {
//    for(int i = 0; i < u.size(); i++)
//        cout << (*u[i]).get_title() << endl;
//    }

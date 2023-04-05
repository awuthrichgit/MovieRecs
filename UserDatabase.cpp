#include "UserDatabase.h"
#include "User.h"

#include <string>
#include <vector>
using namespace std;

UserDatabase::UserDatabase()
{
    // Replace this line with correct code. //FIXME: do I need anything?
}

UserDatabase::~UserDatabase()
{
    //delete all dynamically allocated users
    for(int i = 0; i < m_users.size(); i++)
    {
        delete m_users.at(i);
    }
}

bool UserDatabase::load(const string& filename)
{
    //FIXME: test RETURN FALSE if file has been loaded already?? Load called more than once?
    //open file
    ifstream infile(filename);
    if ( ! infile )                // Did opening the file fail?
    {
        cerr << "Error: Cannot open " << filename << "!" << endl;
        return false;
    }
    
    std::string line;
    
    std::string currName;
    std::string currEmail;
    int currNumMovies = 0;
    std::vector<std::string> currWatchHist;
    
    int counter = 0; //to keep track of the lines
    bool moreToRead;
    bool completeUser;
    
    do
    {
        //outer loop dynamically allocates a user each iteration
        
        moreToRead = false;
        completeUser = false;
        //reads in a line and processes until end of file
        while (!completeUser && getline(infile, line))
        {
            moreToRead = true;
            completeUser = false;
            
            if(line == "") //in between people
            {
                counter = -1; //reset the counter
            }
    
            switch (counter)
            {
                case -1: //blank line so continue
                    completeUser = true; //signal new user
                    break;
                case 0: //name
                    currName = line; //FIXME: not sure if this also picks up the newline character...
                    break;
                case 1: //email
                    currEmail = line;
                    break;
                case 2: //number of Movies
                    //currNumMovies; //FIXME: do i need this?
                    break;
    
                default: //One of many movies
                    currWatchHist.push_back(line);
                    break;
            }
    
            counter++; //incriment counter
        }
        
        if(currEmail != "") //all info for user attained
        {
            //create a new user with the info and insert into tree
            User* p = new User(currName, currEmail, currWatchHist);
            m_users.push_back(p);
            m_userTreeMM.insert(currEmail, p);
            
            //reset all of the variables
            currNumMovies = 0;
            currName = "";
            currEmail = "";
            currWatchHist.clear();
        }
        
    } while(moreToRead);
    
    if(m_users.size() > 0) //check if any users were created
        return true;
    else
        return false;  // default
}

User* UserDatabase::get_user_from_email(const string& email) const
{
    TreeMultimap<std::string, User*>::Iterator it = m_userTreeMM.find(email);
    if(it.is_valid())
        return it.get_value();
    return nullptr;
}

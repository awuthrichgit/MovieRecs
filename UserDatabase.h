#ifndef USERDATABASE_INCLUDED
#define USERDATABASE_INCLUDED

#include "treemm.h"
#include "User.h"
#include <string>
#include <iostream>
#include <fstream>

//class User;

class UserDatabase
{
  public:
    UserDatabase();
    ~UserDatabase();
    bool load(const std::string& filename);
    User* get_user_from_email(const std::string& email) const;

  private:
    TreeMultimap<std::string, User*> m_userTreeMM; //stores Users
    std::vector<User*> m_users;
    
};

#endif // USERDATABASE_INCLUDED

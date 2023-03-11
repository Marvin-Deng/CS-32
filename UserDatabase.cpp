#include "UserDatabase.h"
#include "User.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

UserDatabase::UserDatabase()
{
    m_loaded = false;
}

bool UserDatabase::load(const string& filename)
{
    if (m_loaded) { // Checks if file already loaded
        return false;
    }
    ifstream infile(filename);    
    if (!infile)		        // Ckeck if opening the file failed
    {
        return false;
    }


    m_loaded = true;
    return true;  
}

User* UserDatabase::get_user_from_email(const string& email) const
{
    return nullptr;  // Replace this line with correct code.
}

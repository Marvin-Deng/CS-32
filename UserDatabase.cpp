#include "UserDatabase.h"
#include "User.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

UserDatabase::UserDatabase() {
    m_loaded = false;
}

UserDatabase::~UserDatabase() {
    TreeMultimap<std::string, User*>::Iterator iter;
    for (int i = 0; i < m_emails.size(); i++) {
        iter = m_map.find(m_emails.at(i));
        if (iter.is_valid()) {
            delete iter.get_value();
        }
        // cout << "Deleted" << endl;
    }
}

bool UserDatabase::load(const string& filename)
{
    if (m_loaded) { // Checks if file already loaded
        return false;
    }
    ifstream infile(filename);
    if (!infile)  // Ckeck if opening the file failed
    {
        return false;
    }
    string str;
    string name;
    string email;
    vector<string> movies;
    int numMovies = 0;
    bool isFirst = true;

    while (getline(infile, str)) {
        if (str.empty()) { // Empty line, insert new User profile
            m_map.insert(email, new User(name, email, movies));
            name.clear();
            email.clear();
            movies.clear();
            numMovies = 0;
        }
        if (name.empty()) { // First string is name
            name = str;
        }
        else if (email.empty()) { // Second string is email
            email = str;
            m_emails.push_back(email);
        }
        else if (numMovies == 0) { // Third string is number of movies
            numMovies = stoi(str);
        }
        else { // Other strings are movie IDs
            movies.push_back(str);
            numMovies--;
        }
    } 
    if (!email.empty()) { // Last line was not empty, insert last User into map
        m_map.insert(email, new User(name, email, movies));
    }
    m_loaded = true;
    return true;
}

User* UserDatabase::get_user_from_email(const string& email) const
{
    // Get the iterator that points to the target key
    TreeMultimap<std::string, User*>::Iterator iter = m_map.find(email); 
    if (iter.is_valid()) {
        return iter.get_value();
    }
    return nullptr;  
}
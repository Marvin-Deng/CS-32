#include "Recommender.h"
#include "UserDatabase.h"
#include "MovieDatabase.h"

#include <string>
#include <vector>
using namespace std;

Recommender::Recommender(const UserDatabase& user_database, const MovieDatabase& movie_database)
{
    m_userData = user_database;
    m_movieData = movie_database;
}

vector<MovieAndRank> Recommender::recommend_movies(const string& user_email, int movie_count) const
{
    User* user = m_userData.get_user_from_email(user_email);

    return vector<MovieAndRank>();  // Replace this line with correct code.
}


#include "Recommender.h"
#include "UserDatabase.h"
#include "MovieDatabase.h"

#include <string>
#include <vector>
#include <unordered_map>
using namespace std;

Recommender::Recommender(const UserDatabase& user_database, const MovieDatabase& movie_database)
{
    m_userData = user_database;
    m_movieData = movie_database;
}

vector<MovieAndRank> Recommender::recommend_movies(const string& user_email, int movie_count) const
{
    User* user = m_userData.get_user_from_email(user_email);
    vector<string> watchHistory = user->get_watch_history();
    int numRecs = watchHistory.size();
    if (numRecs <= 0) {
        return vector<MovieAndRank>();
    }
    
    unordered_map<Movie*, int> movieToScore;
    for (string id : watchHistory) {
        Movie* movie = m_movieData.get_movie_from_id(id);
        
        int score = 0;
        vector<string> directors = movie->get_directors();
        vector<string> actors = movie->get_actors();
        vector<string> genres = movie->get_genres();
        for (string dir : directors) {
            score += m_movieData.get_movies_with_director(dir).size() * 20;
        }
        for (string actor : actors) {
            score += m_movieData.get_movies_with_actor(actor).size() * 20;
        }
        for (string genre : genres) {
            score += m_movieData.get_movies_with_director(genre).size();
        }
        MovieAndRank movieRank(id, score);

    }

    return vector<MovieAndRank>();  // Replace this line with correct code.
}


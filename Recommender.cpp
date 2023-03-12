#include "Recommender.h"
#include "UserDatabase.h"
#include "MovieDatabase.h"
#include <unordered_map>
#include<set>
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
    vector<string> watchHistory = user->get_watch_history();
    int numRecs = static_cast<int> (watchHistory.size());
    if (numRecs <= 0) {
        return vector<MovieAndRank>();
    }
    unordered_map<Movie*, int> movieToScore;

    // Loop through the user's eatch history and find related movies
    for (string id : watchHistory) {
        Movie* movie = m_movieData.get_movie_from_id(id);
        
        int score = 0;
        vector<string> directors = movie->get_directors();
        vector<string> actors = movie->get_actors();
        vector<string> genres = movie->get_genres();
        vector<Movie*> directorMovies;
        vector<Movie*> actorMovies;
        vector<Movie*> genreMovies;

        // Get all the movies with the same directors, actors, and genres
        for (string dir : directors) {
            directorMovies = m_movieData.get_movies_with_director(dir);
        }
        for (string actor : actors) {
            actorMovies = m_movieData.get_movies_with_actor(actor);
        }
        for (string genre : genres) {
            genreMovies = m_movieData.get_movies_with_director(genre);
        }

        // Loop throught the common director, actor, and genre movies and determine the score
        for (Movie* movie : directorMovies) {
            movieToScore[movie] += 20;
        }

        for (Movie* movie : actorMovies) {
            movieToScore[movie] += 30;
        }

        for (Movie* movie : genreMovies) {
            movieToScore[movie] += 1;
        }
        directors.clear();
        actors.clear();
        genres.clear();
        directorMovies.clear();
        actorMovies.clear();
        genreMovies.clear();
    }
    

    // Create MovieAndRank objects and insert into set with comaprator set
    unordered_map<Movie*, int>::iterator iter = movieToScore.begin();
    while (iter != movieToScore.end()) {
        MovieAndRank rank(iter->first->get_id(), iter->second); // create MovieAndRank object
        m_movieRanking.insert(rank);
        iter++;
    }

    return vector<MovieAndRank>();  // Replace this line with correct code.
}


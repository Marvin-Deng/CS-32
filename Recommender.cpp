#include "Recommender.h"
#include "UserDatabase.h"
#include "MovieDatabase.h"
#include <unordered_map>
#include <map>
#include <set>
#include <string>
#include <vector>
#include <iostream>
using namespace std;

Recommender::Recommender(const UserDatabase& user_database, const MovieDatabase& movie_database)
{
    m_userData = &user_database;
    m_movieData = &movie_database;
}

vector<MovieAndRank> Recommender::recommend_movies(const string& user_email, int movie_count) const
{
    User* user = m_userData->get_user_from_email(user_email);
    vector<string> watchHistory = user->get_watch_history();
    int numRecs = movie_count;
    if (numRecs <= 0) {
        return vector<MovieAndRank>();
    }

    // Store the ids of watched movies
    set<string> watched;
    for (string id : watchHistory) {
        watched.insert(id);
    }

    // Stores id and compatability scores
    unordered_map<string, int> movieToScore;
    
    // Loop through the user's eatch history and find related movies
    for (string id : watchHistory) {
        Movie* movie = m_movieData->get_movie_from_id(id);

        int score = 0;
        vector<string> directors = movie->get_directors();
        vector<string> actors = movie->get_actors();
        vector<string> genres = movie->get_genres();

        // Evaluating Score 
        // 1. Get all the movies with the same directors, actors, and genres
        // 2. Loop through the common director, actor, and genre movies and increase the score
        for (string dir : directors) {
            vector<Movie*> directorMovies; m_movieData->get_movies_with_director(dir);
            for (Movie* movie : directorMovies) {
                string id = movie->get_id();
                if (watched.find(movie->get_id()) == watched.end()) {
                    movieToScore[id] += 20;
                    //cout << movie->get_title() << " " << movieToScore[movie->get_id()] << endl;
                }
            }
        }
        for (string actor : actors) {
            vector<Movie*> actorMovies = m_movieData->get_movies_with_actor(actor);
            for (Movie* movie : actorMovies) {
                string id = movie->get_id();
                if (watched.find(movie->get_id()) == watched.end()) {
                    movieToScore[id] += 30;
                }
            }
        }
        for (string genre : genres) {
            vector<Movie*> genreMovies = m_movieData->get_movies_with_director(genre);
            for (Movie* movie : genreMovies) {
                string id = movie->get_id();
                if (watched.find(movie->get_id()) == watched.end()) {
                    movieToScore[id] += 1;
                }
            }
        }
        directors.clear();
        actors.clear();
        genres.clear();
    }

    // Stores unqiue movie reccomendations and the movie id and score in sorted order
    map<RecMovie, pair<string, int>> movieRanking;

    // Create MovieRec objects and insert into set 
    unordered_map<string, int>::iterator iter = movieToScore.begin();
    while (iter != movieToScore.end()) {
        string id = iter->first;
        int score = iter->second;
        string title = m_movieData->get_movie_from_id(id)->get_title();
        float rating = m_movieData->get_movie_from_id(id)->get_rating();
        //cout << title << " " << score << endl;

        RecMovie rec(score, rating, title);
        movieRanking[rec] = { id, score };
        iter++;
    }

    // Get the top movie reccomendations
    vector<MovieAndRank> movieRecs;
    map<RecMovie, pair<string, int>>::iterator it = movieRanking.begin();
    while (numRecs > 0 && it != movieRanking.end()) {
        string id = it->second.first;
        int score = it->second.second;
        movieRecs.push_back(MovieAndRank(id, score));
        numRecs--;
        it++;
    }
    return movieRecs;
}



#include "Recommender.h"
#include "UserDatabase.h"
#include "MovieDatabase.h"
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <set>
#include <string>
#include <vector>

using namespace std;

Recommender::Recommender(const UserDatabase& user_database, const MovieDatabase& movie_database)
{
    m_userData = &user_database;
    m_movieData = &movie_database;
}

vector<MovieAndRank> Recommender::recommend_movies(const string& user_email, int movie_count) const
{
    User* user = m_userData->get_user_from_email(user_email);

    // Check if user is valid
    if (user == nullptr) {
        return vector<MovieAndRank>();
    }
    vector<string> watchHistory = user->get_watch_history();
    
    // Check if number of recs is valid
    int numRecs = movie_count;
    if (numRecs <= 0) {
        return vector<MovieAndRank>();
    }

    // Store the ids of watched movies
    unordered_set<string> watched;
    for (string id : watchHistory) {
        watched.insert(id);
    }

    // Stores info name and compatability scores
    unordered_map<string, int> directorsToScore;
    unordered_map<string, int> actorsToScore;
    unordered_map<string, int> genresToScore;
    
    // Loop through the user's eatch history and find related movies
    for (int i = 0; i < watchHistory.size(); i++) {
        string id = watchHistory[i];
        Movie* currMovie = m_movieData->get_movie_from_id(id);

        // Check if the movie the user watched is in the movie database
        if (currMovie == nullptr) {
            return vector<MovieAndRank>();
        }

        // Add info and score to map
        for (string dir : currMovie->get_directors()) {
            directorsToScore[dir] += 20;
        }
        for (string actor : currMovie->get_actors()) {
            actorsToScore[actor] += 30;
        }
        for (string genre : currMovie->get_genres()) {
            genresToScore[genre] += 1;
        }
    }

    // Maps movie id to score
    unordered_map<string, int> movieToScore;

    // Adds the scores from directors to the movie - score map
    unordered_map<string, int>::const_iterator iter = directorsToScore.begin();
    while (iter != directorsToScore.end()) {
        string director = iter->first;
        int score = iter->second;
        for (Movie* movie : m_movieData->get_movies_with_director(director)) {
            if (watched.find(movie->get_id()) == watched.end()) { // If the movie has been watched, don't add the movie to the map
                movieToScore[movie->get_id()] += score;
            }
        }
        iter++;
    }

    // Adds the scores from actors to the movie - score map
    iter = actorsToScore.begin();
    while (iter != actorsToScore.end()) {
        string actor = iter->first;
        int score = iter->second;
        for (Movie* movie : m_movieData->get_movies_with_actor(actor)) {
            if (watched.find(movie->get_id()) == watched.end()) { // If the movie has been watched, don't add the movie to the map
                movieToScore[movie->get_id()] += score;
            }
        }
        iter++;
    }

    // Adds the scores from genre to the movie - score map
    iter = genresToScore.begin();
    while (iter != genresToScore.end()) {
        string genre = iter->first;
        int score = iter->second;
        for (Movie* movie : m_movieData->get_movies_with_genre(genre)) {
            if (watched.find(movie->get_id()) == watched.end()) { // If the movie has been watched, don't add the movie to the map
                movieToScore[movie->get_id()] += score;
            }
        }
        iter++;
    }
    
    // Stores unqiue movie reccomendations and the movie id and score in sorted order
    map<RecMovie, pair<string, int>> movieRanking;

    // Create MovieRec objects and insert into set 
    iter = movieToScore.begin();
    while (iter != movieToScore.end()) {
        string id = iter->first;
        int score = iter->second;
        float rating = m_movieData->get_movie_from_id(id)->get_rating();
        string title = m_movieData->get_movie_from_id(id)->get_title();
        RecMovie rec(score, rating, title);
        movieRanking[rec] = { id, score }; // movie id and score
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






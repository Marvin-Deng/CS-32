#ifndef RECOMMENDER_INCLUDED
#define RECOMMENDER_INCLUDED

#include "UserDatabase.h"
#include "MovieDatabase.h"
#include "Movie.h"
#include "User.h"
#include <map>
#include <unordered_map>
#include <string>
#include <vector>

struct MovieAndRank
{
    MovieAndRank(const std::string& id, int score)
        : movie_id(id), compatibility_score(score)
    {}

    std::string movie_id;
    int compatibility_score;
};

class Recommender
{
public:
    Recommender(const UserDatabase& user_database, const MovieDatabase& movie_database);
    std::vector<MovieAndRank> recommend_movies(const std::string& user_email, int movie_count) const;

private:
    struct RecMovie {
        int m_score;
        float m_rating;
        std::string m_name;

        RecMovie(int score, float rating, std::string name)
            : m_score(score), m_rating(rating), m_name(name)
        {}

        // Custom < for movies
        bool operator<(const RecMovie& other) const {
            if (m_score != other.m_score) { // Compare compatability score
                return m_score > other.m_score;
            }
            else if (m_rating != other.m_rating) { // Compare rating
                return m_rating > other.m_rating;
            }
            else { // Compare name
                return m_name < other.m_name;
            }
        }
    };
    const UserDatabase* m_userData;
    const MovieDatabase* m_movieData;
};

#endif // RECOMMENDER_INCLUDED

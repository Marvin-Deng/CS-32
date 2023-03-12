#ifndef RECOMMENDER_INCLUDED
#define RECOMMENDER_INCLUDED

#include "UserDatabase.h"
#include "MovieDatabase.h"
#include "Movie.h"
#include "User.h"
#include <map>
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
    struct cmp {
        bool compareMovies(MovieAndRank a, MovieAndRank b) {
            Movie* movieA = m_movieData.get_movie_from_id(a.movie_id);
            Movie* movieB = m_movieData.get_movie_from_id(b.movie_id);
            if (a.compatibility_score > b.compatibility_score) {
                return true;
            }
            else if (movieA->get_rating() > movieB->get_rating()) {
                return true;
            }
            else if (movieA->get_title() > movieB->get_title()) {
                return true;
            }
            return false;
        }
    };
    UserDatabase m_userData;
    MovieDatabase m_movieData;
    std::map<MovieAndRank, Movie, cmp> m_map;
    
};

#endif // RECOMMENDER_INCLUDED
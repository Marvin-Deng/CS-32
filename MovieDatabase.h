#ifndef MOVIEDATABASE_INCLUDED
#define MOVIEDATABASE_INCLUDED
#include "treemm.h"
#include <string>
#include <vector>

class Movie;

class MovieDatabase
{
public:
    MovieDatabase();
    bool load(const std::string& filename);
    Movie* get_movie_from_id(const std::string& id) const;
    std::vector<Movie*> get_movies_with_director(const std::string& director) const;
    std::vector<Movie*> get_movies_with_actor(const std::string& actor) const;
    std::vector<Movie*> get_movies_with_genre(const std::string& genre) const;

private:
    void splitByComma(std::string text, std::vector<std::string>& info);
    void insertIntoMap(TreeMultimap<std::string, Movie*>& map, std::vector<std::string> info, Movie* movie);
    TreeMultimap<std::string, Movie*> m_movieIds;
    TreeMultimap<std::string, Movie*> m_movieDirectors;
    TreeMultimap<std::string, Movie*> m_movieActors;
    TreeMultimap<std::string, Movie*> m_movieGenres;
    bool m_loaded;
};

#endif // MOVIEDATABASE_INCLUDED
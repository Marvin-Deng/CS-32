#include "MovieDatabase.h"
#include "Movie.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cctype>
using namespace std;

MovieDatabase::MovieDatabase()
{
    m_loaded = false;
}

bool MovieDatabase::load(const string& filename)
{
    if (m_loaded) { // Checks if file already loaded
        return false;
    }
    ifstream infile(filename);
    if (!infile)  // Check if opening the file failed
    {
        return false;
    }
    string str;
    string id;
    string name;
    string year;
    vector<string> directors;
    vector<string> actors;
    vector<string> genres;
    float rating = 0;
    while (getline(infile, str)) {
        if (str.empty()) { // Empty line, move to next movie info
            continue;
        }
        else if (id.empty()) { // 1st line is movie ID
            id = str;
        }
        else if (name.empty()) { // 2nd line is move name
            name = str;
        }
        else if (year.empty()) { // 3rd line is movie release year
            year = str;
        }
        else if (directors.empty()) { // 4th line contains movie's directors
            splitByComma(str, directors);
        }
        else if (actors.empty()) { // 5th line contains the movie's actors
            splitByComma(str, actors);
        }
        else if (genres.empty()) { // 6th line contains the movie's genres
            splitByComma(str, genres);
        }
        else { // 7th line contains movie's rating
            rating = stof(str);

            // Insert movie into maps
            Movie movie(id, name, year, directors, actors, genres, rating);
            m_movieIds.insert(toLowercase(id), movie);         // Insert movie into ID map
            insertIntoMap(m_movieDirectors, directors, movie); // Insert movie into directors map
            insertIntoMap(m_movieActors, actors, movie);       // Insert movie into actors map
            insertIntoMap(m_movieGenres, genres, movie);       // Insert genre into genres map

            // Reset movie info
            id.clear();
            name.clear();
            year.clear();
            directors.clear();
            actors.clear();
            genres.clear();
            rating = 0;
        }
    }
    m_loaded = true;
    return true;
}

// Inserts movies to a map 
void MovieDatabase::insertIntoMap(TreeMultimap<std::string, Movie>& map, const vector<std::string>& info, Movie movie) {
    for (string s : info) {
        map.insert(toLowercase(s), movie);
    }
}

// Split a string by commas, push words into vector
void MovieDatabase::splitByComma(string text, vector<string>& info) {
    string str = "";
    for (char c : text) {
        if (c == ',') { // If comma, push word into vector
            info.push_back(str);
            str.clear();
        }
        else {
            str += c; // Not at comma, append to string
        }
    }
    info.push_back(str); // Push last word into vector
}

// Converts string to lowercase string
string MovieDatabase::toLowercase(const string& s) const {
    string str = "";
    for (char c : s) {
        str += tolower(c);
    }
    return str;
}

//// Get Functions ////
// 1. Use the Iterator find() to get the iterator to the target value in the multimap
// 2. Loop through the values at the node specified by the iterator

Movie* MovieDatabase::get_movie_from_id(const string& id) const
{
    TreeMultimap<std::string, Movie>::Iterator iter = m_movieIds.find(toLowercase(id));
    if (iter.is_valid()) {
        return &iter.get_value();
    }
    return nullptr;
}

vector<Movie*> MovieDatabase::get_movies_with_director(const string& director) const
{
    vector<Movie*> movies;
    TreeMultimap<std::string, Movie>::Iterator iter = m_movieDirectors.find(toLowercase(director));
    while (iter.is_valid()) {
        movies.push_back(&iter.get_value());
        iter.advance();
    }
    return movies;
}

vector<Movie*> MovieDatabase::get_movies_with_actor(const string& actor) const
{
    vector<Movie*> movies;
    TreeMultimap<std::string, Movie>::Iterator iter = m_movieActors.find(toLowercase(actor));
    while (iter.is_valid()) {
        movies.push_back(&iter.get_value());
        iter.advance();
    }
    return movies;
}

vector<Movie*> MovieDatabase::get_movies_with_genre(const string& genre) const
{
    vector<Movie*> movies;
    TreeMultimap<std::string, Movie>::Iterator iter = m_movieGenres.find(toLowercase(genre));
    while (iter.is_valid()) {
        movies.push_back(&iter.get_value());
        iter.advance();
    }
    return movies;
}
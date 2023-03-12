#include "UserDatabase.h"
#include "User.h"
#include "MovieDatabase.h"
#include "Movie.h"
#include "treemm.h"
#include <iostream>
#include <string>

using namespace std;

const string USER_DATAFILE = "users.txt";
const string TESTUSER_DATAFILE = "testUsers.txt";
const string TESTMOVIE_DATAFILE = "testMovies.txt";
const string MOVIE_DATAFILE = "movies.txt";

void testTreeMap() {
	TreeMultimap<std::string, int> tmm;
	tmm.insert("carey", 5);
	tmm.insert("carey", 6);
	tmm.insert("carey", 7);
	tmm.insert("david", 25);
	tmm.insert("david", 425);
	TreeMultimap<std::string, int>::Iterator it = tmm.find("david");
	// prints 5, 6, and 7 in some order
	/*std::cout << it.get_value();
	it.advance();
	std::cout << it.get_value();
	it.advance();*/
	while (it.is_valid()) {
		std::cout << it.get_value() << std::endl;
		it.advance();
	}
	it = tmm.find("laura");
	if (!it.is_valid())
		std::cout << "laura is not in the multimap!\n";
}

void printUserInfo(User* u) {
	if (u == nullptr) {
		cout << "No user in the database has that email address." << endl;
		return;
	}
	cout << "Name: " << u->get_full_name() << endl;
	cout << "Email: " << u->get_email() << endl;
	cout << "Movie Ids: " << endl;
	vector<string> movieIDs = u->get_watch_history();
	for (string s : movieIDs) {
		cout << s << endl;
	}
	cout << endl;
}

void testUserDatabase() {
	UserDatabase udb;
	if (!udb.load(TESTUSER_DATAFILE)) {
		cout << "Failed to load user data file !" << endl;
		return;
	}
	else {
		cout << "File found" << endl;
	}
	// testUser.txt tests
	printUserInfo(udb.get_user_from_email("c@gmail.com"));
	printUserInfo(udb.get_user_from_email("s@gmail.com"));

	// users.txt tests
	//printUserInfo(udb.get_user_from_email("BrunB@yahoo.com"));
	//printUserInfo(udb.get_user_from_email("AbFow2483@charter.net"));
}

void printMovieInfo(Movie* m) {
	if (m == nullptr) {
		cout << "No user in the database with that key." << endl;
		return;
	}
	cout << "ID: " << m->get_id() << endl;
	cout << "Title: " << m->get_title() << endl;
	cout << "Year: " << m->get_release_year() << endl;
	for (string s : m->get_directors()) {
		cout << s << ", ";
	}
	cout << endl;
	for (string s : m->get_actors()) {
		cout << s << ", ";
	}
	cout << endl;
	for (string s : m->get_genres()) {
		cout << s << ", ";
	}
	cout << endl;
	cout << "Rating: " << m->get_rating();
	cout << endl << endl;
}

void testMovieDatabases() {
	MovieDatabase udb;
	if (!udb.load(TESTMOVIE_DATAFILE)) {
		cout << "Failed to load user data file !" << endl;
		return;
	}
	else {
		cout << "File found" << endl;
	}
	// ID
	printMovieInfo(udb.get_movie_from_id("ID25779"));
	printMovieInfo(udb.get_movie_from_id("ID35763"));
}

int main()
{
	//testTreeMap();
	// testUserDatabase();
	testMovieDatabases();
}

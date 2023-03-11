#include "UserDatabase.h"
#include "User.h"
#include <iostream>
#include <string>
#include "treemm.h"

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
	else {
		cout << "Name: " << u->get_full_name() << endl;
	}
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
	if (!udb.load(USER_DATAFILE)) {
		cout << "Failed to load user data file " << USER_DATAFILE << "!" << endl;
		return;
	}
	else {
		cout << "File found" << endl;
	}
	// tetsUSer.txt tests
	//printUserInfo(udb.get_user_from_email("c@gmail.com"));
	//printUserInfo(udb.get_user_from_email("s@gmail.com"));

	// users.txt tests
	printUserInfo(udb.get_user_from_email("BrunB@yahoo.com"));
	printUserInfo(udb.get_user_from_email("AbFow2483@charter.net"));
}

int main()
{
	//testTreeMap();
	testUserDatabase();
	
}

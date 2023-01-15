

#include "Rabbit.h"
#include "Arena.h"
int main()
{
	Arena a(10, 20);
	Player p(&a, 2, 3);
	Rabbit r(&a, 1, 1);
}

//int main()
//{
//    // Create a game
//    Game g(3, 5, 3);
//    //Game g(10, 12, 10);
//    g.play();
//}
#include <iostream> 
#include <ctime> 
#include <cstdlib>

enum OgreStates
{
	Sleep, // starting state, transitions when patrol is done; randomly generate number (0 or 1); if it's 1, switch to attack
	Attack, // randomly generate number (0 or 1); if it's 1, switch to eat
	Eat, // randomly generate number (0 or 1); if it's 1, switch to patrol
	Patrol // few seconds after eat, switch to patrol. randomly generate number (0 or 1); 0 - sleep; 1 - attack
};

int main()
{
	OgreStates currentState = Sleep;
	while (true)
	{
		switch (currentState)
		{
			case OgreStates::Sleep:
			{
				std::cout << "Zzzzzz...\n";

				int sleepstate = rand() % 2; // generate the random number
				if (sleepstate == 1) // and swap state
				{
					currentState = Attack;
				}
				else
				{
					currentState = Sleep;
				}
				break;
			}

			case OgreStates::Attack:
			{
				std::cout << "ME KILL!\n";

				int attackstate = rand() % 2;
				if (attackstate == 1)
				{
					currentState = Eat;
				}
				else
				{
					currentState = Attack;
				}
				break;
			}
			
			case OgreStates::Eat:
			{
				std::cout << "Yummy flesh.\n";

				int eatstate = rand() % 2;
				if (eatstate == 1)
				{
					currentState = Patrol;
				}
				else
				{
					currentState = Eat;
				}
				break;
			}

			case OgreStates::Patrol:
			{
				std::cout << "Where you go?\n";

				int patrolstate = rand() % 2;
				if (patrolstate == 1)
				{
					currentState = Sleep;
				}
				else
				{
					currentState = Patrol;
				}
				break;
			}

			
		}
	}
}


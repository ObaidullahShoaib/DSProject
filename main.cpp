#include "Repository.h"
#include "openssl/sha.h"
#include "Hashing.h"
int main()
{
    Repository repo;
    String input;

    while (true) {
        cout << "-----> "; // Prompt
        cin >> input;

        if (input == "init") {
            repo.init(); // Initialize repository
		}
		else if (input == "exit") {
			break; // Exit the program
		}
		else {
			cout << "Invalid command. Please try again.\n";
		}

	}
    return 0;
}

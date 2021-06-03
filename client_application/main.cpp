// Main file for multi touch lab #4
//
#include <iostream>
#include "client_app.h"
#undef main


int main(int argc, char* argv[]) {

	int port = 3333;
	clientApp* app = new clientApp(port);
	
	app->run();

	delete app;

	return 0;
}
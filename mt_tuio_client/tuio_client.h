#pragma once
// Includes
#include "TuioClient.h"
#include "TuioListener.h"


// Defines
#define DEFAULT_PORT 3333

// Class for our client application for multi-touch lab exercise #4
class TUIOClient {

public:

	TUIOClient(unsigned int port);
	TUIOClient();
	~TUIOClient();




	// For the later lab exercises..
	void detectDiscreteGestures();
	void detectContinousGestures();

private:

	// Functions for program workflow
	// These are just stubs....
	void connect();
	void run();


	// Functions for drawing a window
	// These are just stubs....
	void initWindow();
	void drawWindow();


	// Client handle
	TUIO::TuioClient* client;

};
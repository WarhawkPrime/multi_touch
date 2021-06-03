#pragma once

// Includes
#include <SDL.h>
#include <SDL_timer.h>
#include <TuioClient.h>
#include <TuioListener.h>
#include <TuioObject.h>
#include <OscReceiver.h>
#include <string>
#include <iostream>

// Defines
#define DEFAULT_PORT 3333
#define SCR_HEIGHT 600
#define SCR_WIDTH 800


// The client class for listening to TUIO events and handling the blobs 
class clientApp : public TUIO::TuioListener {


public:

	// TUIO Listener functions
	void updateTuioObject(TUIO::TuioObject* tobj);
	void removeTuioObject(TUIO::TuioObject* tobj);

	void addTuioCursor(TUIO::TuioCursor* tcur);
	void updateTuioCursor(TUIO::TuioCursor* tcur);
	void removeTuioCursor(TUIO::TuioCursor* tcur);

	void addTuioBlob(TUIO::TuioBlob* tblb);
	void updateTuioBlob(TUIO::TuioBlob* tblb);
	void removeTuioBlob(TUIO::TuioBlob* tblb);

	void refresh(TUIO::TuioTime frameTime);

	// ClientApp functions 
	clientApp();
	clientApp(unsigned int port);
	~clientApp();



private:

	void initWindow();
	void destructWindow();

	// Variables
	TUIO::TuioClient* client;
	SDL_Window* window;
	SDL_Renderer* renderer;
	std::string windowTitle;
	unsigned int scrHeight, scrWidth;
};
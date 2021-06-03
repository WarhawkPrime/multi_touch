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
	void addTuioObject(TUIO::TuioObject* tobj) override;
	void updateTuioObject(TUIO::TuioObject* tobj) override;
	void removeTuioObject(TUIO::TuioObject* tobj) override;

	void addTuioCursor(TUIO::TuioCursor* tcur) override;
	void updateTuioCursor(TUIO::TuioCursor* tcur) override;
	void removeTuioCursor(TUIO::TuioCursor* tcur) override;

	void addTuioBlob(TUIO::TuioBlob* tblb) override;
	void updateTuioBlob(TUIO::TuioBlob* tblb) override;
	void removeTuioBlob(TUIO::TuioBlob* tblb) override;

	void refresh(TUIO::TuioTime frameTime) override;

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
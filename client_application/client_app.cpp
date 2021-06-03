#include "client_app.h"


// Implementing the virtual functions of TUIO Listener
void clientApp::updateTuioObject(TUIO::TuioObject* tobj)
{

}

void clientApp::removeTuioObject(TUIO::TuioObject* tobj)
{

}

void clientApp::addTuioCursor(TUIO::TuioCursor* tcur)
{

}

void clientApp::updateTuioCursor(TUIO::TuioCursor* tcur)
{

}

void clientApp::removeTuioCursor(TUIO::TuioCursor* tcur)
{

}

void clientApp::addTuioBlob(TUIO::TuioBlob* tblb)
{

}

void clientApp::updateTuioBlob(TUIO::TuioBlob* tblb)
{

}

void clientApp::removeTuioBlob(TUIO::TuioBlob* tblb)
{

}

void clientApp::refresh(TUIO::TuioTime frameTime)
{

}


// clientApp function implementations
clientApp::clientApp()
{
	// Create a new TuioClient instance
	this->client = new TUIO::TuioClient(DEFAULT_PORT);
	this->windowTitle = "Tuio Client for Lab 4";
	this->scrHeight = SCR_HEIGHT;
	this->scrWidth = SCR_WIDTH;

	if (!this->client) {
		std::cerr << "Failed to create TUIO Client!" << std::endl;
	}
	this->initWindow();
}

clientApp::clientApp(unsigned int port)
{

	// Create a new TuioClient instance
	this->client = new TUIO::TuioClient(port);
	this->windowTitle = "Tuio Client for Lab 4";
	this->scrHeight = SCR_HEIGHT;
	this->scrWidth = SCR_WIDTH;

	if (!this->client) {
		std::cerr << "Failed to create TUIO Client!" << std::endl;
	}
	this->initWindow();
}

clientApp::~clientApp()
{

	this->destructWindow();
}

// Initialize a SDL context for drawing on screen
void clientApp::initWindow()
{
	// Initialize
	SDL_Init(SDL_INIT_EVERYTHING);

	this->window = SDL_CreateWindow(this->windowTitle.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		this->scrWidth, this->scrHeight, SDL_WINDOW_SHOWN);

	this->renderer = SDL_CreateRenderer(this->window, -1, 0);

	// Quick test
	SDL_SetRenderDrawColor(this->renderer, 0, 255, 0, 255);
	SDL_RenderClear(this->renderer);
	SDL_RenderPresent(this->renderer);

}

// Close all the SDL related resources
void clientApp::destructWindow()
{

	SDL_DestroyRenderer(this->renderer);
	SDL_DestroyWindow(this->window);
	SDL_Quit();
}


#include "client_app.h"



// Implementing the virtual functions of TUIO Listener
void clientApp::addTuioObject(TUIO::TuioObject* tobj)
{


}

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
clientApp::clientApp(unsigned int port)
{

	// Create a new TuioClient instance
	this->client = new TUIO::TuioClient(port);
	this->client->addTuioListener(this);
	this->client->connect();
	this->windowTitle = "Tuio Client for Lab 4";
	this->scrHeight = SCR_HEIGHT;
	this->scrWidth = SCR_WIDTH;
	this->isRunning = true;

	if (TTF_Init() == -1) {
		std::cerr << "Failed to initialize truetype font API" << std::endl;
		std::cerr << TTF_GetError() << std::endl;;
	}
	else {
		std::cout << "Successfully initialized truetype font API" << std::endl;
	}

	if (!this->client) {
		std::cerr << "Failed to create TUIO Client!" << std::endl;
	}
	else {
		std::cout << "Successfully created the TUIO Client" << std::endl;
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
	this->screen = SDL_GetWindowSurface(this->window);

	// Quick test
	SDL_SetRenderDrawColor(this->renderer, 0, 0, 0, 255);
	SDL_RenderClear(this->renderer);
	SDL_RenderPresent(this->renderer);
	

}

// Close all the SDL related resources
void clientApp::destructWindow()
{
	TTF_Quit();
	SDL_DestroyRenderer(this->renderer);
	SDL_DestroyWindow(this->window);
	SDL_Quit();
}

// Draw the cursors in this function, either via SDL Points or by adding glut 
void clientApp::render()
{




}

void clientApp::drawSampleText()
{
	TTF_Font* font;
	font = TTF_OpenFont("ARIAL.ttf", 16);

	if (!font) {
		std::cerr << "Failed to load font!" << std::endl;
		std::cerr << TTF_GetError();
	}


	SDL_Color fgColor = { 0, 0, 0 };
	SDL_Color bgColor = { 255, 255, 255 };

	SDL_Surface* textSurface = TTF_RenderText_Solid(font, "Id: 0", bgColor);

	this->message = SDL_CreateTextureFromSurface(this->renderer, textSurface);

	
	this->msg_rect.x = 50;
	this->msg_rect.y = 50;
	this->msg_rect.w = 30;
	this->msg_rect.h = 30;

	
}

void clientApp::processEvents()
{
	SDL_Event input;

	while (SDL_PollEvent(&input) > 0) {

		switch (input.type) {

		case SDL_QUIT: {

			this->isRunning = false;
			break;
		}
		case SDL_KEYDOWN: {

			if (input.key.keysym.sym == SDLK_ESCAPE) {

				this->isRunning = false;
				break;
			}
		}
		}
	}
}

void clientApp::run()
{

	drawSampleText();

	while (this->isRunning) {

		processEvents();

		SDL_RenderClear(this->renderer);

		
		SDL_RenderCopy(this->renderer, this->message, NULL, &this->msg_rect);
		
		
		SDL_RenderPresent(this->renderer);
		
		
	}
}


#include "client_app.h"



// Implementing the virtual functions of TUIO Listener
void clientApp::addTuioObject(TUIO::TuioObject* tobj)
{
	if (this->verbose)
		std::cout << "Added TUIO Object" << std::endl;

}

void clientApp::updateTuioObject(TUIO::TuioObject* tobj)
{
	if (this->verbose)
		std::cout << "Updated TUIO Object" << std::endl;
}

void clientApp::removeTuioObject(TUIO::TuioObject* tobj)
{
	if (this->verbose)
		std::cout << "Removed TUIO Object" << std::endl;
}

void clientApp::addTuioCursor(TUIO::TuioCursor* tcur)
{
	if (this->verbose)
		std::cout << "Added TUIO Cursor" << std::endl;
}

void clientApp::updateTuioCursor(TUIO::TuioCursor* tcur)
{
	if(this->verbose)
		std::cout << "Updated TUIO Cursor" << std::endl;
}

void clientApp::removeTuioCursor(TUIO::TuioCursor* tcur)
{
	if (this->verbose)
		std::cout << "Removed TUIO Cursor" << std::endl;
}

void clientApp::addTuioBlob(TUIO::TuioBlob* tblb)
{
	if (this->verbose)
		std::cout << "Added TUIO Blob" << std::endl;
}

void clientApp::updateTuioBlob(TUIO::TuioBlob* tblb)
{
	if (this->verbose)
		std::cout << "Updated TUIO Blob" << std::endl;
}

void clientApp::removeTuioBlob(TUIO::TuioBlob* tblb)
{
	if (this->verbose)
		std::cout << "Removed TUIO Blob" << std::endl;
}

void clientApp::refresh(TUIO::TuioTime frameTime)
{
	if (this->verbose)
		std::cout << "Refresh called!" << std::endl;
	
}


// clientApp function implementations
clientApp::clientApp(unsigned int port)
{

	// Create a new TuioClient instance
	this->client = new TUIO::TuioClient(port);

	if (!this->client) {
		std::cerr << "Failed to create TUIO Client!" << std::endl;
		this->isRunning = false;
	}
	else {
		std::cout << "Successfully created the TUIO Client" << std::endl;
	}

	this->client->addTuioListener(this);
	this->client->connect();

	if (!this->client->isConnected()) {
		std::cerr << "TuioClient failed to connect!" << std::endl;
		SDL_Quit();
		this->isRunning = false;
	}

	this->windowTitle = "Tuio Client for Lab 4";
	this->scrHeight = SCR_HEIGHT;
	this->scrWidth = SCR_WIDTH;
	this->isRunning = false;
	this->verbose = false;

	// Initialize SDL_TTF
	if (TTF_Init() == -1) {
		std::cerr << "Failed to initialize truetype font API" << std::endl;
		std::cerr << TTF_GetError() << std::endl;;
	}
	else {
		std::cout << "Successfully initialized truetype font API" << std::endl;
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

	// SDL window
	this->window = SDL_CreateWindow(this->windowTitle.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		this->scrWidth, this->scrHeight, SDL_WINDOW_OPENGL);

	if (!this->window) {
		std::cerr << "Failed to open window: " << SDL_GetError() << std::endl;
		SDL_Quit();
	}

	// OpenGL context
	this->gl_context = SDL_GL_CreateContext(this->window);

	if (!this->gl_context) {
		std::cerr << "Failed to create OpenGL context: " << SDL_GetError() << std::endl;
		return;
	}


	this->renderer = SDL_CreateRenderer(this->window, -1, 0);
	this->screen = SDL_GetWindowSurface(this->window);

	SDL_ShowCursor(true);
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glViewport(0, 0, (GLint)scrWidth, (GLint)scrHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, (GLfloat)scrWidth, (GLfloat)scrHeight, 0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

}

// Close all the SDL related resources
void clientApp::destructWindow()
{
	TTF_Quit();
	SDL_DestroyRenderer(this->renderer);
	SDL_DestroyWindow(this->window);
	SDL_Quit();
}

// Draws the TUIO Cursors to screen
void clientApp::render()
{
	// Variables
	const int pointXLocation = scrWidth / 2;
	const int pointYLocation = scrHeight / 2;
	std::list<TUIO::TuioCursor*> cursorList = client->getTuioCursors();
	char id[16];


	// Render loop
	//SDL_RenderClear(renderer);
	//--//

	// Set draw color to blue
	//SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
	glClear(GL_COLOR_BUFFER_BIT);
	// Draw the cursor objects to screen
	if (this->verbose) {
		if(cursorList.size() != 0)
			std::cout << "Size of cursorList: " << cursorList.size() << std::endl;
	}
		

	client->lockCursorList();

	for (std::list<TUIO::TuioCursor*>::iterator it = cursorList.begin(); it != cursorList.end(); it++) {

		TUIO::TuioCursor* tuioCursor = (*it);
		if (!tuioCursor) {
			std::cout << "tuioCursor was not initialized!" << std::endl;
			return;
		}
		std::list<TUIO::TuioPoint> path = tuioCursor->getPath();
		if (path.size() > 0) {

			TUIO::TuioPoint last_point = path.front();
			glBegin(GL_LINES);
			glColor3f(0.0, 0.0, 1.0);

			for (std::list<TUIO::TuioPoint>::iterator point = path.begin(); point != path.end(); point++) {
				glVertex3f(last_point.getScreenX(scrWidth), last_point.getScreenY(scrHeight), 0.0f);
				glVertex3f(point->getScreenX(scrWidth), point->getScreenY(scrHeight), 0.0f);
				last_point.update(point->getX(), point->getY());
			} 
			glEnd();

			glColor3f(0.0, 0.75, 0.0);
			glPushMatrix();
			glTranslatef(last_point.getScreenX(scrWidth), last_point.getScreenY(scrHeight), 0.0);
			glBegin(GL_TRIANGLE_FAN);

			for (double a = 0.0f; a <= 2 * M_PI; a += 0.2f) {

				glVertex2d(cos(a) * scrHeight / 100.0f, sin(a) * scrHeight / 100.0f);
			} 
			glEnd();
			glPopMatrix();

			glColor3f(0.0, 0.0, 0.0);
			glRasterPos2f(tuioCursor->getScreenX(scrWidth), tuioCursor->getScreenY(scrHeight));
			strcpy(id, std::to_string(tuioCursor->getCursorID()).c_str());
			drawString(id);
		}
	}
	client->unlockCursorList();


	// Set draw color to white
	//SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

	//--//
	//SDL_RenderPresent(renderer);
	SDL_GL_SwapWindow(window);

}

void clientApp::drawSampleText()
{
	TTF_Font* font;
	font = TTF_OpenFont("ARIAL.ttf", 8);

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
			else if (input.key.keysym.sym == SDLK_v) {

				this->verbose = !this->verbose;
				std::string res = this->verbose ? "true" : "false";
				std::cout << "Verbose: " << res << std::endl;
				break;
			}
		}
		}
	}
}

void clientApp::drawString(char* str)
{
	if (str && strlen(str)) {
		while (*str) {
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *str);
			str++;
		}
	}
}

void clientApp::run()
{
	this->isRunning = true;
	while (this->isRunning) {

		render();
		processEvents();
		SDL_Delay(10);
		
	}
}


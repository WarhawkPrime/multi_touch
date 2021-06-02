#include "tuio_client.h"

TUIOClient::TUIOClient(unsigned int port)
{
	
	// Create a new TuioClient instance
	if (!this->client) {
		this->client = new TUIO::TuioClient(port);
	}
}

TUIOClient::TUIOClient()
{

	// Create a new TuioClient instance
	if (!this->client) {
		this->client = new TUIO::TuioClient(DEFAULT_PORT);
	}
}

TUIOClient::~TUIOClient()
{

	delete this->client;

}

void TUIOClient::connect()
{

}

void TUIOClient::detectDiscreteGestures()
{

}

void TUIOClient::detectContinousGestures()
{

}


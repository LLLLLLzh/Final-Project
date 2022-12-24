#pragma once

#ifndef GUISERVICELISTENER_HPP
#define GUISERVICELISTENER_HPP

#include "soa.hpp"
#include "GUIService.hpp"

using namespace std;

class GUIServiceListener : public ServiceListener<Price<Bond> > {
private:
	GUIService* guiservice;
public:
	// ctor
	GUIServiceListener(GUIService* _guiservice);

	
	// update new price
	virtual void ProcessAdd(Price<Bond>& price) override;

	// no use
	virtual void ProcessRemove(Price<Bond>&) override;

	// no use
	virtual void ProcessUpdate(Price<Bond>&) override;
};

GUIServiceListener::GUIServiceListener(GUIService* _guiservice) {
	guiservice = _guiservice;
}

void GUIServiceListener::ProcessAdd(Price<Bond>& price) {
	guiservice->SendToThrottle(price);
	return;
}


void GUIServiceListener::ProcessRemove(Price<Bond>&) {
}

void GUIServiceListener::ProcessUpdate(Price<Bond>&) {
}


#endif
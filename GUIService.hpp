#pragma once

#ifndef GUISERVICE_HPP
#define GUISERVICE_HPP

#include <boost/date_time.hpp>
#include <iostream>
#include <algorithm>
#include <vector>
#include <string>

#include "pricingservice.hpp"
#include "products.hpp"
#include "soa.hpp"

using namespace std;
using namespace boost::posix_time;



class ThrottlePrice : public Price<Bond> {
private:
	ptime timestamp;
public:
	ThrottlePrice(ptime _timestamp, Price<Bond> _price);
		
	ptime GetTimeStamp(); 
};

ThrottlePrice::ThrottlePrice(ptime _timestamp, Price<Bond> _price) :Price<Bond>(_price) {
	timestamp = _timestamp;
};

ptime ThrottlePrice::GetTimeStamp() {
	return timestamp;
};


class GUIServiceConnector : public Connector<ThrottlePrice > {
public:
	// ctor
	GUIServiceConnector() {};

	// do nothing, publish only
	virtual void Subscribe();

	// print price with time stamp
	virtual void Publish(ThrottlePrice& data) override;
};

void GUIServiceConnector::Subscribe() {
};

void GUIServiceConnector::Publish(ThrottlePrice& data) {
	auto timeStamp = data.GetTimeStamp();
	auto bond = data.GetProduct();
	auto mid = data.GetMid();
	auto spread = data.GetBidOfferSpread();

	ofstream out;
	out.open("gui.txt", ios::app);
	out << timeStamp << "," << bond.GetProductId() << "," << mid << "," << spread << endl;

	return;
};



class GUIService :public PricingService<Bond> {

public:
	// ctor
	GUIService(GUIServiceConnector* _guiserviceconnector) :
		guiserviceconnector(_guiserviceconnector),
		throttetime(millisec(300)),
		lasttime(microsec_clock::local_time()),
		pricelist(map<string, Price<Bond>>()){}

	// called by listener
	void SendToThrottle(Price<Bond>& data);
	
	// no use
	virtual Price<Bond>& GetData(string Bondid) override;

	// no use
	virtual void OnMessage(Price<Bond>& data) override;

	// add a listener, in this case, no use
	virtual void AddListener(ServiceListener<Price<Bond> >* listener) override;

	// no use
	virtual const vector<ServiceListener<Price<Bond> >* >& GetListeners() const override;

private:
	GUIServiceConnector* guiserviceconnector;
	ptime lasttime;
	time_duration throttetime;
	vector<ServiceListener<Price<Bond> >*> listenerlist;
	map<string, Price<Bond> > pricelist;
};


void GUIService::OnMessage(Price<Bond>& data) {
};

void GUIService::AddListener(ServiceListener<Price<Bond> >* listener) {
	listenerlist.push_back(listener);
	return;
};

const vector<ServiceListener<Price<Bond> >*>& GUIService::GetListeners() const {
	return listenerlist;
};

void GUIService::SendToThrottle(Price<Bond>& data) {
	ptime timeNow = microsec_clock::local_time();
	time_duration diff = timeNow - lasttime;

	if (diff < throttetime) {
		//c not enought for 3 millisecond
		return;
	}
	lasttime = timeNow;
	auto pts = ThrottlePrice(timeNow, data);
	guiserviceconnector->Publish(pts);

	return;
};

Price<Bond>& GUIService::GetData(string Bondid) {
	return pricelist[Bondid];
};


#endif
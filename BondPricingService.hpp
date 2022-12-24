#pragma once
#ifndef BONDPRICINGSERVICE_HPP
#define BONDPRICINGSERVICE_HPP

#include <map>
#include <vector>
#include <fstream>

#include "pricingservice.hpp"
#include "BondService.hpp"
#include "products.hpp"
#include "soa.hpp"

using namespace std;



class BondPricingService : public PricingService<Bond> {
private:
	map<string, Price<Bond> > pricelist;
	vector<ServiceListener<Price<Bond> >* > listenerlist;
public:
	// ctor
	BondPricingService();

	// given bondid return bond price
	virtual Price<Bond>& GetData(string id) override;

	// add new bond price to priceslist(use by BondPricingServiceConnector)
	virtual void OnMessage(Price<Bond>& data) override;

	// add a listener
	// listener is BondAlgoStreamingServiceListener
	virtual void AddListener(ServiceListener<Price<Bond> >* listener) override;

	// get listeners
	virtual const vector< ServiceListener<Price<Bond> >* >& GetListeners() const override;

};

BondPricingService::BondPricingService() {
	pricelist = map<string, Price<Bond> >();
}

Price<Bond>& BondPricingService::GetData(string bondid) {
	return pricelist.at(bondid);
}

void BondPricingService::OnMessage(Price<Bond>& data) {
	auto bondid = data.GetProduct().GetProductId();

	// If already exist, remove:
	if (pricelist.find(bondid) != pricelist.end())
		pricelist.erase(bondid);

	pricelist.insert(pair<string, Price<Bond> >(bondid, data));

	// notify the listener this Service add a data
	for (auto& listener : listenerlist) {
		listener->ProcessAdd(data);
	}
	return;
}

void BondPricingService::AddListener(ServiceListener<Price<Bond> >* newlistener) {
	listenerlist.push_back(newlistener);
	return;
}

const vector< ServiceListener<Price<Bond> >* >& BondPricingService::GetListeners() const {
	return listenerlist;
}



class BondPricingServiceConnector : public Connector<Price<Bond> > {

public:
	// ctor
	BondPricingServiceConnector(BondPricingService* _bondpricingservice, BondService* _bondservice) :bondpricingservice(_bondpricingservice), bondservice(_bondservice) {};

	// read data from prices.txt file
	virtual void Subscribe() override;

	// do nothing, subscribe only
	virtual void Publish(Price<Bond>& data) override;

private:
	BondPricingService* bondpricingservice;
	BondService* bondservice;

	// transform a string to price numver
	float PriceStringToNum(string& s);
};


float BondPricingServiceConnector::PriceStringToNum(string& s) {
	int n = s.size();
	if (s[n - 1] == '+') { s[n - 1] = '4'; }
	float p = stoi(s.substr(0, n - 4));
	p += stoi(s.substr(n - 3, 2)) / 32.0;
	p += stoi(s.substr(n - 1, 1)) / 256.0;
	return p;
}

void BondPricingServiceConnector::Subscribe() {
	ifstream input;
	input.open("prices.txt");

	string row;
	while (getline(input, row)) {
		stringstream line(row);
		string s;
		getline(line, s, ',');
		auto bond = bondservice->GetData(s);
		getline(line, s, ',');
		auto price = PriceStringToNum(s);
		getline(line, s, ',');
		auto spread = PriceStringToNum(s);

		Price<Bond> bondPrice(bond, price, spread);
		bondpricingservice->OnMessage(bondPrice);
	}
	return;
}

void BondPricingServiceConnector::Publish(Price<Bond>& data) {
	return;
}

#endif
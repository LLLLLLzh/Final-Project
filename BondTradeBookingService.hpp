#pragma once
#ifndef BONDTRADEBOOKINGSERVICE_HPP
#define BONDTRADEBOOKINGSERVICE_HPP

#include <map>
#include <vector>
#include <string>
#include <fstream>

#include "products.hpp"
#include "BondService.hpp"
#include "tradebookingservice.hpp"

using namespace std;

class BondTradeBookingService : public TradeBookingService<Bond> {
private:
	map<string, Trade<Bond> > tradeslist;
	vector<ServiceListener<Trade<Bond> >*> listenerlist;
public:
	// ctor
	BondTradeBookingService();

	// get trade from tradeslist with trading id
	virtual Trade<Bond>& GetData(string id) override;

	// add trade, used by BondTradeBookingServiceConnector
	virtual void OnMessage(Trade<Bond>& trade) override;

	// add a listener
	// BondPositionServiceListener
	virtual void AddListener(ServiceListener<Trade<Bond> >* listener) override;

	// get listeners
	virtual const vector<ServiceListener<Trade<Bond> >*>& GetListeners() const override;

	// add a trade to tradeslist
	virtual void BookTrade( Trade<Bond>& trade) override; 
};


BondTradeBookingService::BondTradeBookingService() {
	tradeslist = map<string, Trade<Bond> >();
}

Trade<Bond>& BondTradeBookingService::GetData(string id) {
	return tradeslist.at(id);
}

void BondTradeBookingService::OnMessage(Trade<Bond>& trade) {
	BookTrade(trade);
	return;
}

void BondTradeBookingService::AddListener(ServiceListener<Trade<Bond> >* listener) {
	listenerlist.push_back(listener);
	return;
}

const vector<ServiceListener<Trade<Bond> >*>& BondTradeBookingService::GetListeners() const {
	return listenerlist;
};


void BondTradeBookingService::BookTrade( Trade<Bond>& trade) {
	string bondid = trade.GetProduct().GetProductId();

	// if the trade with the id already exist, remove:
	if (tradeslist.find(bondid) != tradeslist.end())
		tradeslist.erase(bondid);

	// add:
	tradeslist.insert(pair<string, Trade<Bond> >(bondid, trade));

	for (auto& listener : listenerlist) { listener->ProcessAdd(trade); }
	return;
};



class BondTradeBookingServiceConnector : public Connector<Trade<Bond> > {

public:
	// ctor
	BondTradeBookingServiceConnector(BondTradeBookingService* _bondtradebookingservice, BondService* _bondservice) :
		btdservice(_bondtradebookingservice),
		bondservice(_bondservice) {}

	// read data from trades.txt file
	virtual void Subscribe();

	// no publish in this connector
	virtual void Publish(Trade<Bond>& data) override;

private:
	BondTradeBookingService* btdservice;
	BondService* bondservice;

	// transform a string to price number
	float PriceStringToNum(string& s);
};

float BondTradeBookingServiceConnector::PriceStringToNum(string& s) {
	int n = s.size();
	if (s[n - 1] == '+') { s[n - 1] = '4'; }
	float p = stoi(s.substr(0, n - 4));
	p += stoi(s.substr(n - 3, 2)) / 32.0;
	p += stoi(s.substr(n - 1, 1)) / 256.0;
	return p;
}




void BondTradeBookingServiceConnector::Publish(Trade<Bond>& data) {
	return;
}

void BondTradeBookingServiceConnector::Subscribe() {
	ifstream input;
	input.open("trades.txt");

	string row;
	while (getline(input, row)) {
		stringstream line(row);
		string stringtrade;
		getline(line, stringtrade, ',');
		auto bond = bondservice->GetData(stringtrade);
		getline(line, stringtrade, ',');
		string tradeID = stringtrade;
		getline(line, stringtrade, ',');
		float price = PriceStringToNum(stringtrade);
		getline(line, stringtrade, ',');
		string book = stringtrade;
		getline(line, stringtrade, ',');
		long quantity = stol(stringtrade);
		getline(line, stringtrade, ',');
		Side side = (stringtrade == "BUY" ? BUY : SELL);

		Trade<Bond> bondNew(bond, tradeID, price, book, quantity, side);
		btdservice->OnMessage(bondNew);
	}
	return;
}







#endif
#pragma once
#ifndef BONDMARKETDATASERVICE_HPP
#define BONDMARKETDATASERVICE_HPP

#include <vector>
#include <string>
#include <unordered_map>
#include <fstream>

#include "marketdataservice.hpp"
#include "BondService.hpp"
#include "products.hpp"

using namespace std;

class BondMarketDataService :public MarketDataService<Bond> {
private:
	map<string, OrderBook<Bond>> orderslist;
	vector<ServiceListener<OrderBook<Bond> >*> listenerlist;

public:
	// ctor
	BondMarketDataService();

	// get order book withbondid
	virtual OrderBook<Bond>& GetData(string id) override;

	
	// used by BondMarketDataServiceConnector 
	virtual void OnMessage(OrderBook<Bond>& orderBook) override;

	// add its listener
	// BondAlgoExecutionServiceListener
	virtual void AddListener(ServiceListener<OrderBook<Bond> >* listener) override;

	// get its listener
	virtual const vector<ServiceListener<OrderBook<Bond> >*>& GetListeners() const override;

	// return the highest bid and lowest ask
	virtual BidOffer GetBestBidOffer(const string& productId) override;

	// if prices in the bid/ask vectors are same, merge
	virtual OrderBook<Bond> AggregateDepth(const string& productId) override;
};

BondMarketDataService::BondMarketDataService() {
	orderslist = map<string, OrderBook<Bond> >();
}

OrderBook<Bond>& BondMarketDataService::GetData(string id) {
	return orderslist.at(id);
}

void BondMarketDataService::AddListener(ServiceListener<OrderBook<Bond> >* listener) {
	listenerlist.push_back(listener);
	return;
}

const vector<ServiceListener<OrderBook<Bond> >*>& BondMarketDataService::GetListeners() const {
	return listenerlist;
}

void BondMarketDataService::OnMessage(OrderBook<Bond>& orderBook) {
	Bond bond = orderBook.GetProduct();
	auto bondid = bond.GetProductId();

	// If already have the bond, remove:
	if (orderslist.find(bondid) != orderslist.end())
		orderslist.erase(bondid);

	// add new bond with its order book
	orderslist.insert(pair<string, OrderBook<Bond> >(bondid, orderBook));


	auto bestorder = GetBestBidOffer(bondid);
	vector<Order> bid, ask;
	bid.push_back(bestorder.GetBidOrder());
	ask.push_back(bestorder.GetOfferOrder());
	OrderBook<Bond> bestBook = OrderBook<Bond>(bond, bid, ask);

	for (auto& listener : listenerlist) {
		listener->ProcessAdd(bestBook);
	}
	return;
}

BidOffer BondMarketDataService::GetBestBidOffer(const string& productId) {
	auto orderbook = orderslist.at(productId);
	auto bidstack = orderbook.GetBidStack();
	auto askstack = orderbook.GetOfferStack();
	auto bestbid = bidstack[0];
	auto bestask = askstack[0];

	for (Order& bidorder : bidstack) {
		if (bidorder.GetPrice() > bestbid.GetPrice())
			bestbid = bidorder;
	}

	for (Order& offerorder : askstack) {
		if (offerorder.GetPrice() < bestask.GetPrice())
			bestask = offerorder;
	}
	return BidOffer(bestbid, bestask);
}

OrderBook<Bond> BondMarketDataService::AggregateDepth(const string& productId) {
	OrderBook<Bond> orderbook = orderslist[productId];
	vector<Order> bidstack = orderbook.GetBidStack();
	vector<Order> askstack = orderbook.GetOfferStack();

	// if price is same, merge quantity
	unordered_map<double, long> bidrecord, askrecord;
	for (Order& bidorder : bidstack) {
		if (bidrecord.find(bidorder.GetPrice()) == bidrecord.end()) {
			bidrecord[bidorder.GetPrice()] = bidorder.GetQuantity();
		}
		else {
			bidrecord[bidorder.GetPrice()] += bidorder.GetQuantity();
		}
			
	}
	for (Order& offerorder : askstack) {
		if (askrecord.find(offerorder.GetPrice()) != askrecord.end()) {
			askrecord[offerorder.GetPrice()] += offerorder.GetQuantity();
		}
		else {
			askrecord[offerorder.GetPrice()] = offerorder.GetQuantity();
		}
	}

	// rebuild orderbook
	vector<Order> new_bidstack, new_askstack;
	for (auto& item : bidrecord) {
		new_bidstack.push_back(Order(item.first, item.second, BID));
	}
	for (auto& item : askrecord) {
		new_askstack.push_back(Order(item.first, item.second, OFFER));
	}
	auto b = orderbook.GetProduct();
	return OrderBook<Bond>(b, new_bidstack, new_askstack);
}


class BondMarketDataServiceConnector : public Connector<OrderBook<Bond> > {

public:
	// ctor
	BondMarketDataServiceConnector(BondMarketDataService* _bondMarketDataService, BondService* _bondservice) :
		bondmarketdataservice(_bondMarketDataService),
		bondservice(_bondservice) {};

	// override pure vritual functions in Connector class
	// in this case, do not publis, subsribe only
	virtual void Publish(OrderBook<Bond>& data) override;

	// read data from marketdata.txt and call BondMarketDataService.OnMessage
	virtual void Subscribe() override;

private:
	BondMarketDataService* bondmarketdataservice;
	BondService* bondservice;

	// transform a string to price numver
	float PriceStringToNum(string& s);
};

float BondMarketDataServiceConnector::PriceStringToNum(string& s) {
	int n = s.size();
	if (s[n - 1] == '+') { s[n - 1] = '4'; }
	float p = stoi(s.substr(0, n - 4));
	p += stoi(s.substr(n - 3, 2)) / 32.0;
	p += stoi(s.substr(n - 1, 1)) / 256.0;
	return p;
}


void BondMarketDataServiceConnector::Publish(OrderBook<Bond>& data) {
	return;
}

void BondMarketDataServiceConnector::Subscribe() {
	ifstream input;
	input.open("marketdata.txt");
	string row;

	while (getline(input, row)) {
		stringstream line(row);
		string temp;
		vector<Order> bidStack, askStack;

		getline(line, temp, ',');
		auto bond = bondservice->GetData(temp);

		for (int i = 0; i < 10; ++i) {
			getline(line, temp, ',');
			auto price = PriceStringToNum(temp);
			getline(line, temp, ',');
			auto quantity = stol(temp);
			getline(line, temp, ',');
			auto side = temp;
			if (i < 5) {
				bidStack.push_back(Order(price, quantity, BID));
			}
			else {
				askStack.push_back(Order(price, quantity, OFFER));
			}
		}

		OrderBook<Bond> orderBook(bond, bidStack, askStack);
		bondmarketdataservice->OnMessage(orderBook);
	}
	return;
}

#endif
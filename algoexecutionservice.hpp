#pragma once

#ifndef ALGOEXECUTION_HPP
#define ALGOEXECUTION_HPP

#include <string>
#include <random>
#include <vector>

#include "BondMarketDataService.hpp"
#include "executionservice.hpp"

using namespace std;

template<typename T>
class AlgoExecution {
private:
	static int orderid;
	ExecutionOrder<T> executionorder;

public:
	// ctor
	AlgoExecution(){}
	AlgoExecution(ExecutionOrder<T> _executionorder);

	// use the algorithm with bond order book
	void AlgoResult(OrderBook<T> orderbook);

	// get the executionOrder
	ExecutionOrder<T> GetExecutionOrder() const;
};

template<typename T>
int AlgoExecution<T>::orderid = 0;

template<typename T>
AlgoExecution<T>::AlgoExecution(ExecutionOrder<T> _executionorder)
	:executionorder(_executionorder) {

}

template<typename T>
void AlgoExecution<T>::AlgoResult(OrderBook<T> orderbook) {
	Bond bond = orderbook.GetProduct();
	// not this executionOrder to update
	if (bond.GetProductId() != executionorder.GetProduct().GetProductId()) return;

	// OrderString
	string orderstr = to_string(orderid);
	for (int i = 0; i < 10-orderstr.length(); i++) {
		orderstr = '0'+orderstr;
	}
	orderstr = "ORDER" + orderstr;
	
	int rd = rand() % 5;
	vector<OrderType>  oredertypelist = { FOK, IOC, MARKET, LIMIT, STOP };
	OrderType ordertype = oredertypelist[rd];

	PricingSide side = (rand() % 2 == 0 ? BID : OFFER);


	auto bidorder = orderbook.GetBidStack()[0];
	auto offerorder = orderbook.GetOfferStack()[0];
	double price;
	long visiableQ;

	Order priceorder = bidorder; 
	if (side == OFFER) {
		 priceorder = offerorder;
	}
	price = priceorder.GetPrice();
	if (offerorder.GetPrice() - bidorder.GetPrice() < 2.5 / 256.0)
		visiableQ = priceorder.GetQuantity();
	else { visiableQ = 0; }
	long hiddenQ = 2 * visiableQ;
	
	string parentstr = "P" + orderstr;

	executionorder = ExecutionOrder<Bond>(bond, side, orderstr, ordertype, price, visiableQ, hiddenQ, parentstr, true);

	++orderid;
	return;
}

template<typename T>
ExecutionOrder<T> AlgoExecution<T>::GetExecutionOrder() const {
	return executionorder;
}


template<typename T>
class AlgoExecutionService : public Service<string, AlgoExecution<T> >
{
};

#endif
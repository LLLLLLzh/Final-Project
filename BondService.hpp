#pragma once

#ifndef BONDSERVICE_HPP
#define BONDSERVICE_HPP

#include <iostream>
#include <map>
#include <algorithm>

#include "products.hpp"
#include "soa.hpp"

using namespace std;


class BondService : public Service<string, Bond> {

public:
	// BondService ctor
	BondService();

	// Return bond's data for the bondid
	Bond& GetData(string BondId);

	// Add new bond to the service
	void Add(Bond& bond);

	// Get all Bonds in the system
	vector<Bond> GetAllBonds();

	// Vitual Functions:
	virtual void OnMessage(Bond& data) override {}
	virtual void AddListener(ServiceListener<Bond >* listener) override {}
	virtual const vector< ServiceListener<Bond >* >& GetListeners() const override {
		return{};
	}


private:
	map<string, Bond> bondlist;
};

BondService::BondService() {
	bondlist = map<string, Bond>();
};

Bond& BondService::GetData(string BondId) {
	return bondlist[BondId];
};

void BondService::Add(Bond& bond) {
	bondlist.insert(pair<string, Bond>(bond.GetProductId(), bond));
};

vector<Bond> BondService::GetAllBonds() {
	vector<Bond> res;
	for (auto it = bondlist.begin(); it != bondlist.end(); ++it) {
		res.push_back(it->second);
	}
	return res;
};

#endif
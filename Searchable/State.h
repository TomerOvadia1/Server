//
// Created by tomer on 1/8/19.
//

#ifndef SERVER_STATE_H
#define SERVER_STATE_H
template <class Node>
class State {
private:
	Node node;
	double cost;
	double path_cost;
	const State<Node>* come_from;
	bool isVisited;

public:
	State(){}

	State(Node state){
		this->node = state;
		this->isVisited = false;
	}

	void setVisited(bool val){
		this->isVisited = val;
	}

	bool getIsVisited(){
		return this->isVisited;
	}

	bool operator==(State<Node> s) {
		return (s.come_from == come_from && s.cost == cost && s.node == node);
	}

	bool operator!=(const State<Node> s) const{
		return (s.come_from != come_from || s.cost != cost || s.node != node);
	}

	bool operator<(State<Node> s){
		return (cost < s.cost);
	}

	bool less(State<Node>* s1, State<Node>* s2){
		return s1 < s2;
	}

	Node getState() const {
		return node;
	}

	State setState(Node state) {
		this->node = state;
	}

	double getCost() const {
		return cost;
	}

	State setCost(double cost) {
		State::cost = cost;
	}

	State setPathCost(double cost){
		State::path_cost = cost;
	}

	double getPathCost(){
        return this->path_cost;
	}

	const State<Node>* getParent() const {
		return come_from;
	}

	State setParent(const State<Node>* come_from) {
		State::come_from = come_from;
	}

};

template <class Node>
class Compare{
public:
	bool operator()(State<Node> *state1, State<Node> *state2){
		return state1->getPathCost() > state2->getPathCost();
	}
};


#endif //SERVER_STATE_H

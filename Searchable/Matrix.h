//
// Created by tomer on 1/8/19.
//

#ifndef SERVER_MATRIX_H
#define SERVER_MATRIX_H

#include "Searchable/ISearchable.h"
#include <vector>
#include <string>

const int INITIAL_ROW = 0;
const int INITIAL_COL = 0;
const char SEPERATOR = ',';
const char NEWLINE_FEED = '\n';

class Matrix {
	using MatrixVec = std::vector<std::vector<double>> ;

public:
	using Point = std::pair<int, int>;

	Matrix(const MatrixVec& vec)    {
		this->_data = vec;
	}

	bool pointIsLegal(Point p)const{
		return (p.first < getRowSize()) && (p.second < getColSize());
	}

	/**
	 * Allows writing to given matrix
	 * @param p
	 * @return
	 */
	double& operator[](const Point& p)  {
		if(!pointIsLegal(p)){
			throw "OutOfBound position in matrix";
		}
		return _data[p.first][p.second];
	}

	/**
	 * Allows point value reading from matrix
	 * @param p
	 * @return
	 */
	double operator[](const Point& p) const {
		if(!pointIsLegal(p)){
			throw "OutOfBound position in matrix";
		}
		return _data[p.first][p.second];
	}

	size_t getRowSize()const{
		return _data.size();

	}

	size_t getColSize()const{
		size_t rowSize = getRowSize();
		if(rowSize>0){
			return _data[INITIAL_ROW].size();
		}
		return INITIAL_COL;
	}

	bool empty()const{
		return (getColSize()!=0 );
	}

private:
	MatrixVec _data ;
};



class SearchableMatrix : public ISearchable<std::pair<int, int>>
{
	using Point = std::pair<int, int>;
	Matrix matrix;
	Point entry_point;
	Point exit_point;
	std::vector <State<Point>*> to_delete;

public:
	SearchableMatrix(Matrix& m , Point initial , Point exit):matrix(m){
		if(!matrix.pointIsLegal(initial) || !matrix.pointIsLegal(exit)){
			throw "Illegal entry/exit points";
		}
		entry_point = initial ;
		exit_point = exit;
	}

	double getCost(Point p){
		return matrix[p];
	}

	State<Point>* getInitialState() override {
		State<Point>* initial_state = new State<Point>() ;
		initial_state->setParent(nullptr);
		initial_state->setCost(matrix[entry_point]);
		initial_state->setState(entry_point);
		this->to_delete.push_back(initial_state);
		return initial_state;
	}

	Point getInitialNode() const  {
		return entry_point;
	}

	Point getGoalNode() const  {
		return exit_point;
	}

	std::vector<Point> getAllPossibleNodes(Point point)const override {

			std::vector<Point> pos_states;

			//current coordinates
			int cur_x_pos = point.first;
			int cur_y_pos = point.second;

			//last coordinates in matrix
			//TODO CHECK IF IT IS NEEDED TO GET LAST COORDINATES OF CURRENT POSITION
			size_t last_x_pos = matrix.getRowSize() - 1;
			size_t last_y_pos = matrix.getColSize() - 1;

			//if it is possible to return current y position on the above row
			if (cur_x_pos > INITIAL_ROW) {
				Point pos_point = Point(cur_x_pos - 1,cur_y_pos);
				if(matrix[pos_point]>0){
					pos_states.emplace_back(pos_point);
				}
			}

			//next row
			if (cur_x_pos < last_x_pos) {
				Point pos_point = Point(cur_x_pos + 1,cur_y_pos);
				if(matrix[pos_point]>0){
					pos_states.emplace_back(pos_point);
				}
			}


			//prev col
			if (cur_y_pos > INITIAL_COL) {
				Point pos_point = Point(cur_x_pos,cur_y_pos - 1);
				if(matrix[pos_point]>0){
					pos_states.emplace_back(pos_point);
				}
			}

			//next col
			if (cur_y_pos < last_y_pos) {
				Point pos_point = Point(cur_x_pos,cur_y_pos + 1);
				if(matrix[pos_point]>0){
					pos_states.emplace_back(pos_point);
				}
			}


			return pos_states;
		}

	std::vector <State<Point>*> getAllPossibleStates(State<Point>* state) override{
		std::vector<Point> pos_nodes = getAllPossibleNodes(state->getState());
		std::vector <State<Point>*> pos_states ;
		for(Point p:pos_nodes){
			State<Point>* cur_state = new State<Point>(p);
			cur_state->setParent(state);
			cur_state->setCost(matrix[p]);
			pos_states.push_back(cur_state);
			to_delete.push_back(cur_state);
		}
		return pos_states;

	}


	//TODO
	// THIS IS MATRIX CLASS
	// JUST IMPLEMENT THE FOLLOWING FUNCTION

	std::string toString(){
		std::string string_matrix;
		size_t rows = matrix.getRowSize();
		size_t cols = matrix.getColSize();
		for(int i=0 ; i<rows ; i++){
			for (int j=0 ; j< cols; j++){
				Point cur_pos(i,j);
				std::string val = std::to_string(matrix[cur_pos]);
				string_matrix += val;
				if(j != cols-1){
					string_matrix+= SEPERATOR ;
				}
			}
			string_matrix += NEWLINE_FEED;
		}
		string_matrix+=point_toString(entry_point);
		string_matrix+=point_toString(exit_point);
		return string_matrix;
	}

	std::string point_toString(Point p){
		return "(" + std::to_string(p.first) + "," +
								  std::to_string(p.second) + ")" + "\n" ;

	}

	~SearchableMatrix(){
		for(std::vector<State<Point>*>::iterator it =
				this->to_delete.begin(); it != this->to_delete.end(); ++it) {
			delete(*it);
		}
	}

};




#endif //SERVER_MATRIX_H


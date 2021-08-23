#include <iostream>
#include <iomanip>
#include <fstream>
#include <stdexcept>
#include <vector>

using namespace std;

// i made a harder work because didnt understand that we can use Matrix::At (x2) so our vector<int> matr could be
// in private section. solution is much more beautiful than my code, i'm sorry mother..

class Matrix {
public:

	vector<int> matr; // i could make vector<vector<int>> but no..

	void Reset(int new_rows, int new_cols){
		if (new_rows < 0 || new_cols < 0) throw out_of_range("Out of range");    // exceptions here are not specified
									                 // bc this class is just "test"
		num_rows = new_rows;
		num_cols = new_cols;
		if (num_rows == 0 || num_cols == 0){
			num_rows = 0;
			num_cols = 0;
		}
		matr.resize(num_rows * num_cols);

		for (int& el : matr){
			el = 0;
		}
	}

	int At(int row, int col) const {
		if (row < 0 || col < 0 || row >= num_rows || col >= num_cols) throw out_of_range("out_of_range");
		return matr[num_cols * row + col];
	}
	int& At(int row, int col) {
		if (row < 0 || col < 0 || row >= num_rows || col >= num_cols) throw out_of_range("out_of_range");
		return matr[num_cols * row + col];
	}

	int GetNumRows() const {
		return num_rows;
	}
	int GetNumColumns() const {
		return num_cols;
	}

	Matrix(){
		num_rows = 0;
		num_cols = 0;
	}
	Matrix(int new_rows, int new_cols){
		if (new_rows < 0 || new_cols < 0) throw out_of_range("out_of_range");

		num_rows = new_rows;
		num_cols = new_cols;
		if (num_rows * num_cols == 0){
			num_rows = 0;
			num_cols = 0;
		}
		matr.resize(num_rows * num_cols);
	}

private:
	int num_rows, num_cols;
};

istream& operator>>(istream& input, Matrix& m){
	int new_row, new_col;
	input >> new_row >> new_col;

	m.Reset(new_row, new_col);
	for (int i = 0; i < new_row*new_col; ++i){
		input >> m.matr[i];
	}

	return input;
}

ostream& operator<<(ostream& output, const Matrix& m){
	int rows = m.GetNumRows(), cols = m.GetNumColumns();
	output << rows << ' ' << cols << '\n';

	for (int i = 0; i < rows; ++i){
		for (int j = 0; j < cols; ++j){
			output << m.matr[cols*i + j] << ' ';
		}
		output << '\n';
	}

	return output;
}

bool operator==(const Matrix& lhs, const Matrix& rhs){
	int rows = lhs.GetNumRows(), cols = lhs.GetNumColumns();
	if (cols != rhs.GetNumColumns() || rows != rhs.GetNumRows()) return false;

	for (int i = 0; i < rows*cols; ++i){
		if (lhs.matr[i] != rhs.matr[i]) return false;
	}

	return true;
}

Matrix operator+(const Matrix& lhs, const Matrix& rhs){
	int rows = lhs.GetNumRows(), cols = lhs.GetNumColumns();
	if (cols != rhs.GetNumColumns() || rows != rhs.GetNumRows()) throw invalid_argument("Invalid argument");

	Matrix sum(rows, cols);

	for (int i = 0; i < rows*cols; ++i){
		sum.matr[i] = lhs.matr[i] + rhs.matr[i];
	}

	return sum;
}

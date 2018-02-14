/**
 ** This library contains the functions for using linear programming to 
 ** approximately solve the link stealing algorithm
 ** 
 **/
#include "link_stealing.h"
#include <cmath>
#include <algorithm>
#include <string>
#include <iostream>
#include <fstream>
/**
 * Finds the minimum index and value in a vector of integers
 **/
int find_min_element(std::vector<int>& input, int& min_val) {
	int i = 0;
	min_val = input[0];
	int min_index = 0;
	for (auto val : input) {
		if (val < min_val) {
			min_val = val;
			min_index = i;
		}
		i++;
	}
	return min_index;
};

/**
 * Returns true if there is.a violation of constraints, and false otherwise
 **/
bool check_violation(matrix_uint &matrix, 
					vector_uint &row_constraints, 
					vector_uint &col_constraints) {
	int size = matrix.size();
	int row_sum = 0;
	int col_sum = 0;
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			row_sum += matrix[i][j];
			col_sum += matrix[j][i];
		}
		if (row_sum > row_constraints[i] || col_sum > row_constraints[i]) {
			return true;
		}
		col_sum = 0;
		row_sum = 0;
	}
	return false;
};

/**
 * Takes in an group matrix cover (the matrix of potential # of links a group can form with another) and 
 * converts that into constraints
 **/
void program_constraints(std::vector<std::vector<int>>& adjacency_matrix) {

};

//begin_optimize() {

//};

void rounding_symmetrical(	std::vector<std::vector<double>>& orig_traffic_matrix,
							matrix_float& optimal_float_solution, 
							matrix_uint& optimal_int_solution,
							vector_int& row_constraints,
							vector_int& col_constraints,
				double epsilon) {
	int size = optimal_float_solution.size();
	for (int i = 0; i < size; i++) {
		for (int j = i; j < size; j++) {
			if (i == j) {
				optimal_int_solution[i][j] = 0;
			}
			optimal_int_solution[i][j] = std::round(optimal_float_solution[i][j]);
			if ((orig_traffic_matrix[i][j] >= epsilon) && (optimal_int_solution[i][j] == 0)) {
				optimal_int_solution[i][j] = 1;
				optimal_int_solution[j][i] = 1;
			}	
		}
	}
};

void round_down_symmetrical(std::vector<std::vector<double>>& orig_traffic_matrix,
							matrix_float& optimal_float_solution, 
							matrix_uint& optimal_int_solution,
							vector_int& row_constraints, 
							vector_int& col_constraints,
							double epsilon) {
	int size = optimal_float_solution.size();
	for (int i = 0; i < size; i++) {
		for (int j = i; j < size; j++) {
			if (i == j) {
				optimal_int_solution[i][j] = 0;
			}
			optimal_int_solution[i][j] = (int) optimal_float_solution[i][j];
			if ((orig_traffic_matrix[i][j] >= epsilon) && (optimal_int_solution[i][j] == 0)) {
				optimal_int_solution[i][j] = 1;
				optimal_int_solution[j][i] = 1;
			}	
		}
	}
};

/**
 * The main link_stealing algorithm
 **/
void link_stealing(matrix_float& traffic_matrix, 
					matrix_uint& solution, 
					vector_uint& row_constraints, 
					vector_uint& col_constraints) {
	int size = traffic_matrix.size();
	//program_constraints(matrix_int adjacency_matrix);
	//begin_optimize(constraints, soln_float);
	//rounding(soln_float, solution);

	std::vector<int> row_links_left;
	std::vector<int> col_links_left;
	row_links_left.resize(size);
	col_links_left.resize(size);
	for (int i = 0; i < size; i++) {
		int row_sum = 0;
		int col_sum = 0;
		for (int j = 0; j < size; j++) {
			row_sum += solution[i][j]; 
			col_sum += solution[j][i];
		}
		row_links_left[i] = row_constraints[i] - row_sum;
		col_links_left[i] = col_constraints[i] - col_sum;
	}
};

void read_float_matrix(std::string filename, matrix_float& float_matrix) {
	std::ifstream mat_file(filename);
	if (mat_file.is_open()) {
      std::string line;
      std::getline(mat_file, line);
      int size = std::stoi(line, 0);
      //assert(size == max_switch_id_ + 1);
      int i = 0;
      float_matrix.resize(size);
      while ( std::getline(mat_file, line) && i < size) {
        char* dup = (char *) line.c_str();
        const char* entry = std::strtok(dup, " [],");
        int j = 0;
        float_matrix[i].resize(size);
        while (entry!= NULL) {
          float cnt = std::stof(entry, 0);
          
          float_matrix[i][j] = cnt;

          entry = std::strtok(NULL, " [],");
          j++;
        }
        i++;
      }
    } else {
      printf("Cannot Open the adjacency_matrix file");
      exit(0);
    }
	return;
};

/**
 * Checks if the current matrix fulfills and fits preciesly the row and column constraints
 **/
bool check_validity(matrix_int& int_matrix, vector_int& row_constraints, vector_int& col_constraints) {
	int size = int_matrix.size();
	for (int i = 0; i < size; i++) {
		int row_sum = 0;
		int col_sum = 0;
		for (int j = 0; j < size; j++) {
			row_sum += int_matrix[i][j];
			col_sum += int_matrix[j][i];
		}
		if ((row_constraints[i] != row_sum) || (col_constraints[i] != col_sum)) {
			return false;
		}
	}
	return true;
}

void massage_random(matrix_int& int_matrix, vector_int& row_constraints, vector_int& col_constraints) {
	int size = int_matrix.size();
	vector_int col_link_left;
	vector_int row_link_left;
	col_link_left.resize(size);
	row_link_left.resize(size);
	vector_int groups_left_row;
	vector_int groups_left_col;
	for (int i = 0; i < size; i++) {
		int row_sum = 0;
		int col_sum = 0;
		for (int j = 0; j < size; j++) {
			row_sum += int_matrix[i][j];
			col_sum += int_matrix[j][i];
		}
		col_link_left[i] = col_constraints[i] - col_sum;
		row_link_left[i] = row_constraints[i] - row_sum;
		assert(col_link_left[i] >= 0);
		assert(row_link_left[i] >= 0);
		if (col_link_left[i] > 0) {
			groups_left_col.push_back(i);
		}
		if (row_link_left[i] > 0) {
			groups_left_row.push_back(i);
		}
	}
	std::srand(100);
	//if (groups_left_row.size() != groups_left_col.size()) {
	//	std::cout << "Nope, not a good configuration" << std::endl;
	//	return;
	//}
	/*
	 *  
	 */
	bool stop = false;
	while (!stop) {
		int row = groups_left_row[0];
		int col_size = groups_left_col.size();
		int col_index = std::rand() % col_size;
		int col = groups_left_col[col_index];
		if (col == row) {
			col_index = (col_index + 1) % col_size;
			col = groups_left_col[col_index];
		}
		int_matrix[row][col]++;
		row_link_left[row]--;
		col_link_left[col]--;
		if (row_link_left[row] == 0) {
			auto it = std::find(groups_left_row.begin(), groups_left_row.end(), row);
			assert(it != groups_left_row.end());
    		groups_left_row.erase(it);
		} 
		if (col_link_left[col] == 0) {
			auto it = std::find(groups_left_col.begin(), groups_left_col.end(), col);
			assert(it != groups_left_col.end());
    		groups_left_col.erase(it);
		}

		if (groups_left_col.empty() || groups_left_row.empty()) {
			break;
		}
	}


	if (!check_validity(int_matrix, row_constraints, col_constraints)) {
		std::cout << "Also not a good configuration jabfgberUIGBUIADLUIVHALUDIGFUAUL" << std::endl;
	}
}
/**
 * Just a simple rounding down scheme
 **/ 
void simple_rounding_down(matrix_float& float_matrix, matrix_int& int_matrix) {
	int size = float_matrix.size();
	int_matrix.resize(size);
	for (int i = 0; i < size; i++) {
		int_matrix[i].resize(size);
		for (int j = 0; j < size; j++) {
			int_matrix[i][j] = (int) float_matrix[i][j];
		}
	}
	return;
}





int main(int argc, char* argv[]) {
	std::cout << "main function has been reached" << std::endl;
	if (argc >= 2) {
		std::string filename;
		//filename << argv[1];
		filename = std::string(argv[1]);
		matrix_float float_mat;
		matrix_int int_mat;

		read_float_matrix(filename, float_mat);
		std::cout << "hello" << std::endl;
		simple_rounding_down(float_mat, int_mat);

		int size = int_mat.size();
		vector_int row_constraints;
		vector_int col_constraints;
		row_constraints.resize(size);
		col_constraints.resize(size);
		std::fill(row_constraints.begin(), row_constraints.end(), size - 1);
		std::fill(col_constraints.begin(), col_constraints.end(), size - 1);
		massage_random(int_mat, row_constraints, col_constraints);


		for (auto row : int_mat) {
			for (auto entry : row) {
				std::cout << std::to_string(entry) + " ";
			}
			std::cout << std::endl;
		}
	}

}





/*

	bool keep_going = true;
	while (keep_going) {
		int row_min = std::min_element(row_links_left.begin(), row_links_left.end());
		int col_min = std::min_element(col_links_left.begin(), col_links_left.end());
		if (row_min < col_min) {
			if (row_min < 0) {

			} else {

			}
		} else {
			if (row_min < 0) {

			} else {

			}
		}
	}
};
*/

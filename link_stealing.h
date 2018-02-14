#include <vector>
#include <cassert>

typedef std::vector<std::vector<int>> matrix_int;
typedef std::vector<std::vector<uint32_t>> matrix_uint;
typedef std::vector<std::vector<float>> matrix_float;
typedef std::vector<std::vector<double>> matrix_double;
typedef std::vector<int> vector_int;
typedef std::vector<uint32_t> vector_uint;
typedef std::vector<float> vector_float;
typedef std::vector<double> vector_double;


void read_float_matrix(std::string filename, matrix_float& float_matrix);

void simple_rounding_down(matrix_float& float_mat, matrix_int& int_mat);

void massage_random(matrix_int& int_matrix, vector_int& row_constraints, vector_int& col_constraints);

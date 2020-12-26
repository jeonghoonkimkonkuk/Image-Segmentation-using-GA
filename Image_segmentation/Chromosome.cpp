#include "pch.h"
#include "Chromosome.h"

Chromosome::Chromosome()
	: label(nullptr), feature_vector(nullptr), fitness(0)
{
}

Chromosome::~Chromosome()
{
	if (label != nullptr) {
		delete[] label;
	}
	if (feature_vector != nullptr) {
		for (int i = 0; i < 1 + feature_matrix_size * 2; i++) {
			for (int j = 0; j < 1 + feature_matrix_size * 2; j++) {
				delete[] feature_vector[i][j];
			}
			delete[] feature_vector[i];
		}
		delete[] feature_vector;
	}
}

Chromosome::Chromosome(int label_val)
	:fitness(0)
{
	this->label = new bool[label_size];
	for (int i = 0; i <label_size ; i++) {
		this->label[i] = false;
		if (label_val % 2) {
			this->label[i] = true;
		}
		label_val /= 2;
	}

	feature_vector = new bool** [1 + feature_matrix_size * 2];
	for (int i = 0; i < 1 + feature_matrix_size * 2; i++) {
		feature_vector[i] = new bool* [1 + feature_matrix_size * 2];
		for (int j = 0; j < 1 + feature_matrix_size * 2; j++) {
			feature_vector[i][j] = new bool[feature_size];
			for (int k = 0; k < feature_size; k++) {
				feature_vector[i][j][k] = false;
			}
		}
	}
}

Chromosome::Chromosome(const Chromosome& copy)
	:fitness(copy.fitness)
{
	this->label = new bool[label_size];
	for (int i = 0; i < label_size; i++) {
		this->label[i] = copy.label[i];
	}
	feature_vector = new bool** [1 + feature_matrix_size * 2];
	for (int i = 0; i < 1 + feature_matrix_size * 2; i++) {
		feature_vector[i] = new bool* [1 + feature_matrix_size * 2];
		for (int j = 0; j < 1 + feature_matrix_size * 2; j++) {
			feature_vector[i][j] = new bool[feature_size];
			for (int k = 0; k < feature_size; k++) {
				feature_vector[i][j][k] = copy.feature_vector[i][j][k];
			}
		}
	}
}

void Chromosome::set_feature_vector(const int& x, const int& y, int value)
{

	for (int i = 0; i < feature_size; i++) {
		this->feature_vector[y+feature_matrix_size][x+feature_matrix_size][i] = false;
		if (value % 2) {
			this->feature_vector[y+feature_matrix_size][x+feature_matrix_size][i] = true;
		}
		value /= 2;
	}
}

int Chromosome::get_feature_vector(const int& x, const int& y)
{
	int tmp = 1;
	int sum = 0;
	for (int i = 0; i < feature_size; i++) {
		if (feature_vector[y+feature_matrix_size][x+feature_matrix_size][i])
			sum += tmp;
		tmp *= 2;
	}
	return sum;
}

int Chromosome::get_label()
{
	int tmp = 1;
	int sum = 0;
	for (int i = 0; i < label_size; i++) {
		if (label[i])
			sum += tmp;
		tmp *= 2;
	}
	return sum;
}


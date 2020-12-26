#pragma once
#define label_size 2
#define feature_size 8
#define feature_matrix_size 1
#define neighborhood_size 1
#define attempt_mating 1

class Chromosome
{
public:
	bool* label;
	bool*** feature_vector;
	int fitness;
public:
	Chromosome();
	~Chromosome();
	Chromosome(int label);
	Chromosome(const Chromosome& copy);
	void set_feature_vector(const int& x, const int& y, int value);
	int get_feature_vector(const int& x, const int& y);
	int get_label();
};


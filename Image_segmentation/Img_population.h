#pragma once
#include<math.h>
#include<time.h>
#include"Chromosome.h"

class Img_population
{
public:
	BITMAP Imginfo;
	int** org_img;
	Chromosome*** chr_img;
public:
	Img_population();
	~Img_population();
	Img_population(BITMAP Imginfo, int** gray_img, bool RI = true); // RI วüลย
	void compute_fitness();
	void new_population_LTS();
	void new_population_RWS();
	void mating();
	void mating2();
	double gaussianRandom(double average, double stdev);
};


#include "pch.h"
#include "Img_population.h"

Img_population::Img_population()
	:org_img(nullptr), chr_img(nullptr), Imginfo(BITMAP())
{
}

Img_population::~Img_population()
{
	for (int i = 0; i < Imginfo.bmHeight; i++) {
		for (int j = 0; j < Imginfo.bmWidth; j++) {
			delete chr_img[i][j];
		}
		delete[] org_img[i];
		delete[] chr_img[i];
	}
	delete[] org_img;
	delete[] chr_img;
}

Img_population::Img_population(BITMAP Imginfo, int** gray_img, bool RI)
	:Imginfo(Imginfo)
{
	srand(time(NULL));
	int gaussian;
	if (RI) {
		org_img = new int *[Imginfo.bmHeight];
		chr_img = new Chromosome * *[Imginfo.bmHeight];
		for (int y = 0; y < Imginfo.bmHeight; y++) {
			org_img[y] = new int[Imginfo.bmWidth];
			chr_img[y] = new Chromosome * [Imginfo.bmWidth];
			for (int x = 0; x < Imginfo.bmWidth; x++) {
				org_img[y][x] = gray_img[y][x];
				chr_img[y][x] = new Chromosome(rand() % (label_size+1));
			}
		}
		for (int y = 0; y < Imginfo.bmHeight; y++) {
			for (int x = 0; x < Imginfo.bmWidth; x++) {
				for (int i = (-1) * feature_matrix_size; i <= feature_matrix_size; i++) {
					if ((y + i < 0) || (y + i >= Imginfo.bmHeight)) continue;
					for (int j = (-1) * feature_matrix_size; j <= feature_matrix_size; j++) {
						if ((x + j < 0) || (x + j >= Imginfo.bmWidth)) continue;
						chr_img[y][x]->set_feature_vector(j, i,rand()%256);
					}
				}
			}
		}
		this->compute_fitness();
	}
	else {
		org_img = new int* [Imginfo.bmHeight];
		chr_img = new Chromosome * *[Imginfo.bmHeight];
		for (int y = 0; y < Imginfo.bmHeight; y++) {
			org_img[y] = new int[Imginfo.bmWidth];
			chr_img[y] = new Chromosome * [Imginfo.bmWidth];
			for (int x = 0; x < Imginfo.bmWidth; x++) {
				org_img[y][x] = gray_img[y][x];
				chr_img[y][x] = new Chromosome(rand() % (label_size + 1));
			}
		}
		for (int y = 0; y < Imginfo.bmHeight; y++) {
			for (int x = 0; x < Imginfo.bmWidth; x++) {
				for (int i = (-1) * feature_matrix_size; i <= feature_matrix_size; i++) {
					if ((y + i < 0) || (y + i >= Imginfo.bmHeight)) continue;
					for (int j = (-1) * feature_matrix_size; j <= feature_matrix_size; j++) {
						if ((x + j < 0) || (x + j >= Imginfo.bmWidth)) continue;
						if( (gaussian = this->gaussianRandom(chr_img[y][x]->get_label(), 256/(label_size*4))) < 0 )
							gaussian += 256;

						chr_img[y][x]->set_feature_vector(j, i,gaussian);
					}
				}
			}
		}
		this->compute_fitness();
	}
}

void Img_population::compute_fitness()
{
	int tmp;
	int feature;
	for (int y = 0; y < Imginfo.bmHeight; y++) {
		for (int x = 0; x < Imginfo.bmWidth; x++) {
			tmp = 0;
			for (int i = (-1) * feature_matrix_size; i <= feature_matrix_size; i++) {
				if ((y + i < 0) || (y + i >= Imginfo.bmHeight)) continue;
				for (int j = (-1) * feature_matrix_size; j <= feature_matrix_size; j++) {
					if ((x + j < 0) || (x + j >= Imginfo.bmWidth)) continue;
					feature = chr_img[y][x]->get_feature_vector(j, i);
					if (feature - org_img[y + i][x + j] > 0)
						tmp += feature - org_img[y + i][x + j];
					else
						tmp += org_img[y + i][x + j] - feature;
				}
			}
			chr_img[y][x]->fitness = -tmp;
		}
	}
}

void Img_population::new_population_LTS()
{
	Chromosome*** tmp_img;
	tmp_img = new Chromosome * *[Imginfo.bmHeight];
	for (int y = 0; y < Imginfo.bmHeight; y++) {
		tmp_img[y] = new Chromosome * [Imginfo.bmWidth];
	}
	Chromosome* tmp_fitness;
	for (int y = 0; y < Imginfo.bmHeight; y++) {
		for (int x = 0; x < Imginfo.bmWidth; x++) {
			tmp_fitness = chr_img[y][x];
			for (int i = (-1) * neighborhood_size; i <= neighborhood_size; i++) {
				if ((y + i < 0) || (y + i >= Imginfo.bmHeight)) continue;
				for (int j = (-1) * neighborhood_size; j <= neighborhood_size; j++) {
					if ((x + j < 0) || (x + j >= Imginfo.bmWidth)) continue;
					if ((i == 0) && (j == 0)) continue;
					
					if (tmp_fitness->fitness <= chr_img[y + i][x + j]->fitness)
						tmp_fitness = chr_img[y + i][x + j];

				}
			}
			tmp_img[y][x] = new Chromosome(*tmp_fitness);
		}
	}

	tmp_fitness = nullptr;
	for (int i = 0; i < Imginfo.bmHeight; i++) {
		for (int j = 0; j < Imginfo.bmWidth; j++) {
			delete chr_img[i][j];
		}
		delete[] chr_img[i];
	}
	delete[] chr_img;
	chr_img = tmp_img;
	tmp_img = nullptr;
}

void Img_population::new_population_RWS()
{

	Chromosome*** tmp_img;
	tmp_img = new Chromosome * *[Imginfo.bmHeight];
	for (int y = 0; y < Imginfo.bmHeight; y++) {
		tmp_img[y] = new Chromosome * [Imginfo.bmWidth];
	}
	for (int y = 0; y < Imginfo.bmHeight; y++) {
		for (int x = 0; x < Imginfo.bmWidth; x++) {
			int fSum = 0;
			int fMin = chr_img[y][x]->fitness;
			int count = 0;
			Chromosome** tmp_nei = new Chromosome*[(2 * neighborhood_size + 1) * (2 * neighborhood_size + 1)];
			for (int i = (-1) * neighborhood_size; i <= neighborhood_size; i++) {
				if ((y + i < 0) || (y + i >= Imginfo.bmHeight)) continue;
				for (int j = (-1) * neighborhood_size; j <= neighborhood_size; j++) {
					if ((x + j < 0) || (x + j >= Imginfo.bmWidth)) continue;

					tmp_nei[count++]= chr_img[y + i][x + j];

					fSum += chr_img[y + i][x + j]->fitness;
					if (fMin > chr_img[y + i][x + j]->fitness) {
						fMin = chr_img[y + i][x + j]->fitness;
					}
				}
			}

			fSum -= fMin * count;
			if (fSum==0) {
				tmp_img[y][x] = new Chromosome(*tmp_nei[rand() % count]);
			}
			else {
				int sel = rand() % fSum;
				for (int i = 0; i < count; i++) {
					int ff = tmp_nei[i]->fitness - fMin;
					sel -= ff;
					if (sel < 0) {
						tmp_img[y][x] = new Chromosome(*tmp_nei[i]);
						break;
					}
				}
			}
			delete[] tmp_nei;
			tmp_nei = nullptr;
		}
	}

	for (int i = 0; i < Imginfo.bmHeight; i++) {
		for (int j = 0; j < Imginfo.bmWidth; j++) {
			delete chr_img[i][j];
		}
		delete[] chr_img[i];
	}
	delete[] chr_img;
	chr_img = tmp_img;
	tmp_img = nullptr;
}

void Img_population::mating()
{
	Chromosome*** tmp_img;
	tmp_img = new Chromosome * *[Imginfo.bmHeight];
	for (int y = 0; y < Imginfo.bmHeight; y++) {
		tmp_img[y] = new Chromosome * [Imginfo.bmWidth];
	}
	
	int tmp_pick_x, tmp_pick_y;
	int attempt;
	for (int y = 0; y < Imginfo.bmHeight; y++) {
		for (int x = 0; x < Imginfo.bmWidth; x++) {
			tmp_img[y][x] = new Chromosome(*chr_img[y][x]);
			attempt = 0;
			while (attempt <= attempt_mating) {
				tmp_pick_y = rand() % (1 + neighborhood_size * 2) - 1;
				tmp_pick_x = rand() % (1 + neighborhood_size * 2) - 1;
				if (tmp_pick_x == 0 && tmp_pick_y == 0) continue;
				if ((y + tmp_pick_y < 0) || (y + tmp_pick_y >= Imginfo.bmHeight)) continue;
				if ((x + tmp_pick_x < 0) || (x + tmp_pick_x >= Imginfo.bmHeight)) continue;

				if (chr_img[y][x]->label == chr_img[y + tmp_pick_y][x + tmp_pick_x]->label)
					for (int i = 0; i < 1 + feature_matrix_size * 2; i++) {
						for (int j = 0; j < 1 + feature_matrix_size * 2; j++) {
							int pos = rand() % (feature_size - 1) + 1;
							if (rand() % 2) {
								for (int k = 0; k < pos; k++) {
									tmp_img[y][x]->feature_vector[i][j][k] = chr_img[y + tmp_pick_y][x + tmp_pick_x]->feature_vector[i][j][k];
								}
							}
							else {
								for (int k = pos; k < feature_size; k++) {
									tmp_img[y][x]->feature_vector[i][j][k] = chr_img[y + tmp_pick_y][x + tmp_pick_x]->feature_vector[i][j][k];
								}
							}

						}

					}
				else {
					attempt++;
				}

			}
		}
	}

	for (int i = 0; i < Imginfo.bmHeight; i++) {
		for (int j = 0; j < Imginfo.bmWidth; j++) {
			delete chr_img[i][j];
		}
		delete[] chr_img[i];
	}
	delete[] chr_img;
	chr_img = tmp_img;
	tmp_img = nullptr;

}

void Img_population::mating2() {
	Chromosome*** tmp_img;
	tmp_img = new Chromosome * *[Imginfo.bmHeight];
	for (int y = 0; y < Imginfo.bmHeight; y++) {
		tmp_img[y] = new Chromosome * [Imginfo.bmWidth];
	}

	int tmp_pick_x, tmp_pick_y;
	int attempt;
	for (int y = 0; y < Imginfo.bmHeight; y++) {
		for (int x = 0; x < Imginfo.bmWidth; x++) {
			tmp_img[y][x] = new Chromosome(*chr_img[y][x]);
			attempt = 0;
			while (attempt <= attempt_mating) {
				tmp_pick_y = rand() % (1 + neighborhood_size * 2) - 1;
				tmp_pick_x = rand() % (1 + neighborhood_size * 2) - 1;
				if (tmp_pick_x == 0 && tmp_pick_y == 0) continue;
				if ((y + tmp_pick_y < 0) || (y + tmp_pick_y >= Imginfo.bmHeight)) continue;
				if ((x + tmp_pick_x < 0) || (x + tmp_pick_x >= Imginfo.bmHeight)) continue;

				if (chr_img[y][x]->label == chr_img[y + tmp_pick_y][x + tmp_pick_x]->label) {


					int pos = rand() % ((1 + feature_matrix_size * 2) * (1 + feature_matrix_size * 2));

					int share = pos / (1 + feature_matrix_size * 2);
					int rest = pos % (1 + feature_matrix_size * 2);
					if (rand() % 2) {

						for (int i = 0; i < share; i++) {
							for (int j = 0; j < 1 + feature_matrix_size * 2; j++) {
								for (int k = 0; k < feature_size; k++) {
									tmp_img[y][x]->feature_vector[i][j][k] = chr_img[y + tmp_pick_y][x + tmp_pick_x]->feature_vector[i][j][k];
								}
							}
						}

						for (int j = 0; j < rest; j++) {
							for (int k = 0; k < feature_size; k++) {
								tmp_img[y][x]->feature_vector[share][j][k] = chr_img[y + tmp_pick_y][x + tmp_pick_x]->feature_vector[share][j][k];
							}
						}
					}
					else {
						
						for (int j = rest; j<1+feature_matrix_size*2; j++) {
							for (int k = 0; k < feature_size; k++) {
								tmp_img[y][x]->feature_vector[share][j][k] = chr_img[y + tmp_pick_y][x + tmp_pick_x]->feature_vector[share][j][k];
							}
						}

						for (int i = share + 1; i < feature_matrix_size; i++) {
							for (int j = 0; j < 1 + feature_matrix_size * 2; j++) {
								for (int k = 0; k < feature_size; k++) {
									tmp_img[y][x]->feature_vector[i][j][k] = chr_img[y + tmp_pick_y][x + tmp_pick_x]->feature_vector[i][j][k];
								}
							}
						}
					}
				}
				else {
					attempt++;
				}

			}
		}
	}

	for (int i = 0; i < Imginfo.bmHeight; i++) {
		for (int j = 0; j < Imginfo.bmWidth; j++) {
			delete chr_img[i][j];
		}
		delete[] chr_img[i];
	}
	delete[] chr_img;
	chr_img = tmp_img;
	tmp_img = nullptr;

}




double Img_population::gaussianRandom(double average, double stdev)
{
	double v1, v2, s, temp;
	do {
		v1 = 2 * ((double)rand() / RAND_MAX) - 1;
		v2 = 2 * ((double)rand() / RAND_MAX) - 1;
		s = v1 * v1 + v2 * v2;
	} while (s >= 1 || s == 0);
	s = sqrt(-2 * log(s) / s);
	temp = v1 * s;
	return stdev * temp + average;
}

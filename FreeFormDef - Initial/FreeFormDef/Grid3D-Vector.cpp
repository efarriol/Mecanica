	std::chrono::high_resolution_clock::time_point tini, tfin;
	const int HEIGHT = 200;
	const int WIDTH = 200;
	const int DEPTH = 200;

	/*--------------------------------------
	 Vector Version of the 3D grid
	----------------------------------------*/

	int ind;
	tini = std::chrono::high_resolution_clock::now();
	std::vector<glm::vec3> grid3Dv;

	grid3Dv.resize(HEIGHT*WIDTH*DEPTH);

	// Assign values
	for (int i = 0; i < HEIGHT; ++i) {
		for (int j = 0; j < WIDTH; ++j) {
			for (int k = 0; k < DEPTH; ++k) {
				ind = (i*HEIGHT + j)*WIDTH + k;
				grid3Dv[ind].x = i;
				grid3Dv[ind].y = j;
				grid3Dv[ind].z = k;
			}
		}
	}

	tfin = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(tfin - tini).count();
	std::cout << "Time using vector  " << duration << "ms" << std::endl;
	//verify input values
	ind = (100 * HEIGHT + 99)*WIDTH + 98;
	std::cout << "valor al lloc [i][j][k] =  " <<
		grid3Dv[ind].x << " " << grid3Dv[ind].y << " " << grid3Dv[ind].z << std::endl;

	system("PAUSE");

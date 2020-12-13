/*
 * X Y Cut Recursive Algorithm
 * Splits the matrix for each row or column containing only white pixels.
 * Calls itself again on the splits matrices until each matrix can not 
 * be split anymore.
 *  Returns the list of these splits matrices.
 */
void xycut(Matrix m, int x1, int y1, int x2, int y2, List *L,Matrix m_to_copy)
{
	int dif_y = y2 - y1;
	int dif_x = x2 - x1;
	int lines[dif_y], columns[dif_x];

	for (int y = 0; y < dif_y; y++)
		lines[y] = 0;

	for (int x = 0; x < dif_x; x++)
		columns[x] = 0;

	for (int h = 0; h < dif_y; h++)
	{
		for (int w = 0; w < dif_x; w++)
		{
			if (matrix_get(m, w + x1, h + y1))
			{
				lines[h] += 1;
				columns[w] += 1;
			}
		}
	}

	int y = 0;
	while (y < dif_y && lines[y] == 0)
			y++;
	while (y < dif_y)
	{
		int first_y = y;

		while (y < dif_y && lines[y] != 0)
			y++;

		int x = 0;
		while(x < dif_x && columns[x] == 0)
				x++;
		while (x < dif_x)
		{
			int first_x = x;

			while(x < dif_x && columns[x] != 0)
				x++;

			int width = x - first_x;
			int height = y - first_y;

			if (width == dif_x && height == dif_y)
			{
				matrix_list_add(L, matrix_copy(m_to_copy, x1, y1, x2, y2));
				// color_char(img, x1, y1, x2, y2); //Debug purpose only
			}
				else
				xycut(m, x1 + first_x, y1 +first_y, x1 + x, y1 + y, L,\
						m_to_copy);
			while(x < dif_x && columns[x] == 0)
				x++;
		}
		while (y < dif_y && lines[y] == 0)
			y++;
	}
}

/*
 * X Cut, used to seperate words and chars
 * (doesn't seperate letters as i and j)
 */

void x_cut(Matrix m, int x1, int y1, int x2, int y2, List *L,Matrix m_to_copy)
{
	size_t dif_y = y2 - y1;
	size_t dif_x = x2 - x1;
	size_t columns[dif_x];

	for (size_t x = 0; x < dif_x; x++)
		columns[x] = 0;

	for (size_t h = 0; h < dif_y; h++)
	{
		for (size_t w = 0; w < dif_x; w++)
		{
			if (matrix_get(m, w + x1, h + y1))
			{
				columns[w] += 1;
			}
		}
	}

	size_t x = 0;
	while (x < dif_x)
	{
		while(x < dif_x && columns[x] == 0)
			x++;

		size_t first_x = x; 

		if (first_x != dif_x)
		{
			while(x < dif_x && columns[x] != 0)
				x++;

			size_t lines[dif_y];
			
			for (size_t y = 0; y < dif_y; y++)
				lines[y] = 0;

			for (size_t h = 0; h < dif_y; h++)
				for (size_t w = first_x; w < x; w++)
					if (matrix_get(m, w + x1, h + y1))
						lines[h] += 1;

			size_t first_y = 0;
			while (first_y < dif_y && lines[first_y] == 0)
				first_y++;

			size_t last_y = dif_y;
			while (last_y > first_y && lines[last_y - 1] == 0)
				last_y--;

			matrix_list_add(L, matrix_copy(m_to_copy, x1 + first_x, y1 + first_y, 
						x1 + x, y1 + last_y));
			// color_char(img, x1, y1, x2, y2); //Debug purpose only
		}
	}
}

/* 
 * SEGMENTATION
 * This is the main function of segmentation.
 * It takes in the image and a counter that corresponds to the length of the
 * string that will be reconstructed.
 * It calls functions from matrix.c to turn the image into a grayscale matrix
 * and then into a binar matrix using Otsu's algorithm.
 * Then, it uses the RLSA and XYCUT algorithms to divide the matrix into blocs,
 * lines, words and finaly chars.
 * It returns a tree of matrices containing the characters in the leaves.
 */
List * segmentation_main(SDL_Surface *img, int *length)
{
	Matrix m_grayscale = surface_to_matrix_grayscale(img);

	int threshold = otsu(m_grayscale);
	Matrix m_otsu = matrix_grayscale_to_binar(m_grayscale, threshold);
	free_matrix(m_grayscale);

	invert_colors(&m_otsu);

	// Blocs
	Matrix m_hor = rlsa_horizontal(m_otsu, 25);
	Matrix m_ver = rlsa_vertical(m_otsu, 15);
	Matrix m_or = matrix_or(m_hor, m_ver);

	List *list_blocs = new_matrix_list();
	xycut(m_or, 0, 0, m_or.cols, m_or.rows, list_blocs, m_otsu);

	free_matrix(m_otsu);
	free_matrix(m_hor);
	free_matrix(m_ver);
	free_matrix(m_or);

	// Lignes
	Matrix_list *m_list_blocs = list_blocs->first_matrix;
	while (m_list_blocs != NULL)
	{
		List *list_lines = new_matrix_list();
		m_list_blocs->children = list_lines;

		invert_colors(&m_list_blocs->matrix);

		Matrix m_hor_lines = rlsa_horizontal(m_list_blocs->matrix, 25);
		xycut(m_hor_lines, 0, 0, m_hor_lines.cols,\
				m_hor_lines.rows, list_lines, m_list_blocs->matrix);
		free_matrix(m_hor_lines);

		// Words
		Matrix_list *m_list_lines = list_lines->first_matrix;
		while (m_list_lines != NULL)
		{
			List *list_words = new_matrix_list();
			m_list_lines->children = list_words;

			Matrix m_hor_words = rlsa_horizontal(m_list_lines->matrix, 5);
			x_cut(m_hor_words, 0, 0, m_hor_words.cols, m_hor_words.rows,
					list_words, m_list_lines->matrix);
			free_matrix(m_hor_words);

			// Characters
			Matrix_list *m_list_words = list_words->first_matrix;
			while (m_list_words != NULL)
			{
				List *list_chars = new_matrix_list();
				m_list_words->children = list_chars;

				x_cut(m_list_words->matrix, 0, 0, m_list_words->matrix.cols,\
m_list_words->matrix.rows, list_chars, m_list_words->matrix);

				*length += (int) matrix_list_length(list_chars);
				*length += 1;
				m_list_words = m_list_words->next_matrix;
			}

			*length += 1;
			m_list_lines = m_list_lines->next_matrix;
		}

		*length += 1;
		m_list_blocs = m_list_blocs->next_matrix;
	}

	return list_blocs;
}
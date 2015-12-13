#include <stdio.h>
#include <stdlib.h>

#define BASE_SIZE_MIN 1UL
#define BASE_SIZE_MAX 16UL

typedef struct cell_s cell_t;
struct cell_s {
	unsigned long number;
	unsigned long row;
	unsigned long column;
	unsigned long block;
	unsigned long value;
	unsigned long allowed;
	cell_t *left;
	cell_t *right;
};

void init_cell(cell_t *, unsigned long, cell_t *);
unsigned long **create_allowed_matrix(void);
int solve_grid(void);
int value_allowed(const cell_t *, unsigned long);
void set_cell(cell_t *, unsigned long);
void unset_cell(cell_t *, unsigned long);
int constraint_propagated(cell_t *, unsigned long, unsigned long);
void search(void);
void print_grid(void);
void free_data(void);
void free_allowed_matrix(unsigned long **, unsigned long);

unsigned long base_size, line_size, grid_size, **allowed_in_rows = NULL, **allowed_in_columns = NULL, **allowed_in_blocks = NULL, cost, solutions;
cell_t *cells = NULL, *cell_last;

int main(void) {
int exit_code;
unsigned long grids, i;
cell_t *cell;
	if (scanf("%lu", &base_size) != 1 || base_size < BASE_SIZE_MIN || base_size > BASE_SIZE_MAX) {
		fprintf(stderr, "Base size must lie between %lu and %lu\n", BASE_SIZE_MIN, BASE_SIZE_MAX);
		return EXIT_FAILURE;
	}
	line_size = base_size*base_size;
	grid_size = line_size*line_size;
	cells = malloc(sizeof(cell_t)*(grid_size+1));
	if (!cells) {
		fprintf(stderr, "Error allocating memory for cells\n");
		return EXIT_FAILURE;
	}
	cell_last = cells+grid_size;
	init_cell(cells, grid_size, cell_last);
	for (i = 0, cell = cells; i < grid_size; i++, cell++) {
		init_cell(cell+1, i, cell);
	}
	allowed_in_rows = create_allowed_matrix();
	if (!allowed_in_rows) {
		free_data();
		return EXIT_FAILURE;
	}
	allowed_in_columns = create_allowed_matrix();
	if (!allowed_in_columns) {
		free_data();
		return EXIT_FAILURE;
	}
	allowed_in_blocks = create_allowed_matrix();
	if (!allowed_in_blocks) {
		free_data();
		return EXIT_FAILURE;
	}
	if (scanf("%lu", &grids) != 1 || !grids) {
		fprintf(stderr, "Number of grids must be greater than 0\n");
		free_data();
		return EXIT_FAILURE;
	}
	exit_code = solve_grid();
	for (i = 1; i < grids && exit_code == EXIT_SUCCESS; i++) {
		exit_code = solve_grid();
	}
	free_data();
	return exit_code;
}

void init_cell(cell_t *cell, unsigned long number, cell_t *left) {
	cell->number = number;
	cell->row = number/line_size;
	cell->column = number%line_size;
	cell->block = cell->row/base_size*base_size+cell->column/base_size;
	cell->allowed = line_size;
	cell->left = left;
	left->right = cell;
}

unsigned long **create_allowed_matrix(void) {
unsigned long **matrix = malloc(sizeof(unsigned long *)*line_size), i, j;
	if (!matrix) {
		fprintf(stderr, "Error allocating memory for matrix\n");
		return NULL;
	}
	for (i = 0; i < line_size; i++) {
		matrix[i] = malloc(sizeof(unsigned long)*line_size);
		if (!matrix[i]) {
			fprintf(stderr, "Error allocating memory for matrix row\n");
			free_allowed_matrix(matrix, i);
			return NULL;
		}
		for (j = 0; j < line_size; j++) {
			matrix[i][j] = grid_size;
		}
	}
	return matrix;
}

int solve_grid(void) {
unsigned long value;
cell_t * cell;
	for (cell = cells+1; cell <= cell_last; cell++) {
		if (scanf("%lu", &value) != 1 || value > line_size) {
			fprintf(stderr, "Bad value in R%luC%lu\n", cell->row+1, cell->column+1);
			return EXIT_FAILURE;
		}
		else if (value) {
			if (value_allowed(cell, value-1)) {
				set_cell(cell, value-1);
			}
			else {
				fprintf(stderr, "Duplicate value in R%luC%lu\n", cell->row+1, cell->column+1);
				return EXIT_FAILURE;
			}
		}
		else {
			cell->value = 0;
		}
	}
	print_grid();
	solutions = cost = 0;
	search();
	printf("\nCost %lu\nSolutions %lu\n", cost, solutions);
	for (cell = cell_last; cell > cells; cell--) {
		if (cell->value) {
			unset_cell(cell, cell->value-1);
		}
	}
	return EXIT_SUCCESS;
}

int value_allowed(const cell_t *cell, unsigned long index) {
	return allowed_in_rows[cell->row][index] == grid_size && allowed_in_columns[cell->column][index] == grid_size && allowed_in_blocks[cell->block][index] == grid_size;
}

void set_cell(cell_t *set, unsigned long index) {
cell_t *cell;
	set->value = index+1;
	set->right->left = set->left;
	set->left->right = set->right;
	allowed_in_rows[set->row][index] = set->number;
	allowed_in_columns[set->column][index] = set->number;
	allowed_in_blocks[set->block][index] = set->number;
	for (cell = cells->right; cell != cells; cell = cell->right) {
		if (constraint_propagated(cell, index, set->number)) {
			cell->allowed--;
		}
	}
}

void unset_cell(cell_t *unset, unsigned long index) {
cell_t *cell;
	for (cell = cells->right; cell != cells; cell = cell->right) {
		if (constraint_propagated(cell, index, unset->number)) {
			cell->allowed++;
		}
	}
	allowed_in_blocks[unset->block][index] = grid_size;
	allowed_in_columns[unset->column][index] = grid_size;
	allowed_in_rows[unset->row][index] = grid_size;
	unset->left->right = unset->right->left = unset;
	unset->value = 0;
}

int constraint_propagated(cell_t *cell, unsigned long index, unsigned long number) {
int propagated = 0;
	if (allowed_in_rows[cell->row][index] == number) {
		propagated++;
	}
	else if (allowed_in_rows[cell->row][index] < grid_size) {
		return 0;
	}
	if (allowed_in_columns[cell->column][index] == number) {
		propagated++;
	}
	else if (allowed_in_columns[cell->column][index] < grid_size) {
		return 0;
	}
	if (allowed_in_blocks[cell->block][index] == number) {
		propagated++;
	}
	else if (allowed_in_blocks[cell->block][index] < grid_size) {
		return 0;
	}
	return propagated;
}

void search(void) {
unsigned long i;
cell_t *cell_min, *cell;
	cost++;
	if (cells->right == cells) {
		solutions++;
		print_grid();
	}
	else {
		cell_min = cells->right;
		for (cell = cell_min->right; cell != cells; cell = cell->right) {
			if (cell->allowed < cell_min->allowed) {
				cell_min = cell;
			}
		}
		if (cell_min->allowed) {
			for (i = 0; i < line_size; i++) {
				if (value_allowed(cell_min, i)) {
					set_cell(cell_min, i);
					search();
					unset_cell(cell_min, i);
				}
			}
		}
	}
}

void print_grid(void) {
unsigned long i, j;
cell_t *cell;
	puts("");
	for (i = 0, cell = cells+1; i < line_size; i++) {
		printf("%lu", cell->value);
		for (j = 1, cell++; j < line_size; j++, cell++) {
			printf(" %lu", cell->value);
		}
		puts("");
	}
}

void free_data(void) {
	free_allowed_matrix(allowed_in_blocks, line_size);
	free_allowed_matrix(allowed_in_columns, line_size);
	free_allowed_matrix(allowed_in_rows, line_size);
	free(cells);
}

void free_allowed_matrix(unsigned long **matrix, unsigned long size) {
unsigned long i;
	if (matrix) {
		for (i = 0; i < size; i++) {
			free(matrix[i]);
		}
		free(matrix);
	}
}

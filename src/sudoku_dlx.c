#include <stdio.h>
#include <stdlib.h>

#define BASE_SIZE_MIN 1UL
#define BASE_SIZE_MAX 16UL
#define CONSTRAINTS_PER_ROW 4UL

typedef struct cell_s cell_t;
struct cell_s {
	unsigned long value;
	unsigned long row;
	unsigned long column;
	unsigned long block;
};

typedef struct node_s node_t;
struct node_s {
	node_t *right;
	node_t *down;
	node_t *left;
	node_t *top;
	unsigned long rows_or_value;
	cell_t *cell;
	node_t *column;
};

void init_column(node_t *, node_t *);
void init_rows(unsigned long, unsigned long);
void init_cell_rows(unsigned long, unsigned long, unsigned long, unsigned long);
void init_row_node(node_t *, node_t **, unsigned long, cell_t *, node_t *);
void link_left(node_t *, node_t *);
void link_top(node_t *, node_t *);
int solve_grid(void);
int set_cell(cell_t *, node_t *);
void unset_cell(cell_t *, node_t *);
void search(void);
void cover_column(node_t *);
void uncover_column(node_t *);
void print_grid(void);

unsigned long base_size, line_size, grid_size, cost, solutions;
cell_t *cells, *cells_out;
node_t *nodes, *nodes_out, **tops, *header, *row_node;

int main(void) {
int exit_code;
unsigned long columns, grids, i;
cell_t *cell;
node_t *node, **top;
	if (scanf("%lu", &base_size) != 1 || base_size < BASE_SIZE_MIN || base_size > BASE_SIZE_MAX) {
		fprintf(stderr, "Base size must lie between %lu and %lu\n", BASE_SIZE_MIN, BASE_SIZE_MAX);
		return EXIT_FAILURE;
	}
	line_size = base_size*base_size;
	grid_size = line_size*line_size;
	cells = malloc(sizeof(cell_t)*grid_size);
	if (!cells) {
		fprintf(stderr, "Error allocating memory for cells\n");
		return EXIT_FAILURE;
	}
	cells_out = cells+grid_size;
	for (i = 0, cell = cells; i < grid_size; i++, cell++) {
		cell->row = i/line_size;
		cell->column = i%line_size;
		cell->block = cell->row/base_size*base_size+cell->column/base_size;
	}
	columns = grid_size*CONSTRAINTS_PER_ROW;
	nodes = malloc(sizeof(node_t)*(columns+1+grid_size*line_size*CONSTRAINTS_PER_ROW));
	if (!nodes) {
		fprintf(stderr, "Error allocating memory for nodes\n");
		free(cells);
		return EXIT_FAILURE;
	}
	nodes_out = nodes+grid_size;
	tops = malloc(sizeof(node_t *)*columns);
	if (!tops) {
		fprintf(stderr, "Error allocating memory for tops\n");
		free(nodes);
		free(cells);
		return EXIT_FAILURE;
	}
	header = nodes+columns;
	init_column(nodes, header);
	for (node = nodes, top = tops; node < header; node++, top++) {
		init_column(node+1, node);
		*top = node;
	}
	row_node = header+1;
	init_rows(grid_size*2, grid_size*3);
	for (node = nodes, top = tops; node < header; node++, top++) {
		link_top(node, *top);
	}
	if (scanf("%lu", &grids) != 1 || !grids) {
		fprintf(stderr, "Number of grids must be greater than 0\n");
		free(tops);
		free(nodes);
		free(cells);
		return EXIT_FAILURE;
	}
	exit_code = solve_grid();
	for (i = 1; i < grids && exit_code == EXIT_SUCCESS; i++) {
		exit_code = solve_grid();
	}
	free(tops);
	free(nodes);
	free(cells);
	return exit_code;
}

void init_column(node_t *node, node_t *left) {
	node->rows_or_value = line_size;
	link_left(node, left);
}

void init_rows(unsigned long column_offset, unsigned long block_offset) {
unsigned long i;
cell_t *cell;
	for (i = 0, cell = cells; i < grid_size; i++, cell++) {
		init_cell_rows(i, grid_size+cell->row*line_size, column_offset+cell->column*line_size, block_offset+cell->block*line_size);
	}
}

void init_cell_rows(unsigned long cell_index, unsigned long row_index, unsigned long column_index, unsigned long block_index) {
unsigned long i;
	for (i = 0; i < line_size; i++) {
		init_row_node(row_node+3, tops+cell_index, i+1, cells+cell_index, nodes+cell_index);
		init_row_node(row_node-1, tops+row_index+i, i+1, cells+cell_index, nodes+row_index+i);
		init_row_node(row_node-1, tops+column_index+i, i+1, cells+cell_index, nodes+column_index+i);
		init_row_node(row_node-1, tops+block_index+i, i+1, cells+cell_index, nodes+block_index+i);
	}
}

void init_row_node(node_t *left, node_t **top, unsigned long value, cell_t *cell, node_t *column) {
	link_left(row_node, left);
	link_top(row_node, *top);
	row_node->rows_or_value = value;
	row_node->cell = cell;
	row_node->column = column;
	*top = row_node++;
}

void link_left(node_t *node, node_t *left) {
	node->left = left;
	left->right = node;
}

void link_top(node_t *node, node_t *top) {
	node->top = top;
	top->down = node;
}

int solve_grid(void) {
int exit_code = set_cell(cells, nodes);
cell_t *cell;
node_t *node;
	for (cell = cells+1, node = nodes+1; cell < cells_out && exit_code == EXIT_SUCCESS; cell++, node++) {
		exit_code = set_cell(cell, node);
	}
	if (exit_code == EXIT_SUCCESS) {
		print_grid();
		solutions = cost = 0;
		search();
		printf("\nCost %lu\nSolutions %lu\n", cost, solutions);
		for (cell = cells_out-1, node = nodes_out-1; cell >= cells; cell--, node--) {
			unset_cell(cell, node);
		}
	}
	return exit_code;
}

int set_cell(cell_t *cell, node_t *column) {
node_t *row, *node;
	if (scanf("%lu", &cell->value) != 1 || cell->value > line_size) {
		fprintf(stderr, "Bad value in R%luC%lu\n", cell->row+1, cell->column+1);
		return EXIT_FAILURE;
	}
	if (cell->value) {
		cover_column(column);
		for (row = column->down; row != column && row->rows_or_value != cell->value; row = row->down);
		if (row != column) {
			for (node = row->right; node != row; node = node->right) {
				cover_column(node->column);
			}
		}
		else {
			fprintf(stderr, "Duplicate value in R%luC%lu\n", cell->row+1, cell->column+1);
			return EXIT_FAILURE;
		}
	}
	return EXIT_SUCCESS;
}

void unset_cell(cell_t *cell, node_t *column) {
node_t *row, *node;
	if (cell->value) {
		for (row = column->down; row != column && row->rows_or_value != cell->value; row = row->down);
		if (row != column) {
			for (node = row->left; node != row; node = node->left) {
				uncover_column(node->column);
			}
		}
		uncover_column(column);
		cell->value = 0;
	}
}

void search(void) {
node_t *column, *node, *row;
	cost++;
	if (header->right == header) {
		solutions++;
		print_grid();
	}
	else {
		column = header->right;
		for (node = column->right; node != header; node = node->right) {
			if (node->rows_or_value < column->rows_or_value) {
				column = node;
			}
		}
		cover_column(column);
		for (row = column->down; row != column; row = row->down) {
			row->cell->value = row->rows_or_value;
			for (node = row->right; node != row; node = node->right) {
				cover_column(node->column);
			}
			search();
			for (node = row->left; node != row; node = node->left) {
				uncover_column(node->column);
			}
			row->cell->value = 0;
		}
		uncover_column(column);
	}
}

void cover_column(node_t *column) {
node_t *row, *node;
	column->right->left = column->left;
	column->left->right = column->right;
	for (row = column->down; row != column; row = row->down) {
		for (node = row->right; node != row; node = node->right) {
			node->column->rows_or_value--;
			node->down->top = node->top;
			node->top->down = node->down;
		}
	}
}

void uncover_column(node_t *column) {
node_t *row, *node;
	for (row = column->top; row != column; row = row->top) {
		for (node = row->left; node != row; node = node->left) {
			node->top->down = node->down->top = node;
			node->column->rows_or_value++;
		}
	}
	column->left->right = column->right->left = column;
}

void print_grid(void) {
unsigned long i, j;
cell_t *cell;
	puts("");
	for (i = 0, cell = cells; i < line_size; i++) {
		printf("%lu", cell->value);
		for (j = 1, cell++; j < line_size; j++, cell++) {
			printf(" %lu", cell->value);
		}
		puts("");
	}
}

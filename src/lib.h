#define INFINITY 999999
#define GRID_SIZE 10

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

typedef enum Surface
{
  A, B, C
} Surface;

typedef struct Coordinates
{
  int x;
  int y;
} Coordinates;

typedef struct Node
{
  struct Coordinates coordinates;
  double g;
  double h;
  double f;
  struct Node * parent;
} Node;

void populate_grid(int [GRID_SIZE][GRID_SIZE]);

Node * find_path(Node * start, Node * end, int costs [GRID_SIZE][GRID_SIZE]);

Node * create_node(Coordinates coordinates, Node* parent, double g, double h);

double calculate_heuristic_cost(Coordinates current, Coordinates end);

void delete_node(Node * nodes[], int * length, int index);

void print_grid(int [GRID_SIZE][GRID_SIZE]);

void print_path(Node * path);

void print_colored();

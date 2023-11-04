#define INF 999999
#define GRID_SIZE 100

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

typedef enum Method
{
    SHORTEST = 0,
    FASTEST = 1,
    SAFEST = 2
} Method;

typedef enum Surface
{
    ROAD = 1,
    GRASS = 5,
    CITY = 10,
    FORREST = 20,
    WATER = 100,
    LANDMINE = INF
} Surface;

typedef struct Coordinates
{
    int x;
    int y;
} Coordinates;

typedef struct Node
{
    struct Coordinates coordinates;
    long double g;
    long double h;
    long double f;
    struct Node * parent;
} Node;

int * init_grid(Method method);

Node * find_path(Node * start, Node * end, const int * grid);

Node * create_node(Coordinates coordinates, Node* parent, long double g, long double h);

double calculate_heuristic_cost(Coordinates current, Coordinates end);

void delete_node(Node * nodes[], int * length, int index);

void print_grid(const int * grid);

void print_path(Node * path);

void print_colored();

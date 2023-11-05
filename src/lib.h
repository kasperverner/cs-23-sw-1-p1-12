#define INF 999999
#define GRID_SIZE 20

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
    LANDMINE = INF,
    START = 101,
    END = 102
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

Surface random_surface();

Surface * generate_surface_map();

int * generate_costs_map(const Surface * surface_map, Method method);

Node * find_path(Node * start, Node * end, const int * costs_map);

Node * create_node(Coordinates coordinates, Node* parent, long double g, long double h);

double calculate_heuristic_cost(Coordinates current, Coordinates end);

void delete_node(Node * nodes[], int * length, int index);

void add_path_to_surface_map(Node * start, Node * end, Node * path, Surface * surface_map);

void print_surface_map(const Surface * surface_map);

void print_surface_node(Surface surface);

void print_costs_map(const int * costs_map);

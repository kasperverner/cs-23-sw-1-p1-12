#define INF 999999
#define GRID_SIZE 40
#define DANGER_ZONE_RADIUS 4

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

typedef enum SquareMode
{
    TRANSPARENT = 0,
    OPAQUE = 1,
    SOLID = 2,
} SquareMode;

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

int * generate_costs_map(const Surface * surface_map, Method method, char only_danger_zones);

void add_danger_zones(int * costs_map);

Node * find_path(Node * start, Node * end, const int * costs_map);

int find_next_node(Node * open_nodes[], int open_nodes_length, Node * current_node);

void close_open_node(Node * open_nodes[], int * open_nodes_length, Node * closed_nodes[], int * closed_nodes_length, Node * current_node, int current_node_index);

void add_node_neighbours_to_open_nodes(Node * open_nodes[], int * open_nodes_length, Node * closed_nodes[], int closed_nodes_length, Node * current_node, Node * end, const int * costs_map);

char coordinates_are_closed(Node * node, Node * closed_nodes[], int closed_nodes_length);

char coordinates_are_open(Node * node, Node * open_nodes[], int open_nodes_length);

Node * create_node(Coordinates coordinates, Node* parent, long double g, long double h);

double calculate_heuristic_cost(Coordinates current, Coordinates end);

int coordinate_is_match(Coordinates a, Coordinates b);

void add_path_to_surface_map(Node * start, Node * end, Node * path, Surface * surface_map);

void print_surface_map(const Surface * surface_map);

void print_surface_node(Surface surface);

void print_costs_map(const int * costs_map);

void print_square(SquareMode mode);

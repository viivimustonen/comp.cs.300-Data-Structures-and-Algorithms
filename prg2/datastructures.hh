// Datastructures.hh
//
// Student name: Viivi Mustonen
// Student email: viivi.mustonen@tuni.fi
// Student number: H293452

#ifndef DATASTRUCTURES_HH
#define DATASTRUCTURES_HH

#include <string>
#include <vector>
#include <tuple>
#include <utility>
#include <limits>
#include <functional>
#include <exception>

// Types for IDs
using TownID = std::string;
using Name = std::string;

// Return values for cases where required thing was not found
TownID const NO_TOWNID = "----------";

// Return value for cases where integer values were not found
int const NO_VALUE = std::numeric_limits<int>::min();

// Return value for cases where name values were not found
Name const NO_NAME = "!!NO_NAME!!";

// Type for a coordinate (x, y)
struct Coord
{
    int x = NO_VALUE;
    int y = NO_VALUE;
};

// Example: Defining == and hash function for Coord so that it can be used
// as key for std::unordered_map/set, if needed
inline bool operator==(Coord c1, Coord c2) { return c1.x == c2.x && c1.y == c2.y; }
inline bool operator!=(Coord c1, Coord c2) { return !(c1==c2); } // Not strictly necessary

struct CoordHash
{
    std::size_t operator()(Coord xy) const
    {
        auto hasher = std::hash<int>();
        auto xhash = hasher(xy.x);
        auto yhash = hasher(xy.y);
        // Combine hash values (magic!)
        return xhash ^ (yhash + 0x9e3779b9 + (xhash << 6) + (xhash >> 2));
    }
};

// Example: Defining < for Coord so that it can be used
// as key for std::map/set
inline bool operator<(Coord c1, Coord c2)
{
    if (c1.y < c2.y) { return true; }
    else if (c2.y < c1.y) { return false; }
    else { return c1.x < c2.x; }
}

// Return value for cases where coordinates were not found
Coord const NO_COORD = {NO_VALUE, NO_VALUE};

// Type for a distance (in metres)
using Distance = int;

// Return value for cases where Distance is unknown
Distance const NO_DISTANCE = NO_VALUE;

// This exception class is there just so that the user interface can notify
// about operations which are not (yet) implemented
class NotImplemented : public std::exception
{
public:
    NotImplemented() : msg_{} {}
    explicit NotImplemented(std::string const& msg) : msg_{msg + " not implemented"} {}

    virtual const char* what() const noexcept override
    {
        return msg_.c_str();
    }
private:
    std::string msg_;
};

const float TEN_PERCENT = 0.1;

enum node {WHITE, GRAY, BLACK}; // not found, found, processed

struct Road {

    std::pair<TownID, TownID> roadID_;
    TownID stringID = NO_TOWNID;
    Distance distance = NO_DISTANCE;
};

struct Town
{
    TownID id_;
    Name name_;
    Coord location_;
    int tax_;
    TownID mastertown_ = NO_TOWNID;
    std::vector<TownID> vassaltowns_ = {};

    std::vector<Road> roads_ = {};
    node state = WHITE;
    Town* lastTown = nullptr;
    int searchDepth = 0;
};




// This is the class you are supposed to implement

class Datastructures
{
public:
    Datastructures();
    ~Datastructures();

    // Estimate of performance:
    // Short rationale for estimate:
    unsigned int town_count();

    // Estimate of performance:O(n²)
    // Short rationale for estimate: For-loop and std::clear complexities are
    //linear to size, so estimated performance O(n²).
    void clear_all();

    // Estimate of performance:
    // Short rationale for estimate:
    bool add_town(TownID id, Name const& name, Coord coord, int tax);

    // Estimate of performance:
    // Short rationale for estimate:
    Name get_town_name(TownID id);

    // Estimate of performance:
    // Short rationale for estimate:
    Coord get_town_coordinates(TownID id);

    // Estimate of performance:
    // Short rationale for estimate:
    int get_town_tax(TownID id);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<TownID> all_towns();

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<TownID> find_towns(Name const& name);

    // Estimate of performance:
    // Short rationale for estimate:
    bool change_town_name(TownID id, Name const& newname);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<TownID> towns_alphabetically();

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<TownID> towns_distance_increasing();

    // Estimate of performance:
    // Short rationale for estimate:
    TownID min_distance();

    // Estimate of performance:
    // Short rationale for estimate:
    TownID max_distance();

    // Estimate of performance:
    // Short rationale for estimate:
    bool add_vassalship(TownID vassalid, TownID masterid);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<TownID> get_town_vassals(TownID id);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<TownID> taxer_path(TownID id);

    // Non-compulsory phase 1 operations

    // Estimate of performance:
    // Short rationale for estimate:
    bool remove_town(TownID id);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<TownID> towns_nearest(Coord coord);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<TownID> longest_vassal_path(TownID id);

    // Estimate of performance:
    // Short rationale for estimate:
    int total_net_tax(TownID id);


    // Phase 2 operations

    // Estimate of performance: O(n²)
    // Short rationale for estimate: For-loop and std::clear complexities are
    //linear to size, so estimated performance O(n²).
    void clear_roads();

    // Estimate of performance: O(n log(n))
    // Short rationale for estimate: std::sort complexity is O(n log(n))
    std::vector<std::pair<TownID, TownID>> all_roads();

    // Estimate of performance: O(n) ~ Theta(1)
    // Short rationale for estimate: In the worst case std::unordered_map::insert
    //complexity is linear in container size. On average complexity is constant.
    bool add_road(TownID town1, TownID town2);

    // Estimate of performance: O(n)
    // Short rationale for estimate: For-loop complexity is linear to size
    std::vector<TownID> get_roads_from(TownID id);

    // Estimate of performance: O(n²)
    // Short rationale for estimate: any_route calls DFS-function whose
    //complexity is O(n²) (due to nested loops).
    std::vector<TownID> any_route(TownID fromid, TownID toid);

    // Non-compulsory phase 2 operations

    // Estimate of performance: O(n)
    // Short rationale for estimate: In worst case for-loop goes through all
    //elements
    bool remove_road(TownID town1, TownID town2);

    // Estimate of performance: O(n²)
    // Short rationale for estimate:least_towns_route calls BFS-function whose
    //complexity is O(n²) (due to nested loops).
    std::vector<TownID> least_towns_route(TownID fromid, TownID toid);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<TownID> road_cycle_route(TownID startid);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<TownID> shortest_route(TownID fromid, TownID toid);

    // Estimate of performance:
    // Short rationale for estimate:
    Distance trim_road_network();

private:
    // Add stuff needed for your class implementation here
    std::unordered_map<TownID, Town> towns_;

    int count_distance(Coord town1, Coord town2);
    std::vector<TownID> find_master_town(TownID id, std::vector<TownID> masterTowns);
    void find_vassal_town(TownID id, std::vector<TownID>& vassalTowns, std::vector<TownID>& longestPath);
    float find_town_tax(TownID id);

    Coord origo_;

    std::unordered_map<TownID, Road> roads;

    void DFS(Town* from, Town* to);
    void BFS(Town* from, Town* to);

    std::vector<TownID> foundRoute;

};

#endif // DATASTRUCTURES_HH

// Datastructures.cc
//
// Student name: Viivi Mustonen
// Student email: viivi.mustonen@tuni.fi
// Student number: H293452

#include "datastructures.hh"

#include <random>

#include <cmath>

#include <stack>

#include <list>

#include <queue>

#include <QDebug>


std::minstd_rand rand_engine; // Reasonably quick pseudo-random generator

template <typename Type>
Type random_in_range(Type start, Type end)
{
    auto range = end-start;
    ++range;

    auto num = std::uniform_int_distribution<unsigned long int>(0, range-1)(rand_engine);

    return static_cast<Type>(start+num);
}

// Modify the code below to implement the functionality of the class.
// Also remove comments from the parameter names when you implement
// an operation (Commenting out parameter name prevents compiler from
// warning about unused parameters on operations you haven't yet implemented.)

Datastructures::Datastructures()
{
    // Write any initialization you need here
    origo_.x = 0;
    origo_.y = 0;
}

Datastructures::~Datastructures()
{
    // Write any cleanup you need here
}

unsigned int Datastructures::town_count()
{
    return towns_.size();
}

void Datastructures::clear_all()
{
    for(auto& iter : towns_){
        iter.second.roads_.clear();
    }
    roads.clear();
    towns_.clear();
}

bool Datastructures::add_town(TownID id, const Name &name, Coord coord, int tax)
{
    Town newTown = {id, name, coord, tax};
    std::pair<std::unordered_map<TownID, Town>::iterator, bool> insertCheck;
    insertCheck = towns_.insert({id, newTown});
    return insertCheck.second;
}

Name Datastructures::get_town_name(TownID id)
{
    if(towns_.find(id) == towns_.end()){
        return NO_NAME;
    }
    return towns_.at(id).name_;
}

Coord Datastructures::get_town_coordinates(TownID id)
{
    if(towns_.find(id) == towns_.end()){
        return NO_COORD;
    }
    return towns_.at(id).location_;
}

int Datastructures::get_town_tax(TownID id)
{
    if(towns_.find(id) == towns_.end()){
           return NO_VALUE;
    }
    return towns_.at(id).tax_;
}

std::vector<TownID> Datastructures::all_towns()
{
    std::vector<TownID> allTowns = {};
    for(auto& iter : towns_){
        allTowns.push_back(iter.first);
    }
    return allTowns;
}

std::vector<TownID> Datastructures::find_towns(const Name &name)
{
    std::vector<TownID> sameNameTowns = {};
    for(auto& iter : towns_){
        if(iter.second.name_ == name){
            sameNameTowns.push_back(iter.first);
        }
    }
    return sameNameTowns;
}

bool Datastructures::change_town_name(TownID id, const Name &newname)
{
    if(towns_.find(id) != towns_.end()){
        towns_.at(id).name_ = newname;
        return true;
    }
    return false;
}

std::vector<TownID> Datastructures::towns_alphabetically()
{
    std::vector<std::pair<TownID, Name>> pairVector = {};
    std::vector<TownID> sortedTowns = {};

    for(auto& iter : towns_){
        pairVector.push_back({iter.first, iter.second.name_});
    }
    std::sort(pairVector.begin(), pairVector.end(),
              [](std::pair<TownID, Name>& a, std::pair<TownID, Name>& b)
                { return a.second < b.second;});

    for(auto& iter : pairVector){
        sortedTowns.push_back(iter.first);
    }
    return sortedTowns;
}

std::vector<TownID> Datastructures::towns_distance_increasing()
{
    std::vector<std::pair<TownID, int>> pairVector = {};
    std::vector<TownID> sortedTowns = {};

    for(auto& iter : towns_){
        int distance = count_distance(iter.second.location_, origo_);
        pairVector.push_back({iter.first, distance});
    }
    std::sort(pairVector.begin(), pairVector.end(),
              [](std::pair<TownID, int>& a, std::pair<TownID, int>& b)
                { return a.second < b.second;});

    for(auto& iter : pairVector){
        sortedTowns.push_back(iter.first);
    }
    return sortedTowns;
}

TownID Datastructures::min_distance()
{
    if(towns_.size() == 0){
        return NO_TOWNID;
    }
    std::vector<std::pair<TownID, int>> pairVector = {};
    for(auto& iter : towns_){
        int distance = count_distance(iter.second.location_, origo_);
        pairVector.push_back({iter.first, distance});
    }
    std::sort(pairVector.begin(), pairVector.end(),
              [](std::pair<TownID, int>& a, std::pair<TownID, int>& b)
                { return a.second < b.second;});
    return pairVector.at(0).first;
}

TownID Datastructures::max_distance()
{
    if(towns_.size() == 0){
        return NO_TOWNID;
    }
    std::vector<std::pair<TownID, int>> pairVector = {};
    for(auto& iter : towns_){
        int distance = count_distance(iter.second.location_, origo_);
        pairVector.push_back({iter.first, distance});
    }
    std::sort(pairVector.begin(), pairVector.end(),
              [](std::pair<TownID, int>& a, std::pair<TownID, int>& b)
                { return a.second > b.second;});
    return pairVector.at(0).first;
}

bool Datastructures::add_vassalship(TownID vassalid, TownID masterid)
{
    if(towns_.find(vassalid) == towns_.end() or
            towns_.find(masterid) == towns_.end() or
            towns_.at(vassalid).mastertown_ != NO_TOWNID){
           return false;
     }
    towns_.at(masterid).vassaltowns_.push_back(vassalid);
    towns_.at(vassalid).mastertown_ = masterid;
    return true;
}

std::vector<TownID> Datastructures::get_town_vassals(TownID id)
{
    if(towns_.find(id) == towns_.end()){
        return {NO_TOWNID};
    }
    return towns_.at(id).vassaltowns_;
}

std::vector<TownID> Datastructures::taxer_path(TownID id)
{
    std::vector<TownID> taxerTowns = {};
    if(towns_.find(id) == towns_.end()){
        return {NO_TOWNID};
    }
    return find_master_town(id, taxerTowns);
}

bool Datastructures::remove_town(TownID id)
{
    if(towns_.find(id) == towns_.end()){
        return false;
    }
    TownID masterTown = towns_.at(id).mastertown_;

    if(masterTown != NO_TOWNID){
        if(towns_.at(id).vassaltowns_.size() != 0){
            for(auto& iter : towns_.at(id).vassaltowns_){
                //Change new mastertown
                towns_.at(iter).mastertown_ = masterTown;
                //Put new vassals to mastertown's vassals
                towns_.at(masterTown).vassaltowns_.push_back(iter);
            }
        }
        int it = 0;
        //Find town to be removed from vassaltowns-vector
        for(auto& iter : towns_.at(masterTown).vassaltowns_){
            if(iter == id){
                break;
            }
            it++;
        }
        towns_.at(masterTown).vassaltowns_.erase( towns_.at(masterTown).vassaltowns_.begin() + it);
    } else {
        for(auto& iter : towns_.at(id).vassaltowns_){
            towns_.at(iter).mastertown_ = NO_TOWNID;
        }
    }
    towns_.erase(id);
    return true;
}

std::vector<TownID> Datastructures::towns_nearest(Coord coord)
{
    std::vector<std::pair<TownID, int>> pairVector = {};
    std::vector<TownID> nearestTowns = {};

    for(auto& iter : towns_){
        int distance = count_distance(iter.second.location_, coord);
        pairVector.push_back({iter.first, distance});
    }

    std::sort(pairVector.begin(), pairVector.end(),
              [](std::pair<TownID, int>& a, std::pair<TownID, int>& b)
                { return a.second < b.second;});

    for(auto& iter : pairVector){
        nearestTowns.push_back(iter.first);
    }
    return nearestTowns;
}

std::vector<TownID> Datastructures::longest_vassal_path(TownID id)
{
    std::vector<TownID> vassalPath = {};
    if(towns_.find(id) == towns_.end()){
        return {NO_TOWNID};
    }
    std::vector<TownID> longestPath = {};
    find_vassal_town(id, vassalPath, longestPath);
    return longestPath;
}

int Datastructures::total_net_tax(TownID id)
{
    if(towns_.find(id) == towns_.end()){
           return NO_VALUE;
    }
    int townTax = 0;
    townTax = floor(find_town_tax(id));

    if(towns_.at(id).mastertown_ != NO_TOWNID and towns_.at(id).vassaltowns_.size() != 0){
        return townTax - floor(TEN_PERCENT * townTax);
    }else if (towns_.at(id).mastertown_ != NO_TOWNID){
        return townTax - floor(townTax * TEN_PERCENT);
    }
    return townTax;
}

int Datastructures::count_distance(Coord town1, Coord town2)
{
    return floor(sqrt(pow((town1.x - town2.x),2)+pow((town1.y - town2.y), 2)));
}

std::vector<TownID> Datastructures::find_master_town(TownID id, std::vector<TownID> masterTowns)
{
    if(towns_.find(id) == towns_.end()){
            return masterTowns;
    }
    masterTowns.push_back(id);
    return find_master_town(towns_.at(id).mastertown_, masterTowns);
}

void Datastructures::find_vassal_town(TownID id, std::vector<TownID>& vassalTowns, std::vector<TownID>& longestPath)
{
    vassalTowns.push_back(id);
    if(towns_.at(id).vassaltowns_.empty()){
        if(vassalTowns.size() > longestPath.size()){
            longestPath = vassalTowns;
            vassalTowns = {};
            return;
        }
    }
    for(TownID& iter : towns_.at(id).vassaltowns_){
        find_vassal_town(iter, vassalTowns, longestPath);
    }
}

float Datastructures::find_town_tax(TownID id)
{
    float currentIncome = 0;
    if(towns_.at(id).vassaltowns_.empty()){
        return towns_.at(id).tax_;
    }

    currentIncome += towns_.at(id).tax_;
    for(TownID& iter : towns_.at(id).vassaltowns_){
        currentIncome += TEN_PERCENT * find_town_tax(iter);
    }
    return currentIncome;
}

//
// Phase 2 operations
//

void Datastructures::DFS(Town *from, Town *to)
{
    std::stack<Town*> stack;
    bool routeFound = false;
    Town* currentTown;
    stack.push(to);

    while(stack.size() > 0 and routeFound == false){
        currentTown = stack.top();
        stack.pop();

        if(currentTown->state == WHITE){
            if(currentTown == from){
                routeFound = true;
                break;
            }
            currentTown->state = GRAY;
            stack.push(currentTown);
            for(auto& iter : currentTown->roads_){
                Town* nextTown;
                //find next town
                if(currentTown->id_ == iter.roadID_.first){
                    nextTown = &towns_.at(iter.roadID_.second);
                }else{
                    nextTown = &towns_.at(iter.roadID_.first);
                }

                if(nextTown->state == WHITE){
                    stack.push(nextTown);
                    nextTown->lastTown = currentTown;
                }
            }
        }else{
            currentTown->state = BLACK;
        }
    }

    if(routeFound){
        currentTown = from;
        TownID townId;

        while(townId != to->id_){
            townId = currentTown->id_;

            foundRoute.push_back(townId);
            if(townId != to->id_){
                currentTown = currentTown->lastTown;
            }
        }
    }
}

void Datastructures::BFS(Town* from, Town* to)
{
    std::list<Town*> queue;

    bool routeFound = false;
    Town* currentTown;
    queue.push_back(to);

    while(queue.size() > 0 and routeFound == false){
        currentTown = queue.front();
        queue.pop_front();

        if(currentTown->state == WHITE){
            if(currentTown == from){
                routeFound = true;
                break;
            }
            currentTown->state = GRAY;
            queue.push_back(currentTown);

            for(auto& iter : currentTown->roads_){
                Town* nextTown;
                //find next town
                if(currentTown->id_ == iter.roadID_.first){
                    nextTown = &towns_.at(iter.roadID_.second);
                }else{
                    nextTown = &towns_.at(iter.roadID_.first);
                }

                if(nextTown->state == WHITE){
                    queue.push_back(nextTown);
                    //check if less nodes to next town
                    if(nextTown->searchDepth != 0 and  nextTown->searchDepth
                            < currentTown->searchDepth+1){
                        continue;
                    }else{
                        nextTown->searchDepth = currentTown->searchDepth +1;
                        nextTown->lastTown = currentTown;
                    }
                }
            }
        } else{
            currentTown->state = BLACK;
        }
    }


    if(routeFound){
        currentTown = from;
        TownID townId;

        while(townId != to->id_){
            townId = currentTown->id_;

            foundRoute.push_back(townId);
            if(townId != to->id_){
                currentTown = currentTown->lastTown;
            }
        }
    }
}


void Datastructures::clear_roads()
{
    for(auto& iter : towns_){
        iter.second.roads_.clear();
    }
    roads.clear();
}

std::vector<std::pair<TownID, TownID>> Datastructures::all_roads()
{
    std::vector<std::pair<TownID, TownID>> allRoads = {};

    for(auto& iter : roads){
        allRoads.push_back(iter.second.roadID_);
    }

    std::sort(allRoads.begin(), allRoads.end());

    return allRoads;
}

bool Datastructures::add_road(TownID town1, TownID town2)
{
    if(towns_.find(town1) == towns_.end() or towns_.find(town2) == towns_.end()){
           return false;
       }

    std::pair<TownID, TownID> id;

    if(town1 < town2){
        id.first =town1;
        id.second =town2;
    }else{
        id.first = town2;
        id.second = town1;
    }

    int distance = count_distance(towns_.at(town1).location_, towns_.at(town2).location_);

    Road newRoad = {id, id.first + id.second, distance};

    towns_.at(town1).roads_.push_back(newRoad);
    towns_.at(town2).roads_.push_back(newRoad);

    std::pair<std::unordered_map<TownID, Road>::iterator, bool> insertCheck;
    insertCheck = roads.insert({id.first + id.second, newRoad});
    return insertCheck.second;
}

std::vector<TownID> Datastructures::get_roads_from(TownID id)
{
    std::vector<TownID> foundRoads = {};

    if(towns_.find(id) == towns_.end()) {
        return {NO_TOWNID};
    }

    if(towns_.at(id).roads_.size() == 0) {
        return {};
    }

    for(auto& road : towns_.at(id).roads_) {
        if(road.roadID_.first == id) foundRoads.push_back(road.roadID_.second);
        else foundRoads.push_back(road.roadID_.first);
    }

    return foundRoads;
}

std::vector<TownID> Datastructures::any_route(TownID fromid, TownID toid)
{
    foundRoute.clear();
    if(towns_.find(fromid) == towns_.end() or towns_.find(toid) == towns_.end()){
        return {NO_TOWNID};
    }

    for(auto& iter : towns_){
        iter.second.state = WHITE;
    }
    DFS(&towns_.at(fromid), &towns_.at(toid));

    if(foundRoute.size() != 0){
        return foundRoute;
    }else{
        return {};
    }
}

bool Datastructures::remove_road(TownID town1, TownID town2)
{
    if(towns_.find(town1) == towns_.end() or towns_.find(town2) == towns_.end()) {
        return false;
    }
    TownID roadId;
    if(town1 < town2){
        roadId = town1 + town2;
    } else{
        roadId = town2 + town1;
    }
    if(roads.find(roadId) == roads.end()){
        return false;
    }

    //Road was found
    int it = 0;
    for(auto& iter : towns_.at(town1).roads_){
        if(iter.stringID == roadId){
            break;
        }
        it++;
    }
    towns_.at(town1).roads_.erase(towns_.at(town1).roads_.begin()+it);

    int i = 0;
    for(auto& it : towns_.at(town2).roads_){
        if(it.stringID == roadId){
            break;
        }
        i++;
    }
    towns_.at(town2).roads_.erase(towns_.at(town2).roads_.begin()+i);

    roads.erase(roadId);
    return true;
}

std::vector<TownID> Datastructures::least_towns_route(TownID fromid, TownID toid)
{
    foundRoute.clear();
    if(towns_.find(fromid) == towns_.end() or towns_.find(toid) == towns_.end()){
        return {NO_TOWNID};
    }
    for(auto& iter : towns_){
        iter.second.state = WHITE;
    }

    BFS(&towns_.at(fromid), &towns_.at(toid));

    if(foundRoute.size() != 0){
        return foundRoute;
    }else{
        return {};
    }
}

std::vector<TownID> Datastructures::road_cycle_route(TownID /*startid*/)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    throw NotImplemented("road_cycle_route()");
}

std::vector<TownID> Datastructures::shortest_route(TownID /*fromid*/, TownID /*toid*/)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    throw NotImplemented("shortest_route()");
}

Distance Datastructures::trim_road_network()
{
    // Replace the line below with your implementation
    throw NotImplemented("trim_road_network()");
}

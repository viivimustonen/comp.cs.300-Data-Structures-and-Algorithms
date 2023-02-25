// Datastructures.cc
//
// Student name: Viivi Mustonen
// Student email: viivi.mustonen@tuni.fi
// Student number: H293452

#include "datastructures.hh"

#include <random>

#include <cmath>


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
    origo_.x = 0;
    origo_.y = 0;
}

Datastructures::~Datastructures()
{

}

unsigned int Datastructures::town_count()
{
    return towns_.size();
}

void Datastructures::clear_all()
{
    towns_.clear();
}

bool Datastructures::add_town(TownID id, const Name &name, Coord coord, int tax)
{
    Town newTown = {name, coord, tax};
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



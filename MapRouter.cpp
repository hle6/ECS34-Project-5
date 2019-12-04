#include "MapRouter.h" 	  			 	 
#include <cmath>
#include "CSVReader.h"
#include "XMLReader.h"
#include "XMLWriter.h"
#include "CSVWriter.h"
#include "XMLEntity.h"
#include <fstream>
#include <sstream>
const CMapRouter::TNodeID CMapRouter::InvalidNodeID = -1;

CMapRouter::CMapRouter(){

}

CMapRouter::~CMapRouter(){

}

double CMapRouter::HaversineDistance(double lat1, double lon1, double lat2, double lon2){
    auto DegreesToRadians = [](double deg){return M_PI * (deg) / 180.0;};
	double LatRad1 = DegreesToRadians(lat1);
	double LatRad2 = DegreesToRadians(lat2);
	double LonRad1 = DegreesToRadians(lon1);
	double LonRad2 = DegreesToRadians(lon2);
	double DeltaLat = LatRad2 - LatRad1;
	double DeltaLon = LonRad2 - LonRad1;
	double DeltaLatSin = sin(DeltaLat/2);
	double DeltaLonSin = sin(DeltaLon/2);	
	double Computation = asin(sqrt(DeltaLatSin * DeltaLatSin + cos(LatRad1) * cos(LatRad2) * DeltaLonSin * DeltaLonSin));
	const double EarthRadiusMiles = 3959.88;
	
	return 2 * EarthRadiusMiles * Computation;
}        

double CMapRouter::CalculateBearing(double lat1, double lon1,double lat2, double lon2){
    auto DegreesToRadians = [](double deg){return M_PI * (deg) / 180.0;};
    auto RadiansToDegrees = [](double rad){return 180.0 * (rad) / M_PI;};
    double LatRad1 = DegreesToRadians(lat1);
	double LatRad2 = DegreesToRadians(lat2);
	double LonRad1 = DegreesToRadians(lon1);
	double LonRad2 = DegreesToRadians(lon2);
    double X = cos(LatRad2)*sin(LonRad2-LonRad1);
    double Y = cos(LatRad1)*sin(LatRad2)-sin(LatRad1)*cos(LatRad2)*cos(LonRad2-LonRad1);
    return RadiansToDegrees(atan2(X,Y));
}

bool CMapRouter::LoadMapAndRoutes(std::istream &osm, std::istream &stops, std::istream &routes){
    // Your code HERE
   double WalkingSpeed = 3.0;
    CXMLReader OSMReader(osm);
    while(!OSMReader.End()) {
        SXMLEntity Entity;
        if(OSMReader.ReadEntity(Entity)) {
            if(Entity.DType == SXMLEntity::EType::StartElement) {
                if(Entity.DNameData == "node") {
                    auto ID = std::stoul(Entity.AttributeValue("id"));
                    double Latitude = std::stod(Entity.AttributeValue("lat"));
                    double Longitude = std::stod(Entity.AttributeValue("lon"));
                    SNode TempNode;
                    TempNode.DNodeID = ID;
                    TempNode.DLatitude = Latitude;
                    TempNode.DLongitude = Longitude;
                    DNodeIDToNodeIndex[ID] = DNodes.size();
                    DNodes.push_back(TempNode);
                    DSortedNodeIDs.push_back(TempNode.DNodeID);

                }
                if(Entity.DNameData == "way") {
                    std::vector<TNodeIndex> TempIndices;
                    bool IsOneWay = false;
                    double MaxSpeed = 25;
                    while(Entity.DNameData != "way" or Entity.DType != SXMLEntity::EType::EndElement){
                        OSMReader.ReadEntity(Entity);

                        if(Entity.DType == SXMLEntity::EType::StartElement) {
                            if(Entity.DNameData == "nd") {
                                TNodeID TempID = std::stoul(Entity.AttributeValue("ref"));
                                auto Search = DNodeIDToNodeIndex.find(TempID);
                                if(Search != DNodeIDToNodeIndex.end()) {
                                    TempIndices.push_back(Search->second);
                                }
                            }
                            if(Entity.DNameData == "tag") {
                                if(Entity.AttributeValue("k") == "oneway") {
                                    IsOneWay = Entity.AttributeValue("v") == "yes";
                                }
                                if(Entity.AttributeValue("k") == "maxspeed") {
                                    std::stringstream TempStream(Entity.AttributeValue("v"));
                                    TempStream>>MaxSpeed;
                                }
                            }
                        }
                    }
                    for (size_t Index = 0; Index + 1 < TempIndices.size(); Index++) {
                        SEdge TempEdge;
                        auto FromNode = TempIndices[Index];
                        auto ToNode = TempIndices[Index+1];
                        TempEdge.DOtherNodeIndex = ToNode;
                        TempEdge.DDistance = HaversineDistance(DNodes[FromNode].DLatitude,DNodes[FromNode].DLongitude, DNodes[ToNode].DLatitude, DNodes[ToNode].DLongitude);
                        TempEdge.DTime = TempEdge.DDistance / WalkingSpeed;
                        TempEdge.DMaxSpeed = MaxSpeed;
                        DNodes[FromNode].DEdges.push_back(TempEdge);
                        if (!IsOneWay) {
                            TempEdge.DOtherNodeIndex = FromNode;
                            DNodes[ToNode].DEdges.push_back(TempEdge);
                        }
                    }
                }
            }
        }
   
}}

size_t CMapRouter::NodeCount() const{
    // Your code HERE
    return DNodes.size();
}

CMapRouter::TNodeID CMapRouter::GetSortedNodeIDByIndex(size_t index) const{
    // Your code HERE
}

CMapRouter::TLocation CMapRouter::GetSortedNodeLocationByIndex(size_t index) const{
    // Your code HERE
}

CMapRouter::TLocation CMapRouter::GetNodeLocationByID(TNodeID nodeid) const{
    // Your code HERE
}

CMapRouter::TNodeID CMapRouter::GetNodeIDByStopID(TStopID stopid) const{
    // Your code HERE
}

size_t CMapRouter::RouteCount() const{
    // Your code HERE
}

std::string CMapRouter::GetSortedRouteNameByIndex(size_t index) const{
    // Your code HERE
}

bool CMapRouter::GetRouteStopsByRouteName(const std::string &route, std::vector< TStopID > &stops){
    // Your code HERE
}

double CMapRouter::FindShortestPath(TNodeID src, TNodeID dest, std::vector< TNodeID > &path){
    // Your code HERE
}

double CMapRouter::FindFastestPath(TNodeID src, TNodeID dest, std::vector< TPathStep > &path){
    // Your code HERE
}

bool CMapRouter::GetPathDescription(const std::vector< TPathStep > &path, std::vector< std::string > &desc) const{
    // Your code HERE
}

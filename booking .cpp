#include<iostream>
#include<vector>
#include<unordered_map>
using namespace std;

class Map{
private:
vector<vector<pair<int,int>>>adj; // {node,distance}
 unordered_map<int,string> IdtoLocations; 
 unordered_map<string,int> LocationToId;
 public:
 // add location to the map with its id and name
 void addLocation(int id , string name){
    IdtoLocations[id] = name;
    LocationToId[name] = id;
     }
     // connecting two locations with an edge and dist between them
     void addEdge(string u , string v , int distance){
    
        int id1 = LocationToId[u];
        int id2 = LocationToId[v];
        if(adj.size() <= max(id1,id2)){
            adj.resize(max(id1,id2)+1);
        }
        adj[id1].push_back({id2,distance});
        adj[id2].push_back({id1,distance});
     }
     void printAllLocations(){
        cout << "\nAvailable Locations:\n";
        for(auto it :LocationToId){
            cout << it.first <<'\n';
        }
     }
// to get Location name from its id;
                 int getLocationId(string locn ){
                    if(LocationToId.find(locn) == LocationToId.end())return -1;
                    return  LocationToId[locn];
                 }
                 bool hasLocation( string location){
                    if(LocationToId.find(location) != LocationToId.end() )return true;
                    return false;
                 }

     // dijkstra's algo to print and get distance of the shortest path between two locations;
      void printShortestPath(string source , string  destination){
        int src = LocationToId[source];
        int dst = LocationToId[destination];
        vector<int>dist(adj.size(),INT_MAX);
        vector<int>parent(adj.size(),-1);
          dist[src] = 0;
          priority_queue< pair<int,int>,vector<pair<int,int>>,greater<pair<int,int>>>pq;
          pq.push({0,src});
        while(!pq.empty()){
            int u = pq.top().second;
            int currdist = pq.top().first;
          pq.pop();
          if(currdist > dist[u])continue;
            
             for( auto it : adj[u]){
                int v = it.first;
                int d = it.second;
                if(dist[u] + d < dist[v]){
                    dist[v] = dist[u] + d;
                    parent[v] = u;
                    pq.push({dist[v], v});
                }
             }
        }
          
       
        if(dist[dst] == INT_MAX){
            cout << "No path exists between " << source << " and " << destination << endl;
             
        }
        else{
        cout << "Shortest distance from " << source << " to " << destination << " is :" << dist[dst] <<" km" <<endl;
        cout << "Route is :" << endl;
        vector<string>path;
         int v = dst;
         
         while(v != -1){
             
            path.push_back(IdtoLocations[v]) ;
            v = parent[v];
         }
         reverse(path.begin(),path.end());
         for( int i = 0 ; i < path.size() ;i++){
             if(i == path.size()-1) cout << path[i] << endl;
            else cout << path[i] << " -> " ;
         }
         
          }
      }

         
        

        int getMinDistance(string source , string destination){
            if(LocationToId.find(source) == LocationToId.end()|| LocationToId.find(destination) == LocationToId.end()){
              
              return -1;
          }
            int src = LocationToId[source];
            int dst = LocationToId[destination];
            vector<int>dist(adj.size(),INT_MAX);
            dist[src] = 0;
            priority_queue< pair<int,int>,vector<pair<int,int>>,greater<pair<int,int>>>pq;
            pq.push({0,src});
          while(!pq.empty()){
              int u = pq.top().second;
              int currdist = pq.top().first;
              pq.pop();
            if(currdist > dist[u])continue;
              
               for( auto it : adj[u]){
                  int v = it.first;
                  int d = it.second;
                  if(dist[u] + d < dist[v]){
                      dist[v] = dist[u] + d;
                      pq.push({dist[v], v});
                  }
               }
          }  
          return dist[dst];
        }

};

 class Driver {
    public: 
    string name;
    string  location;
    int total_rides;
    int rating = 0;
     Driver( string name , string location){
      this->name = name;
      this->location = location;
      total_rides = 0;
}
     
    string getName(){
        return name;
    }
    string getLocation(){
        return location;
    }
    void setLocation(string location){
        this->location = location;
    }
      
     double avg_rating(){
       if(total_rides == 0) return 0.0;
       return (double)rating/total_rides;
     }


 };
 class CabTypes {
    public:
     string type;
     int FarePerKm;
   CabTypes(string type , int FarePerKm){
    this->type = type;
    this->FarePerKm = FarePerKm;}
    void setFarePerKm(int FarePerKm){
        this->FarePerKm = FarePerKm;
    }
 };
 class Ride{
    public:
string driver;
string pickup;
string  drop;
string cab;
int rating;
int distance;
int fare;
Ride(string & driver , string pickup , string drop , string cab ,
     int rating , int distance , int fare){
        this->driver = driver;
        this->pickup =pickup ;
        this->drop = drop;
        this->cab = cab;
        this->rating = rating;
        this->distance = distance;
        this->fare = fare;

     }

 };
class cabBooking {
    private :
   Map CityMap;
   vector<Driver>drivers;
   vector<Ride>rideHistory;
 public :
  cabBooking( Map map , vector<Driver>drivers ){
    cout<< "welcome to cab booking system " << endl;
    this->CityMap = map;
    this->drivers = drivers;
  }
  

  //find nearest driver to the pickup loc.
   Driver* findNearestDriver(string pickup){
int minDistance = INT_MAX;  
 Driver* nearestDriver = nullptr;
 for( auto  &d : drivers){
    int distance   = CityMap.getMinDistance(pickup,d.location );
    if(distance == -1 || distance == INT_MAX) continue;
    if(distance < minDistance){
        minDistance = distance;
        nearestDriver = &d;
    }
    // higher rating driver will assign
    else if(distance == minDistance && minDistance!= INT_MAX && d.avg_rating() > nearestDriver->avg_rating()){
        nearestDriver = &d ;
    }
 }
   return nearestDriver;

   }
   void bookride( string &pickup , string &drop , CabTypes cab){
    int tripdist = CityMap.getMinDistance(pickup ,drop);
    if(tripdist == -1) {
        cout << "Invalid Locations !! \n";
    return ;
    }
    if(tripdist == INT_MAX){
         cout << "Path does not exist between locations !! \n";
    return ;
    }
     
     
        Driver* nearestDriver =  findNearestDriver(pickup);
   if(nearestDriver == NULL) {
       cout<<" No driver available near pickup location .\n";
      return;
   }
    
   cout <<"\nDriver Assigned\n";
   cout <<"Name   : " <<nearestDriver->name << '\n';
   cout << "Location : " << nearestDriver->location << '\n';
   cout << "Rating : " <<  nearestDriver->avg_rating() << '\n';
    
   CityMap.printShortestPath(pickup , drop);
   int fare = tripdist*cab.FarePerKm ; 
    cout << "Fare    :"<<fare << " Rs\n";
    cout << "please give rating  (0 to 5) \n";
    int rating = -1;
    cin >> rating;
    
    if(rating >= 0 && rating <= 5){
    cout << " Thanks for rating!\n";
     nearestDriver->total_rides ++;
    nearestDriver->rating += rating;
   
}
else cout << "Invalid rating !!" ;
  rideHistory.push_back(Ride( nearestDriver->name , pickup ,drop, cab.type ,rating , tripdist,fare ));
    nearestDriver->setLocation(drop);
    cout <<" Ride booked successfully!\n" ;
   }
   void printDrivers(){
    cout << "\n===== Driver Details =====\n";
    for(auto &d : drivers){
     cout <<"Name : " << d.name << " || Location :" << d.location   
          <<"||Total Rides " << d.total_rides <<'\n';
    }
   }
   void printRideHistory(){
     cout << "\n===== Ride History =====\n";
     if(rideHistory.empty()){
        cout << "No ride booked .\n";
        return;
     }
     int i = 1;
     for( auto r : rideHistory){
       cout <<"Ride "<< i << ": \n";
       cout <<"Driver    :" << r.driver << '\n';
       cout <<"pickup    :" << r.pickup << '\n';
       cout <<"drop       :" << r.drop << '\n';
       cout <<"Cab type  :" << r.cab << '\n';
       cout <<"Distance  :" << r.distance << '\n';
       cout <<"Fare      :" << r.fare << '\n';
       if(r.rating == -1) cout << "Rating not recorded\n";
       else  cout <<"Rating    :" << r.rating << '\n';
       cout << '\n';
     }
   }
  

};
 

int main(){
    Map Citymap;
  cout << "Enter number of locations : ";
  int n ;
  cin >> n;
  cout << "enter location names : \n";
   for(int i  = 0 ; i < n ; ++i){
     string name;
     cout<<"location " << (i+1) << " :" ;
        cin >> name;
        Citymap.addLocation(i, name);
   }
   // add edges(path) between location 
     cout << "\nEnter number of roads/paths : ";
     int edges;
     cin >> edges;
     cout << "Enter path in format location1 location2 distance \n";
     for(int i = 0 ; i < edges ; ++i){
        string u , v;
        int distance;
    cin >> u >> v >> distance;
     if(!Citymap.hasLocation(u) || ! Citymap.hasLocation(v)) {
        cout <<"please enter valid locations\n";
        i--;
        continue;
     }
        Citymap.addEdge(u,v,distance);
     }

     // drivers details
     cout << "\nEnter number of drivers : ";
     int driversCount;
     cin >> driversCount;
     vector<Driver>drivers;
     for(int i = 0 ; i < driversCount ; ++i){
        string name , location;
        cout << "Enter the name and location of driver : ";
        cin >> name >> location;
        if(! Citymap.hasLocation(location)){
            cout <<"Invalid location !! Driver nt added. \n";
            continue ;
        }
        drivers.push_back(  Driver(name,location));
     }
    //  number of cab types and fare per km
       CabTypes Economy("Economy",0);
       CabTypes Premium("Premium",0);
       CabTypes Luxury("Luxury",0);
       cout << "Enter fare per km  for Economy class : "  ;
       cin >> Economy.FarePerKm;
       cout << "Enter fare per km  for Premium class : "  ;   
       cin >> Premium.FarePerKm;
       cout << "Enter fare per km  for Luxury class  : "  ;   
       cin >> Luxury.FarePerKm;
    cabBooking booking(Citymap,drivers);
    int choice;
    while(true){
    cout << "\n======= MENU =======\n";
    cout << "1. Book a ride\n";
    cout << "2. Show all drivers\n";
    cout << "3. Show ride history\n";
    cout << "4. Show all locations\n";
    cout << "5. Exit\n";
    cout << "Enter your choice: ";
    cin >> choice;

    if(choice == 1){
        string pickup , drop , cabtype ;
        cout << "Enter pickup location: "; 
        cin >> pickup; 
        cout << "Enter drop location: "; 
        cin >> drop; 
        cout << "Enter cab type (economy/premium/luxury): "; 
        cin >> cabtype;
         if(cabtype ==  "luxury")
 booking.bookride(pickup,drop,Luxury);
    else if(cabtype ==  "economy")
 booking.bookride(pickup,drop,Economy);
   else if(cabtype ==  "premium")
 booking.bookride(pickup,drop,Premium);
 else   {cout << "Invalid Cab type !! \n";
       continue;}

    }
    else if(choice == 2){
 booking.printDrivers();
    }
    else if(choice == 3){
        booking.printRideHistory();
    }
    else if(choice == 4){
        Citymap.printAllLocations();
    }
    else if(choice == 5){
        cout << "Exiting Cab booking system ...\n" ;
        break;
    }
    else cout << "Invalid choice , Please try again.\n";
}
return 0; 
}

                      
# Project Objective

The focus of this project is to implement a hybrid algorithm (G.R.A.S.P.) that solves a single depot vehicle routing problem.


## Introduction

### Vehicle Routing Problem

The simplest Vehicle Routing Problem is stated as: Given a single depot and certain number vehicles, the problem is to meet the requirements of n number of customers. Each customer requires a certain quantity that is to be delivered from the depot. There are a fixed number of vehicles available and the capacity of the vehicle is known. The distances among the customer location as well as form the depot to location are known. The vehicle starts from the depot and return to depot after meeting the customer requirements. The problem is to deliver the quantities to the customers such that the vehicle used travel minimum total distance.
The above problem is called a single depot vehicle depot problem. There are multiple depot vehicle depot problems, where the customers can be served from any depot.

### Vehicle Routing Problem - Classification:

1. Vehicle Routing Problem with Pickup and Delivery (VRPPD): 
A number of goods need to be moved from certain pickup locations to other delivery locations. The goal is to find optimal routes for a fleet of vehicles to visit the pickup and drop-off locations.
2. Vehicle Routing Problem with LIFO :
 Similar to the VRPPD, except an additional restriction is placed on the loading of the vehicles: at any delivery location, the item being delivered must be the item most recently picked up. This scheme reduces the loading and unloading times at delivery locations because there is no need to temporarily unload items other than the ones that should be dropped off.
3. Vehicle Routing Problem with Time Windows (VRPTW): 
The delivery locations have time windows within which the deliveries (or visits) must be made.
4. Capacitated Vehicle Routing Problem (with or without Time Windows):
CVRP or CVRPTW. The vehicles have limited carrying capacity of the goods that must be delivered.

### Greedy Random Adaptive Search Procedure

GRASP, a randomized adaptive search method is a constructive search methodology. The algorithm focuses on adding elements to a partial solution and make it complete. GRASP doest not perform any search operation, but they construct a single solution in an iterative fashion by evaluating all other solution elements, a.k.a. candidates and add according to there performance, i.e. insertion priority. The iteration continues as long as there is scope for improvement.

#### COMPONENTS

GRASP begins every iteration with an empty solution set, adding elements by the iteration procedure in order to complete the solution. To explain, any iteration starts with an empty solution and evaluates all candidate elements according to a performance function (g) for their influence on the quality of the current list of partial solution. Those elements are sorted in decreasing order in so called restricted candidate list. From this set, a random element is chosen and added to the partial solution. Let us take brief look at each of the components.

1. Candidate evaluation
Every candidate is evaluated using a function called candidate evaluation function (g). The change in the quality of partial solution by the addition of respective candidate element is what is taken into account.
2. Restricted candidate list construction
R.C.L. construction can be carried out in two ways, viz. Cardinality based construction and Value based construction. The former method includes the k (fixed by the user) best candidates in the R.C.L., where k is the parameter. If k =1, the construction is purely greedy, and as k increases the randomness increases. In value based method a parameter a€ [0, 1] is employed for the R.C.L. construction. A threshold value which equals gmin + a (gmax-gmin) is calculated. All elements which is below or above the threshold value is included in the list (for minimization and maximization problem respectively). 
3. Random element chooser
 An element from the list is chosen randomly.

#### Basic Algorithm

The problem has been solved using GRASP algorithm with cardinal based restricted candidate list creation. 

'''
Data: set V containing customers and depot
Data: cost matrix D
Result: set of routes R
while termination criteria not met do	
s         empty solution;
calculate seed customers;
create partial solution s based on seed customers;
while solution s is not complete do
evaluate all candidate elements ci € C with g(ci,s);
build rcl out of the best candidates;
choose random  ci € rcl;
add ci to s;
remove ci from C;
end
improve s until it is locally optimal;
if f(s)< f(sbest) then
sbest = s;
end
end
'''

#### Solution Improvement

##### Intra-Route Improvement – Two Opt Heuristic

A 2 opt neighborhood operator removes two non adjacent edges and reconnects the remaining two paths, yielding to a new solution. For this to work a feasible round trip must be given, which means every node is visited exactly once.
 
##### Inter-Route Improvement – Cross-Exchange

Here, the basic idea is to move nodes (or paths) from one route to a different route. Unlike single route neighborhood operators (intra-route improvement), multi-route neighborhood operators must check capacity constraints, because by moving customers or customer segments between different routes, the aggregated demand of the route can change and hence may become infeasible. Cross exchange reconnects the customer of two different routes as it is done by the two opt operator, by removing edge( i, i+1) from one route and (j, j+1) from a different route and inserting the edges  (i,j) and (i+1, j+1). Here paths change traversing direction too.

## Test Problem under study

A single depot VRP with 600 customers. The constraints include:-
*	Customers can be served only between 5.00 A.M. and 8.00 A.M.
*	Each truck can only travel a maximum of 65 k.m. or 35 stops (customers).
*	Customer demands are specific.
*	Each truck can take 1000 k.g. at full load.

Objective 
*	Find the number of trucks.
*	Vehicle route for each truck
*	Total distance traveled and the associated cost.

### Input Data

The input data consists of a 601 X 601 distance matrix (dataone.txt) and a 600 X 1 demand matrix (datatwo.txt). 

### Functions

int seeder(int array[])
	Calculates and returns the seed customers.

int calculatelength(route)
	Takes the route as argument and returns its length. 

void routeinitializer(route&,int)
	It initializes routes.

float calculatecost(route)
	Takes the route as argument and returns the cost. 	

int feasibility(route)
	Takes the route as argument and checks its feasibility and returns zero if feasible. 

float insert(route,int,int)
Insert a given customer in a given route in a given position and returns its insertion cost. The arguments used are route, customer (candidate), and position.

float permutation(route, int)
This function tries all possible positions in a given route for a given customer and returns a minimum insertion cost. It takes route and customer as arguments.

int mininsertion position(route, int)
This function returns the position that has the minimum insertion cost for a given customer in a given route.

int minroute(route[], int)
This function returns the route with minimum insertion cost for a given customer using route array and customer as arguments.

float inspriority(route[], int)
This function takes route array and customer and returns the insertion priority for the given customer.

int selectcustomer()
	It selects a random customer from the restricted candidate list to be inserted.

int check(int)
This function checks whether a customer (argument) is already routed and returns zero if not routed.

void routedcustomer(int)
	This function adds a customer (argument) to already routed list(if routed).

void changeroute(route&, int, int)
This function makes permanent change to the given route taking route, customer and position as argument.

float updateglobal(route&, route)
	Updates global best solution and returns the total distance or cost.

float calculatetimecost(route)
Calculates the distance from depot to the last customer served in the route taken as argument. This distance is required in order to check whether time window is violated.

float calculategainintra(route, int, int)
Takes route and the customers involved in the two opt move as argument and calculate the net gain in distance for the given two opt move.

void changer(route&, int, int)
Implements permanent change in the route for the given customers, i.e. two opt move.

void intrarouteimprovement(route&)
Takes the given route as argument and performs all possible two opt moves and the function calculategainintra returns the corresponding gain for each move, and the best improvement is implemented using the function void changer.

void interrouteimprovement(route& p,route& q)
Takes the two route as argument and performs all possible edge exchanges and the function calculategain returns the corresponding gain for each move, and the best improvement is implemented using the function void changerinter.

float calculategain(route p,route q,int petro1[],int petro2[],int edge1,int edge2)
Takes routes, route segments and edges to be cut as argument and performs the cross exchange and returns the corresponding gain.

void deletion(route&t,int edge)
This is the operational function used by calculate gain to try the inter tour operation. It helps in removing a segment from the route as argument.

void insert(route&t, int customer)
This is the operational function used by calculate gain to try the inter tour operation. It helps in adding a segment to the route.

void changerinter(route &p,route &q,int petro1[],int petro2[],int edge1,int edge2)
	This function performs the best inter-route improvement permanently. 




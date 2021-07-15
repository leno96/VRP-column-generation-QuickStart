
#pragma once

#include "Problem.h"
#include "ShortestPathBellmanFord.h"
#include "Solution.h"



//bool IsFeasible(Route &r, const Problem &p);
bool IsFeasible_Weight(const Route &r, const Problem &p);
bool IsFeasible_Time(const Route &r, const Problem &p);
double IsFeasible_Weight(const Route &r, const Problem &p, bool feasible);
bool IsFeasible(Route &r, const Problem &p);
void Shortest_Path(const Problem &p, Route &r_select);
void print_shortest_route(const Problem &p);

//----------------------------Route-----------------------
double CalculateRouteCost(Route &r, const Problem &p);




//----------------------------Solution-----------------------
void CalculateSolutionCost(Solution &sol, const Problem &p);
void CalculateSolutionObj(Solution &sol, const Problem &p);

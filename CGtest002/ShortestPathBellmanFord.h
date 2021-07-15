#include "AdjMatrixDirNetwork.h"
#include "Solution.h"
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <string>
using namespace std;
#pragma once

//¿ÉÐÞ¸Änetwork
double ShortestPathBellmanFord(AdjMatrixDirNetwork &network, Route &route_rec_id, int startVex = 0);
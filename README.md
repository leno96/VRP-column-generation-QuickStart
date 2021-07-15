# VRP-column-generation-QuickStart

"Column generation method to solve CVRP problem (C++ calls cplex implementation)"

I mainly work on the VRP problem. This week I want to use the column generation method to solve it. But I found that there are few codes of column generation algorithms online, and there is no such code public on VRP problems (only one is implemented in Java).

Thanks to https://github.com/HendrikBV/ColumnGenerationExampleCPLEX.git. Drawing lessons from his column generation algorithm framework, it is modified into a column generation algorithm for solving CVRP problems.
Disadvantages: The sub-problem is solved by the shortest path algorithm, but the shortest path algorithm has not been fully written. Currently, only feasible solutions can be output, but not the optimal solution.

#include <vector>
#include <iostream>
#include <fstream>
#include <map>
#include <algorithm>
#include <string>
#include <queue>
#include <utility>

#define LOW 0
#define HIGH 1

using namespace std;

pair<int, vector<char>> calc_profit_dyn(vector<vector<int>>, vector<vector<int>> &, vector<vector<vector<char>>> &, char, int, int);

int main()
{
	ifstream record; // input stream
	record.open("input.txt");
	if (!(record.good())) // return if opening was unsuccesfull
		return 0;

	freopen("Enzinna.txt", "w", stdout);

	int num_sets = 0; // stores total number of sets to parse

	// INPUT PHASE
	record >> num_sets;				   // get the number of sets
	for (int i = 0; i < num_sets; i++) // parse each set separately
	{
		int num_weeks = 0;			  // stores weeks per set
		vector<vector<int>> payments; // vector stores payment for low/high stress jobs

		record >> num_weeks; // input the number of weeks in the set
		payments.resize(2, vector<int>(num_weeks, 0));

		for (int r = 0; r < 2; r++)
		{
			for (int j = 0; j < num_weeks; j++) // add low stress payments
			{
				int val = 0;
				record >> val;		  // input the payment
				payments[r][j] = val; // update payment vector
			}
		}

		// COMPUTATION PHASE
		vector<vector<int>> cost_dyn(3, vector<int>(num_weeks, -1));							   // cost array of dynamic memoization algorithm
		vector<vector<vector<char>>> path_dyn(3, vector<vector<char>>(num_weeks, vector<char>())); // corresponding path of dynamic memoization algorithm

		pair<int, vector<char>> result_dyn = calc_profit_dyn(payments, cost_dyn, path_dyn, 'N', -1, num_weeks); // call to memoized recursive alg

		cout << result_dyn.first << endl; // output results of memoization recursive alg
		for (int k = num_weeks - 1; k >= 0; k--)
			cout << result_dyn.second[k] << " ";
		cout << endl
			 << endl;

		payments.clear(); // clear vectors
		cost_dyn.clear();
		path_dyn.clear();
	}
}

/****************************************************************************************************/
// DYNAMIC STUFF
/****************************************************************************************************/
pair<int, vector<char>> calc_profit_dyn(const vector<vector<int>> payments, vector<vector<int>> &cost, vector<vector<vector<char>>> &path, char prev, int week, int num_weeks)
{

	// Base Cases
	if (week > num_weeks - 2) // last week, start walking back up the tree
	{
		if (prev == 'H') // if our last choice was a high stress job
		{
			cost[2][week] = payments[HIGH][week]; // calculate the base cost of a High stress decision on the last week
			path[2][week].push_back('H');		  // update the path for this decision
			return make_pair(cost[2][week], path[2][week]);
		}
		else if (prev == 'L') // if our last choice was a low stress job
		{
			cost[1][week] = payments[LOW][week]; // calculate the base cost of a low stress decision on the last week
			path[1][week].push_back('L');		 // update the path for this decision
			return make_pair(cost[1][week], path[1][week]);
		}
		else // if our last choice was "no choice"
		{
			cost[0][week] = 0;			  // calculate the base cost of "no choice" decision on the last week
			path[0][week].push_back('N'); // update the path for this decision
			return make_pair(cost[0][week], path[0][week]);
		}
	}

	// calc payout for current week
	int money;
	if (prev == 'N')
		money = 0;
	else if (prev == 'L')
		money = payments[LOW][week];
	else
		money = payments[HIGH][week];

	pair<int, vector<char>> bestone = make_pair(0, vector<char>(0));

	//initalize pair objects
	pair<int, vector<char>> high_tree; // stores high stress decision tree (cost, path)
	pair<int, vector<char>> low_tree;  // stores low stress decision tree (cost, path)
	pair<int, vector<char>> null_tree; // stores "no choice" decision tree (cost, path)

	// check for pre-computed vals/compute the low and null path options recursively
	if (cost[0][week + 1] == -1) // if we have not seen a value for the "no choice" decision on the current week
	{
		null_tree = calc_profit_dyn(payments, cost, path, 'N', week + 1, num_weeks); // recursively calculate the maximum payout and path of "no choice" decision
		cost[0][week + 1] = null_tree.first;										 // update the cost array
		path[0][week + 1] = null_tree.second;										 // update the path array
	}
	else
	{
		null_tree = make_pair(cost[0][week + 1], path[0][week + 1]); // here we utilize the memoization of pre-computed values
	}

	if (cost[1][week + 1] == -1)
	{
		low_tree = calc_profit_dyn(payments, cost, path, 'L', week + 1, num_weeks); // recursively calculate the maximum payout and path of low stress decision
		cost[1][week + 1] = low_tree.first;											// update the cost array
		path[1][week + 1] = low_tree.second;										// update the path array
	}
	else
	{
		low_tree = make_pair(cost[1][week + 1], path[1][week + 1]); // here we utilize the memoization of pre-computed values
	}

	// if previous choice was NULL open up the high cost route
	if (prev == 'N')
	{
		if (cost[2][week + 1] == -1)
		{
			high_tree = calc_profit_dyn(payments, cost, path, 'H', week + 1, num_weeks); // recursively calculate the maximum payout and path of high stress decision
			cost[2][week + 1] = high_tree.first;										 // update the cost array
			path[2][week + 1] = high_tree.second;										 // update the path array
		}
		else
		{
			high_tree = make_pair(cost[2][week + 1], path[2][week + 1]); // here we utilize the memoization of pre-computed values
		}
	}
	// speed up memory access to pairs and shorten the lingo
	int null_cost = null_tree.first; // payout of null decsion tree
	int low_cost = low_tree.first;	 // payout of low decsion tree
	int high_cost = high_tree.first; // payout of high decsion tree

	int choice; // convert previous decision to array index
	switch (prev)
	{
	case 'H':
		choice = 2;
		break;
	case 'L':
		choice = 1;
		break;
	case 'N':
		choice = 0;
		break;
	}

	// compute maximum payout and update dynamic arrays
	if (high_cost > low_cost)
	{
		if (high_cost > null_cost) // if high stress path resulted in maximum payout
		{
			bestone.first = high_cost + money; // save the payout of the best decision
			bestone.second = high_tree.second; // save the path of the best decision
			bestone.second.push_back(prev);

			if (week != -1) // if we are amidst calculation (not at week -1)
			{
				cost[choice][week] = bestone.first;	 // update the dynamic cost array
				path[choice][week] = bestone.second; // update the dynamic path array
			}
			return bestone; // return the best path and cost for the previous decision
		}
		else // if "no choice" path resulted in maximum payout
		{
			bestone.first = null_cost + money;
			bestone.second = null_tree.second;
			bestone.second.push_back(prev);

			if (week != -1)
			{
				cost[choice][week] = bestone.first;
				path[choice][week] = bestone.second;
			}
			return bestone;
		}
	}
	else
	{
		if (null_cost > low_cost) // if "no choice" path resulted in maximum payout
		{
			bestone.first = null_cost + money;
			bestone.second = null_tree.second;
			bestone.second.push_back(prev);

			if (week != -1)
			{
				cost[choice][week] = bestone.first;
				path[choice][week] = bestone.second;
			}
			return bestone;
		}
		else // if low stress path resulted in maximum payout
		{
			bestone.first = low_cost + money;
			bestone.second = low_tree.second;
			bestone.second.push_back(prev);

			if (week != -1)
			{
				cost[choice][week] = bestone.first;
				path[choice][week] = bestone.second;
			}
			return bestone;
		}
	}
}

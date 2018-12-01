/**
 *       @file  TTT.c
 *      @brief  
 *
 *   @internal
 *     Created  11/17/2017
 *     Company  LSEC CAS
 *
 * =====================================================================================
 */
#include <stdio.h>
typedef struct NET_ 
{
   int num_node;
   int *num_neighbour;
   int **id_neighbour;
} NET;
typedef struct AGENT_ 
{
   int num_agent;
   int num_payoff;
   double **payoff;
   int *strategy;
}AGENT;


int main()
{
   int iter, max_iter;
   iter = 0;
   max_iter = 10;

   NET net;
   AGENT agent;

   NetGenerationLattice(10, &net);
   AgentGeneration(10*10, &agent, num_strategy, *initial_strategy);

   double *random_vector;
   RandomVectorGeneration(10*10*2*max_iter, random_vector);

   while (iter < max_iter)
   {
      /** 1 */
      for (idx = 0; idx < agent.num_agent; ++idx)
      {
	 PayoffGeneration(idx, agent.strategy+idx, agent.payoff[idx],
	       net.id_neighbour[idx], net.num_neighbour[idx],
	       strategy_neighbour, payoff_neighbour);
	 StratergyGeneration(idx, agent.strategy+idx, agent.payoff[idx], 
	       net.id_neighbour[idx], net.num_neighbour[idx],
	       strategy_neighbour, payoff_neighbour,
	       random_vector+2);
      }
      /** 2 */
      for (idx = 0; idx < agent.num_agent; ++idx)
      {
	 PayoffGeneration(idx, agent.strategy+idx, agent.payoff[idx],
	       net.id_neighbour[idx], net.num_neighbour[idx],
	       strategy_neighbour, payoff_neighbour);
      }
      for (idx = 0; idx < agent.num_agent; ++idx)
      {
	 StratergyGeneration(idx, agent.strategy+idx, agent.payoff[idx], 
	       net.id_neighbour[idx], net.num_neighbour[idx],
	       strategy_neighbour, payoff_neighbour,
	       random_vector);
	 random_vector += 2;
      }

      /** 3 */
      for (idx = 0; idx < agent.num_agent; ++idx)
      {
	 for (neighbours)
	 {
	    PayoffGeneration
	 }
	 StratergyGeneration
      }
      ++iter;
   }

   return 0;
}
/* -----  end for  ----- */ 

int NetGeneration(int num_node, int **id_neighbours, NET *net)
{

}

int NetGenerationLattice(int size, NET *net)
{

}

int StratergyGeneration(int id, int *strategy, double *payoff, 
      int *id_neighbour, int num_neighbour, int *strategy_neighbour, double **payoff_neighbour,
      double *random_vector)
{
   id_neighbour = ChooseNeighbour(num_neighbour, random_vector);
   flag = ChangeStrategy(id_neighbour, payoff_neighbour, payoff, random_vector+1);
   if (flag == 0)
   {
     strategy[0] = strategy_neighbour[id_neighbour][0];
   }
   return 0;
}

int PayoffGeneration(int id, int *strategy, double *payoff, int *id_neighbour, int num_neighbour, 
      int *strategy_neighbour, double **payoff_neighbour)
{
   /** Prisoners */
   payoff[0] = 0.0;
   for (idx = 0; idx < num_neighbour; ++idx)
   {
      payoff_neighbour[idx][0] = 0.0;
   }
   for (idx = 0; idx < num_neighbour; ++idx)
   {
      switch (strategy[0]){
	 case 0:
	    switch (strategy_neighbour[idx]){
	       case 0:
		  payoff[0] += 1.0;
		  break;
	       case 1:
		  payoff[0] += 0.0;
		  break;
	       default: printf("Error")
	    }
	    break;
	 case 1:
	    switch (strategy_neighbour[idx]){
	       case 0:
		  payoff[0] += 3.0;
		  break;
	       case 1:
		  payoff[0] += 0.0;
		  break;
	       default: printf("Error")
	    }
	    break;
	 default: printf("Error")
      }
   }   /* -----  end for  ----- */ 
   payoff[0] = payoff[0] / num_neighbour;
   
   /** Public */
   payoff[0] = 0.0;
   for (idx = 0; idx < num_neighbour; ++idx)
   {
      payoff_neighbour[idx][0] = 0.0;
   }
   double sum = strategy[0];
   for (idx = 0; idx < num_neighbour; ++idx)
   {
      sum += strategy_neighbour[idx]; 
   }
   pf = sum * 1.1 / (1+num_neighbour);
   for (idx = 0; idx < num_neighbour; ++idx)
   {
      payoff_neighbour[idx][0]
	 = pf - (1-strategy_neighbour[idx])
   }
   payoff[0] = pf - (1-strategy[0]);

   return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
typedef struct NET_ 
{
   int num_node;
   int *num_neighbour;
   int **id_neighbour;

   int max_degree;
} NET;
typedef struct AGENT_ 
{
   int num_agent;
   int num_payoff;
   int num_strategy;
   double **payoff;
   int *strategy;
} AGENT;
int NetGeneration(int num_node, int **id_neighbour, NET *net)
{
   return 0;
}
int NetGenerationLattice(int size, NET *net)
{
   net->num_node = size*size;
   net->num_neighbour = malloc(net->num_node*sizeof(int));
   net->id_neighbour = malloc(net->num_node*sizeof(int*));
   int idx, row, col, nei;
   for ( row = 0; row < size; ++row )
   {
      for ( col = 0; col < size; ++col )
      {
	 idx = row*size+col;
	 net->num_neighbour[idx] = 4;
	 net->id_neighbour[idx] = malloc(net->num_neighbour[idx]*sizeof(int));
	 net->id_neighbour[idx][0] = row*size+((col-1+size) % size);
	 net->id_neighbour[idx][1] = row*size+((col+1+size) % size);
	 net->id_neighbour[idx][2] = ((row-1+size) % size)*size+col;
	 net->id_neighbour[idx][3] = ((row+1+size) % size)*size+col;
      }
   }
   net->max_degree = 4;
   return 0;
}
int NetDestory(NET *net)
{
   free(net->num_neighbour);
   int idx;
   for ( idx = 0; idx < net->num_node; ++idx )
   {
      free(net->id_neighbour[idx]);
   }
   free(net->id_neighbour);
   return 0;
}
int NetPrint(NET *net)
{
   int idx, nei;
   for ( idx = 0; idx < net->num_node; ++idx )
   {
      printf ( "%d:\t", idx );
      for (nei = 0; nei < net->num_neighbour[idx]; ++nei)
      {
	 printf ( "%d\t", net->id_neighbour[idx][nei] );
      }
      printf ( "\n" );
   }
}
int AgentGeneration(int num_agent, int num_payoff, int num_strategy, int *initial_strategy, AGENT *agent)
{
   int idx;
   agent->num_agent = num_agent;
   agent->num_payoff = num_payoff;
   agent->num_strategy = num_strategy;
   agent->strategy = malloc(agent->num_agent*sizeof(int));
   agent->payoff = malloc(agent->num_agent*sizeof(double*));
   for ( idx = 0; idx < agent->num_agent; ++idx )
   {
      agent->payoff[idx] = malloc(agent->num_payoff*sizeof(double));
   }
   if (initial_strategy!=NULL)
   {
      for ( idx = 0; idx < agent->num_agent; ++idx )
      {
	 agent->strategy[idx] = initial_strategy[idx];
      }
   }
   else 
   {
      srand((unsigned)time(NULL));
      for ( idx = 0; idx < agent->num_agent; ++idx )
      {
	 agent->strategy[idx] = rand() % agent->num_strategy;
      }
   }
   return 0;
}
int AgentPayoffSetZero(AGENT *agent)
{
   int i, k;
   for ( i = 0; i < agent->num_agent; ++i )
   {
      for ( k = 0; k < agent->num_payoff; ++k )
      {
	 agent->payoff[i][k] = 0;
      }
   }
   return 0;
}
int AgentPayoffShift(AGENT *agent)
{
   int i, k;
   for ( i = 0; i < agent->num_agent; ++i )
   {
      for ( k = 0; k < agent->num_payoff-1; ++k )
      {
	 agent->payoff[i][k] = agent->payoff[i][k+1];
      }
      agent->payoff[i][agent->num_payoff-1] = 0;
   }
   return 0;
}
int AgentDestory(AGENT *agent)
{
   int idx;
   free(agent->strategy);
   for ( idx = 0; idx < agent->num_agent; ++idx )
   {
      free(agent->payoff[idx]);
   }
   free(agent->payoff);
   return 0;
}
int AgentPrint(AGENT *agent)
{
   int i, k;
   for ( i = 0; i < agent->num_agent; ++i )
   {
      printf ( "%d:\t%d\n", i, agent->strategy[i] );
   }
   for ( i = 0; i < agent->num_agent; ++i )
   {
      printf ( "%d:\t", i );
      for ( k = 0; k < agent->num_payoff; ++k )
      {
	 printf ( "%.2f\t", agent->payoff[i][k] );
      }
      printf ( "\n" );
   }
}
int AgentPrintRatio(AGENT *agent)
{
   int *count, idx;
   count = malloc(agent->num_strategy*sizeof(int));
   for (idx = 0; idx < agent->num_strategy; ++idx)
   {
      count[idx] = 0;
   }
   for (idx = 0; idx < agent->num_agent; ++idx)
   {
      count[ agent->strategy[idx] ] += 1;
   }
   for (idx = 0; idx < agent->num_strategy; ++idx)
   {
      printf ( "%d:\t%d\t%.2f\n", idx, count[idx], (double)count[idx]/(double)agent->num_agent );
   }

//   printf ( "%0.5f\n", (double)count[0]/(double)count[1] );

   free(count);
   return 0;
}
int AgentPrintStrategyLattice(AGENT *agent, int size)
{
   int idx, row, col;
   printf ( "===============================================\n" );
   for ( row = 0; row < size; ++row )
   {
      for ( col = 0; col < size; ++col )
      {
	 idx = row*size+col;
	 printf ( "%d ", agent->strategy[idx] );
      }
      printf ( "\n" );
   }
   printf ( "===============================================\n" );
   return 0;
}
int AgentPrintPayoffLattice(AGENT *agent, int size)
{
   int idx, row, col;
   printf ( "===============================================\n" );
   for ( row = 0; row < size; ++row )
   {
      for ( col = 0; col < size; ++col )
      {
	 idx = row*size+col;
	 printf ( "%0.2f ", agent->payoff[idx][ agent->num_payoff-1 ] );
      }
      printf ( "\n" );
   }
   printf ( "===============================================\n" );
   return 0;
}
int RandomVectorGeneration(int size, int **random_vector)
{
   int idx;
   *random_vector = malloc(size*sizeof(int));

   srand((unsigned)time(NULL));
   for (idx = 0; idx < size; idx++)
   {
      (*random_vector)[idx] = rand();
   }

   return 0;
}
int RandomVectorDestory(int *random_vector)
{
   free(random_vector);
   return 0;
}
int ChooseNeighbour(int num_neighbour, int *random_vector)
{
   return (*random_vector) % num_neighbour;
}
int ChangeStrategy(double *payoff_neighbour, double *payoff, int *random_vector)
{
   /* 1表示更换, 0表示不变 */
   double sigmoid = 1./(1.+exp(10*(*payoff - *payoff_neighbour)));
//   double sigmoid = 1./2.;
   double rn = (*random_vector+0.01) / (RAND_MAX + 0.1);

//   printf ( "rn = %0.2f, sigmoid = %0.2f, po = %0.2f, pon = %0.2f\n", rn, sigmoid, *payoff, *payoff_neighbour );
   if (rn < sigmoid)
   {
      return 1;
   }
   else 
   {
      return 0;
   }
}
int StrategyGeneration(int id, int *strategy, double *payoff, 
      int num_neighbour, int *strategy_neighbour, double **payoff_neighbour,
      int *random_vector)
{
   int id_neighbour = ChooseNeighbour(num_neighbour, random_vector);
   int flag = ChangeStrategy(payoff_neighbour[id_neighbour], payoff, random_vector+1);
//   printf ( "%d:\t%d\t%d\n", id, id_neighbour, flag );
   /* flag == 1表示更换 */
   if (flag == 1)
   {
     strategy[0] = strategy_neighbour[id_neighbour];
   }
   return 0;
}
int PayoffGeneration(int id, int *strategy, double *payoff, int num_neighbour, int *id_neighbour, 
      int *strategy_neighbour, double **payoff_neighbour)
{
   /** payoff and payoff_neighbour should be +=  */
   /** Prisoners */
   int idx;
   double po = 0.0;
   for (idx = 0; idx < num_neighbour; ++idx)
   {
      switch (strategy[0])
      {
	 case 0:
	    switch (strategy_neighbour[idx])
	    {
	       case 0:
		  po += 1.0;
		  break;
	       case 1:
		  po += 0.0;
		  break;
	       default: printf("Error");
	    }
	    break;
	 case 1:
	    switch (strategy_neighbour[idx])
	    {
	       case 0:
		  po += 1.0;
		  break;
	       case 1:
		  po += 0.0;
		  break;
	       default: printf("Error");
	    }
	    break;
	 default: printf("Error");
      }
   }   /* -----  end for  ----- */ 
//   payoff[0] += (po / num_neighbour);
   payoff[0] += po;

   return 0;
}
int main()
{
   int idx, iter, nei;

   int max_iter = 3000;
   int size = 100;
   int num_payoff = 1;
   int num_strategy = 2;

   NET net;
   AGENT agent;

   NetGenerationLattice(size, &net);
//   printf ( "NetPrint\n" );
//   NetPrint(&net);

   int *initial_strategy;
   initial_strategy = malloc(size*size*sizeof(int));
   int row, col;
   for ( row = 0; row < size; ++row )
   {
      for ( col = 0; col < size; ++col )
      {
	 idx = row*size+col;
	 initial_strategy[idx] = ((row*2.0)>=size?1:0);
      }
   }

//   AgentGeneration(size*size, num_payoff, num_strategy, initial_strategy, &agent);
   AgentGeneration(size*size, num_payoff, num_strategy, NULL, &agent);
   AgentPayoffSetZero(&agent);

//   printf ( "AgentPrint\n" );
//   AgentPrint(&agent);

   AgentPrintStrategyLattice(&agent, size);
   AgentPrintPayoffLattice(&agent, size);
   AgentPrintRatio(&agent);
   free(initial_strategy);

   int *random_vector, *tmp_rv;
   RandomVectorGeneration(agent.num_agent*2*max_iter, &random_vector);
   tmp_rv = random_vector;
   
   double **payoff_neighbour;
   int *strategy_neighbour, *next_strategy, *tmp_strategy;
   strategy_neighbour = malloc(net.max_degree*sizeof(int));
   payoff_neighbour = malloc(net.max_degree*sizeof(double*));
   next_strategy = malloc(agent.num_agent*sizeof(int));

   iter = 0;
   while (iter < max_iter)
   {
      printf ( "iter = %d\n", iter );
      /* payoff移位, payoff[0] = payoff[1], payoff[1] = payoff[2] */
      AgentPayoffShift(&agent);
      /* 基于strategy更新payoff[2] */
      for (idx = 0; idx < agent.num_agent; ++idx)
      {
	 for (nei = 0; nei < net.num_neighbour[idx]; ++nei)
	 {
	    strategy_neighbour[nei] = agent.strategy[ net.id_neighbour[idx][nei] ];
	    payoff_neighbour[nei] = agent.payoff[ net.id_neighbour[idx][nei] ];
	 }
	 PayoffGeneration(idx, agent.strategy+idx, agent.payoff[idx],
	       net.num_neighbour[idx], net.id_neighbour[idx], 
	       strategy_neighbour, payoff_neighbour);
      }

      printf ( "AfterPayoff\n" );
//      AgentPrintStrategyLattice(&agent, size);
//      AgentPrintPayoffLattice(&agent, size);
//      AgentPrintRatio(&agent);

      /* 将当前strategy复制给next_strategy */
      for (idx = 0; idx < agent.num_agent; ++idx)
      {
	 next_strategy[idx] = agent.strategy[idx];
      }
      /* 更新next_strategy */
      for (idx = 0; idx < agent.num_agent; ++idx)
      {
	 for (nei = 0; nei < net.num_neighbour[idx]; ++nei)
	 {
	    strategy_neighbour[nei] = agent.strategy[ net.id_neighbour[idx][nei] ];
	    payoff_neighbour[nei] = agent.payoff[ net.id_neighbour[idx][nei] ];
	 }
	 StrategyGeneration(idx, next_strategy+idx, agent.payoff[idx], 
	       net.num_neighbour[idx], strategy_neighbour, payoff_neighbour,
	       random_vector);
	 random_vector += 2;
      }
      /* 将next_strategy赋给strategy */
      tmp_strategy = agent.strategy;
      agent.strategy = next_strategy;
      next_strategy = tmp_strategy;

      printf ( "AfterStrategy\n" );
      AgentPrintStrategyLattice(&agent, size);
//      AgentPrintPayoffLattice(&agent, size);
      AgentPrintRatio(&agent);





      ++iter;
   }

//   AgentPrint(&agent);

   free(strategy_neighbour);
   free(payoff_neighbour);
   free(next_strategy);

   random_vector = tmp_rv;
   RandomVectorDestory(random_vector);
   AgentDestory(&agent);
   NetDestory(&net);

   return 0;
}

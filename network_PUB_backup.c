#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

double R = 4.0;
double *RATIO;

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


int NetChooseNeighbour(int id, int max_conection, int *num_neighbour, int *conection)
{
   int sum, node, flag, cn, tmp, pre;

   sum = 0;
   for ( node = 0; node < id; ++node )
   {
      sum += num_neighbour[node];
   }
   for ( cn = 0; cn < max_conection; ) 
   {
      conection[cn] = rand() % sum;
      tmp = 0;
      for ( node = 0; node < id; ++node )
      {
	 tmp += num_neighbour[node];
	 if (conection[cn] < tmp)
	 {
	    conection[cn] = node;
	    break;
	 }
      }
      flag = 0;
      for (pre = 0; pre < cn; ++pre)
      {
	 if (conection[pre] == conection[cn])
	 {
	    flag = 1;
	    break;
	 }
      }
      if (flag == 0)
      {
	 ++cn; 
      }
   }

   return 0;
}



int NetGenerationScaleFree(int num_node, int max_conection, int max_degree, NET *net)
{
   net->num_node = num_node;
   net->num_neighbour = malloc(net->num_node*sizeof(int));
   net->id_neighbour = malloc(net->num_node*sizeof(int*));
   net->max_degree = max_degree;
   int idx, cn, nei;
   for ( idx = 0; idx < net->num_node; ++idx )
   {
      net->num_neighbour[idx] = 0;
   }
   
   int *conection;
   conection = malloc(max_conection*sizeof(int));

   for ( idx = 0; idx <= max_conection; ++idx )
   {
      net->id_neighbour[idx] = malloc(net->max_degree*sizeof(int));
      net->num_neighbour[idx] = max_conection;

      nei = 0;
      for ( cn = 0; cn <= max_conection; ++cn ) 
      {
	 if (idx != cn)
	 {
	    net->id_neighbour[idx][nei] = cn;
	    ++nei;
	 }
      }
   }

   for ( idx = max_conection+1; idx < net->num_node; ++idx )
   {
      net->id_neighbour[idx] = malloc(net->max_degree*sizeof(int));
      NetChooseNeighbour(idx, max_conection, net->num_neighbour, conection);
      for ( cn = 0; cn < max_conection; ++cn ) 
      {
	 net->id_neighbour[ conection[cn] ][ net->num_neighbour[ conection[cn] ] ] = idx;
	 net->id_neighbour[idx][ net->num_neighbour[idx] ] = conection[cn];
	 ++net->num_neighbour[idx];
	 ++net->num_neighbour[ conection[cn] ];
      }
   }   /* -----  end for  ----- */ 

   free(conection);
   return 0;
}

int NetPrintDegreeAge(NET *net)
{
   int idx, sum;
   sum = 0;
   for ( idx = 0; idx < net->num_node; ++idx )
   {
      sum += net->num_neighbour[idx];
   }
   printf ( "Degree Age = %0.2f\n", (double)sum / (double)net->num_node );
 
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

int NetPrintPajek(NET *net, AGENT *agent, char *file_name)
{
   FILE* fstream;
   int idx, row, col;

   fstream = fopen ( file_name,  "w" );
   if ( fstream == NULL )
   {
      printf("Open file %s failed!\n", file_name);
      return 1;
   }
   else 
   {
      fprintf ( fstream,  "*Vertices %d\n", net->num_node );
      for ( idx = 0; idx < net->num_node; ++idx )
      {
	 if (agent->strategy[idx] == 0)
	 {
	    fprintf ( fstream,  "%d \"\" ic green circle\n",  idx+1 );
	 } 
	 else 
	 {
	    fprintf ( fstream,  "%d \"\" ic red   circle\n",  idx+1 );
	 }
      }

      int **adj_mat;
      adj_mat = malloc(net->num_node*sizeof(int*));

      for (idx = 0; idx < net->num_node; ++idx)
      {
	 adj_mat[idx] = malloc(net->num_node*sizeof(int));
      }
      for (row = 0; row < net->num_node; ++row)
      {
	 for (col = 0; col < net->num_node; ++col)
	 {
	    adj_mat[row][col] = 0;
	 }
      }

      for (row = 0; row < net->num_node; ++row)
      {
	 for (idx = 0; idx <  net->num_neighbour[row]; ++idx)
	 {
	    col = net->id_neighbour[row][idx];
	    adj_mat[row][col] = 1;
	 }
      }
      fprintf ( fstream,  "*Matrix\n" );
      for (row = 0; row < net->num_node; ++row)
      {
	 for (col = 0; col < net->num_node; ++col)
	 {
	    fprintf ( fstream,  "%d ", adj_mat[row][col] );
	 }
	 fprintf ( fstream,  "\n" );
      }
      for (idx = 0; idx < net->num_node; ++idx)
      {
	 free(adj_mat[idx]);
      }
      free(adj_mat);
      fclose(fstream);
   }

   return 0;
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
//      srand((unsigned)time(NULL));
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
double AgentPrintRatio(AGENT *agent)
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
//      printf ( "%d:\t%d\t%.4f\n", idx, count[idx], (double)count[idx]/(double)agent->num_agent );
   }

//   printf ( "%0.5f\n", (double)count[0]/(double)count[1] );
   double ratio = (double)count[1]/(double)agent->num_agent;
   free(count);
   return ratio;
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

//   srand((unsigned)time(NULL));
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
int StrategyGeneration(int *strategy, double *payoff, 
      int num_neighbour, int *strategy_neighbour, double **payoff_neighbour,
      int *random_vector)
{
   int id_neighbour = ChooseNeighbour(num_neighbour, random_vector);
   if (strategy[0] != strategy_neighbour[id_neighbour])
   {
      int flag = ChangeStrategy(payoff_neighbour[id_neighbour], payoff, random_vector+1);
      if (flag == 1)
      {
	 strategy[0] = strategy_neighbour[id_neighbour];
      }
   }
//   printf ( "%d:\t%d\t%d\n", id, id_neighbour, flag );
   /* flag == 1表示更换 */
   return 0;
}
int PayoffGeneration(int id, int *strategy, double *payoff, int num_neighbour, int *id_neighbour, 
      int *strategy_neighbour, double **payoff_neighbour)
{
   /** payoff and payoff_neighbour should be +=  */
   /** Public */
   int idx;
   double po;
   double sum = strategy[0];
   for (idx = 0; idx < num_neighbour; ++idx)
   {
      sum += strategy_neighbour[idx]; 
   }
   po = sum * R / (1+num_neighbour);
   for (idx = 0; idx < num_neighbour; ++idx)
   {
      payoff_neighbour[idx][0] += po - strategy_neighbour[idx];
   }
   payoff[0] += po - strategy[0];

   return 0;
}
int main ( int argc, char *argv[] )
{

   if (argc != 2)
   {
      printf ( "Please Input alpha rho sigma Q0 and delta\n" );
      return 0;
   }
   else {
      R = atof(argv[1]);
   }


   int idx, iter, nei;

   int max_iter = 3000;
   int size = 33;
   int num_payoff = 1;
   int num_strategy = 2;

   NET net;
   AGENT agent;

//   NetGenerationLattice(size, &net);
   NetGenerationScaleFree(size*size, 2, size*size, &net);
//   printf ( "NetPrint\n" );
//   NetPrint(&net);

//   NetPrintDegreeAge(&net);

   RATIO = malloc(size*size*sizeof(double));

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
   free(initial_strategy);

//   AgentGeneration(size*size, num_payoff, num_strategy, initial_strategy, &agent);


//   printf ( "AgentPrint\n" );
//   AgentPrint(&agent);

//   AgentPrintStrategyLattice(&agent, size);
//   AgentPrintPayoffLattice(&agent, size);
//   AgentPrintRatio(&agent);

   int *random_vector, *tmp_rv;
   
   double **payoff_neighbour;
   int *strategy_neighbour, *next_strategy, *tmp_strategy;
   strategy_neighbour = malloc(net.max_degree*sizeof(int));
   payoff_neighbour = malloc(net.max_degree*sizeof(double*));
   next_strategy = malloc(agent.num_agent*sizeof(int));

   double ratio = 0;
   int loop;
   int max_loop = 100;
   for (loop = 0; loop < max_loop; ++loop)
   {
      srand((unsigned)time(NULL));
      AgentGeneration(size*size, num_payoff, num_strategy, NULL, &agent);
      AgentPayoffSetZero(&agent);
      RandomVectorGeneration(agent.num_agent*2*max_iter, &random_vector);
      tmp_rv = random_vector;

      iter = 0;
      while (iter < max_iter)
      {
	 //      printf ( "iter = %d\n", iter );
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

	 //      printf ( "AfterPayoff\n" );
	 //      AgentPrintStrategyLattice(&agent, size);
	 //      AgentPrintPayoffLattice(&agent, size);
	 //      AgentPrintRatio(&agent);

	 /* 将当前strategy复制给next_strategy */
//	 for (idx = 0; idx < agent.num_agent; ++idx)
//	 {
//	    next_strategy[idx] = agent.strategy[idx];
//	 }
	 /* 更新next_strategy */
	 int center;
	 for (idx = 0; idx < agent.num_agent; ++idx)
	 {
	    center = rand() % agent.num_agent;
	    //	 center = idx;
	    for (nei = 0; nei < net.num_neighbour[center]; ++nei)
	    {
	       strategy_neighbour[nei] = agent.strategy[ net.id_neighbour[center][nei] ];
	       payoff_neighbour[nei] = agent.payoff[ net.id_neighbour[center][nei] ];
	    }
	    //	 StrategyGeneration(next_strategy+center, agent.payoff[center], 
	    StrategyGeneration(agent.strategy+center, agent.payoff[center], 
		  net.num_neighbour[center], strategy_neighbour, payoff_neighbour,
		  random_vector);
	    random_vector += 2;
	 }
	 /* 将next_strategy赋给strategy */
	 //      tmp_strategy = agent.strategy;
	 //      agent.strategy = next_strategy;
	 //      next_strategy = tmp_strategy;

	 //      printf ( "AfterStrategy\n" );
	 //      AgentPrintStrategyLattice(&agent, size);
	 //      AgentPrintPayoffLattice(&agent, size);




//      printf ( "------------------------\n" );

	 ++iter;
      }

      ratio += AgentPrintRatio(&agent);

      random_vector = tmp_rv;
      RandomVectorDestory(random_vector);

      AgentDestory(&agent);
   }
   printf ( "R = %f, ratio = %f\n", R, ratio/max_loop );
//   NetPrintPajek(&net, &agent, "net.paj");
//   AgentPrint(&agent);

   free(strategy_neighbour);
   free(payoff_neighbour);
   free(next_strategy);
   NetDestory(&net);

   return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <unistd.h>

double evalfunc(int nodeid, int *expandid1, int *expandid2);  // prototype
void threadeval(int n_num);
int expandid1, expandid2;
double rc;
int count=0;
int tid;
int search_space=0;
int num_solutions=0;
int sol_found=0;
omp_lock_t mylock;  
omp_lock_t lock2;
omp_lock_t tail1lock;
omp_lock_t tail2lock;
omp_lock_t freelock;
omp_lock_t lockhead;
struct node *currnode,*tofree,*tailnode,*expandnode1,*expandnode2;
struct node {
    int id;
    struct node *next;
};

int main(int argc, char *argv[])
{
   // int expandid1, expandid2;
   // double rc;
   // struct node *currnode, *tofree, *tailnode, *expandnode1, *expandnode2;
    int n_threads = atoi(argv[1]);
    num_solutions = atoi(argv[2]);
    //printf("num_solutions %d \n",num_solutions);
    int mynode;
     search_space = atoi(getenv("EVALFUNC_MAXNODEIDS"));
   // printf("%d \n",search_space);
  // int count =0;
    omp_init_lock(&mylock);
    omp_init_lock(&freelock);
    omp_init_lock(&tail1lock);
    omp_init_lock(&tail2lock);
    currnode = (struct node *) malloc(sizeof(struct node));
    omp_init_lock(&lock2);
//    printf("Size of struct is %ld \n",sizeof(struct node));
    omp_init_lock(&lockhead);
    currnode->id = 0;
    currnode->next = NULL;
    tailnode = currnode;
    mynode=0;

    omp_set_num_threads(n_threads);
#pragma omp parallel shared(tailnode,currnode)  private(mynode)
    {  tid = omp_get_thread_num();
  
    while (currnode)
    {	//printf("id is %d \n",currnode->id);
	#pragma omp atomic
	  	    count++;
		  //  printf("count is %d \n",count);
	 if((count-n_threads)>=search_space-1)
	 {
	 		 
		   // printf("Count is %d \n",count);
		 //printf("Loop broken because count > searchspace +nthreads\n");
		break;
	 }
 	if(n_threads==4&&count-3>=search_space-1)
	{//	printf("my node is %d \n",mynode);
		break;
	}		
	    if((currnode->id) >= search_space-1)
	  {	//printf("Loop broken because node id is outside of searchspace\n");
		  break;
	  }
	  if(sol_found>=num_solutions)
	  {	//printf("Loop broken because sol_found > num of solutions \n");
		  break;
	  }
	 // if(currnode->next == NULL)
	 // {
//		  sleep(2);
//	  }
//	printf("count is %d, my node is %d \n",count,mynode);
	omp_set_lock(&lockhead);
/*	if(count<4)
	{
	        if(tid==1)
		{	
			sleep(.5);
		}
		if(tid==2)
		{
			sleep(.75);
		}
		if(tid==3)
		{
			sleep(1);
		}
		if(tid==4)
		{
*/
		mynode = currnode->id;
		if(count>2 && currnode->next!=NULL)
		{
		//	currnode=currnode->next;
		//	printf("%d \n",mynode);
		}
		//currnode = currnode->next;
	//	printf("currnode is %d \n",mynode);
		if((currnode->next)!= NULL)
		{
//		      	sleep(1);
	//	printf("Incrementing head of list");
	//	currnode = currnode+16;
		
		}
	//	currnode = currnode->next;
//	printf("Mynode is %d and thread id is %d \n",mynode,tid);
	
	    
	//mynode=(int)currnode->next;
	omp_unset_lock(&lockhead);
//	#pragma omp task 
		//private(expandid1,expandid2)
//	{
//	printf("currnodes is %d \n",mynode);
	if(mynode >= search_space-1)
	{
		break;
	}	
        rc = evalfunc(mynode,&expandid1,&expandid2);
	
        if (rc > 1.0)
        {	sol_found++;
		//printf("%d \n",sol_found);
            // %lf same as %f, but not in scanf because uses pointer
            printf("answer: %d %lf \n",mynode,rc);
	    if(sol_found>=num_solutions)
	    {	//printf("Break because solutions found = num solutions");
		    break;
	    }
        }
//	else
//	{
			
		threadeval(mynode);

	//	#pragma omp taskwait
//	}

//	#pragma omp critical

    }

//	    } //end single
    } //end pragma

    
    return 0;
}


void threadeval(int n_num)
{	int this_node = n_num;

	omp_set_lock(&tail1lock);
//	printf("this node is %d \n",this_node);
	if(this_node>=search_space-1)
	{	//printf("returning because this node > search space \n ");
		return;
	}
        if (expandid1 >= 0)
        {

	//    omp_set_lock(&mylock);
            expandnode1 = (struct node *) malloc(sizeof(struct node));
            expandnode1->id = expandid1;
            expandnode1->next = NULL;
	  //  omp_unset_lock(&mylock);
        }
        else
        {
            expandnode1 = NULL;
	//    sleep(1);
        }
        if (expandid2 >= 0)
        {

	    //omp_set_lock(&lock2);
            expandnode2 = (struct node *) malloc(sizeof(struct node));
            expandnode2->id = expandid2;
            expandnode2->next = NULL;
	//    omp_unset_lock(&lock2);
        }
        else
        {
            expandnode2 = NULL;
	  //  sleep(1);
        }
        if (expandnode1)
        {// omp_set_lock(&tail1lock);
            tailnode->next = expandnode1;
            tailnode = expandnode1;
	   // omp_unset_lock(&tail1lock);
        }
        if (expandnode2)
        {//	omp_set_lock(&tail2lock);
            tailnode->next = expandnode2;
            tailnode = expandnode2;
	   // omp_unset_lock(&tail2lock);
        }

        tofree = currnode;
       	currnode = currnode->next;
        free(tofree);
//	omp_set_lock(&freelock);
//	omp_unset_lock(&freelock);
	
	omp_unset_lock(&tail1lock);



}

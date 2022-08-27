#include <cs50.c>
#include <stdio.h>
#include <cs50.h>
#include <string.h>

#define MAX_CANDIDATES 9
#define MAX_VOTERS 100

typedef struct 
{
    string name ;
    int votes ;
    bool eliminated ;
}
candidate;

// Array of candidates 
candidate candidates[1000];

// Number of voters and candidates 
int candidates_count ;
int voters_count ;

int i, j ;

// this is for the index of the voter's j'th preference 
int preferences[MAX_VOTERS][MAX_CANDIDATES] ;


// FUNCTIONS
bool vote(int voter, int rank, string name);
void tabulate(void);
bool print_winner(void);
int find_min(void);
bool is_tie(int min);
void eliminate(int min);

int main(int argc , string argv[])
{   
    if (argc < 2 )
    {
        printf("Usage : ./runoff candidate .....]\n");
        return 1 ;
    }
    candidates_count = argc - 1 ;
    if(candidates_count > MAX_CANDIDATES )
    {
        printf("max number of candidates is : %i",MAX_CANDIDATES);
        return 2 ;
    }
    for ( int z = 0 ; z < candidates_count ; z++)
    {
      candidates[z].name = argv[z + 1] ;
      candidates[z].votes = 0 ;
      candidates[z].eliminated = false ;
    }

    voters_count = get_int("Number of voters : ");
    if (voters_count > MAX_VOTERS )
    {
        printf("max number of voters is : %i",MAX_VOTERS);
        return 3;
    }


    for (  i = 0 ; i < voters_count ; i++)
    {
        for ( j = 0 ; j < candidates_count ; j++)
        {
            string name = get_string("","Rank %i:", j + 1);

            if(!vote(i,j,name))
            {
                printf("Invalid Vote .");
            }

        }
        printf("\n");
    }

    // Keep holding runoffs until winner exists
    while(true)
    {
        // Calculate votes given remaining candidates
        tabulate();

        // Check if election has been won
        bool won = print_winner();
        if(won)
        {
         break ;
        }

        // Eliminate last-place candidates
        int min = find_min();
        bool tie = is_tie(min);

        // If tie, everyone wins
        if(tie)
        {
            for ( i = 0; i < candidates_count; i++)
            {
              printf("%s\n",candidates[i].name);

            }
         break ;
        }


        eliminate(min);

        // Resetting vote counts back to zero
        for( i = 0 ; i < candidates_count ; i++)
        {
            candidates[i].votes = 0 ;
        }
    }
    return 0 ;
}




bool vote(int voter ,int rank ,string name)
{
    for (int i = 0 ; i < candidates_count ; i++)
    {
        if(strcmp(name,candidates[i].name) == 0)
        {
            preferences[voter][rank] = i ;
            return true ;
        }
    }
    return false ;
}


void tabulate(void)
{
   for( i = 0 ; i < voters_count ; i++)
   {
    for( j = 0 ; j < candidates_count ; j++)
    {
        if(candidates[preferences[i][j]].eliminated == false)
        {
            candidates[preferences[i][j]].votes++ ;
            break ;
        }
    }
   }
    return ;
}

bool print_winner(void)
{
    for ( i = 0 ; i < candidates_count ; i++)
    {
        if(candidates[i].votes > (voters_count/2))
        {
            printf("winner is %s", candidates[i].name);
            return true ;
        }
    }
    return false ;
}

int find_min(void)
{
    int min = voters_count ; //the minimum should be decrementing because the max votes is a candidate having all the votes
    for( i = 0 ; i < candidates_count ; i++)
    {
        if(candidates[i].eliminated == false && candidates[i].votes < min)
        {
            min = candidates[i].votes ;
        }
    }
    return min ; 
}

bool is_tie(int min )
{
  int remaining_candidates = 0 ;
  int candidates_with_min = 0;
  for( i = 0 ; i < candidates_count ;i++)
  {
    if(candidates[i].eliminated == false )
    {
        remaining_candidates++ ;
    }
    if(candidates[i].votes == min)
    {
        candidates_with_min++ ;
    }
  }
  
  if (remaining_candidates == candidates_with_min )
  {
    return true ;
  }
  return false ; 
}


void eliminate(int min)
{
    for ( i = 0 ; i < candidates_count ; i++)
    {
        if (candidates[i].eliminated == false && candidates[i].votes == min )
        {
            candidates[i].eliminated = true ;
        }
    }
    return;
}

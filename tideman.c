#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);
void difference(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    //for (int x = 0; x < candidate_count; x++)
    //{

    //    // Query for each rank
    //    for (int y = 0; y < candidate_count; y++)
    //    {
    //        printf("%d |", preferences[x][y]);

    //    }
    //    printf("\n");
    //}
    sort_pairs();
    //for (int a = 0; a < pair_count; a++)
    //{
    //    printf("%d %d\n", pairs[a].winner, pairs[a].loser);
    //}
    lock_pairs();
    //for (int m = 0; m < candidate_count; m++)
    //{
    //    // Query for each rank
    //    for (int n = 0; n < candidate_count; n++)
    //    {
    //        printf("%d |", locked[m][n]);
    //    }
    //    printf("\n");
    //}
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    // Is the voter valid?
    int i = 0;
    bool found = false;
    while (i < candidate_count && ! found)
    {
        if (strcmp(candidates[i], name) == 0)
        {
            found = true;
            ranks[rank] = i;
        }
        i++;
    }
    return found;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            // Increase the count of i being voted for over j
            preferences[ranks[i]][ranks[j]]++;
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            // Increase the count of i being voted for over j
            if (preferences[i][j] > preferences[j][i])
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pair_count++;

            }
            else if (preferences[j][i] > preferences[i][j])
            {
                pairs[pair_count].winner = j;
                pairs[pair_count].loser = i;
                pair_count++;
            }
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    for (int i = 0; i < pair_count; i++)
    {
        int max_id = i;
        for (int j = i + 1; j < pair_count; j++)
        {
            if (preferences[pairs[max_id].winner][pairs[max_id].loser] < preferences[pairs[j].winner][pairs[j].loser])
            {
                max_id = j;
            }
        }
        //swap the max to the front
        pair temp;
        temp = pairs[i];
        pairs[i] = pairs[max_id];
        pairs[max_id] = temp;
    }
}


// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    bool loop;
    int todo[pair_count]; //will need a pointer for this to allow for append and pop
    for (int a = 0; a < pair_count; a++)
    {
        todo[a] = -1;
    }
    int start_pos = 0;
    int end_pos = 0;
    int loser;
    int winner;
    int old_win;
    //go through the pairs
    for (int i = 0; i < pair_count; i++)
    {
        winner = pairs[i].winner;
        loser = pairs[i].loser;
        loop = false;
        todo[start_pos] = loser;
        while (todo[start_pos] != -1 && !loop)
        {
            //not a hundred percent sure how to emulate pop
            old_win = todo[start_pos];
            todo[start_pos] = -1;
            for (int j = 0; j < candidate_count; j++)
            {
                //if it's won previously
                if (locked[old_win][j])
                {
                    if (j==winner)
                    {
                        loop = true;
                        break;
                    }
                    else
                    {
                        todo[end_pos] = j;
                        end_pos++;
                    }
                }


            }
            if (!loop)
            {
                locked[winner][loser] = true;
            }
        }
    }
    return;
}

// Print the winner of the election
void print_winner(void)
{
    bool winner;
    for (int i = 0; i < candidate_count; i++)
    {
        winner = true;
        for (int j = 0; j < candidate_count; j++)
        {
            if (locked[j][i])
            {
                winner = false;
            }
        }
        if (winner)
        {
            printf("%s\n", candidates[i]);
            break;
        }
    }
    return;
}

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
bool cycle(int c, int l);


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
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    for (int j = 0; j < candidate_count; j++)
    {
        // if candidate is in list updates ranks[] array and returns true//
        if (!strcmp(candidates[j], name))
        {
            ranks[rank] = j;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            preferences[ranks[i]][ranks[j]] += 1; // records voter prefereces //
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    pair_count = 0; // stores pair_count //
    // loop records pair_count and preferferences where one candidate "the winner" is prefered over another candidate "the loser" into the struct "pairs", in pairs[].winner and pairs[].loser respectively //
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            if (preferences[i][j] != preferences[j][i])
            {
                if (preferences[i][j] > preferences[j][i])
                {
                    pairs[pair_count].winner = i;
                    pairs[pair_count].loser = j;
                    pair_count++;
                }
            }
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    int sv[pair_count];
    pair temp[0];
    for (int v = 0; v < pair_count; v++)   // loop to store prefereces[i][j] into array sv for ease of use //
    {
        sv[v] = preferences[pairs[v].winner][pairs[v].loser];
    }
    for (int i = 0; i < pair_count; i++)  // uses bubble sort to sort pairs by strenght of victory in decresasing order //
    {
        if (sv[i] < sv[i + 1])
        {
            temp[0] = pairs[i + 1];
            pairs[i + 1] = pairs[i];
            pairs[i] = temp[i];
        }
    }
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    for (int i = 0; i < pair_count; i++)//
    {
        // recursive function ensures that pairs[i].winner is not already a "locked loser" before we store pairs[i].winner and pairs[i].losers into the locked array, therefore a cylcle is not created//
        if (!cycle(pairs[i].winner, pairs[i].loser))
        {
            locked[pairs[i].winner][pairs[i].loser] = true; // stores "locked winners" and "locked losers" //
        }
    }
    return;
}

// Print the winner of the election
void print_winner(void)
{
    string winner; // stores the candidate that is never a "locked loser" //
    for (int i = 0; i < candidate_count; i++)
    {
        int j = 0; // locked winners counter resets to zero every time a new "locked loser or i" is checked against all locked winners to find the candidate that is never a "locked loser" //
        while (j < candidate_count)
        {
            //loops through all locked winners and if not a locked winner assigns i "the locked loser" as the winner, this ensures the string "winner" only retains the candidate that is never a locked loser//
            if (!locked[j][i])
            {
                winner = candidates[i];
                j++;
            }
            else // if a "locked winner" "j" (i.e locked[j][i] == true) is found checks the next canddiate "i" to ensure only a candiadte that is not a "locked loser" to any other candidate remains //
            {
                break;
            }
        }
    }
    printf("%s\n", winner);
    return;
}

bool cycle(int c, int l)
{
    // uses recursion to check for if a cylce is created //
    if (l == c)
    {
        return true;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        if (locked[l][i]) // ensures "locked winner" is not a "locked loser" in previous iterations thus creating a cycle" //
            if (cycle(c, i))
                return true;
    }
    return false;
}


#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max voters and candidates
#define MAX_VOTERS 100
#define MAX_CANDIDATES 9

// preferences[i][j] is jth preference for voter i
int preferences[MAX_VOTERS][MAX_CANDIDATES];

// Candidates have name, vote count, eliminated status
typedef struct
{
    string name;
    int votes;
    bool eliminated;
}
candidate;

// Array of candidates
candidate candidates[MAX_CANDIDATES];

// Numbers of voters and candidates
int voter_count;
int candidate_count;

// Function prototypes
bool vote(int voter, int rank, string name);
void tabulate(void);
bool print_winner(void);
int find_min(void);
bool is_tie(int min);
void eliminate(int min);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: runoff [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX_CANDIDATES)
    {
        printf("Maximum number of candidates is %i\n", MAX_CANDIDATES);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
        candidates[i].eliminated = false;
    }

    voter_count = get_int("Number of voters: ");
    if (voter_count > MAX_VOTERS)
    {
        printf("Maximum number of voters is %i\n", MAX_VOTERS);
        return 3;
    }

    // Keep querying for votes
    for (int i = 0; i < voter_count; i++)
    {

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            // Record vote, unless it's invalid
            if (!vote(i, j, name))
            {
                printf("Invalid vote.\n");
                return 4;
            }
        }

        printf("\n");
    }

    // Keep holding runoffs until winner exists
    while (true)
    {
        // Calculate votes given remaining candidates
        tabulate();

        // Check if election has been won
        bool won = print_winner();
        if (won)
        {
            break;
        }

        // Eliminate last-place candidates
        int min = find_min();
        bool tie = is_tie(min);

        // If tie, everyone wins
        if (tie)
        {
            for (int i = 0; i < candidate_count; i++)
            {
                if (!candidates[i].eliminated)
                {
                    printf("%s\n", candidates[i].name);
                }
            }
            break;
        }

        // Eliminate anyone with minimum number of votes
        eliminate(min);

        // Reset vote counts back to zero
        for (int i = 0; i < candidate_count; i++)
        {
            candidates[i].votes = 0;
        }
    }
    return 0;
}

// Record preference if vote is valid
bool vote(int voter, int rank, string name)
{
    for (int j = 0; j < candidate_count; j++)
        // compares the vote to the candiates//
    {
        // if candidate is in list it assigns voter preference for each candidate to variable 'j' and returns true//
        if (strcmp(candidates[j].name, name) == 0)
        {
            preferences[voter][rank] = j;
            return true;
        }

    }

    return false;
}

// Tabulate votes for non-eliminated candidates
void tabulate(void)
{
    int p;
    for (int h = 0; h < voter_count; h++)  // loops through all voters //
    {
        for (int q = 0; q < candidate_count; q++) // loops through all candidates //
        {
            p = preferences[h][q]; // sets variable 'p' to represent prefenrences for convenience sake //

            if (!candidates[p].eliminated) // checks if candidate is not eliminated //
            {
                // tabulates +1 vote for voter's prefered non-eliminated candidate //
                candidates[p].votes += 1;
                break;
            }

        }

    }

    return;
}

// Print the winner of the election, if there is one
bool print_winner(void)
{

    string winner; // string to store winner name //

    for (int c = 0; c < candidate_count; c++) // loops through all candidates //
    {

        if (candidates[c].votes > (voter_count / 2)) // ensures only candidate with >50% of votes wins //
        {
            winner = candidates[c].name; // stores winner's name is a string called winner for convenience sake //
            printf("%s\n", winner);
            return true;
        }

    }

    return false;
}

// Return the minimum number of votes any remaining candidate has
int find_min(void)
{
    int min;
    for (int i = 0; i < candidate_count; i++) // loops through all "ith" candidates //
    {
        for (int j = 0; j < i; j++) // loops through all "jth" candidates //
        {
            if (candidates[i].votes <= candidates[j].votes && !candidates[i].eliminated && !candidates[j].eliminated)
            {
                // ensures all non-eliminated candidates are compared to find those or the one with lowest value //
                min = candidates[i].votes; // assigns minimum number of votes //

            }
        }

    }
    return min;
}

// Return true if the election is tied between all candidates, false otherwise
bool is_tie(int min)
{
    int tie = 0;

    for (int i = 0; i < candidate_count; i++)
    {
        // if value of "ith" candidate votes is equal to the min value; the value of "tie" does not change //
        if ((candidates[i].votes == min) && !candidates[i].eliminated)
        {
            tie += 0;
        }
        // if the value of "ith" candidate votes is not equal to min, change "tie" by +1 //
        else if ((candidates[i].votes != min) && !candidates[i].eliminated)
        {
            tie += 1;
        }

    }
    if (tie == 0)
    {
        // retruns a value of true only if all candidates votes are equal to min indicating a tie //
        return true;
    }
    return false;


}

// Eliminate the candidate (or candidiates) in last place
void eliminate(int min)
{
    for (int j = 0; j < candidate_count; j++)
    {
        if (candidates[j].votes == min && !candidates[j].eliminated)
        {
            // change bool statement eliminated to true for 'jth' candidate(s), if 'jth' candidate(s) votes is equal to min //
            candidates[j].eliminated = true;
        }
    }
    return;
}

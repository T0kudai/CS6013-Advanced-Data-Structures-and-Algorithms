import numpy as np  # for easymatrix manipulation
import copy  # for creating deep copies of elements

# order = ['_', 'A', 'C', 'G', 'T'] # this order has not been used. hopefully
# the code will work without this using only gaps and mismatch


def calculate_align_penalty(x, y, string1, string2, gap_cost, mismatch_cost):
    """
        This function calculates the penalty for alignment between the two
        given strings, string1 and string2. This function implements the
        Needleman Wunsch dynamic programming algorithm to find the global
        sequence alignment.
        I believe this function fails when the gap penalty and mismatch
        penalty are equal according to my limited testing. In such cases, the solution
        may not be lexicographically correct.

    References:
        Wikipedia
        Algorithm Design by Jon Kleinberg, Eva Tardos
        Stack Overflow and a few other online resources

    Args:
        string1 (string):   Target string to be matched
        string2 (string):   Query string to be matched
        gap_cost (int):     Gap penalty to be added on adding a gap in either string
        mismatch_cost (int):    Mismatch penalty to be added when the column
                                contains characters that are not the same.

    Returns:
        align1 (string):    Target string changed for alignment with the query string.
        align2 (string):    Query string changed for alignment with the target
                            string. This string should be lexicographically smallest.

    """

    string1_length = len(string1)  # find length of both strings
    string2_length = len(string2)

    assert string1_length == x
    # check if length of string provided and actual length of string is equal
    assert string2_length == y

    # table = [[0] * (string2_length + 1)] * (string1_length + 1)
    # table = np.array(table)
    table = np.zeros(
        shape=(x + 1, y + 1), dtype=int
    )  # create a numpy array of size m*n and a additional row and column for 0th element. Initialize it to 0.

    # print (table)

    # Fill first row with gap cost
    for string1_iterator in range(string1_length + 1):
        table[string1_iterator][0] = string1_iterator * gap_cost

    # Fill first column with gap cost
    for string2_iterator in range(string2_length + 1):
        table[0][string2_iterator] = string2_iterator * gap_cost

    # print(table)

    # iterate over rows
    for string1_iterator in range(1, string1_length + 1):
        # print ("char ", string1_iterator)
        # print(string1_iterator, string2[string1_iterator-1])

        # iterate over columns
        for string2_iterator in range(1, string2_length + 1):
            # print(string2_iterator, string1[string2_iterator-1])

            # if characters are same, there is no cost involved
            if string1[string1_iterator - 1] == string2[string2_iterator - 1]:
                table[string1_iterator][string2_iterator] = table[string1_iterator - 1][string2_iterator - 1]

            # when characters are not matched
            else:
                # find the minimum cost from neighbours by either mismatching or
                # by adding a gap
                table[string1_iterator][string2_iterator] = min(
                    table[string1_iterator - 1][string2_iterator - 1] + mismatch_cost,
                    table[string1_iterator - 1][string2_iterator] + gap_cost,
                    table[string1_iterator][string2_iterator - 1] + gap_cost,
                )

            # print(table[string1_iterator][string2_iterator])

    # print(table)

    max_string_length = (string1_length + string2_length)  # max possible string length for both string1 and string2

    string1_iterator = copy.deepcopy(string1_length)  # use a deep copy otherwise python creates reference to same object
    string2_iterator = copy.deepcopy(string2_length)  # we create iterators for both strings that point initially to the end of both strings

    align1_iterator = copy.deepcopy(max_string_length)  # since the length alignment string can be sum of both strings, we created alignment strings
    align2_iterator = copy.deepcopy(max_string_length)  # of size m+n. We point to end of both alignment strings as we will be using backtracking to arrive at our answer.

    align1 = [None] * (max_string_length + 1)  # create a list for alignment of both strings. List is used as it is easy to manipulate
    align2 = [None] * (max_string_length + 1)  # instead of a string

    # keep moving through table until we reach top or left border.If we reach
    # top, we add all elements left of it. If we reach left border, we add all
    # elements on top of it to appropriate string as no other charcater is left
    # in the other string
    while not (string1_iterator == 0 or string2_iterator == 0):

        # firstly check if the characters match. If it is a match, we add
        # characters to both alignment strings and move diagonally up in the table
        if string1[string1_iterator - 1] == string2[string2_iterator - 1]:
            align1[align1_iterator] = string1[string1_iterator - 1]
            align2[align2_iterator] = string2[string2_iterator - 1]
            align1_iterator -= 1
            align2_iterator -= 1
            string1_iterator -= 1
            string2_iterator -= 1

        # conditions below are for cases when characters do not match.

        # to make sure we get lexicographically smallest query string, we check
        # if the value left of current element in the table is possible
        # solution. This will add _ to target string.
        elif (
            table[string1_iterator][string2_iterator - 1] + gap_cost
            == table[string1_iterator][string2_iterator]
        ):
            align1[align1_iterator] = "_"
            align2[align2_iterator] = string2[string2_iterator - 1]
            align1_iterator -= 1
            align2_iterator -= 1
            string2_iterator -= 1

        # This checks the element above the current position in the table.
        # This adds a _ to query string. Since, _ is the smallest
        # possible character this makes sure query string is lexicographically smallest
        elif (
            table[string1_iterator - 1][string2_iterator] + gap_cost
            == table[string1_iterator][string2_iterator]
        ):
            align1[align1_iterator] = string1[string1_iterator - 1]
            align2[align2_iterator] = "_"
            align1_iterator -= 1
            align2_iterator -= 1
            string1_iterator -= 1

        # If the solution cannot be reached by adding a gap to either the target
        # or the query string, we check if the solution is possible by not
        # matching the characters and incurring a mismatch penalty
        elif (
            table[string1_iterator - 1][string2_iterator - 1] + mismatch_cost
            == table[string1_iterator][string2_iterator]
        ):
            align1[align1_iterator] = string1[string1_iterator - 1]
            align2[align2_iterator] = string2[string2_iterator - 1]
            align1_iterator -= 1
            align2_iterator -= 1
            string1_iterator -= 1
            string2_iterator -= 1

    # print(align1)
    # print(align2)
    # while not (string1_iterator == 0 or string2_iterator == 0):

    #     if string1[string1_iterator - 1] == string2[string2_iterator - 1]:
    #         align1[align1_iterator] = string1[string1_iterator - 1]
    #         align2[align2_iterator] = string2[string2_iterator - 1]
    #         align1_iterator -= 1
    #         align2_iterator -= 1
    #         string1_iterator -= 1
    #         string2_iterator -= 1

    #     elif table[string1_iterator][string2_iterator - 1] == min (table[string1_iterator - 1][string2_iterator], table[string1_iterator][string2_iterator-1]):
    #         align1[align1_iterator] = '_'
    #         align2[align2_iterator] = string2[string2_iterator - 1]
    #         align1_iterator -= 1
    #         align2_iterator -= 1
    #         string2_iterator -= 1

    #     elif table[string1_iterator - 1][string2_iterator] == min (table[string1_iterator - 1][string2_iterator], table[string1_iterator][string2_iterator-1]):
    #         align1[align1_iterator] = string1[string1_iterator - 1]
    #         align2[align2_iterator] = '_'
    #         align1_iterator -= 1
    #         align2_iterator -= 1
    #         string1_iterator -= 1

    # At this step we have reached either the top border or the left border of
    # the table. As there are no more comparisions possible, we can directly add
    # remaining elements to alignment string. If we have reached the top left of
    # the table, we just add blanks to alignment strings to truncate them to get
    # the final answer.

    while align1_iterator >= 0:
        if string1_iterator > 0:
            string1_iterator -= 1
            align1[align1_iterator] = string1[string1_iterator]
            align1_iterator -= 1

        else:
            align1[align1_iterator] = "_"
            align1_iterator -= 1

    while align2_iterator >= 0:
        if string2_iterator > 0:
            string2_iterator -= 1
            align2[align2_iterator] = string2[string2_iterator]
            align2_iterator -= 1

        else:
            align2[align2_iterator] = "_"
            align2_iterator -= 1

    # print(align1)
    # print(align2)

    # count the number of blanks in the alignment strings. Since we are filling
    # the alignment string from right to left, the only place where blanks
    # appear is in the front of the strings. We count the number of blanks and
    # remove them to get final alignment strings
    start_index = 0
    for iterator in range(max_string_length):
        if (align1[iterator] == "_" and align2[iterator] == "_") or (
            align1[iterator] == None or align2[iterator] == None
        ):
            continue
        else:
            start_index = iterator
            break

    align1 = "".join(
        align1[start_index:]
    )  # truncate the alignment strings to remove blanks and
    align2 = "".join(
        align2[start_index:]
    )  # join the lists in a string to form a single string to be returned as result

    # since the bottom right of the table contains the optimal cost for
    # alignment, we directly return it as the cost of alignment
    return align1, align2, table[string1_length][string2_length]


if __name__ == "__main__":
    x, y = [int(x) for x in input().strip().split()]
    gap_cost, mismatch_cost = [int(x) for x in input().strip().split()]
    string1 = input()
    string2 = input()
    # x = 6
    # y = 6
    # string1 = "AGTACG"
    # string2 = "ACATAG"

    # gap_cost = 1
    # mismatch_cost = 2
    align1, align2, penalty = calculate_align_penalty(
        x, y, string1, string2, gap_cost, mismatch_cost
    )  # call function to find the alignment
    print(penalty)
    print(align1)
    print(align2)
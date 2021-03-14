import sys
sys.setrecursionlimit(10**6)

def merge_inversions(left, right):
    result = []
    i= 0
    j =0
    inversions = 0

    ll = len(left)
    lr = len(right)
    
    while i < ll and j < lr:
        
        if left[i] <= right[j]:
            result.append(left[i])
            i = i + 1
        
        else:
            result.append(right[j])
            j = j + 1
            inversions = inversions + ll-i
    
    result = result + left[i:]
    result = result + right[j:]
    
    return result,inversions

def count_inversions(array):
    if len(array) <= 1:
        return array, 0
    middle = len(array) // 2
    
    left,inv_left = count_inversions(array[:middle])
    right,inv_right = count_inversions(array[middle:])
    merged, inversions = merge_inversions(left,right)
    
    inversions = inversions + inv_left + inv_right
    return merged, inversions

def inv(array):
    _, inversions = count_inversions(array)
    return inversions

if __name__ == "__main__":
    n = int(input())
    array = [int(x) for x in input().strip().split(" ")]
    inversions = inv(array)
    print(inversions)
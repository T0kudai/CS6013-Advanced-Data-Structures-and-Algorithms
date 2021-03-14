def power(x,y):
    result = 1
    if y==0:
        return 1
    while( y != 0 ):
        result = result * x
        y = y-1
    return result

def nthroot(low,high,n,k):
    if low <= high:
        mid = (low + high) // 2
        if power(mid,k) <= n and power(mid+1,k) > n:
            return mid
        elif power(mid,k) < n:
            return nthroot(mid+1,high,n,k)
        else:
            return nthroot(low, mid-1, n ,k)
    return low


if __name__ == "__main__":
    n = int(input())
    k = int(input())

    result = int(nthroot(0,n,n,k))
    print(result)
    

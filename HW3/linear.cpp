bool allFalse(const string a[], int n)
{
    if (n <= 0)
        return true;
    
    if (somePredicate(a[0]))
        return false;

    return allFalse(a + 1, n - 1); 
}

int countFalse(const string a[], int n)
{
    if (n <= 0)
        return 0;
    
    int count = 0;
    if (!somePredicate(a[0]))
        count = 1;
    
    return count + countFalse(a + 1, n - 1);
}

int firstFalse(const string a[], int n)
{
    if (n <= 0)
        return -1;
    
    if (!somePredicate(a[0]))
        return 0;
    
    int subscript = firstFalse(a + 1, n - 1);
    
    if (subscript == -1) // we've reached the last element of the array
        return -1;
    
    return 1 + subscript;
}

int positionOfLeast(const string a[], int n)
{
    if (n <= 0)
        return -1;
    
    if (n == 1)
        return 0;
    
    int m = positionOfLeast(a, n - 1);
    if (a[n - 1] < a[m])
        return n - 1;
    
    return m;
}

bool has(const string a1[], int n1, const string a2[], int n2)
{
    if (n2 <= 0) // we get through the second array
        return true;
    
    if (n1 <= 0) // we don't get through second array but we get through first
        return false;
    
    if(a1[0] == a2[0])
        return has(a1 + 1, n1 - 1, a2 + 1, n2 - 1);
    
    return has(a1 + 1, n1 - 1, a2, n2);
}

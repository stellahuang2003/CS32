int numberOfTimesHas(const string a1[], int n1, const string a2[], int n2)
{
    if (n2 <= 0)
        return 1; // we've reached the end of the small array
    
    if (n1 <= 1) {
        if (a1[0] == a2[0] && n2 <= 1) // if last element of a1 equals a2's
            return 1;
        else
            return 0;
    }
    
    int has = 0;
    if (a1[0] == a2[0])
        has = numberOfTimesHas(a1 + 1, n1 - 1, a2 + 1, n2 - 1);
    
    return has + numberOfTimesHas(a1 + 1, n1 - 1, a2, n2);
}

// Exchange two strings
void exchange(string& x, string& y)
{
  string t = x;
  x = y;
  y = t;
}

/*
 ie. arr = {c, b, a, d}
 split (arr, 4, arr[0], fNL, fG) would produce:
 arr = {b, a, c, d}, fNL = 2, fG = 3
 */
void split(string a[], int n, string splitter,
                                  int& firstNotLess, int& firstGreater)
{
  if (n < 0)
      n = 0;
    
  firstNotLess = 0;
  firstGreater = n;
  int firstUnknown = 0;
  while (firstUnknown < firstGreater)
  {
      if (a[firstUnknown] > splitter)
      {
          firstGreater--;
          exchange(a[firstUnknown], a[firstGreater]);
      }
      else
      {
          if (a[firstUnknown] < splitter)
          {
              exchange(a[firstNotLess], a[firstUnknown]);
              firstNotLess++;
          }
          firstUnknown++;
      }
  }
}

void order(string a[], int n)
{
    if (n <= 1)
        return;

    int firstNotLess = 0;
    int firstGreater = 0;
    
    split(a, n, a[0], firstNotLess, firstGreater);
    
    order(a, firstNotLess);
    order(a + firstGreater, n - firstGreater);
}

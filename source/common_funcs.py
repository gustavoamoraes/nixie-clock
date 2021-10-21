def digit_at (n, i):
    return int ( (n % pow(10, i+1)) - (n % pow(10, i) ) / pow(10, i) )
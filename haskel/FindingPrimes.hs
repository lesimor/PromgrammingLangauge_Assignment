primeCheck :: Int -> Bool
primeCheck n
  | (n == 1) = False
  | length[digit|digit <- [2..(n-1)], n `mod` digit == 0] == 0 = True
  | otherwise = False

findingPrimes :: Int -> Int -> [Int]
findingPrimes n m = take m [k|k<-[n..], primeCheck k]
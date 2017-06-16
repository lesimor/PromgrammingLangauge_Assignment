indiceCheck :: String -> Int -> Bool
indiceCheck str idxArr
  | idxArr == length str - 1 && str!!idxArr == 'H' = False
  | idxArr == length str - 1 && str!!idxArr == 'T' = True
  | str!!(idxArr + 1) == str!!idxArr = False
  | otherwise = True

flipCoin :: String -> [Int]
flipCoin str = [idxArr + 1 | idxArr <- [0..length str - 1], indiceCheck str idxArr] ++ [0]
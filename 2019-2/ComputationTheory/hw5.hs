module HW5 where
-- 정의하려는 각 함수의 타입 선언문의 주석을 풀고,
-- 그 아래에 각 하수의 정의문(들)을 작성하십시오.


iterApp :: Integral i => (t -> t) -> i -> t -> t
iterApp f n x
    | n >  0  =  iterApp f (n-1) (f x)
    | n == 0  =  x
    | n <  0  =  error "Cannot apply negative times!"


progress :: Integral i => (t -> t) -> i -> t -> [t]
progress f n x
    | n <  0     =  error "Cannot apply negative times!"
    | n == 0     =  [x]
    | otherwise  =  x : progress f (n-1) (f x)


fixpoint :: Eq t => (t -> t) -> t -> t
fixpoint f x
    | x == (f x)    = x
    | otherwise     = fixpoint f (f x)


fixProgress :: Eq t => (t -> t) -> t -> [t]
fixProgress f x
    | x == (f x)    = [x]
    | otherwise     = [x] ++ (fixProgress f (f x))


delSameAdj :: Eq a => [a] -> [a]
delSameAdj (x:x':xs)
    | x == x'    =      delSameAdj (x :xs)
    | otherwise  =  x : delSameAdj (x':xs)
delSameAdj xs    =  xs
-- delSameAdj [x]   =  [x]
-- delSameAdj []    =  []


bubble :: Ord a => [a] -> [a]
bubble (x:x':xs)
    | x < x'    = x : bubble(x':xs)
    | x >= x'   = x' : bubble(x:xs)
bubble xs       = xs


bbSort :: Ord a => [a] -> [a]
bbSort x        = fixpoint bubble x


bbSortProg :: Ord a => [a] -> [[a]]
bbSortProg x    = fixProgress bubble x

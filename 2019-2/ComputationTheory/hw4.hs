module HW4 where
-- 이 파일은 HW4(함수 재귀 Part 1)의 완성판에 대한 코드 파일입니다.


---------------------------------------------------------
-- SECTION 1


iterApp :: Integral i => (t -> t) -> i -> t -> t
iterApp f n x
   | n < 0      = error "Cannot apply negative times!"
   | n == 0     = x
   | otherwise  = iterApp f (n-1) (f x)


iterApp' :: Integral i => (t -> t) -> i -> t -> t
iterApp' f n x
    | n < 0      = error "Cannot apply negative times!"
    | n == 0     = x
    | otherwise  = f (iterApp' f (n-1) x)

iterApp'' :: Integral i => (t -> t) -> i -> t -> t
iterApp'' f n
    | n < 0     = error "Cannot apply negative times!"
    | n == 0    = id 
    | n > 0     = f.(iterApp'' (f) (n-1))


---------------------------------------------------------
-- SECTION 2


progress1 :: Integral i => (t -> t) -> i -> t -> [t]
progress1 f n x
    | n <  0     = error "Cannot apply negative times!"
    | n == 0     = [x]
    | n > 0      = (progress1 f (n-1) x) ++ [(iterApp f n x)]


progress2 :: Integral i => (t -> t) -> i -> t -> [t]
progress2 f n x
    | n <  0     = error "Cannot apply negative times!"
    | n == 0     = [x]
    | n > 0      = x : (progress2 f (n-1) (f x))


progress3 :: Integral i => (t -> t) -> i -> t -> [t]
progress3 f n x
    | n <  0     = error "Cannot apply negative times!"
    | n == 0     = [x]
    | n > 0      = x : (map f (progress3 f (n-1) x))
    -- 아래와 같은 방법도 가능하다.
    -- | n > 0      = x : [ f e | e <- progress3 f (n-1) x ]


progress4 :: Integral i => (t -> t) -> i -> t -> [t]
progress4 f n x
    | n <  0     = error "Cannot apply negative times!"
    | otherwise  = [g x | g <- map (iterApp f) [0..n]]
    -- | otherwise  = map (iterApp f flip x) [0..n]


progress :: Integral i => (t -> t) -> i -> t -> [t]
progress = progress2     -- progress1 ~ progress4 중에서 택1 하셔요.


-- progress0 :: Integral i => (t -> t) -> i -> t -> [t]   -- incorrect
progress0 :: (t -> t) -> Int -> t -> [t]                  --   correct
progress0 f n x  =  take (n+1) (iterate f x)

progress0' :: (t -> t) -> Int -> t -> [t]
progress0' f n x
    | n < 0      =  error "Cannot apply negative times!"
    | otherwise  =  take (n+1) (iterate f x)

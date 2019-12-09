-- 6차 숙제: 위치 기수법 (Positional Notation)

module HW6 where

---------------------------------------------------------------------
--    숫자 (DIGITS)

-- data Digit = D0 | D1 | D2 | D3 | D4 | D5 | D6 | D7
data Digit = Zero | One | Two | Three | Four | Five | Six | Seven | Eight | Nine
-- data Digit = Zero | One | Two | Three | Four | Five | Six | Seven | Eight | Nine
--            | Ten | Eleven | Twelve | Thirteen | Fourteen | Fifteen
    deriving (Show, Eq, Ord, Bounded, Enum)

digitCount  =  length [(minBound::Digit)..maxBound]

digitCharCandids  =  ['0'..'9'] ++ ['A'..'Z']

digitChars  =  take digitCount digitCharCandids


-- 문제
nextD :: Digit -> Digit
nextD x
    | x == maxBound     = minBound
    | otherwise         = succ x


-- 문제
prevD :: Digit -> Digit
prevD x
    | x == minBound     = maxBound
    | otherwise         = pred x


---------------------------------------------------------------------
--    캐리 (CARRY)

-- True면 캐리가 발생한 것, False면 캐리가 없는 것
type Carry = Bool


-- 문제
addDd :: Carry -> Digit -> Digit -> Digit
addDd c x y
    | c == True         = addDd False (nextD x) y
    | y > minBound      = addDd c (nextD x) (prevD y)
    | y == minBound     = x


-- 문제
addDc :: Carry -> Digit -> Digit -> Carry
addDc c x y
    | x == maxBound && (y > minBound || c == True)     = True
    | c == True         = addDc False (nextD x) y
    | y > minBound      = addDc c (nextD x) (prevD y)
    | otherwise         = False


-- For Testing
testAdd tc  =  mapM_ putStrLn [ f cr d1 d2 | (cr,d1,d2) <- tc ] where
    f c x y  =  g c ++ showD x ++ " + " ++ showD y ++ " = " ++
                h (addDc c x y) ++ showD (addDd c x y)
    g c      =  if c then showD (succ minBound) ++ " + " else "    "
    h c      =  if c then showD (succ minBound) else " "
    showD :: Digit -> String    -- 없으면 타입 자동 추론 불가
    showD d  =  [digitChars !! fromEnum d]

tc1  =  [ (c,x,y) | c <- [False, True],
                    x <- [Zero, Three, Six, Nine],
                    y <- [Zero, Three, Six, Nine]]

-- tc2  =  [ (c,x,y) | c <- [False, True],
--                     x <- [Zero, Five, Ten, Fifteen],
--                     y <- [Zero, Five, Ten, Fifteen]]

-- tc3  =  [ (c,x,y) | c <- [False, True],
--                     x <- [Zero, One],
--                     y <- [Zero, One]]



---------------------------------------------------------------------
--    자연수 (NATURAL NUMBERS)

-- 자연수란 "숫자들의 리스트"의 별칭일 뿐
-- 리스트 속의 숫자 순서는 일상적인 위치 기수법 순서 대로 나열
type Nat = [Digit]

showN :: Nat -> String
showN   []    =  ""
showN (d:ds)  =  digitChars !! fromEnum d : showN ds

readN :: String -> Nat
readN   ""    =  []
readN (c:cs)  =  toEnum (fstIdx c digitChars) : readN cs where
    fstIdx x (y:ys)
        | x == y     =  0
        | otherwise  =  1 + fstIdx x ys
    fstIdx _   _     =  error "Cannot be parsed as Natural number"


normalize :: Nat -> Nat
normalize (d:ds)
    | d == minBound  =  normalize ds
    | otherwise      =  d:ds
normalize   []       =  [minBound]


incN :: Nat -> Nat
incN  =  reverse . fn . reverse  where
    fn (d:ds)
        | d == maxBound  =  minBound : fn ds
        | otherwise      =  succ d : ds
    fn   []              =  [succ minBound]

decN :: Nat -> Nat
decN  =  reverse . fn . reverse  where
    fn (d:ds)
        | d == minBound  =  maxBound : fn ds
        | otherwise      =  pred d : ds
    fn   []              =  error "Cannot decrease more"


isNull, isMono :: Nat -> Bool
isNull n  =  all (minBound ==) n

-- isMono n  =  f (reverse n)  where
--     f   []    =  False
--     f (d:ds)  =  d == succ minBound && isNull ds

-- isMono n  =  not (isNull n) && isNull (init n) && last n == succ minBound

isMono n  =  normalize n == [succ minBound]

leN :: Nat -> Nat -> Bool
leN n m  =  fn (normalize n) (normalize m)  where
    fn x y  =  length x < length y || (length x == length y && x <= y)


addNslow :: Nat -> Nat -> Nat
addNslow x y  =  if leN x y then add x y else add y x  where
    add a b
        | isNull a   =  b
        | otherwise  =  add (decN a) (incN b)


-- 문제
addNfast :: Nat -> Nat -> Nat
addNfast x y  =  if leN x y then reverse (add (reverse x) (reverse y) False) else reverse (add (reverse y) (reverse x) False) where
    add (a:as) (b:bs) c     = (addDd c a b) : (add as bs (addDc c a b))
    add a b c
        | isNull a && c             = reverse (incN (reverse b))
        | isNull a                  = b


-- 문제 (작업)
addN :: Nat -> Nat -> Nat
addN  =  addNfast      -- addNslow 혹은 addNfast


mulNslow :: Nat -> Nat -> Nat
mulNslow x y  =  if leN x y then mul x y [minBound] else mul y x [minBound]  where
    mul a b acc
        | isNull a   =  acc
        | otherwise  =  mul (decN a) b (addN b acc)


-- 문제
mulNfast :: Nat -> Nat -> Nat
mulNfast x y  =  if leN x y then mul x y [minBound] else mul y x [minBound]  where
    mul (a:as) b acc    = mul as b ((addN acc (mulNslow [a] b)) ++ [minBound])
    mul a b acc         = init acc


mulN :: Nat -> Nat -> Nat
mulN  =  mulNslow      -- mulNslow 혹은 mulNfast

facN :: Nat -> Nat
facN n  =  f n [succ minBound]  where
    f x acc
        | isNull x   =  acc
        | otherwise  =  f (decN x) (mulN x acc)

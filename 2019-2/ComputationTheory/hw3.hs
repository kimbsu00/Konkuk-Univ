module HW3 where

-- 패턴 매칭 (설명 예시 코드)

f :: Integer -> Bool
f 3  =  True
f 5  =  False

f' :: Integer -> Bool
f' 3  =  True
f' n  =  False

f'' :: Integer -> Bool
f'' n  =  False
f'' 3  =  True



fac :: Integer -> Integer
-- 윗줄의 주석을 풀고, 여기에 fac 함수를 정의하셔요.
fac 0 = 1
fac n = n * fac(n-1)


rev :: [a] -> [a]
-- 윗줄의 주석을 풀고, 여기에 rev 함수를 정의하셔요.
rev [] = []
rev (x : xs) = rev(xs) ++ [x]

-- 타입 정의 (설명 예시 코드)

data List t  =  EmptyList | ListCons t (List t)

data Nat  =  Z
          |  S Nat
    deriving Show

toNat :: Integral i => i -> Nat
toNat 0          =  Z
toNat n | n > 0  =  S (toNat (n-1))



fromNat :: Integral i => Nat -> i
-- 윗줄의 주석을 풀고, 여기에 fromNat 함수를 정의하셔요.
fromNat Z = 0
fromNat (S n) = 1 + fromNat(n)


addNat :: Nat -> Nat -> Nat
-- 윗줄의 주석을 풀고, 여기에 addNat 함수를 정의하셔요.
addNat (Z) (n) = n
addNat (n) (Z) = n
addNat (S n) (S m) = S (S (addNat (n) (m)))

-- 아래 1줄의 코드는 재귀호출 횟수를 고려하기 전의 코드입니다.
-- addNat (S n) (m) = S (addNat (n) (m))


mulNat :: Nat -> Nat -> Nat
-- 윗줄의 주석을 풀고, 여기에 mulNat 함수를 정의하셔요.
mulNat (Z) (n) = Z
mulNat (n) (Z) = Z
mulNat (S Z) (n) = n
mulNat (n) (S Z) = n
mulNat (S n) (S m) = addNat((addNat (S Z) (addNat n m))) (mulNat n m)

-- 아래 3줄의 코드는 실패한 코드입니다.
-- mulNat (Z) (n) = Z
-- mulNat (S Z) (n) = n
-- mulNat (S n) (m) = mulNat (n) (addNat (m) (m))


facNat :: Nat -> Nat
-- 윗줄의 주석을 풀고, 여기에 facNat 함수를 정의하셔요.
facNat Z = S Z
facNat (S n) = mulNat (S n) (facNat n)

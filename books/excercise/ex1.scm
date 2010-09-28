;;
;; Exercise 1.1
;;
10
; => 10

(+ 5 3 4)
; => 12

(- 9 1)
; => 8

(/ 6 2)
; => 3

(+ (* 2 4) (- 4 6))
; => 6

(define a 3)
; => a

(define b (+ a 1))
; => b

(+ a b (* a b))
; => 19

(= a b)
; => #f

(if (and (> b a) (< b (* a b)))
    b
    a)
; => 4

(cond ((= a 4) 6)
      ((= b 4) (+ 6 7 a))
      (else 25))
; => 16

(+ 2 (if (> b a) b a))
; => 6


(* (cond ((> a b) a)
         ((< a b) b)
         (else -1))
   (+ a 1))
; => 16

;;
;; Exercise 1.2
;;

(/ (+ 5
      4
      (- 2 (- 3 (+ 6 (/ 4 5)))))
   (* 3
      (- 6 2)
      (- 2 7)))

;;
;; Exercise 1.3
;;

; 自分で考えた解答
(define (sum-of-squares-of-two-larger a b c)
  (define (two-larger x y z)
    (if (> x y) 
        (list x (if (> y z) y z))
        (list y (if (> x z) x z))))
  (define (square x) (* x x))
  (define (sum-of-squares x y) (+ (square x) (square y)))
  (apply sum-of-squares (two-larger a b c)))

; http://www.kendyck.com/archives/2005/03/13/solution-to-sicp-exercise-13/ 
(define (sum-of-squares-of-two-larger a b c)
  (define (square x) (* x x))
  (define (sum-of-squares x y) (+ (square x) (square y)))
  (cond ((and (<= a b) (<= a c)) (sum-of-squares b c))
        ((and (<= b a) (<= b c)) (sum-of-squares a c))
        (else (sum-of-squares a b))))

;;
;; Exercise 1.4
;;

(define (a-plus-abs-b a b)
  ((if (> b 0) + -) a b))

; (a-plus-abs-b 3 -4)
; ((if (> -4 0) + -) 3 -4))
; ((if (> -4 0) + -) 3 -4))
; (- 3 -4)
; 7

;;
;; Exercise 1.5
;;

(define (p) (p))

(define (test x y)
  (if (= x 0)
      0
      y))

(test 0 (p))

;
; applicative-order evaluation => infinite loop
; (p) が評価されると自分自身を評価して無限ループになってしまうため。
;
; normal-order => 0
; normal-order では (p) は評価されない。
;

;;
;; Exercise 1.6
;;

(define (new-if predicate then-clause else-clause)
  (cond (predicate then-clause)
        (else else-clause)))
;
; new-if は special form ではなく procedure なので predicate の
; 結果によらず then-clause, else-clause が評価される。
;

(define (square x) (* x x))

(define (improve guess x)
  (average guess (/ x guess)))

(define (average x y)
  (/ (+ x y) 2))

(define (good-enough? guess x)
  (< (abs (- (square guess) x)) 0.001))

(define (sqrt-iter guess x)
  (new-if (good-enough? guess x)
          guess
          (sqrt-iter (improve guess x)
                     x)))
;
; new-if が評価される前に sqrt-iter が評価されるために無限ループしてしまう。
;

;;
;; Excercise 1.7
;;

;
; 非常に小さい数だと精度が悪くなる。
; 非常に大きい数だと収束しなくなる。
;
; good-enough? を変更する。
;

(define (square x) (* x x))

(define (improve guess x)
  (average guess (/ x guess)))

(define (average x y)
  (/ (+ x y) 2))

(define (good-enough? guess prev-guess)
  (< (/ (abs (- guess prev-guess))
        guess)
     0.001))

(define (sqrt-iter guess prev-guess x)
  (if (good-enough? guess prev-guess)
      guess
      (sqrt-iter (improve guess x) guess x)))

(define (sqrt x)
  (sqrt-iter 1.0 0.0 x))

; (sqrt (square 0.001))
; => 0.0010000001533016628

; (sqrt 1e13)
; => 3162277.6640104805

;;
;; Excercise 1.8
;;


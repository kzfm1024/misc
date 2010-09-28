;;
;; Excercise 3.1
;;

(define (make-accumulator sum)
  (lambda (x)
    (set! sum (+ sum x))
    sum))

(define A (make-accumulator 5))
(A 10)
(A 10)


;;
;; Excercise 3.2
;;

(define (make-monitored f)
  (let ((count 0))
    (define (dispatch m)
      (cond ((eq? m 'how-many-calls?) count)
            ((eq? m 'reset-count) (begin (set! count 0) count))
            (else (begin (set! count (+ count 1)) (f m)))))
    dispatch))

(define s (make-monitored sqrt))
(s 100)
(s 'how-many-calls?)


;;
;; Excercise 3.3
;;

(define (make-account balance password)
  (define (withdraw amount)
    (if (>= balance amount)
        (begin (set! balance (- balance amount))
               balance)
        "Insufficient funds"))
  (define (deposit amount)
    (set! balance (+ balance amount))
    balance)
  (define (incorrect-password amount) "Incorrect password")
  (define (dispatch pw m)
    (if (eq? pw password)
        (cond ((eq? m 'withdraw) withdraw)
              ((eq? m 'deposit) deposit)
              (else (error "Unknown request -- MAKE-ACCOUNT"
                           m)))
        incorrect-password))
  dispatch)

(define acc (make-account 100 'secret-password))
((acc 'secret-password 'withdraw) 40)
((acc 'some-other-password 'deposit) 50)


;;
;; Excercise 3.4
;;

(define (make-account balance password)
  (let ((incorrect-count 0)
        (incorrect-limit 7))
    (define (withdraw amount)
      (if (>= balance amount)
          (begin (set! balance (- balance amount))
                 balance)
          "Insufficient funds"))
    (define (deposit amount)
      (set! balance (+ balance amount))
      balance)
    (define (dispatch2 m)
      (set! incorrect-count 0) 
      (cond ((eq? m 'withdraw) withdraw)
            ((eq? m 'deposit) deposit)
            (else (error "Unknown request -- MAKE-ACCOUNT"
                         m))))
    (define (incorrect-password amount)
      (set! incorrect-count (+ incorrect-count 1))
      (if (< incorrect-count incorrect-limit)
          "Incorrect password"
          (call-the-cops)))
    (define (call-the-cops) "Call the cops")
    (define (check-password pw m)
      (if (eq? pw password)
          (dispatch2 m)
          incorrect-password))
    check-password))

(define acc (make-account 100 'secret-password))
((acc 'secret-password 'withdraw) 40)
((acc 'some-other-password 'deposit) 1)
((acc 'some-other-password 'deposit) 2)
((acc 'some-other-password 'deposit) 3)
((acc 'some-other-password 'deposit) 4)
((acc 'some-other-password 'deposit) 5)
((acc 'some-other-password 'deposit) 6)
((acc 'some-other-password 'deposit) 7)
((acc 'secret-password 'withdraw) 30)


;;
;; Excercise 3.5
;;

(use srfi-27)

(define (random-init)
  (random-source-randomize! default-random-source))

(define (random-in-range low high)
  (let ((range (- high low)))
    (+ low (* range (random-real)))))

(define (monte-carlo trials experiment)
  (define (iter trials-remaining trials-passed)
    (cond ((= trials-remaining 0)
           (/. trials-passed trials))
          ((experiment)
           (iter (- trials-remaining 1) (+ trials-passed 1)))
          (else
           (iter (- trials-remaining 1) trials-passed))))
  (iter trials 0))

(define (square x) (* x x))

(define (sum-of-squares x y) (+ (square x) (square y)))

(define (within-the-circle x y)
  (let ((x0 5)
        (y0 7)
        (r 3))
    (< (sum-of-squares (- x x0) (- y y0)) (square r))))

(define (estimate-integral pred x1 x2 y1 y2 trials)
  (define (experiment)
    (pred (random-in-range x1 x2) (random-in-range y1 y2)))
  (random-init)
  (* (monte-carlo trials experiment) (* (- x2 x1) (- y2 y1))))

(estimate-integral within-the-circle 2 8 4 10 10000000)


;;
;; Excercise 3.6
;;

(use math.mt-random)

(define *mt* (make <mersenne-twister>))

(define (rand msg)
  (cond ((eq? msg 'reset)
         (lambda (seed) (mt-random-set-seed! *mt* seed)))
        ((eq? msg 'generate) (mt-random-real *mt*))
        (else (error "Unknown request -- RAND" msg))))

((rand 'reset) 12345)
(rand 'generate)
(rand 'generate)
(rand 'generate)

((rand 'reset) 12345)
(rand 'generate)
(rand 'generate)
(rand 'generate)


;;
;; Excercise 3.7
;;

(define (make-joint account password new-password)
  (define (incorrect-password amount) "Incorrect password")
  (define (dispatch pw m)
    (if (eq? pw new-password)
        (cond ((eq? m 'withdraw) (account password m))
              ((eq? m 'deposit) (account password m))
              (else (error "Unknown request -- MAKE-JOINT"
                           m)))
        incorrect-password))
  dispatch)

(define peter-acc (make-account 100 'open-sesame))
(define paul-acc (make-joint peter-acc 'open-sesame 'rosebud))
((peter-acc 'open-sesame 'withdraw) 40)
((paul-acc 'rosebud 'withdraw) 30)
((peter-acc 'open-sesame 'deposit) 10)
((paul-acc 'rosebud 'deposit) 10)


;;
;; Excercise 3.8
;;
  
(define *prev-value* 0)

(define (f x)
  (let ((val *prev-value*))
    (set! *prev-value* x)
    val))


;;
;; Excercise 3.9
;;

(define choose-fail #f)

(define (initialize-choose-fail)
  (call/cc
    (lambda (cc)
      (set! choose-fail
            (lambda ()
              (cc 'no-choice))))))

(define (choose . ls)
  (if (null? ls)
      (choose-fail)
      (let ((choose-fail0 choose-fail))
        (call/cc
         (lambda (cc)
           (set! choose-fail
                 (lambda ()
                   (set! choose-fail choose-fail0)
                   (cc (apply choose (cdr ls)))))
           (cc (car ls)))))))

;----------------------------------------------------------------------

(define (make-state boat-pos numbers-left) (cons boat-pos numbers-left))

(define (boat-side state) (car state))

(define (numbers-left-side state) (cdr state))

(define (move-left state move)
  (make-state 'L (map + (numbers-left-side state) move)))
              
(define (move-right state move)
  (make-state 'R (map - (numbers-left-side state) move)))

(define (cross-river initial-state
                     move-list
                     goal-test? safe-test?)

  (define (cross state history move1 move2)
    (and-let* ((m (apply choose move-list))
               ((not (eq? m 'no-choice)))
               (next-state (move1 state m)))
      (cond ((goal-test? next-state) (reverse (cons next-state history)))
            ((member next-state history) (choose))
            ((safe-test? initial-state next-state)
             (cross next-state (cons next-state history) move2 move1))
            (else (choose)))))

  (initialize-choose-fail)
  (cross initial-state (list initial-state) move-right move-left))

;----------------------------------------------------------------------

(define (all test?)
  (lambda (lst)
    (or (null? lst)
        (and (test? (car lst))
             ((all test?) (cdr lst))))))

(define (not-negative? x) (not (negative? x)))

(define (true? x) (if x #t #f))

(define all-zero? (all zero?))

(define all-not-negative? (all not-negative?))

(define all-true? (all true?))

(define (all-less-than-equal? lst1 lst2)
  (all-true? (map (lambda (x y) (<= x y)) lst1 lst2)))

;----------------------------------------------------------------------

#|
３匹ずつの狼と小鳥を、すべて向こう岸に渡せ。
ただし、以下の条件がある。

- いかだに乗れるのは２匹まで。
- １匹も乗っていないと動かない。
- どちらの岸でも狼が小鳥の数より多くなると、小鳥が食べられて失敗する。

回数に制限はないが、最短は１１回だ。
|#

; ステートは L, R を区別する必要がある
;
; 状態
; ボートの位置　'L or 'R
; 左岸の狼の数
; 左岸の小鳥の数
;

(define (goal? state)
  (and (eq? (boat-side state) 'R)
       (all-zero? (numbers-left-side state))))

(define (possible? initial-state state)
  (let ((n0 (numbers-left-side initial-state))
        (n (numbers-left-side state)))
    (and (all-not-negative? n)
         (all-less-than-equal? n n0))))

(define (num-wolves side) (car side))

(define (num-birds side) (cadr side))

(define (safe? initial-state state)
  (let* ((l (numbers-left-side state))
         (r (map - (numbers-left-side initial-state) l)))
    (and (or (= (num-birds l) 0)
             (<= (num-wolves l) (num-birds l)))
         (or (= (num-birds r) 0)
             (<= (num-wolves r) (num-birds r))))))

(define (possible-and-safe? initial-state state)
  (and (possible? initial-state state)
       (safe? initial-state state)))

(define *initial-state* (make-state 'L '(3 3)))

(define *move-list* '((1 1) (1 0) (0 1) (2 0) (0 2)))
         
(print
 (cross-river *initial-state*
              *move-list*
              goal? possible-and-safe?))

;----------------------------------------------------------------------

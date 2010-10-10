#|
３匹ずつの狼と小鳥を、すべて向こう岸に渡せ。
ただし、以下の条件がある。

- いかだに乗れるのは２匹まで。
- １匹も乗っていないと動かない。
- どちらの岸でも狼が小鳥の数より多くなると、小鳥が食べられて失敗する。

回数に制限はないが、最短は１１回だ。
|#

(define amb-fail #f)

(define (initialize-amb-fail)
  (call/cc
    (lambda (cc)
      (set! amb-fail
            (lambda ()
              (cc 'no-more-values))))))

;;; nondeterminsm macro operator
(define-syntax amb
  (syntax-rules ()
    ((_) (amb-fail))
    ((_ a) a)
    ((_ a b ...)
     (let ((amb-fail0 amb-fail))
       (call/cc
         (lambda (cc)
           (set! amb-fail
                 (lambda ()
                   (set! amb-fail amb-fail0)
                   (cc (amb b ...))))
           (cc a)))))))

(define (make-state boat num-wolves num-birds) (list boat num-wolves num-birds))

(define (boat state) (car state))

(define (num-wolves state) (cadr state))

(define (num-birds state) (caddr state))

(define (wolves-and-birds-on-this-side wb-on-board)
  (define (to-other-side wb on-board)
    (if (null? on-board)
        wb
        (to-this-side
         (make-state
          'OTHER-SIDE
          (- (num-wolves wb) (car (car on-board)))
          (- (num-birds wb) (cdr (car on-board))))
         (cdr on-board))))
  (define (to-this-side wb on-board)
    (if (null? on-board)
        wb
        (to-other-side
         (make-state
          'THIS-SIDE
          (+ (num-wolves wb) (car (car on-board)))
          (+ (num-birds wb) (cdr (car on-board))))
         (cdr on-board))))
  (to-other-side (make-state 'THIS-SIDE 3 3) wb-on-board))

(define (end? state)
  (and (eq? (boat state) 'OTHER-SIDE)
       (= (num-wolves state) 0)
       (= (num-birds state) 0)))

(define (start? state)
  (and (eq? (boat state) 'THIS-SIDE)
       (= (num-wolves state) 3)
       (= (num-birds state) 3)))

(define (valid? state)
  (and (>= (num-wolves state) 0)
       (<= (num-wolves state) 3)
       (>= (num-birds state) 0)
       (<= (num-birds state) 3)))

(define (safe? state)
  (or (= (num-wolves state) (num-birds state))
      (= (num-birds state) 0)
      (= (num-birds state) 3)))

(define (solve)
  (define (loop wb-log wb-on-board)
    (let ((wb (wolves-and-birds-on-this-side #?=wb-on-board)))
      (print wb)
      (cond ((end? wb) wb-on-board)
            ((start? wb) (amb))
            ((not (valid? wb)) (amb))
            ((member wb wb-log) (amb))
            ((safe? wb) (loop (append wb-log (list wb))
                              (append wb-on-board
                                      (list (amb '(1 . 1) '(1 . 0) '(0 . 1) '(2 . 0) '(0 . 2))))))
            (else (amb)))))
  (initialize-amb-fail)
  (loop (list '(3 . 3)) (list (amb '(1 . 1) '(1 . 0) '(0 . 1) '(2 . 0) '(0 . 2)))))

(print (solve))
; => ((1 . 1) (0 . 1) (2 . 0) (1 . 0) (0 . 2) (1 . 1) (0 . 2) (1 . 0) (2 . 0) (1 . 0) (2 . 0))

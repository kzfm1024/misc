;;
;; 22.4 に出てきた Scheme ソースコード
;;

(define old-cc #f)

(+ 1 (call/cc (lambda (cc)
               (set! old-cc cc)
               (+ 20 (cc 300)))))

(old-cc 500)

;;
;; amb
;;
(use srfi-27)

(random-source-randomize! default-random-source)

(define (random n) (random-integer n))

(define-macro (amb x y)
  `(random-choice (lambda () ,x) (lambda () ,y)))

(define backtrack-points '())

(define (fail)
  (let ((last-choice (car backtrack-points)))
    (set! backtrack-points (cdr backtrack-points))
    (last-choice)))

(define (random-choice f g)
  (if (= 1 (random 2))
      (choose-first f g)
      (choose-first g f)))

(define (choose-first f g)
  (call/cc
   (lambda (k)
     (set! backtrack-points
           (cons (lambda () (k (g))) backtrack-points))
     (f))))

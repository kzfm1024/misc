(use srfi-1)
(use util.combinations)

(define *grid* (cartesian-product (list (iota 5) (iota 5))))

(define (rows-of circles)
  (map (lambda (p) (car p)) circles))

(define (cols-of circles)
  (map (lambda (p) (cadr p)) circles))

(define (OK? circles)
  (and (lset= eqv? '(0 1 2 3 4) (rows-of circles))
       (lset= eqv? '(0 1 2 3 4) (cols-of circles))))

(combinations-for-each
 (lambda (circles)
   (when (OK? circles) (print circles)))
 *grid* 6)

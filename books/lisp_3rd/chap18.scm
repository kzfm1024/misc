(define (m-append l1 l2)
  (if (null? l1)
      l2
      (cons (car l1) (m-append (cdr l1) l2))))

(apply (lambda (x y) (cons x (cons y '()))) '(a b))

(apply (lambda (x y)
         (apply (lambda (l m) (cons l (cons m '())))
                (cons x (cons y '()))))
       '(a b))

(apply (lambda (x y)
         (apply (lambda () (cons x (cons y '()))) '()))
       '(a b))

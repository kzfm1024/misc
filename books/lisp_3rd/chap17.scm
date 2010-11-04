(define (user-reverse l)
  (define (reverse-iter l result)
    (if (null? l)
        result
        (reverse-iter (cdr l) (cons (car l) result))))
  (reverse-iter l '()))

(define (user-defined-reverse! l)
  (define (reverse-iter l reversed-list)
    (if (null? l)
        reversed-list
        (let ((remaining-elements (cdr l)))
          (set-cdr! l reversed-list)
          (reverse-iter remaining-elements l))))
  (reverse-iter l '()))

                                 
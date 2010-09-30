(use srfi-1)

(define ls '((17 . duck) (15 . goose) (41 . quail) (12 . goose) (37 . quail)))

(define (make-pair num animal) (cons num animal))

(define (get-number pair) (car pair))

(define (get-animal pair) (cdr pair))

(define (remove-same-animal animal ls)
 (filter (lambda (p) (not (eq? (get-animal p) animal))) ls))

(define (sum-same-animal animal ls)
 (apply
  +
  (map (lambda (p) (get-number p))
       (filter (lambda (p) (eq? (get-animal p) animal)) ls))))

(define (collect ls)
(cond ((null? ls) '())
      (else (let ((a (get-animal (car ls)))
                  (n (get-number (car ls))))
              (cons (make-pair a (+ n (sum-same-animal a (cdr ls))))
                    (collect (remove-same-animal a (cdr ls))))))))

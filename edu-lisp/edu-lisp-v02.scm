;;教育用LISPインタープリタ　Ver0.2
(define (atom? s)
  (not (pair? s)))

(define *env* '((T T)(NIL NIL)))


(define (repl)
  (init)
  (let loop ((e (read)))
    (if (equal? e '(quit))
        'done
        (begin (print (ed-eval e *env*))
               (prompt)
               (loop (read))))))

(define (init)
  (begin 
    (display "Pure LISP for education")(newline)
    (prompt)))

(define (print s)
  (display s)
  (newline))

(define (prompt)
  (display "L> ")
  (flush)) ; 改行コードが表示されないとフラッシュされないようなので追加

;; 使ってない
#;(define (among x y)
  (and (not(null? y))
       (equal? x (car y))
       (among x (cdr y))))

(define (pair x y)
  (cond ((and (null? x)(null? y)) '())
        ((and (not (atom? x))(not (atom? y))) 
         (cons (list (car x) (car y))
               (pair (cdr x) (cdr y))))))

(define (ed-assoc x y)
  (cond ((null? y) 'NIL) ;追加
        ((eq? (caar y) x) (cadar y))
        (else (ed-assoc x (cdr y)))))

;; sub2, sublis は使ってない
#|
(define (sub2 x z)
  (cond ((null? x) z)
        ((eq? (caar x) z) (cadar x))
        (else (sub2 (cdr x) z))))

(define (sublis x y)
  (cond ((atom? y) (sub2 x y))
        (else (cons (sublis x (car y))
                    (sublis x (cdr y))))))
|#

(define (ed-eval e a)
  (cond ((atom? e)
         (cond ((number? e) e)
               ((string? e) e)
               ((symbol? e) (ed-assoc e a))))
        ((atom? (car e))
         (cond ((eq? (car e) 'quote) (cadr e))
               ((eq? (car e) 'cond) (evcon (cdr e) a))
               ((eq? (car e) 'set!) (ed-set! (cadr e) (ed-eval (caddr e) a)))
               ((eq? (car e) 'define) (ed-set! (cadr e) (caddr e)))
               ((function? (car e)) (apply (function (car e)) (evlis (cdr e) a))) ;Ver0.1で追加
               (else (ed-eval (cons (ed-assoc (car e) a)
                                    (evlis (cdr e) a))
                              a))))
        ((eq? (caar e) 'labal) (ed-eval (cons (caddar e) (cdr e))
                                        (cons (list (cadar e)
                                                    (car e))
                                              a)))
        ((eq? (caar e) 'lambda) (ed-eval (caddar e)
                                         (append (pair (cadar e)
                                                       (evlis (cdr e) a))
                                                 a))))
  )

(define (function? e)
  (assoc e func-symbol))

;; (interaction-environment) を追加
(define (function e)
  (eval (cdr (assoc e func-symbol)) (interaction-environment)))

(define func-symbol
  '((+ . +)
    (- . -)
    (* . *)
    (/ . /)
    (sin . sin)
    (cos . cos)
    (tan . tan)
    (asin . asin)
    (acos . acos)
    (atan . atan)
    (car . car)
    (cdr . ed-cdr)
    (cons . cons)
    (atom? . ed-atom)
    (eq? . ed-eq)))

(define (evcon c a)
  (cond ((not (eq? (ed-eval (caar c) a) 'NIL)) (ed-eval (cadar c) a))
        (else (evcon (cdr c) a))))

(define (evlis m a)
  (cond ((null? m) ())
        (else (cons (ed-eval (car m) a)
                    (evlis (cdr m) a)))))

(define (ed-cdr m)
  (cond ((null? m) 'NIL)
        ((null? (cdr m))'NIL)
        (else (cdr m))))

(define (ed-atom x)
  (if (atom? x)
      'T
      'NIL))

(define (ed-eq x y)
  (if (eq? x y)
      'T
      'NIL))

(define (ed-set! x y)
  (set! *env* (cons (list x y) *env*))
  y)

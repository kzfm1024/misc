(define non (lambda (x) 'non))

(define add1 (lambda (n) (+ n 1)))
(define sub1 (lambda (n) (- n 1)))

(define ++ (lambda (i) (+ i 1)))
(define -- (lambda (i) (- i 1)))

(define ** (lambda (a b) (expt a b)))

(define // (lambda (a b) (quotient a b)))
(define /@ (lambda (a b) (remainder a b)))
(define /: (lambda (a b) (modulo a b)))

(define make-even (lambda (n) (* 2 n)))
(define make-odd (lambda (n) (+ (* 2 n) 1)))

(define parity-of
  (lambda (p)
    (if (odd? p) -1 1)))

(define swap-test
  (lambda (a b)
    (** a b)))

(define swap-test
  (lambda (a b)
    (let* ((dummy a) (a b) (b dummy))
      (** a b))))

(define adjust-of
  (lambda (x)
    (let ((digit 1000)
          (slide (if (positive? x) 1/2 -1/2)))
      (/ (truncate (+ slide (* digit x))) digit))))

(define iota
  (lambda (min max)
    (if (> min max)
        '()
        (cons min (iota (++ min) max)))))

(define iota-reverse
  (lambda (min max)
    (if (> min max)
        '()
        (cons max (iota-reverse min (-- max))))))

(define iota
  (lambda (min max)
    (let iota-loop ((i max) (tmp '()))
      (if (< i min)
          tmp
          (iota-loop (-- i) (cons i tmp))))))

(define iota-reverse
  (lambda (min max)
    (let iota-loop ((i min) (tmp '()))
      (if (> i max)
          tmp
          (iota-loop (++ i) (cons i tmp))))))

(define iota
  (lambda (max . opt)
    (let* ((min (if (null? opt) 1 (car opt)))
           (step (if (or (null? opt) (null? (cdr opt)))
                     1 (cadr opt)))
           (dummy max)
           (max (if (null? opt) max min))
           (min (if (null? opt) min dummy)))
      (let loop ((i (- min step)) (tmp '()))
        (if (< (- max step) i)
            (reverse tmp)
            (loop (+ i step)
                  (cons (adjust-of (+ i step)) tmp)))))))
        
    

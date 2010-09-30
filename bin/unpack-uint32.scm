;;
;;  $ gosh unpack-uint32.scm -1 -2147418110
;;   

(use binary.pack)

(define (unpack-uint32 n)
  (car (unpack "L" :from-string (pack "l" (list n) :to-string? #t))))

(define (print-list ls)
  (cond [(null? ls) (newline)]
        [else (display (car ls))
              (display " ")
              (print-list (cdr ls))]))

(define (main args)
  (print-list
   (map
    (lambda (x) (number->string (unpack-uint32 (string->number x)) 16))
    (cdr args)))
  0)

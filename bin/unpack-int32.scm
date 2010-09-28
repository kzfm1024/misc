;;
;;  $ gosh unpack-int32.scm "#xffffffff" "#xfffffffe" "#xfffffffd"
;;   

(use binary.pack)

(define (unpack-int32 n)
  (car (unpack "l" :from-string (pack "L" (list n) :to-string? #t))))

(define (print-list ls)
  (cond [(null? ls) (newline)]
        [else (display (car ls))
              (display " ")
              (print-list (cdr ls))]))

(define (main args)
  (print-list
   (map
    (lambda (x) (unpack-int32 (string->number x)))
    (cdr args)))
  0)

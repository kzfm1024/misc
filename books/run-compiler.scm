#!/usr/bin/env gosh

;;;
;;;Gauche で動作させるための追加
;;;
(define true #t)
(define false #f)

(load "./ch5-compiler.scm")

(define factorial-code
  (compile
   '(define (factorial n)
      (if (= n 1)
          1
          (* (factorial (- n 1)) n)))
   'val
   'next))

(map print (statements factorial-code))

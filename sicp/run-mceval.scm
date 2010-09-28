#!/usr/bin/env gosh

(load "./ch4-mceval.scm")

;; Overrides the list in ch4-mceval.scm

(define primitive-procedures
  (list (list 'car car)
        (list 'cdr cdr)
        (list 'cons cons)
        (list 'null? null?)
        (list 'list list)
        (list '+ +)
        (list '- -)
        (list '* *)
        (list '/ /)
        (list '= =)
        (list 'newline newline)
        (list 'display display)
;;      more primitives
        (list 'exit exit) ; for Gauche
        ))

(define the-global-environment (setup-environment))

#|
(eval '(define (fact n)
         (if (= n 1)
             1
             (* n (fact (- n 1)))))
      the-global-environment)
|#

(driver-loop)

#!/usr/bin/env gosh

(load "./ch5-regsim.scm")

(define gcd-machine
  (make-machine
   '(a b t)
   (list (list 'rem remainder) (list '= =))
   '(test-b
       (test (op =) (reg b) (const 0))
       (branch (label gcd-done))
       (assign t (op rem) (reg a) (reg b))
       (assign a (reg b))
       (assign b (reg t))
       (goto (label test-b))
     gcd-done)))

(set-register-contents! gcd-machine 'a 206)

(set-register-contents! gcd-machine 'b 40)

(start gcd-machine)

(get-register-contents gcd-machine 'a)

#|
;
; assemble で (print insts) した結果 
;
(
 ((test (op =) (reg b) (const 0)))
 ((branch (label gcd-done)))
 ((assign t (op rem) (reg a) (reg b)))
 ((assign a (reg b)))
 ((assign b (reg t)))
 ((goto (label test-b)))
)

;
; assemble で (print insts) した結果 
;
(
 (test-b
  ((test (op =) (reg b) (const 0)))
  ((branch (label gcd-done)))
  ((assign t (op rem) (reg a) (reg b)))
  ((assign a (reg b))) ((assign b (reg t)))
  ((goto (label test-b))))
 (gcd-done)
)

;
; (gcd-machine 'instruction-sequence) =>
;
(
 ((test (op =) (reg b) (const 0)) . #<closure (make-test make-test)>) 
 ((branch (label gcd-done)) . #<closure (make-branch make-branch)>)
 ((assign t (op rem) (reg a) (reg b)) . #<closure (make-assign make-assign)>)
 ((assign a (reg b)) . #<closure (make-assign make-assign)>) 
 ((assign b (reg t)) . #<closure (make-assign make-assign)>) 
 ((goto (label test-b)) . #<closure (make-goto make-goto)>)
)
|#

(define fact-machine
  (make-machine
   '(continue val n)
   (list (list '= =) (list '- -) (list '* *))
   '(controller
       (assign continue (label fact-done))     ; set up final return address
     fact-loop
       (test (op =) (reg n) (const 1))
       (branch (label base-case))
       ;; Set up for the recursive call by saving n and continue.
       ;; Set up continue so that the computation will continue
       ;; at after-fact when the subroutine returns.
       (save continue)
       (save n)
       (assign n (op -) (reg n) (const 1))
       (assign continue (label after-fact))
       (goto (label fact-loop))
     after-fact
       (restore n)
       (restore continue)
       (assign val (op *) (reg n) (reg val))   ; val now contains n(n-1)!
       (goto (reg continue))                   ; return to caller
     base-case
       (assign val (const 1))                  ; base case: 1!=1
       (goto (reg continue))                   ; return to caller
     fact-done)))

(define fib-machine
  (make-machine
   '(continue val n)
   (list (list '= =) (list '< <) (list '+ +) (list '- -))
   '(controller
       (assign continue (label fib-done))
     fib-loop
       (test (op <) (reg n) (const 2))
       (branch (label immediate-answer))
       ;; set up to compute Fib(n-1)
       (save continue)
       (assign continue (label afterfib-n-1))
       (save n)                           ; save old value of n
       (assign n (op -) (reg n) (const 1)); clobber n to n-1
       (goto (label fib-loop))            ; perform recursive call
     afterfib-n-1                         ; upon return, val contains Fib(n-1)
       (restore n)
       (restore continue)
       ;; set up to compute Fib(n-2)
       (assign n (op -) (reg n) (const 2))
       (save continue)
       (assign continue (label afterfib-n-2))
       (save val)                         ; save Fib(n-1)
       (goto (label fib-loop))
     afterfib-n-2                         ; upon return, val contains Fib(n-2)
       (assign n (reg val))               ; n now contains Fib(n-2)
       (restore val)                      ; val now contains Fib(n-1)
       (restore continue)
       (assign val                        ; Fib(n-1)+Fib(n-2)
               (op +) (reg val) (reg n)) 
       (goto (reg continue))              ; return to caller, answer is in val
     immediate-answer
       (assign val (reg n))               ; base case: Fib(n)=n
       (goto (reg continue))
     fib-done)))

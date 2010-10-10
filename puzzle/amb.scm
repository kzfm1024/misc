;;
;; http://www.shido.info/lisp/scheme_amb.html
;;
;; オリジナルは上記 URL からダウンロード

(define amb-fail #f)

(define (initialize-amb-fail)
  (call/cc
    (lambda (cc)
      (set! amb-fail
            (lambda ()
              (cc 'no-more-values))))))

;;; nondeterminsm macro operator
(define-syntax amb
  (syntax-rules ()
    ((_) (amb-fail))
    ((_ a) a)
    ((_ a b ...)
     (let ((amb-fail0 amb-fail))
       (call/cc
         (lambda (cc)
           (set! amb-fail
                 (lambda ()
                   (set! amb-fail amb-fail0)
                   (cc (amb b ...))))
           (cc a)))))))

;;; returning all possibilities
(define-syntax set-of
  (syntax-rules () 
    ((_ s) 
     (let ((acc '())) 
       (amb (let ((v s)) 
              (set! acc (cons v acc)) 
              (amb-fail)) 
            (reverse acc))))))

;;; if not pred backtrack
(define (assert pred)
  (if (not pred) (amb)))

;;; returns arbitrary number larger or equal to n
(define (an-integer-starting-from n)
  (amb n (an-integer-starting-from (+ n 1))))

;;; returns arbitrary number between a and b
(define (number-between a b)
  (let loop ((i a))
    (if (> i b)
        (amb)
        (amb i (loop (+ i 1))))))

(define-module whitespace.vm
  (export run))
(select-module whitespace.vm)

(define *vm-stack* '())

(define *vm-heap* (make-hash-table))  

(define (%push num) (push! *vm-stack* num))

(define (%dup) (push! *vm-stack* (car *vm-stack*)))

(define (%copy nth) (push! *vm-stack* (ref *vm-stack* nth)))

(define (%swap)
  (let ((y (pop! *vm-stack*))
        (x (pop! *vm-stack*)))
    (push! *vm-stack* y)
    (push! *vm-stack* x)))

(define (%discard) (pop! *vm-stack*))

(define (%slide n)
  (define (pop-loop n)
    (when (positive? n)
      (pop! *vm-stack*)
      (pop-loop (- n 1))))
  (let1 x (pop! *vm-stack*)
    (pop-loop n)
    (push! *vm-stack* x)))

(define (%add)
  (let ((y (pop! *vm-stack*))
        (x (pop! *vm-stack*)))
    (push! *vm-stack* (+ x y))))

(define (%sub)
  (let ((y (pop! *vm-stack*))
        (x (pop! *vm-stack*)))
    (push! *vm-stack* (- x y))))

(define (%mul)
  (let ((y (pop! *vm-stack*))
        (x (pop! *vm-stack*)))
    (push! *vm-stack* (* x y))))

(define (%div)
  (let ((y (pop! *vm-stack*))
        (x (pop! *vm-stack*)))
    (push! *vm-stack* (/ x y))))

(define (%mod)
  (let ((y (pop! *vm-stack*))
        (x (pop! *vm-stack*)))
    (push! *vm-stack* (modulo x y))))

(define (%heap_write)
  (let ((value (pop! *vm-stack*))
        (address (pop! *vm-stack*)))
    (hash-table-put! *vm-heap* address value)))

(define (%heap_write)
  (let ((value (pop! *vm-stack*))
        (address (pop! *vm-stack*)))
    (hash-table-put! *vm-heap* address value)))

(define (%heap_read)
  (let ((address (pop! *vm-stack*)))
    (push! *vm-stack* (hash-table-get *vm-heap* address))))

(define (%label label) #t)
(define (%call label) #t)
(define (%jump label) #t)
(define (%jump_zero label) #t)
(define (%jump_negative label) #t)

(define (%return) #t)
(define (%exit) #t)

(define (%char_out) #t)
(define (%num_out) #t)
(define (%char_in) #t)
(define (%num_in) #t)

#|
(define (run insns)
  (for-each (lambda (expr) (eval expr interaction-environment)) insns))
|#
(define (run insns) (print insns))

(provide "whitespace.vm")

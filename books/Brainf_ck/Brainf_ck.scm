#!/usr/bin/env gosh

(use gauche.sequence)

;;
;; <tape>
;;
(define-class <tape> ()
  ((cur :init-value 0)
   (lst :init-keyword :lst)))

(define-method set-value ((tape <tape>) (val <integer>))
  (set! (ref (ref tape 'lst) (ref tape 'cur)) val))

(define-method get-value ((tape <tape>))
  (ref (ref tape 'lst) (ref tape 'cur)))

(define-method display-value ((tape <tape>))
  (display (integer->char (get-value tape))))

(define-method inc-value ((tape <tape>))
  (set-value tape (+ (get-value tape) 1)))

(define-method dec-value ((tape <tape>))
  (set-value tape (- (get-value tape) 1)))

(define-method move-next ((tape <tape>))
  (set! (ref tape 'cur) (+ (ref tape 'cur) 1))
  (if (>= (ref tape 'cur) (length (ref tape 'lst)))
      (append! (ref tape 'lst) (list 0))))

(define-method move-prev ((tape <tape>))
  (set! (ref tape 'cur) (- (ref tape 'cur) 1))
  (when (< (ref tape 'cur) 0) (error "can't move prev")))

;;
;; <Brainf*ck>
;;
(define (pairs-of-brackets src)
  (let ((stack (make-hash-table)))
    (define (push val) (hash-table-push! stack 'brackets val))
    (define (pop) (hash-table-pop! stack 'brackets))
    (define (analyze-pairs itk)
      (define (index itk) (caar itk))
      (define (token itk) (cdar itk))
      (cond [(null? itk) '()]
            [(equal? (token itk) #\[)
             (push (index itk))
             (analyze-pairs (cdr itk))]
            [(equal? (token itk) #\])
             (cons (cons (pop) (index itk))
                   (analyze-pairs (cdr itk)))]
            [else (analyze-pairs (cdr itk))]))
    (analyze-pairs (map-with-index cons src))))

(define (right-bracket pc pairs)
  (if (= pc (caar pairs))
      (cdar pairs)
      (right-bracket pc (cdr pairs))))

(define (left-bracket pc pairs)
  (if (= pc (cdar pairs))
      (caar pairs)
      (left-bracket pc (cdr pairs))))

(define-class <Brainf*ck> ()
  ((tape :init-keyword :tape)))

(define-method run ((brainf*ck <Brainf*ck>) (src <string>))
  (let ((pc 0)
        (tokens (list->vector (string->list src)))
        (pairs (pairs-of-brackets src)))
    (define (loop)
      (let ((tk (ref tokens pc))
            (tape (ref brainf*ck 'tape)))
        (case tk
          [(#\+) (inc-value tape)]
          [(#\-) (dec-value tape)]
          [(#\>) (move-next tape)]
          [(#\<) (move-prev tape)]
          [(#\.) (display-value tape)]
          [(#\,) (set-value (char->integer (read-char)))]
          [(#\[) (when (= (get-value tape) 0)
                       (set! pc (right-bracket pc pairs)))]
          [(#\]) (unless (= (get-value tape) 0)
                         (set! pc (left-bracket pc pairs)))])
        (set! pc (+ pc 1))
        (when (< pc (vector-length tokens)) (loop))))
    (loop)))

;(pairs-of-brackets "+++[+++[++]++]++")

(define (run-Brainf*ck)
  (let ((bf (make <Brainf*ck> :tape (make <tape> :lst '(0)))))
    (port-for-each (lambda (line) (run bf line)) read-line)
    (newline)))

(define (main args)
  (let ((files (cdr args)))
    (if (null? files)
        (run-Brainf*ck)
        (for-each (lambda (file) (with-input-from-file file run-Brainf*ck))
                  files))))

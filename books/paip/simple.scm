;;
;; Common Lisp 
;;  (cdr '()) は NIL を返す。
;;  assoc は一致しない場合は NIL を返す。
;;  NIL は false として扱われる。
;;
;; Scheme 
;;  (cdr '()) はエラーになる。
;;  assoc は一致しない場合は #f を返す。
;;  '() は true として扱われる。
;;

(use srfi-1)
(use srfi-27)

(define (rest lst)
  (if (null? lst)
      '()
      (cdr lst)))

(define (elt lst n) (first (drop lst n)))

(define (random-elt choices)
  (elt choices (random-integer (length choices))))

(random-source-randomize! default-random-source)

;;; ==============================

(define *simple-grammar*
  '((sentence -> (noun-phrase verb-phrase))
    (noun-phrase -> (Article Noun))
    (verb-phrase -> (Verb noun-phrase))
    (Article -> the a)
    (Noun -> man ball woman table)
    (Verb -> hit took saw liked)))

(define *grammar* *simple-grammar*)

;;; ==============================

(define (rule-lhs rule) (rest rule))

(define (rule-rhs rule) (rest (rest rule)))

(define (rewrites category)
  (cond ((assoc category *grammar*) => rule-rhs)
        (else '())))

;;; ==============================

(define (generate phrase)
  (cond ((null? phrase) '())
        ((list? phrase)
         (append-map generate phrase))
        ((not (null? (rewrites phrase)))
         (generate (random-elt (rewrites phrase))))
        (else (list phrase))))

;;; ==============================

(define *bigger-grammar*
  '((sentence -> (noun-phrase verb-phrase))
    (noun-phrase -> (Article Adj* Noun PP*) (Name) (Pronoun))
    (verb-phrase -> (Verb noun-phrase PP*))
    (PP* -> () (PP PP*))
    (Adj* -> () (Adj Adj*))
    (PP -> (Prep noun-phrase))
    (Prep -> to in by with on)
    (Adj -> big little blue green adiabatic)
    (Article -> the a)
    (Name -> Pat Kim Lee Terry Robin)
    (Noun -> man ball woman table)
    (Verb -> hit took saw liked)
    (Pronoun -> he she it these those that)))

;; (set! *grammar* *bigger-grammar*)

;;; ==============================

(define (generate-tree phrase)
  (cond ((null? phrase) '())
        ((list? phrase)
         (map generate-tree phrase))
        ((not (null? (rewrites phrase)))
         (cons phrase
               (generate-tree (random-elt (rewrites phrase)))))
        (else (list phrase))))

;;; ==============================

(define (generate-all phrase)
  (cond ((null? phrase) (list '()))
        ((list? phrase)
         (combine-all (generate-all (first phrase))
                      (generate-all (rest phrase))))
        ((not (null? (rewrites phrase)))
         (append-map generate-all (rewrites phrase)))
        (else (list (list phrase)))))

#|
"Return a list of lists formed by appending a y to an x.
E.g., (combine-all '((a) (b)) '((1) (2)))
-> ((A 1) (B 1) (A 2) (B 2))."
|#
(define (combine-all xlist ylist)
  (append-map (lambda (y)
                (map (lambda (x) (append x y)) xlist))
              ylist))

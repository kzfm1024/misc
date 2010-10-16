(use srfi-1)
(use srfi-27)

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
;; rest
;; (rest #f) => #f
;; (rest '()) => #f
;;
(define (rest lst)
  (if (or (eq? lst #f) (null? lst))
      #f
      (cdr lst)))

(define (elt lst n) (first (drop lst n)))

(define (random-elt choices)
  (elt choices (random-integer (length choices))))

(random-source-randomize! default-random-source)

(define null null?)

(define listp pair?)

(define mappend append-map)

(define mapcar map)

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
  (rule-rhs (assoc category *grammar*)))

;;; ==============================

(define (generate phrase)
  (cond ((listp phrase)
         (mappend generate phrase))
        ((rewrites phrase)
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
  (cond ((listp phrase)
         (mapcar generate-tree phrase))
        ((rewrites phrase)
         (cons phrase
               (generate-tree (random-elt (rewrites phrase)))))
        (else (list phrase))))

;;; ==============================

(define (generate-all phrase)
  (cond ((null phrase) (list nil))
        ((listp phrase)
         (combine-all (generate-all (first phrase))
                      (generate-all (rest phrase))))
        ((rewrites phrase)
         (mappend generate-all (rewrites phrase)))
        (else (list (list phrase)))))

#|
  "Return a list of lists formed by appending a y to an x.
  E.g., (combine-all '((a) (b)) '((1) (2)))
  -> ((A 1) (B 1) (A 2) (B 2))."
|#
(define (combine-all xlist ylist)
  (mappend (lambda (y)
             (mapcar (lambda (x) (append x y)) xlist))
           ylist))

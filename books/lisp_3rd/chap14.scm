(define-class <article> ()
  ((title :init-keyword :title)
   (author :init-keyword :author)))

(define-class <computer-article> (<article>) ())

(define-class <business-article> (<article>) ())

(define-class <political-article> (<article>) ())

(define-class <friend> ()
  ((name :init-keyword :name)))

(define-class <hacker-friend> (<friend>) ())

(define-class <entrepreneur-friend> (<friend>) ())

(define-class <philosopher-friend> (<friend>) ())

(define articles
  (list (make <business-article> :title "Memory Prices Down")
        (make <computer-article> :title "Memory Speeds Up")
        (make <political-article> :title "Memory Impugned")))

(define friends
  (list (make <hacker-friend> :name 'Dan)
        (make <hacker-friend> :name 'Gerry)
        (make <entrepreneur-friend> :name 'Philip)
        (make <philosopher-friend> :name 'David)))

(define (print-notification article friend)
  (format #t "~%Tell ~s about ~s"
          (ref friend 'name)
          (ref article 'title)))

(dolist (friend friends)
  (dolist (article articles)
    (print-notification article friend)))

;;----------------------------------------------------------------------

(define-method process ((friend <hacker-friend>)
                        (article <computer-article>))
  (print-notification article friend))

(define-method process ((friend <entrepreneur-friend>)
                        (article <business-article>))
  (print-notification article friend))

(define-method process ((friend <philosopher-friend>)
                        (article <article>))
  (print-notification article friend))

(define-method process ((friend <friend>) (article <article>)))

(dolist (friend friends)
  (dolist (article articles)
    (process friend article)))

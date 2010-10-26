(defclass article ()
  ((title :accessor article-title :initarg :title)
   (author :accessor article-author :initarg :author)))

(defclass computer-article (article) ())

(defclass business-article (article) ())

(defclass political-article (article) ())

(defclass friend ()
  ((name :accessor friend-name :initarg :name)))

(defclass hacker-friend (friend) ())

(defclass entrepreneur-friend (friend) ())

(defclass philosopher-friend (friend) ())

(defvar articles)

(defvar friends)

(setf articles
      (list (make-instance 'business-article
                           :title "Memory Prices Down")
            (make-instance 'computer-article
                           :title "Memory Speeds Up")
            (make-instance 'political-article
                           :title "Memory Impugned")))

(setf friends
      (list (make-instance 'hacker-friend :name 'Dan)
            (make-instance 'hacker-friend :name 'Gerry)
            (make-instance 'entrepreneur-friend :name 'Philip)
            (make-instance 'philosopher-friend :name 'David)))

(defun print-notification (article friend)
  (format t "~%Tell ~a about \"~a.\""
          (friend-name friend)
          (article-title article)))

(dolist (friend friends)
  (dolist (article articles)
    (print-notification article friend)))

;;----------------------------------------------------------------------

(defmethod process ((friend hacker-friend)
                    (article computer-article))
  (print-notification article friend))

(defmethod process ((friend entrepreneur-friend)
                    (article business-article))
  (print-notification article friend))

(defmethod process ((friend philosopher-friend)
                    (article article))
  (print-notification article friend))

(defmethod process ((friend friend) (article article)))

;(defmethod process ((friend t) (article t)))
                    
;(defmethod process (friend article))

(dolist (friend friends)
  (dolist (article articles)
    (process friend article)))

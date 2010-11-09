(defun bind-variables (variables values &optional a-list)
  (append (mapcar #'list variables values) a-list))

(defun micro-eval (form &optional environment)
  (if (atom form)
      (cond ((numberp form) form)
            ((eq t form) t)
            ((eq nil form) nil)
            (t (let ((binding (assoc form environment)))
                 (if binding
                     (second binding)
                     (error
                      "I could find no variable binding for ~a."
                      form)))))
      (case (first form)
        (m-quote (second form))
        (m-if (if (micro-eval (second form) environment)
                  (micro-eval (third form) environment)
                  (micro-eval (fourth form) environment)))
        (m-defun (setf (get (second form) 'm-lambda)
                       `(m-lambda ,(third form) ,(fourth form)))
                 (second form))
        (m-apply (micro-apply
                  (micro-eval (second form) environment)
                  (micro-eval (third form) environment)
                  environment))
        (m-setq (setf (second (assoc (second form) environment))
                      (micro-eval (third form) environment)))
        (m-function
         `(m-closure ,(second (second form))
                     ,(third (second form))
                     ,environment))
        (t (micro-apply
            (first form)
            (mapcar #'(lambda (x) (micro-eval x environment))
                    (rest form))
            environment)))))

(defun micro-apply (procedure argument-values environment)
  (if (symbolp procedure)
      (case procedure
        (m-first (first (first argument-values)))
        (m-rest (rest (first argument-values)))
        (m-cons (cons (first argument-values)
                      (second argument-values)))
        (m-endp (endp (first argument-values)))
        (m-not (not (first argument-values)))
        (m-eq (eq (first argument-values)
                  (second argument-values)))
        (t (let ((procedure-description (get procedure 'm-lambda)))
             (if procedure-description
                 (micro-apply procedure-description
                              argument-values
                              environment)
                 (error
                  "I could find no procedure description for ~a."
                  procedure)))))
      (case (first procedure)
        (m-lambda
         (micro-eval (third procedure)
                     (bind-variables (second procedure)
                                     argument-values
                                     environment)))
        (m-closure
         (micro-eval (third procedure)
                     (bind-variables (second procedure)
                                     argument-values
                                     (fourth procedure)))))))

(micro-eval '(m-apply
              (m-function
               (m-lambda (x y)
                (m-apply
                 (m-function
                  (m-lambda ()
                   (m-cons x (m-cons y nil))))
                 (m-quote ()))))
              (m-quote (a b))))


(setf (get 'm-toggle 'm-lambda)
      (micro-eval
       '((m-lambda (x)
          (m-function
           (m-lambda () (m-setq x (m-not x)))))
         t)))

(micro-eval '(m-apply
              (m-quote
               (m-lambda (x y)
                (m-apply
                 (m-quote
                  (m-lambda ()
                   (m-cons x (m-cons y nil))))
                 (m-quote ()))))
              (m-quote (a b))))
